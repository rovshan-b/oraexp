CREATE OR REPLACE PACKAGE reports_tl
IS

  -- Author  : ELDANIZ
  -- Created : 19.01.2009 16:26:28
  -- Purpose :

  TYPE result_cursor IS REF CURSOR;

  -- TL1010 - Teller detailed report
  FUNCTION tl1010(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor;

  -- TL1011 - Teller detailed report (GL)
  -- TL1060 - Daily teller report by GL
  FUNCTION tl1011(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor;

  -- TL1020 - Teller aggregated report
  FUNCTION tl1020(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor;
  
  -- TL1030 - Incomplete transactions report
  FUNCTION tl1030_txn_pending_for_auth(p_branch_id NUMBER)
  RETURN result_cursor;
  FUNCTION tl1030_txn_pending_for_subm(p_branch_id NUMBER)
  RETURN result_cursor;
  FUNCTION tl1030_txn_unprocessed(p_branch_id NUMBER)
  RETURN result_cursor;
  -- TL1030 - Incomplete transactions report
  
  -- TL1040 - Batch status inquiry
  FUNCTION tl1040(p_branch_id NUMBER,
                  p_date DATE,
                  p_batch_status NUMBER,
                  p_batch_type NUMBER)
  RETURN result_cursor;

  -- TL1050 - LNM35 screen transactions
  FUNCTION tl1050(p_start_date DATE,
                  p_end_date DATE)
  RETURN result_cursor;

  -- TL1070 - Teller batch operations (open-close)
  FUNCTION tl1070(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE)
  RETURN result_cursor;

  -- TL1080 - Incorrect transactions
  FUNCTION tl1080(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2,
                  p_txn_type VARCHAR2)
  RETURN result_cursor;

END;
/
CREATE OR REPLACE PACKAGE BODY reports_tl
IS

  -- TL1010 - Teller detailed report
  FUNCTION tl1010(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_rec_denm AS (SELECT rd.cod_brn,
                                       rd.user_no,
                                       rd.cod_denm_cur ccy,
                                       rd.dat_post post_date,
                                       SUM(rd.tot_amount) beginning_balance
                                FROM v_rec_denm rd,
                                     (SELECT m.cod_brn,
                                             m.user_no,
                                             m.cod_denm_cur,
                                             MAX(m.dat_post) max_date
                                      FROM v_rec_denm m
                                      WHERE m.dat_post < TRUNC(p_start_date)
                                            AND m.cod_brn = NVL(p_branch_id, m.cod_brn)
                                            AND m.user_no = NVL(p_user_no, m.user_no)
                                      GROUP BY m.cod_brn,
                                               m.user_no,
                                               m.cod_denm_cur) md
                                WHERE rd.cod_brn = md.cod_brn
                                      AND rd.user_no = md.user_no
                                      AND rd.cod_denm_cur = md.cod_denm_cur
                                      AND rd.dat_post = md.max_date
                                GROUP BY rd.cod_brn,
                                         rd.user_no,
                                         rd.cod_denm_cur,
                                         rd.dat_post),
                 w_rec_txnlog AS (SELECT xt.cod_cc_brn,
                                         xt.cod_txn_mnemonic,
                                         xt.cod_userno,
                                         xt.cod_auth_id author_id,
                                         xt.ref_usr_no user_ref_no,
                                         xt.dat_post_str post_date,
                                         xt.dat_txn_str txn_date,
                                         xt.dat_value_str value_date,
                                         xt.cod_txn_ccy tcy_code,
                                         DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0) txn_amount_tcy_cr,
                                         DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0) txn_amount_tcy_db,
                                         --xt.txn_nrrtv narrative,
                                         SUBSTR(xt.txn_nrrtv, INSTR(xt.txn_nrrtv, ':', 1)) narrative,
                                         (CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) txn_state,
                                         --TRIM(xt.cod_acct_no) account_no,
                                         SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1) account_no,
                                         SUBSTR(xt.ref_sys_tr_aud_no, 1, LENGTH(xt.ref_sys_tr_aud_no)-2) txn_no
                                  FROM xf_ol_st_txnlog_mmdd xt
                                  WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                        --
                                        AND DECODE(p_date_type,
                                                   'value_date', xt.dat_value_str,
                                                   'tran_date', TRUNC(xt.dat_txn_str),
                                                   'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                        AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                                  --
                                  UNION
                                  --
                                  SELECT xt.cod_cc_brn,
                                         xt.cod_txn_mnemonic,
                                         xt.cod_userno,
                                         xt.cod_auth_id author_id,
                                         TRIM(xt.ref_usr_no) user_ref_no,
                                         xt.dat_post_str post_date,
                                         xt.dat_txn_str txn_date,
                                         xt.dat_value_str value_date,
                                         xt.cod_txn_ccy tcy_code,
                                         DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0) txn_amount_tcy_cr,
                                         DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0) txn_amount_tcy_db,
                                         --xt.txn_nrrtv narrative,
                                         TRIM(SUBSTR(xt.txn_nrrtv, INSTR(xt.txn_nrrtv, ':', 1))) narrative,
                                         (CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) txn_state,
                                         --TRIM(xt.cod_acct_no) account_no,
                                         TRIM(SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1)) account_no,
                                         SUBSTR(xt.ref_sys_tr_aud_no, 1, LENGTH(xt.ref_sys_tr_aud_no)-2) txn_no
                                  FROM xf_ol_st_txnlog_current@host_link xt
                                  WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                        --
                                        AND DECODE(p_date_type,
                                                   'value_date', xt.dat_value_str,
                                                   'tran_date', TRUNC(xt.dat_txn_str),
                                                   'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                        AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn))
            SELECT t.*,
                   --bl.*
                   NVL(bl.beginning_balance, 0) beginning_balance
            FROM w_rec_denm bl,
                 (SELECT rt.cod_cc_brn,
                         (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                         (tl.nam_user || ' ( ' || tl.cod_user_id || ' )') user_name,
                         up.cod_userno user_no,
                         tl.cod_userno tl_user_no,
                         rt.author_id,
                         rt.post_date,
                         rt.txn_date,
                         rt.value_date,
                         rt.tcy_code,
                         cc.nam_ccy_short tcy,
                         (TRIM(tm.cod_txn_mnemonic) || ': ' || tm.txt_txn_desc) tran_desc,
                         rt.narrative,
                         DECODE(rt.txn_state, 1, 'Submitted', 'Reversed') txn_state,
                         rt.account_no,
                         rt.txn_no,
                         rt.user_ref_no,
                         --c.cod_cust_id customer_id,
                         DECODE(c.cod_cust_id, NULL, NULL, (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full)) customer_name,
                         DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                         ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_cr ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_db, rt.txn_amount_tcy_cr) END) * rt.txn_state) txn_amount_tcy_cr,
                         ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_db ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_cr, rt.txn_amount_tcy_db) END) * rt.txn_state) txn_amount_tcy_db
                  FROM w_rec_txnlog rt,
                       ln_acct_dtls a,
                       ci_custmast c,
                       ba_txn_mnemonic tm,
                       ba_cc_brn_mast bm,
                       sm_user_profile tl,
                       sm_user_profile up,
                       ba_ccy_code cc,
                       (SELECT DISTINCT gd.cod_group,
                               gd.nam_group,
                               gd.cod_group_cycle,
                               gd.cod_acct_no
                        FROM ba_group_details gd
                        WHERE gd.flg_group_acc_attr = 'A') g
                  WHERE bm.cod_cc_brn = rt.cod_cc_brn
                        AND tl.cod_userno = rt.cod_userno
                        AND cc.cod_ccy = rt.tcy_code
                        --
                        AND rt.cod_txn_mnemonic = tm.cod_txn_mnemonic(+)
                        AND rt.account_no = g.cod_acct_no(+)
                        AND rt.account_no = a.cod_acct_no(+)
                        AND a.cod_cust_id = c.cod_cust_id(+)
                        --
                        AND up.cod_cc_brn = rt.cod_cc_brn
                        --
                        AND rt.cod_userno = DECODE(up.cod_branch_templ, 18, rt.cod_userno, up.cod_userno)
                        AND INSTR(DECODE(up.cod_branch_templ, 18,
                                         DECODE(rt.cod_userno, up.cod_userno,
                                                TRIM(rt.cod_txn_mnemonic),
                                                '9007,9008'),
                                         TRIM(rt.cod_txn_mnemonic)),
                                  TRIM(rt.cod_txn_mnemonic)) > 0
                        --
                        AND up.cod_userno = NVL(p_user_no, up.cod_userno)
                        AND DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS') = NVL(p_txn_mode, DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS'))) t
            WHERE t.cod_cc_brn = bl.cod_brn(+)
                  AND t.tcy_code = bl.ccy(+)
                  AND t.post_date >= bl.post_date(+)
                  AND t.user_no = bl.user_no(+)
            ORDER BY t.tcy,
                     DECODE(p_date_type,
                            'value_date', t.value_date,
                            'tran_date', t.txn_date,
                            'post_date', t.post_date),
                     t.group_name,
                     t.account_no;

       RETURN c_result;
  END;
  -- TL1010 - Teller detailed report

  -- TL1011 - Teller detailed report (GL)
  -- TL1060 - Daily teller report by GL
  FUNCTION tl1011(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_rec_denm AS (SELECT rd.cod_brn,
                                       rd.user_no,
                                       rd.cod_denm_cur ccy,
                                       rd.dat_post post_date,
                                       SUM(rd.tot_amount) beginning_balance
                                FROM v_rec_denm rd,
                                     (SELECT m.cod_brn,
                                             m.user_no,
                                             m.cod_denm_cur,
                                             MAX(m.dat_post) max_date
                                      FROM v_rec_denm m
                                      WHERE m.dat_post < TRUNC(p_start_date)
                                            AND m.cod_brn = NVL(p_branch_id, m.cod_brn)
                                            AND m.user_no = NVL(p_user_no, m.user_no)
                                      GROUP BY m.cod_brn,
                                               m.user_no,
                                               m.cod_denm_cur) md
                                WHERE rd.cod_brn = md.cod_brn
                                      AND rd.user_no = md.user_no
                                      AND rd.cod_denm_cur = md.cod_denm_cur
                                      AND rd.dat_post = md.max_date
                                GROUP BY rd.cod_brn,
                                         rd.user_no,
                                         rd.cod_denm_cur,
                                         dat_post),
                 w_rec_txnlog AS (SELECT rt.cod_brn,
                                         rt.cod_tran,
                                         rt.user_no,
                                         rt.dat_post post_date,
                                         rt.tim_sys tran_date,
                                         TRUNC(rt.dat_val) value_date,
                                         rt.cod_tran_cur tcy_code,
                                         (CASE WHEN rt.cod_tran_stat IN(4224, 4736) THEN -rt.amt_tran_tcye ELSE rt.amt_tran_tcye END) txn_amount_tcy,
                                         rt.cod_acct_cur acy_code,
                                         (CASE WHEN rt.cod_tran_stat IN(4224, 4736) THEN -rt.amt_tran_acye ELSE rt.amt_tran_acye END) txn_amount_acy,
                                         (CASE WHEN rt.cod_tran_stat IN(4224, 4736) THEN -rt.amt_tran_lcye ELSE rt.amt_tran_lcye END) txn_amount_lcy,
                                         TRIM(rt.buf_stat_scr) narrative,
                                         (CASE WHEN rt.cod_tran_stat = 32 THEN 'Authorization requested'
                                               WHEN rt.cod_tran_stat IN(33,97) THEN 'Cancelled'
                                               WHEN rt.cod_tran_stat = 96 THEN 'Authorization granted'
                                               WHEN rt.cod_tran_stat IN(128, 768, 640) THEN 'Submitted to host'
                                               WHEN rt.cod_tran_stat IN(4224, 4736) THEN 'Reversed'
                                               ELSE 'Code: ' || rt.cod_tran_stat
                                         END) tran_state,
                                         rt.id_acct account_no,
                                         rt.id_to_acct gl_account_no,
                                         DECODE(INSTR('9009,9010', rt.cod_tran), 0, 'C', 'D') bvt_cr, -- DR for BVT
                                         DECODE(INSTR('9009,9010', rt.cod_tran), 0, 'D', 'C') bvt_db, -- CR for BVT
                                         DECODE(INSTR('3003,3081', rt.cod_tran), 0, 'D', 'C') teller_cr, -- DR for Teller
                                         DECODE(INSTR('3003,3081', rt.cod_tran), 0, 'C', 'D') teller_db -- CR for Teller
                                  FROM v_rec_txnlog rt
                                  WHERE rt.cod_tran_cur != 0
                                        AND rt.cod_tran_stat IN (128, 4224, 768, 640, 4736) -- 128 AUTHORIZED, 4224 - reversed, 768 - unknown (Yalchin biler)
                                        ---
                                        AND DECODE(p_date_type,
                                                   'value_date', TRUNC(rt.dat_val),
                                                   'tran_date', TRUNC(rt.dat_sys),
                                                   'post_date', TRUNC(rt.dat_post)) BETWEEN p_start_date AND p_end_date
                                        AND rt.cod_brn = NVL(p_branch_id, rt.cod_brn))
            SELECT t.*,
                   --bl.*
                   nvl(bl.beginning_balance, 0) beginning_balance
            FROM w_rec_denm bl,
                 (SELECT rt.cod_brn,
                         (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                         (tl.nam_user || ' ( ' || tl.cod_user_id || ' )') user_name,
                         up.cod_userno user_no,
                         tl.cod_userno tl_user_no,
                         rt.post_date,
                         rt.tran_date,
                         rt.value_date,
                         rt.acy_code,
                         c1.nam_ccy_short acy,
                         rt.tcy_code,
                         c2.nam_ccy_short tcy,
                         'AZN' lcy,
                         (TRIM(tm.cod_txn_mnemonic) || ': ' || tm.txt_txn_desc) tran_desc,
                         rt.narrative,
                         rt.tran_state,
                         rt.account_no,
                         rt.gl_account_no,
                         c.cod_cust_id customer_id,
                         DECODE(c.cod_cust_id, NULL, NULL, (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full)) customer_name,
                         DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                         /*rt.txn_amount_tcy,
                         rt.txn_amount_acy,
                         rt.txn_amount_lcy,*/
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_cr, rt.teller_cr)
                               THEN rt.txn_amount_tcy
                               ELSE 0
                          END) tran_amount_tcy_cr,
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_db, rt.teller_db)
                               THEN rt.txn_amount_tcy
                               ELSE 0
                          END) tran_amount_tcy_db,
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_cr, rt.teller_cr)
                               THEN rt.txn_amount_acy
                               ELSE 0
                          END) tran_amount_acy_cr,
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_db, rt.teller_db)
                               THEN rt.txn_amount_acy
                               ELSE 0
                          END) tran_amount_acy_db,
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_cr, rt.teller_cr)
                               THEN rt.txn_amount_lcy
                               ELSE 0
                          END) tran_amount_lcy_cr,
                         (CASE WHEN NVL(tm.cod_drcr, 'C') = DECODE(up.cod_branch_templ, 18, rt.bvt_db, rt.teller_db)
                               THEN rt.txn_amount_lcy
                               ELSE 0
                          END) tran_amount_lcy_db
                  FROM w_rec_txnlog rt,
                       ln_acct_dtls a,
                       ci_custmast c,
                       ba_txn_mnemonic tm,
                       ba_cc_brn_mast bm,
                       sm_user_profile tl,
                       sm_user_profile up,
                       ba_ccy_code c1,
                       ba_ccy_code c2,
                       (SELECT DISTINCT gd.cod_group,
                               gd.nam_group,
                               gd.cod_group_cycle,
                               gd.cod_acct_no
                        FROM ba_group_details gd
                        WHERE gd.flg_group_acc_attr = 'A') g
                  WHERE bm.cod_cc_brn = rt.cod_brn
                        AND tl.cod_userno = rt.user_no
                        AND c1.cod_ccy = rt.acy_code
                        AND c2.cod_ccy = rt.tcy_code
                        --
                        AND rt.cod_tran = tm.cod_txn_mnemonic(+)
                        AND rt.account_no = g.cod_acct_no(+)
                        AND rt.account_no = a.cod_acct_no(+)
                        AND a.cod_cust_id = c.cod_cust_id(+)
                        --
                        AND rt.user_no = DECODE(up.cod_branch_templ, 18, rt.user_no, up.cod_userno)
                        AND INSTR(DECODE(up.cod_branch_templ, 18,
                                         DECODE(rt.user_no, up.cod_userno,
                                                rt.cod_tran,
                                                '9008,9007'),
                                         rt.cod_tran),
                                  rt.cod_tran) > 0
                        --
                        AND up.cod_userno = NVL(p_user_no, up.cod_userno)
                        AND DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS') = NVL(p_txn_mode, DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS'))) t
            WHERE t.cod_brn = bl.cod_brn(+)
                  AND t.tcy_code = bl.ccy(+)
                  AND t.post_date >= bl.post_date(+)
                  AND t.user_no = bl.user_no(+)
            ORDER BY t.tcy,
                     DECODE(p_date_type,
                            'value_date', t.value_date,
                            'tran_date', t.tran_date,
                            'post_date', t.post_date),
                     t.account_no;

       RETURN c_result;
  END;
  -- TL1011 - Teller detailed report (GL)
  -- TL1060 - Daily teller report by GL

  -- TL1020 - Teller aggregated report
  FUNCTION tl1020(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_branch_list VARCHAR2(50) := report_methods.get_region_branches(p_branch_id);
  BEGIN
       OPEN c_result FOR
            WITH w_rec_denm AS (SELECT rd.cod_brn,
                                       rd.user_no,
                                       rd.cod_denm_cur ccy,
                                       rd.dat_post post_date,
                                       SUM(rd.tot_amount) beginning_balance
                                FROM v_rec_denm rd,
                                     (SELECT m.cod_brn,
                                             m.user_no,
                                             m.cod_denm_cur,
                                             MAX(m.dat_post) max_date
                                      FROM v_rec_denm m
                                      WHERE m.dat_post < TRUNC(p_start_date)
                                            --AND m.cod_brn = NVL(p_branch_id, m.cod_brn)
                                            AND INSTR(v_branch_list, m.cod_brn) > 0
                                      GROUP BY m.cod_brn,
                                               m.user_no,
                                               m.cod_denm_cur) md
                                WHERE rd.cod_brn = md.cod_brn
                                      AND rd.user_no = md.user_no
                                      AND rd.cod_denm_cur = md.cod_denm_cur
                                      AND rd.dat_post = md.max_date
                                GROUP BY rd.cod_brn,
                                         rd.user_no,
                                         rd.cod_denm_cur,
                                         rd.dat_post),
                 w_rec_txnlog AS (SELECT xt.cod_cc_brn,
                                         xt.cod_userno,
                                         xt.dat_post_str post_date,
                                         xt.cod_txn_ccy tcy_code,
                                         xt.cod_txn_mnemonic,
                                         SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0)) txn_amount_tcy_cr,
                                         SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0)) txn_amount_tcy_db
                                  FROM xf_ol_st_txnlog_mmdd xt
                                  WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                        --
                                        AND DECODE(p_date_type,
                                                   'value_date', xt.dat_value_str,
                                                   'tran_date', TRUNC(xt.dat_txn_str),
                                                   'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                        --AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                                        AND INSTR(v_branch_list, xt.cod_cc_brn) > 0
                                  GROUP BY xt.cod_cc_brn,
                                           xt.cod_userno,
                                           xt.dat_post_str,
                                           xt.cod_txn_ccy,
                                           xt.cod_txn_mnemonic
                                  UNION
                                  SELECT xt.cod_cc_brn,
                                         xt.cod_userno,
                                         xt.dat_post_str post_date,
                                         xt.cod_txn_ccy tcy_code,
                                         xt.cod_txn_mnemonic,
                                         SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0)) txn_amount_tcy_cr,
                                         SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0)) txn_amount_tcy_db
                                  FROM xf_ol_st_txnlog_current@host_link xt
                                  WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                        --
                                        AND DECODE(p_date_type,
                                                   'value_date', xt.dat_value_str,
                                                   'tran_date', TRUNC(xt.dat_txn_str),
                                                   'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                        --AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                                        AND INSTR(v_branch_list, xt.cod_cc_brn) > 0
                                  GROUP BY xt.cod_cc_brn,
                                           xt.cod_userno,
                                           xt.dat_post_str,
                                           xt.cod_txn_ccy,
                                           xt.cod_txn_mnemonic)
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (tl.nam_user || ' ( ' || tl.cod_user_id || ' )') user_name,
                   t.tcy,
                   NVL(bl.beginning_balance, 0) opening_balance_tcy,
                   NVL(t.txn_amount_tcy_db, 0) amount_tcy_db,
                   NVL(t.txn_amount_tcy_cr, 0) amount_tcy_cr,
                   (NVL(bl.beginning_balance, 0) + NVL(t.txn_amount_tcy_db, 0) - NVL(t.txn_amount_tcy_cr, 0)) closing_balance_tcy
            FROM w_rec_denm bl,
                 ba_cc_brn_mast bm,
                 sm_user_profile tl,
                 (SELECT rt.cod_cc_brn,
                         up.cod_userno user_no,
                         rt.post_date,
                         btc.ccy_code tcy_code,
                         btc.ccy_name tcy,
                         SUM(DECODE(rt.tcy_code, btc.ccy_code, (CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_cr ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_db, rt.txn_amount_tcy_cr) END), 0)) txn_amount_tcy_cr,
                         SUM(DECODE(rt.tcy_code, btc.ccy_code, (CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_db ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_cr, rt.txn_amount_tcy_db) END), 0)) txn_amount_tcy_db
                  FROM w_rec_txnlog rt,
                       ba_txn_mnemonic tm,
                       sm_user_profile up,
                       (SELECT bt.branch_id,
                               bt.user_no,
                               cc.cod_ccy ccy_code,
                               cc.nam_ccy_short ccy_name
                        FROM mv_branch_tellers bt,
                             ba_ccy_code cc) btc,
                       (SELECT rs.setting_value unused_users
                        FROM rpt_report_settings rs
                        WHERE rs.setting_key = 'UNUSED_BVT_USERS') uu
                  WHERE up.cod_cc_brn = rt.cod_cc_brn
                        AND rt.cod_txn_mnemonic = tm.cod_txn_mnemonic(+)
                        AND rt.cod_cc_brn = btc.branch_id
                        AND rt.cod_userno = btc.user_no
                        --
                        AND rt.cod_userno = DECODE(up.cod_branch_templ, 18, rt.cod_userno, up.cod_userno)
                        AND INSTR(DECODE(up.cod_branch_templ, 18,
                                         DECODE(rt.cod_userno, up.cod_userno,
                                                TRIM(rt.cod_txn_mnemonic),
                                                '9007,9008'),
                                         TRIM(rt.cod_txn_mnemonic)),
                                  TRIM(rt.cod_txn_mnemonic)) > 0
                        AND INSTR(uu.unused_users, up.cod_user_id) = 0
                        --
                        AND DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS') = NVL(p_txn_mode, DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS'))
                  GROUP BY rt.cod_cc_brn,
                           up.cod_userno,
                           rt.post_date,
                           btc.ccy_code,
                           btc.ccy_name) t
            WHERE t.cod_cc_brn = bl.cod_brn(+)
                  AND t.tcy_code = bl.ccy(+)
                  AND t.post_date >= bl.post_date(+)
                  AND t.user_no = bl.user_no(+)
                  AND bm.cod_cc_brn = t.cod_cc_brn
                  AND tl.cod_userno = t.user_no
            ORDER BY 1, 2, 3;

       RETURN c_result;
  END;
  -- TL1020 - Teller aggregated report

  -- TL1030 - Incomplete transactions report
  FUNCTION tl1030_txn_pending_for_auth(p_branch_id NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN -- Transaction pending for authorization user wise
       OPEN c_result FOR
            SELECT (trim(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   (trim(up.nam_user) || ' (' || trim(up.cod_user_id) || ')') user_name,
                   --rb.ctr_batch_no batch_no,
                   (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)) txn_desc,
                   rt.dat_post posting_date,
                   rt.dat_val value_date,
                   ra.txn_amount,
                   TRIM(cc.nam_ccy_short) tcy
            FROM rec_auth@brn_link ra,
                 dl_ba_txn_mnemonic@brn_link tm,
                 rec_txnlog@brn_link rt,
                 rec_bctl@brn_link rb,
                 dl_vw_sm_user_profile@brn_link up,
                 dl_ba_cc_brn_mast@brn_link bm,
                 dl_ba_ccy_code@brn_link cc
            WHERE tm.cod_txn_mnemonic = ra.cod_tran
                  AND rt.id_tran_seq = ra.id_tran_seq
                  AND rt.user_no = ra.user_no
                  AND trunc(rt.dat_post) = trunc(ra.dat_post)
                  AND rb.user_no = ra.user_no
                  AND trunc(rb.dat_post) = trunc(ra.dat_post)
                  AND up.cod_userno = ra.user_no
                  AND rt.cod_brn = bm.cod_cc_brn
                  AND cc.cod_ccy = rt.cod_tran_cur
                  AND ra.flg_auth_stat = 0
                  AND rb.flg_bat_stat = 1
                  --
                  AND rt.cod_brn = nvl(p_branch_id, rt.cod_brn)
            ORDER BY TRIM(bm.nam_branch),
                     2;

       RETURN c_result;
  END;
  
  FUNCTION tl1030_txn_pending_for_subm(p_branch_id NUMBER)
  RETURN result_cursor
  IS -- Authorized transactions pending for submission by users
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (trim(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   (trim(up.nam_user) || ' (' || trim(up.cod_user_id) || ')') user_name,
                   --rt.id_tran_seq,
                   (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)) txn_desc,
                   rt.dat_post posting_date,
                   rt.dat_val value_date,
                   --rt.cod_tran_stat,
                   rt.amt_tran_tcye txn_amount,
                   TRIM(cc.nam_ccy_short) tcy
            FROM rec_txnlog@brn_link rt,
                 dl_ba_txn_mnemonic@brn_link tm,
                 dl_ba_cc_brn_mast@brn_link bm,
                 dl_vw_sm_user_profile@brn_link up,
                 dl_ba_ccy_code@brn_link cc
            WHERE tm.cod_txn_mnemonic = rt.cod_tran
                  AND bm.cod_cc_brn = rt.cod_brn
                  AND up.cod_userno = rt.user_no
                  AND cc.cod_ccy = rt.cod_tran_cur
                  AND rt.id_tran_seq BETWEEN 1 AND 9999
                  AND rt.cod_tran_stat = 96
                  --
                  AND rt.cod_brn = nvl(p_branch_id, rt.cod_brn)
            ORDER BY TRIM(bm.nam_branch),
                     2;

       RETURN c_result;
  END;
  
  FUNCTION tl1030_txn_unprocessed(p_branch_id NUMBER)
  RETURN result_cursor
  IS --Unprocessed Transactions through GLM01
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   (TRIM(up.nam_user) || ' (' || TRIM(up.cod_user_id) || ')') user_name,
                   gb.ctr_batch_no batch_no,
                   gb.dat_phy_batch_open batch_open_date,
                   gb.dat_phy_batch_clos batch_close_date,
                   gb.amt_tot_dr_txns total_amount_dr,
                   gb.amt_tot_cr_txns total_amount_cr,
                   TRIM(cc.nam_ccy_short) tcy,
                   DECODE(gb.flg_batch, 'O', 'Open', 'U', 'In Use', 'C', 'Checked', 'P', 'Processed') batch_status
            FROM gl_batctrl@host_link gb,
                 dl_ba_cc_brn_mast@brn_link bm,
                 dl_vw_sm_user_profile@brn_link up,
                 dl_ba_ccy_code@brn_link cc
            WHERE bm.cod_cc_brn = gb.cod_cc_brn
                  AND up.cod_userno = gb.cod_userno
                  AND gb.cod_ccy_batch = cc.cod_ccy
                  AND gb.flg_type = 'V'
                  AND gb.flg_batch NOT IN('P', 'C')
                  AND (gb.ctr_cr_txns + gb.ctr_dr_txns) > 0
                  --
                  AND gb.cod_cc_brn = NVL(p_branch_id, gb.cod_cc_brn);

       RETURN c_result;
  END;
  -- TL1030 - Incomplete transactions report
  
  -- TL1040 - Batch status inquiry
  FUNCTION tl1040(p_branch_id NUMBER,
                  p_date DATE,
                  p_batch_status NUMBER,
                  p_batch_type NUMBER)
  RETURN result_cursor
  IS --To check the Batch status (Branch, Vault and Teller Batch)
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   (TRIM(up.nam_user) || ' (' || TRIM(up.cod_user_id) || ')') user_name,
                   rb.ctr_batch_no batch_no,
                   trunc(rb.dat_post) posting_date,
                   (TRUNC(rb.dat_bat_opn) + (rb.tim_bat_opn - TRUNC(rb.tim_bat_opn))) batch_open_date,
                   (CASE WHEN rb.dat_bat_cls < rb.dat_bat_opn
                         THEN NULL
                         ELSE (TRUNC(rb.dat_bat_cls) + (rb.tim_bat_cls - TRUNC(rb.tim_bat_cls)))
                    END) batch_close_date,
                   DECODE(rb.flg_bat_stat, 1, 'Open', 'Closed') batch_status,
                   DECODE(rb.flg_bat_type, 1, 'Branch', 2, 'Vault Batch', 3, 'Teller Batch') batch_type/*,
                   rb.**/
            FROM rec_bctl@brn_link rb,
                 dl_vw_sm_user_profile@brn_link up,
                 dl_ba_cc_brn_mast@brn_link bm
            WHERE rb.user_no = up.cod_userno
                  AND up.cod_cc_brn = bm.cod_cc_brn
                  --AND rb.flg_bat_stat != 0  -- If u want to only see open batch then remove the comment
                  --AND rb.flg_bat_stat != 1  -- If u want to only see close batch then remove the comment
                  --
                  AND rb.dat_post >= p_date
                  AND rb.cod_brn = nvl(p_branch_id, rb.cod_brn)
                  AND rb.flg_bat_stat = nvl(p_batch_status, rb.flg_bat_stat)
                  AND rb.flg_bat_type = nvl(p_batch_type, rb.flg_bat_type)
            ORDER BY rb.dat_post DESC,
                     TRIM(bm.nam_branch),
                     rb.flg_bat_type;

       RETURN c_result;
  END;
  -- TL1040 - Batch status inquiry
  
  -- TL1050 - LNM35 screen transactions
  FUNCTION tl1050(p_start_date DATE,
                  p_end_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT --rl.cod_task task_code,
                   --rl.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   rl.dat_txn txn_date,
                   rl.cod_user_id user_id,
                   tu.nam_user user_name,
                   rl.cod_auth_id author_id,
                   au.nam_user author_name,
                   rl.cod_acct_no account_no,
                   rl.cod_cust customer_id,
                   fl.cod_field_name field_name,
                   DECODE(fl.cod_field_name,
                          'COD_ACCT_OFF', 'Credit Officer',
                          'FLG_LEGAL', 'Legal Dept. Transfer',
                          'FLG_NEGOTIATED', 'Restructured Loan') txn_type,
                   fl.old_field_value old_value,
                   fl.new_field_value new_value,
                   ou.cod_user_id old_user_id,
                   ou.nam_user old_user_name,
                   nu.cod_user_id new_user_id,
                   nu.nam_user new_user_name
            FROM ba_change_record_log@host_link rl,
                 ba_change_field_log@host_link fl,
                 sm_user_profile@host_link ou,
                 sm_user_profile@host_link nu,
                 sm_user_profile@host_link tu,
                 sm_user_profile@host_link au,
                 rpt_ba_cc_brn_mast@host_link bm
            WHERE rl.ctr_change_no = fl.ctr_change_no
                  AND TRIM(ou.cod_userno(+)) = TRIM(fl.old_field_value)
                  AND TRIM(nu.cod_userno(+)) = TRIM(fl.new_field_value)
                  AND TRIM(tu.cod_user_id) = TRIM(rl.cod_user_id)
                  AND TRIM(au.cod_user_id) = TRIM(rl.cod_auth_id)
                  AND bm.cod_cc_brn = rl.cod_cc_brn
                  AND rl.cod_task = 'LNM35'
                  AND fl.cod_field_name IN('COD_ACCT_OFF', 'FLG_LEGAL', 'FLG_NEGOTIATED')
                  AND ou.flg_mnt_status(+) = 'A'
                  AND nu.flg_mnt_status(+) = 'A'
                  AND tu.flg_mnt_status = 'A'
                  AND au.flg_mnt_status = 'A'
                  --
                  AND TRUNC(rl.dat_txn) BETWEEN p_start_date AND p_end_date
            ORDER BY rl.dat_txn DESC;

       RETURN c_result;
  END;
  -- TL1050 - LNM35 screen transactions

  -- TL1070 - Teller batch operations (open-close)
  FUNCTION tl1070(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT t.branch_name,
                   (up.nam_user || ' (' || up.cod_user_id || ')') user_name,
                   t.txn_date,
                   t.txn_desc,
                   t.txn_time,
                   t.txn_count
            FROM sm_user_profile up,
                 (SELECT (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(nam_branch)) branch_name,
                         rt.user_no,
                         TRUNC(rt.tim_sys) txn_date,
                         (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)) txn_desc,
                         TO_CHAR(MAX(rt.tim_sys), 'hh24:mi:ss') txn_time,
                         COUNT(*) txn_count
                  FROM v_rec_txnlog rt,
                       ba_cc_brn_mast bm,
                       ba_txn_mnemonic tm
                  WHERE bm.cod_cc_brn = rt.cod_brn
                        AND tm.cod_txn_mnemonic = rt.cod_tran
                        --
                        AND rt.cod_tran IN(9001,9003,9005,9006)
                        --
                        AND rt.dat_sys BETWEEN p_start_date AND p_end_date
                        AND rt.cod_brn = nvl(p_branch_id, rt.cod_brn)
                  GROUP BY (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(nam_branch)),
                           (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)),
                           rt.user_no,
                           TRUNC(rt.tim_sys)) t 
            WHERE up.cod_userno = t.user_no
            ORDER BY 1, 2, 3, 4 DESC;

       RETURN c_result;
  END;
  -- TL1070 - Teller batch operations (open-close)

  -- TL1080 - Incorrect transactions
  FUNCTION tl1080(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2,
                  p_txn_mode VARCHAR2,
                  p_txn_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_inc_txn AS (SELECT xt.cod_cc_brn,
                                      xt.cod_txn_mnemonic,
                                      xt.cod_userno,
                                      xt.cod_auth_id author_id,
                                      xt.ref_usr_no user_ref_no,
                                      xt.dat_post_str post_date,
                                      xt.dat_txn_str txn_date,
                                      xt.dat_value_str value_date,
                                      xt.cod_txn_ccy tcy_code,
                                      DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0) txn_amount_tcy_cr,
                                      DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0) txn_amount_tcy_db,
                                      xt.cod_acc_ccy acy_code,
                                      DECODE(xt.flg_drcr, 'C', xt.amt_txn_acy, 0) txn_amount_acy_cr,
                                      DECODE(xt.flg_drcr, 'D', xt.amt_txn_acy, 0) txn_amount_acy_db,
                                      SUBSTR(xt.txn_nrrtv, INSTR(xt.txn_nrrtv, ':', 1)) narrative,
                                      SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1) account_no,
                                      SUBSTR(xt.ref_sys_tr_aud_no, 1, LENGTH(xt.ref_sys_tr_aud_no)-2) txn_no,
                                      (CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) txn_state,
                                      (CASE WHEN DECODE(xt.cod_txn_ccy, xt.cod_acc_ccy, 0, DECODE(xt.cod_acc_ccy, 840, ROUND(xt.amt_txn_tcy / xt.rat_conv_aclcy, 2), 21)) > DECODE(p_txn_type, 'XA', 0, 20) AND xt.cod_txn_mnemonic NOT IN(9007,9008,9009,9010) THEN 'XA' ELSE '' END) cur_change,
                                      (CASE WHEN xt.cod_txn_mnemonic IN(1060,1460,9009,9010) THEN (CASE WHEN xt.cod_txn_mnemonic IN(1060,9010) THEN 'CO' ELSE 'C' END) ELSE '' END) cash_type,
                                      (CASE WHEN to_char(xt.dat_txn_str, 'hh24mi') > 1705 THEN 'D' ELSE '' END) delayed_txn
                               FROM xf_ol_st_txnlog_mmdd xt
                               WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                     --
                                     AND DECODE(p_date_type,
                                                'value_date', xt.dat_value_str,
                                                'tran_date', TRUNC(xt.dat_txn_str),
                                                'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                     AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                               --
                               UNION
                               --
                               SELECT xt.cod_cc_brn,
                                      xt.cod_txn_mnemonic,
                                      xt.cod_userno,
                                      xt.cod_auth_id author_id,
                                      xt.ref_usr_no user_ref_no,
                                      xt.dat_post_str post_date,
                                      xt.dat_txn_str txn_date,
                                      xt.dat_value_str value_date,
                                      xt.cod_txn_ccy tcy_code,
                                      DECODE(xt.flg_drcr, 'C', xt.amt_txn_tcy, 0) txn_amount_tcy_cr,
                                      DECODE(xt.flg_drcr, 'D', xt.amt_txn_tcy, 0) txn_amount_tcy_db,
                                      xt.cod_acc_ccy acy_code,
                                      DECODE(xt.flg_drcr, 'C', xt.amt_txn_acy, 0) txn_amount_acy_cr,
                                      DECODE(xt.flg_drcr, 'D', xt.amt_txn_acy, 0) txn_amount_acy_db,
                                      SUBSTR(xt.txn_nrrtv, INSTR(xt.txn_nrrtv, ':', 1)) narrative,
                                      SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1) account_no,
                                      SUBSTR(xt.ref_sys_tr_aud_no, 1, LENGTH(xt.ref_sys_tr_aud_no)-2) txn_no,
                                      (CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) txn_state,
                                      (CASE WHEN DECODE(xt.cod_txn_ccy, xt.cod_acc_ccy, 0, DECODE(xt.cod_acc_ccy, 840, ROUND(xt.amt_txn_tcy / xt.rat_conv_aclcy, 2), 21)) > DECODE(p_txn_type, 'XA', 0, 20) AND xt.cod_txn_mnemonic NOT IN(9007,9008,9009,9010) THEN 'XA' ELSE '' END) cur_change,
                                      (CASE WHEN xt.cod_txn_mnemonic IN(1060,1460,9009,9010) THEN (CASE WHEN xt.cod_txn_mnemonic IN(1060,9010) THEN 'CO' ELSE 'C' END) ELSE '' END) cash_type,
                                      (CASE WHEN to_char(xt.dat_txn_str, 'hh24mi') > 1705 THEN 'D' ELSE '' END) delayed_txn
                               FROM xf_ol_st_txnlog_current@host_link xt
                               WHERE xt.ref_subseq_no = (CASE WHEN xt.cod_txn_mnemonic IN(9007,9008,9009,9010) THEN 0 ELSE 1 END)
                                     --
                                     AND DECODE(p_date_type,
                                                'value_date', xt.dat_value_str,
                                                'tran_date', TRUNC(xt.dat_txn_str),
                                                'post_date', xt.dat_post_str) BETWEEN p_start_date AND p_end_date
                                     AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn))
            SELECT rt.cod_cc_brn,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (tl.nam_user || ' ( ' || tl.cod_user_id || ' )') user_name,
                   up.cod_userno user_no,
                   tl.cod_userno tl_user_no,
                   rt.author_id,
                   rt.post_date,
                   rt.txn_date,
                   rt.value_date,
                   rt.tcy_code,
                   rt.acy_code,
                   cc1.nam_ccy_short tcy,
                   cc2.nam_ccy_short acy,
                   (TRIM(tm.cod_txn_mnemonic) || ': ' || tm.txt_txn_desc) tran_desc,
                   rt.narrative,
                   DECODE(rt.txn_state, 1, 'Submitted', 'Reversed') txn_state,
                   rt.account_no,
                   rt.txn_no,
                   rt.user_ref_no,
                   --c.cod_cust_id customer_id,
                   DECODE(c.cod_cust_id, NULL, NULL, (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full)) customer_name,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_cr ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_db, rt.txn_amount_tcy_cr) END) * rt.txn_state) txn_amount_tcy_cr,
                   ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_tcy_db ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_tcy_cr, rt.txn_amount_tcy_db) END) * rt.txn_state) txn_amount_tcy_db,
                   ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_acy_cr ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_acy_db, rt.txn_amount_acy_cr) END) * rt.txn_state) txn_amount_acy_cr,
                   ((CASE WHEN rt.cod_txn_mnemonic IN (9009,9010) THEN rt.txn_amount_acy_db ELSE DECODE(up.cod_branch_templ, 18, rt.txn_amount_acy_cr, rt.txn_amount_acy_db) END) * rt.txn_state) txn_amount_acy_db
            FROM w_inc_txn rt,
                 ln_acct_dtls a,
                 ci_custmast c,
                 ba_txn_mnemonic tm,
                 ba_cc_brn_mast bm,
                 sm_user_profile tl,
                 sm_user_profile up,
                 ba_ccy_code cc1,
                 ba_ccy_code cc2,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g
            WHERE bm.cod_cc_brn = rt.cod_cc_brn
                  AND tl.cod_userno = rt.cod_userno
                  AND cc1.cod_ccy = rt.tcy_code
                  AND cc2.cod_ccy = rt.acy_code
                  --
                  AND rt.cod_txn_mnemonic = tm.cod_txn_mnemonic(+)
                  AND rt.account_no = g.cod_acct_no(+)
                  AND rt.account_no = a.cod_acct_no(+)
                  AND a.cod_cust_id = c.cod_cust_id(+)
                  --
                  AND up.cod_cc_brn = rt.cod_cc_brn
                  --
                  AND rt.cod_userno = DECODE(up.cod_branch_templ, 18, rt.cod_userno, up.cod_userno)
                  AND INSTR(DECODE(up.cod_branch_templ, 18,
                                   DECODE(rt.cod_userno, up.cod_userno,
                                          TRIM(rt.cod_txn_mnemonic),
                                          '9007,9008'),
                                   TRIM(rt.cod_txn_mnemonic)),
                            TRIM(rt.cod_txn_mnemonic)) > 0
                  --
                  AND up.cod_userno = NVL(p_user_no, up.cod_userno)
                  AND DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS') = NVL(p_txn_mode, DECODE(tm.cod_txn_mode, 'XFER', 'GL', 'CS'))
                  AND INSTR(rt.cur_change || DECODE(rt.txn_state, -1, 'R', '') || rt.cash_type || rt.delayed_txn, p_txn_type) > 0
            ORDER BY tcy,
                     DECODE(p_date_type,
                            'value_date', rt.value_date,
                            'tran_date', rt.txn_date,
                            'post_date', rt.post_date),
                     group_name,
										           account_no;

       RETURN c_result;
  END;
  -- TL1080 - Incorrect transactions

END;
/
