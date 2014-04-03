CREATE OR REPLACE PACKAGE data_collector
IS

  -- Author  : ELDANIZ
  -- Created : 09.06.2011 16:26:28
  -- Purpose :

  PROCEDURE refresh_dtl_po;
  PROCEDURE refresh_dtl_po_for_date(p_date DATE);
  PROCEDURE refresh_rpt_gl_totals;
  PROCEDURE refresh_rpt_ln_totals;

END;
/
CREATE OR REPLACE PACKAGE BODY data_collector
IS
  
  PROCEDURE refresh_dtl_po
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0)) "ln_daily_txn_log_hist",
                        SUM(DECODE(t.table_name, 'rpt_ln_daily_txn_log_po', t.cnt, 0)) "rpt_ln_daily_txn_log_po",
                        (SUM(DECODE(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0)) - 
                         SUM(DECODE(t.table_name, 'rpt_ln_daily_txn_log_po', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_daily_txn_log_hist' table_name,
                              TRUNC(tl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_daily_txn_log_hist tl,
                            ln_prod_mast pm
                       WHERE tl.cod_prod = pm.cod_prod
                             AND tl.cod_gl IN (pm.cod_gl_ast_bal, pm.cod_gl_susp_ast_bal)
                             AND tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(tl.dat_txn)
                       UNION ALL
                       SELECT 'rpt_ln_daily_txn_log_po' table_name,
                              TRUNC(tl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM rpt_ln_daily_txn_log_po tl
                       WHERE tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(tl.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0)) - 
                         SUM(DECODE(t.table_name, 'rpt_ln_daily_txn_log_po', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_collector.refresh_dtl_po_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_dtl_po_for_date(p_date DATE)
  IS
    -- rpt_ln_daily_txn_log_po for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'data_collector.refresh_dtl_po_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_ln_daily_txn_log_po dtl
       WHERE trunc(dtl.dat_txn) = trunc(p_date);
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';
       
       /*log_manager.add_log('MIG_FD', -- for date
                           v_proc_name,
                           v_params,
                           v_started,
                           SYSDATE,
                           'delete succeed; ' || v_row_count_processed,
                           0);*/

       --v_started := SYSDATE;
       INSERT INTO rpt_ln_daily_txn_log_po
       SELECT TRIM(tl.cod_acct_no) cod_acct_no,
              tl.cod_userno,
              tl.cod_txn_mnemonic,
              tl.cod_txn_type,
              tl.amt_txn_acy,
              tl.cod_ccy_txn,
              tl.amt_txn_tcy,
              tl.amt_txn_lcy,
              tl.dat_value,
              tl.dat_txn,
              tl.dat_post,
              tl.dat_process,
              tl.flg_drcr,
              tl.cod_prod,
              tl.cod_cc_brn_txn,
              tl.ref_txn_no,
              tl.cod_gl,
              TRIM(tl.txt_txn_desc) txt_txn_desc
       FROM ln_daily_txn_log_hist tl,
            ln_prod_mast pm
       WHERE tl.cod_prod = pm.cod_prod
             AND tl.cod_gl IN (pm.cod_gl_ast_bal, pm.cod_gl_susp_ast_bal)
             AND trunc(tl.dat_txn) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || TRIM(SQL%ROWCOUNT) || ' rows inserted';
       
       log_manager.add_log('MIG_FD',
                           v_proc_name,
                           v_params,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_FD_ERR',
                                       v_proc_name,
                                       v_params,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;
  
  PROCEDURE refresh_rpt_gl_totals
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'data_collector.refresh_rpt_gl_totals';
    v_params VARCHAR2(100);
    v_row_count_processed VARCHAR2(50) := '';
    v_date DATE := (CASE WHEN TRUNC(SYSDATE) = TRUNC(SYSDATE, 'MONTH') THEN to_date('200901', 'yyyymm') ELSE TRUNC(ADD_MONTHS(SYSDATE, (CASE WHEN TO_NUMBER(TO_CHAR(SYSDATE, 'mm')) < 4 THEN -4 ELSE 0 END)), 'YEAR') END);
  BEGIN
       v_params := 'v_date (from date) = ' || to_char(v_date, 'yyyy.mm.dd');

       v_started := SYSDATE;
       DELETE FROM rpt_gl_totals gt WHERE gt.year_month >= to_char(v_date, 'yyyymm');
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_gl_totals
       SELECT to_char(ah.value_dt, 'yyyymm') year_month,
              --ah.ac_branch branch_id,
              (CASE WHEN ah.value_dt < to_date('201201', 'yyyymm') THEN ah.ac_branch ELSE NVL(TRIM(up.cod_cc_brn), ah.ac_branch) END) branch_id,
              ah.ac_no gl_account_no,
              ah.ac_ccy ccy,
              SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) amount_lcy
       FROM actb_history ah,
            gltm_glmaster gg,
            ln_acct_attributes aa,
            sm_user_profile up
       WHERE gg.gl_code = ah.ac_no
             AND ah.related_account = aa.cod_acct_no(+)
             AND aa.cod_acct_off = up.cod_userno(+)
             --
             AND gg.category IN (3, 4)
             AND gg.leaf = 'Y'
             --
             AND ah.value_dt BETWEEN v_date AND TRUNC(SYSDATE, 'MONTH')-1
             AND TRIM(ah.txt_txn_desc) != 'YEAR END'
       GROUP BY to_char(ah.value_dt, 'yyyymm'),
                --ah.ac_branch,
                (CASE WHEN ah.value_dt < to_date('201201', 'yyyymm') THEN ah.ac_branch ELSE NVL(TRIM(up.cod_cc_brn), ah.ac_branch) END),
                ah.ac_no,
                ah.ac_ccy
       HAVING SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) != 0
       ORDER BY 1, 2, 3, 4;
       v_row_count_processed := v_row_count_processed || '; ' || TRIM(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           v_params,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_ERR',
                                       v_proc_name,
                                       v_params,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;
  
  -- portfolio outstanding by month
  PROCEDURE refresh_rpt_ln_totals
  IS
    v_start_date DATE := to_date('20090831', 'yyyymmdd');
    v_end_date DATE := TRUNC(SYSDATE);
    CURSOR c_dates IS
           SELECT add_months(to_date(to_char(v_start_date, 'yyyymm'), 'yyyymm') - 1, LEVEL) rep_month
           FROM dual
           CONNECT BY LEVEL < months_between(add_months(v_end_date, 1), to_date(to_char(v_start_date, 'yyyymm'), 'yyyymm') - 1);
    
    CURSOR c_po_for_month(v_date DATE) IS
           SELECT up.cod_cc_brn branch_id,
                  TRIM(to_char(v_date, 'yyyymm')) ym,
                  COUNT(DISTINCT up.cod_userno) co_count,
                  SUM(COUNT(DISTINCT up.cod_userno)) OVER (ORDER BY NULL) co_count_total,
                  COUNT(DISTINCT a.cod_cust_id) active_clients,
                  SUM(COUNT(DISTINCT a.cod_cust_id)) OVER (ORDER BY NULL) active_clients_total,
                  ROUND(SUM((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate), 6) po_usd,
                  SUM(ROUND(SUM((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate), 6)) OVER (ORDER BY NULL) po_usd_total
           FROM ln_acct_dtls a,
                ln_acct_attributes aa,
                ln_acct_balances ab,
                ac_acct_writeoff wo,
                sm_user_profile up,
                conv_ln_acct_balances cab,
                conv_ln_acct_int_balance_dtls caib,
                (SELECT t1.currency_no,
                        TRIM(bc.nam_ccy_short) currency_name,
                        ROUND(t1.currency_rate/t2.usd_rate, 4) currency_rate
                 FROM (SELECT cr.cod_ccy currency_no,
                              cr.rat_ccy_cb currency_rate
                       FROM ba_ccy_rate cr,
                            (SELECT cc.cod_ccy,
                                    MAX(cc.dat_tim_rate_eff) max_date
                             FROM ba_ccy_rate cc
                             WHERE cc.dat_tim_rate_eff <= v_date
                             GROUP BY cc.cod_ccy) t
                       WHERE cr.cod_ccy = t.cod_ccy
                             AND cr.dat_tim_rate_eff = t.max_date) t1,
                      (SELECT cr.cod_ccy currency_no,
                              cr.rat_ccy_cb usd_rate
                       FROM ba_ccy_rate cr,
                            (SELECT cc.cod_ccy,
                                    MAX(cc.dat_tim_rate_eff) max_date
                             FROM ba_ccy_rate cc
                             WHERE cc.dat_tim_rate_eff <= v_date
                                   AND cc.cod_ccy = 840
                             GROUP BY cc.cod_ccy) t
                       WHERE cr.cod_ccy = t.cod_ccy
                             AND cr.dat_tim_rate_eff = t.max_date) t2,
                      ba_ccy_code bc
                  WHERE t1.currency_no = bc.cod_ccy) cru,
                --
                (SELECT lc.account_no,
                        lc.change_date,
                        lc.user_no_old co_user_no
                 FROM mv_co_loan_changes lc,
                      (SELECT clc.account_no,
                              MIN(clc.change_date) min_date
                       FROM mv_co_loan_changes clc
                       WHERE clc.change_date > v_date
                       GROUP BY clc.account_no) lmd
                 WHERE lc.account_no = lmd.account_no
                       AND lc.change_date = lmd.min_date) mlc,
                (SELECT dtl.cod_acct_no,
                        NVL(SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)), 0) principal_due
                 FROM rpt_ln_daily_txn_log_po dtl
                 WHERE dtl.dat_value <= v_date
                 GROUP BY dtl.cod_acct_no) tl
           WHERE a.cod_acct_no = ab.cod_acct_no
                 AND a.cod_acct_no = aa.cod_acct_no
                 AND a.cod_acct_no = tl.cod_acct_no
                 AND a.cod_acct_no = wo.cod_acct_no(+)
                 AND a.cod_acct_no = cab.cod_acct_no(+)
                 AND a.cod_acct_no = caib.cod_acct_no(+)
                 AND a.cod_acct_no = mlc.account_no(+)
                 AND cru.currency_no = a.cod_ccy
                 AND up.cod_userno = nvl(mlc.co_user_no, aa.cod_acct_off)
                 AND ab.amt_disbursed > 0
                 AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                 --
                 --AND NVL(a.dat_acct_close, v_date + 1) > v_date
                 --AND NVL(wo.dat_writeoff, v_date + 1) > v_date
                 --AND a.dat_first_disb <= v_date
           GROUP BY up.cod_cc_brn;
  BEGIN
       FOR dt IN c_dates
       LOOP
           FOR i IN c_po_for_month(dt.rep_month)
           LOOP
               --PIPE ROW(i);
               NULL;
           END LOOP;
           --dbms_output.put_line(to_char(dt.rep_month, 'yyyy.mm.dd'));
       END LOOP;
       
       --RETURN;
  END;

END;
/
