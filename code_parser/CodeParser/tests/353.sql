CREATE OR REPLACE PACKAGE "QM"."QM_USER" 
IS

  -- Author  : ELDANIZ
  -- Created : 2012.09.17 12:51:45
  -- Purpose : pakage for qmOperator application..

  FUNCTION get_languages RETURN qm_types.result_cursor;

  FUNCTION get_ui_texts(p_host VARCHAR2, p_domain_user VARCHAR2) RETURN qm_types.result_cursor;

  FUNCTION get_user_info(p_host VARCHAR2) RETURN qm_types.result_cursor;

  FUNCTION get_client_info(p_host VARCHAR2) RETURN qm_types.result_cursor;

  PROCEDURE change_user_lang(p_host VARCHAR2, p_lang_key VARCHAR2);

  PROCEDURE change_user_status(p_host VARCHAR2,
                               p_status NUMBER,
                               p_waiting_for VARCHAR2 DEFAULT NULL);
  
  PROCEDURE set_waiting_user_free(p_branch_id NUMBER,
	                                p_user_id NUMBER,
	                                p_receipt_code VARCHAR2);

  PROCEDURE client_assign_next(p_host VARCHAR2);

  PROCEDURE client_service_complete(p_host VARCHAR2, p_status NUMBER);

  PROCEDURE client_send(p_host VARCHAR2,
                        p_to_cash NUMBER,
                        p_to_oper_id NUMBER DEFAULT NULL, -- if p_to_oper_id = -1 then back to caller operator
                        p_wait NUMBER DEFAULT 0); -- 1=wait

  PROCEDURE client_recall(p_host VARCHAR2);

  PROCEDURE client_to_cash(p_host VARCHAR2);

  PROCEDURE client_to_cash_and_back(p_host VARCHAR2, p_wait NUMBER);

  FUNCTION client_to_oper(p_host VARCHAR2, p_to_oper_code VARCHAR2)
  RETURN NUMBER;

  FUNCTION client_call_as_vip(p_host VARCHAR2, p_receipt_code VARCHAR2)
  RETURN NUMBER;

  PROCEDURE close_expired_services;

END;
/



CREATE OR REPLACE PACKAGE BODY "QM"."QM_USER" 
IS

  FUNCTION get_languages
  RETURN qm_types.result_cursor
  IS
    c_result qm_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ql.lang_id,
                   ql.lang_key,
                   ql.lang_code,
                   ql.lang_name
            FROM qm_languages ql
            WHERE ql.is_deleted = 0
                  AND ql.lang_id IN(1,2,3)
            ORDER BY DECODE(ql.lang_key, 'AZ', 1, 'RU', 2, 'EN', 3, 4),
                     ql.lang_key;

       RETURN c_result;
  END;

  FUNCTION get_ui_texts(p_host VARCHAR2, p_domain_user VARCHAR2)
  RETURN qm_types.result_cursor
  IS
    c_result qm_types.result_cursor;
    v_lang_key NVARCHAR2(3);
    v_status NVARCHAR2(1);
  BEGIN
       FOR i IN (SELECT qu.user_id,
                        qu.status_id,
                        ul.lang_key
                 FROM qm_users qu,
                      qm_languages ul
                 WHERE qu.lang_id = ul.lang_id(+)
                       --
                       AND qu.user_host = p_host
                 FOR UPDATE OF qu.first_login_date,
                               qu.last_login_date,
                               qu.last_logged_user)
       LOOP
           UPDATE qm_users qu
           SET qu.first_login_date = NVL(qu.first_login_date, SYSDATE),
               qu.last_login_date = SYSDATE,
               qu.last_logged_user = UPPER(p_domain_user)
           WHERE qu.user_id = i.user_id;
           COMMIT;

           v_lang_key := i.lang_key;
           v_status := i.status_id;

           EXIT;
       END LOOP;

       v_lang_key := NVL(v_lang_key, 'AZ');
       v_status := NVL(v_status, '0');

       OPEN c_result FOR
            SELECT 'USER_LANGUAGE' text_key,
                   v_lang_key text_value
            FROM dual
            UNION
            SELECT 'USER_STARTUP_STATUS' text_key,
                   v_status text_value
            FROM dual
            UNION
            SELECT ut.key_name,
                   ut.lang_text
            FROM v_qm_ui_translations ut
            WHERE ut.module_name = 'QM_USER'
                  --
                  AND ut.lang_key = v_lang_key;

       RETURN c_result;
  END;

  FUNCTION get_user_info(p_host VARCHAR2)
  RETURN qm_types.result_cursor
  IS
    c_result qm_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT qu.user_code,
                   DECODE(qu.group_id, 4, 1, 0) is_cashier,
                   bt.branch_name,
                   st.status_name,
                   gt.group_name,
                   NVL(c.client_count, 0) client_count
            FROM qm_users qu,
                 v_qm_branch_translations bt,
                 v_qm_group_translations gt,
                 v_qm_status_translations st,
                 (SELECT qq.user_id,
                         COUNT(*) client_count
                  FROM qm_queues qq
                  WHERE qq.assigned_at IS NOT NULL
                        AND qq.processed_at IS NOT NULL
                        AND TRUNC(qq.created_at) = TRUNC(SYSDATE)
                  GROUP BY qq.user_id) c
            WHERE qu.branch_id = bt.branch_id
                  AND qu.group_id = gt.group_id
                  AND qu.status_id = st.status_id
                  AND qu.lang_id = bt.lang_id
                  AND qu.lang_id = gt.lang_id
                  AND qu.lang_id = st.lang_id
                  AND qu.user_id = c.user_id(+)
                  --
                  AND qu.group_id IN(2,3,4) -- supervisor = 2, operator = 3, cashier = 4
                  --
                  AND qu.user_host = p_host;

       RETURN c_result;
  END;

  FUNCTION get_client_info(p_host VARCHAR2)
  RETURN qm_types.result_cursor
  IS
    c_result qm_types.result_cursor;
  BEGIN
       --close_expired_services;

       OPEN c_result FOR
            SELECT qq.queue_id,
                   qq.receipt_code,
                   st.service_name,
                   NVL(c.client_count, 0) client_count
            FROM qm_users qu,
                 qm_queues qq,
                 v_qm_service_translations st,
                 (SELECT qq.user_id,
                         COUNT(*) client_count
                  FROM qm_queues qq
                  WHERE qq.assigned_at IS NOT NULL
                        AND qq.processed_at IS NOT NULL
                        AND TRUNC(qq.created_at) = TRUNC(SYSDATE)
                  GROUP BY qq.user_id) c
            WHERE qu.user_id = qq.user_id
                  AND st.service_id = qq.service_id
                  AND st.lang_id = qu.lang_id
                  AND qu.user_id = c.user_id(+)
                  --
                  AND qq.assigned_at IS NOT NULL
                  AND qq.processed_at IS NULL
                  AND TRUNC(qq.created_at) = TRUNC(SYSDATE)
                  AND qu.group_id IN(2,3,4) -- supervisor = 2, operator = 3, cashier = 4
                  --
                  AND qu.user_host = p_host
            ORDER BY qq.created_at;

       RETURN c_result;
  END;

  PROCEDURE change_user_lang(p_host VARCHAR2, p_lang_key VARCHAR2)
  IS
  BEGIN
       FOR i IN(SELECT qu.user_id
                FROM qm_users qu
                WHERE qu.user_host = p_host
								      --AND uo.status_id != 0
                FOR UPDATE OF qu.lang_id)
       LOOP
           UPDATE qm_users qu
           SET qu.lang_id = (SELECT ul.lang_id FROM qm_languages ul WHERE ul.lang_key = NVL(p_lang_key, 'AZ'))
           WHERE qu.status_id != 0
                 AND qu.user_id = i.user_id;
       END LOOP;

       IF SQL%ROWCOUNT > 0
       THEN
           COMMIT;
       END IF;
  END;

  PROCEDURE change_user_status(p_host VARCHAR2,
                               p_status NUMBER,
                               p_waiting_for VARCHAR2 DEFAULT NULL)
  IS
  BEGIN
       FOR i IN(SELECT qu.user_id
                FROM qm_users qu
                WHERE qu.user_host = p_host
                FOR UPDATE OF qu.status_id,
                              qu.status_changed_at,
                              qu.waiting_for)
       LOOP
           UPDATE qm_users qu
           SET qu.status_id = p_status,
               qu.status_changed_at = SYSDATE,
               qu.waiting_for = p_waiting_for--(CASE WHEN p_status = 5 THEN p_waiting_for ELSE qu.waiting_for END)
           WHERE qu.status_id != 0
                 AND qu.user_id = i.user_id;
           
           INSERT INTO qm_user_status_logs
                  (log_id,
                   log_date,
                   user_id,
                   status_id)
           VALUES (qm_user_status_logs_seq.nextval,
                   SYSDATE,
                   i.user_id,
                   p_status);
       END LOOP;

       IF SQL%ROWCOUNT > 0
       THEN
           COMMIT;
       END IF;

       IF p_status IN(0,1,2,5)
       THEN
           client_service_complete(p_host, p_status);
       END IF;
  END;
  
  PROCEDURE set_waiting_user_free(p_branch_id NUMBER,
	                                p_user_id NUMBER,
                                  p_receipt_code VARCHAR2)
	IS
  BEGIN
	     UPDATE qm_users qu
       SET qu.waiting_for = NULL,
           qu.status_id = 4
       WHERE qu.branch_id = p_branch_id
             AND qu.waiting_for = p_receipt_code
             AND qu.user_id != p_user_id;
	END;

  PROCEDURE client_assign_next(p_host VARCHAR2)
  IS
  BEGIN
	     client_service_complete(p_host, 4); -- set free

       FOR i IN (SELECT qq.queue_id,
                        qu.user_id,
                        qq.branch_id,
                        qq.receipt_code,
                        qq.next_steps,
                        --COUNT(*) OVER(PARTITION BY q.service_id, q.receipt_code ORDER BY q.queue_id) queue_cycle,
                        (CASE WHEN qu.waiting_for = qq.receipt_code AND qu.user_id = qq.user_id THEN 0 ELSE 1 END) order1,
                        (CASE WHEN qq.queue_priority = 0 THEN qq.queue_priority ELSE us.service_priority END) order2,
                        qq.queue_priority order3,
                        qu.user_priority order4,
                        qq.created_at order5
                 FROM qm_queues qq,
                      qm_services qs,
                      qm_users qu,
                      qm_groups g,
                      qm_user_services us,
                      qm_user_statuses qus
                 WHERE qq.service_id = qs.service_id
                       AND qu.group_id = g.group_id
                       --AND us.service_id = qs.service_id
                       AND (CASE WHEN (/*qu.group_id = 4*/g.group_key = 'CASHIER' AND qq.next_steps LIKE 'C%') OR
                                      (TRIM(qu.user_id) = qq.next_steps AND NVL(qq.next_steps, 'X') NOT LIKE 'C%')
                                 THEN qs.service_id
																 ELSE us.service_id
                            END) = qs.service_id
                       AND us.user_id = qu.user_id
                       AND qu.status_id = qus.status_id
                       AND qq.branch_id = qu.branch_id
                       --
                       AND qs.disabled = 0
                       AND (qus.can_work = 1 OR qu.waiting_for = qq.receipt_code)
                       AND qq.assigned_at IS NULL
                       --
                       AND TRUNC(qq.created_at) = TRUNC(SYSDATE)
                       AND (CASE WHEN qq.queue_priority = 0
                                 THEN (CASE WHEN (/*qu.group_id = 4*/g.group_key = 'CASHIER' AND qq.next_steps LIKE 'C%') OR -- to cash
                                                 (TRIM(qu.user_id) = qq.next_steps AND NVL(qq.next_steps, 'X') NOT LIKE 'C%') -- to oper
                                            THEN 1
                                            ELSE 0
                                       END)
                                 ELSE 1
                            END) = 1
                       AND qu.user_host = p_host
                 ORDER BY order1,
                          order2,
                          order3,
                          order4,
                          order5
                 FOR UPDATE OF qq.assigned_at)
       LOOP
           UPDATE qm_queues
           SET assigned_at = SYSDATE,
               user_id = i.user_id,
               next_steps = (CASE WHEN next_steps LIKE 'C,%' THEN REPLACE(next_steps, 'C,', '') END),
               last_recalled_at = SYSDATE
           WHERE queue_id = i.queue_id;
           COMMIT;
           
           IF i.next_steps NOT LIKE 'C%'
					 THEN
						   set_waiting_user_free(i.branch_id, i.user_id, i.receipt_code);
           END IF;

           change_user_status(p_host, 3); -- set busy
           EXIT;
       END LOOP;
	END;

  PROCEDURE client_service_complete(p_host VARCHAR2, p_status NUMBER)
  IS
  BEGIN
	     FOR i IN (SELECT qq.queue_id,
                        qq.user_id,
                        qq.branch_id
                 FROM qm_queues qq,
                      qm_users qu
                 WHERE qq.user_id = qu.user_id
                       AND qq.branch_id = qu.branch_id
                       --
                       AND qq.assigned_at IS NOT NULL
                       AND qq.processed_at IS NULL
                       --AND qq.next_steps IS NULL
                       --
                       AND qu.user_host = p_host)
       LOOP
           UPDATE qm_queues qq
           SET qq.assigned_at = DECODE(qq.next_steps, NULL, qq.assigned_at, NULL),
               qq.processed_at = DECODE(qq.next_steps, NULL, SYSDATE, NULL),
               qq.queue_priority = DECODE(qq.next_steps, NULL, qq.queue_priority, 0)
           WHERE qq.queue_id = i.queue_id
                 AND qq.user_id = i.user_id
                 AND qq.branch_id = i.branch_id;

           IF SQL%ROWCOUNT > 0
           THEN
               COMMIT;
           END IF;

           IF p_status NOT IN(0,1,2,5)
           THEN
               change_user_status(p_host, 4); -- set free
           END IF;
       END LOOP;
	END;

  PROCEDURE client_send(p_host VARCHAR2,
                        p_to_cash NUMBER,
                        p_to_oper_id NUMBER DEFAULT NULL, -- if p_to_oper_id = -1 then back to caller operator
                        p_wait NUMBER DEFAULT 0)
  IS
  BEGIN
	     FOR i IN (SELECT qq.queue_id,
                        qq.branch_id,
                        qq.receipt_code,
                        qq.service_id,
                        qq.queue_priority,
                        qq.created_at,
                        qq.assigned_at,
                        qq.user_id,
                        qq.next_steps,
                        qq.processed_at,
                        qq.last_recalled_at,
                        DECODE(p_to_cash, 1, 'C' || DECODE(p_to_oper_id, -1, ',')) || TRIM(DECODE(p_to_oper_id, -1, qu.user_id, p_to_oper_id)) next_step
                 FROM qm_users qu,
                      qm_queues qq,
                      qm_users quo
                 WHERE qu.user_id = qq.user_id
                       AND quo.user_id = qu.user_id--DECODE(p_to_oper_id, -1, uo.oper_id, p_to_oper_id)
                       AND qu.branch_id = qq.branch_id
                       AND quo.branch_id = qq.branch_id
                       AND quo.status_id IN(2,3,4,5)
                       --
                       AND qq.assigned_at IS NOT NULL
                       AND qq.processed_at IS NULL
                       --
                       AND qu.user_host = p_host)
       LOOP
           UPDATE qm_queues
           SET processed_at = SYSDATE
           WHERE queue_id = i.queue_id;
           
           INSERT INTO qm_queues
                  (queue_id,
                   branch_id,
                   receipt_code,
                   service_id,
                   queue_priority,
                   created_at,
                   assigned_at,
                   user_id,
                   next_steps,
                   processed_at,
                   last_recalled_at)
           VALUES (qm_queues_seq.nextval,
                   i.branch_id,
                   i.receipt_code,
                   i.service_id,
                   0,
                   SYSDATE,
                   NULL,
                   NULL,
                   i.next_step,
                   NULL,
                   NULL);
           COMMIT;

           IF p_to_oper_id = -1 AND p_wait = 1
           THEN change_user_status(p_host, 5, i.receipt_code); -- set waiting
           ELSE change_user_status(p_host, 4); -- set free
           END IF;

           EXIT;
       END LOOP;
	END;

  PROCEDURE client_recall(p_host VARCHAR2)
  IS
  BEGIN
       FOR i IN (SELECT qq.queue_id
                 FROM qm_users qu,
                      qm_queues qq
                 WHERE qu.user_id = qq.user_id
                       AND qu.branch_id = qq.branch_id
                       --
                       AND qq.assigned_at IS NOT NULL
                       AND qq.processed_at IS NULL
                       --AND TRUNC(uaq.created) = TRUNC(SYSDATE)
                       --
                       AND qu.user_host = p_host)
       LOOP
           UPDATE qm_queues qq
           SET qq.last_recalled_at = SYSDATE
           WHERE qq.assigned_at IS NOT NULL
                 AND qq.processed_at IS NULL
                 --
                 AND qq.queue_id = i.queue_id;
           COMMIT;
       END LOOP;
  END;

  PROCEDURE client_to_cash(p_host VARCHAR2)
  IS
  BEGIN
       client_send(p_host, 1);
  END;

  PROCEDURE client_to_cash_and_back(p_host VARCHAR2, p_wait NUMBER)
  IS
  BEGIN
       client_send(p_host, 1, -1, p_wait);
  END;

  FUNCTION client_to_oper(p_host VARCHAR2, p_to_oper_code VARCHAR2)
  RETURN NUMBER
  IS
    v_result NUMBER(1) := 0;
  BEGIN
       FOR i IN(SELECT qu.user_id
                FROM qm_users qu
                WHERE qu.status_id IN(2,3,4,5)
                      AND qu.user_code = p_to_oper_code)
       LOOP
           client_send(p_host, 0, i.user_id); -- if p_to_oper_id = -1 then back to caller operator

           v_result := 1;
           EXIT;
       END LOOP;

       RETURN v_result;
  END;

  FUNCTION client_call_as_vip(p_host VARCHAR2, p_receipt_code VARCHAR2)
  RETURN NUMBER
  IS
    v_result NUMBER(1) := 0;
  BEGIN
       FOR i IN(SELECT qq.queue_id,
                       qu.user_id,
                       qq.branch_id
                FROM qm_queues qq,
                     qm_users qu
                WHERE qq.branch_id = qu.branch_id
                      --
                      AND qq.assigned_at IS NULL
                      --
                      AND qq.receipt_code = p_receipt_code
                      AND qu.user_host = p_host)
       LOOP
           UPDATE qm_queues qq
           SET qq.queue_priority = 0,
               qq.next_steps = TRIM(i.user_id)
           WHERE qq.queue_id = i.queue_id
                 AND qq.branch_id = i.branch_id;

           COMMIT;

           v_result := 1;
           EXIT;
       END LOOP;

       RETURN v_result;
  END;

  PROCEDURE close_expired_services
  IS
  BEGIN
       FOR i IN(SELECT /*uo.oper_id,
                       uo.branch_id,
                       uaq.queue_id,*/
                       qu.user_host
                FROM qm_queues qq,
                     qm_users qu
                WHERE qq.user_id = qu.user_id
                      AND qq.branch_id = qq.branch_id
                      --
                      AND qq.assigned_at IS NOT NULL
                      AND qq.processed_at IS NULL
                      AND (SYSDATE - qq.created_at) >= 1/4)
       LOOP
           change_user_status(i.user_host, 1);
       END LOOP;
  END;

END;
/