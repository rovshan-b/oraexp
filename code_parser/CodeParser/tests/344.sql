CREATE OR REPLACE PACKAGE reports_ln
IS

  -- Author  : ELDANIZ
  -- Created : 19.01.2009 16:26:28
  -- Purpose :

  TYPE result_cursor IS REF CURSOR;

  -- LNU100 - Account statement for UAT
  FUNCTION get_uat_account_statement(p_account_no VARCHAR2) RETURN result_cursor;

  -- LN1010 - Loan account statement (migrated)
  FUNCTION ln1010(p_account_no VARCHAR2) RETURN result_cursor;
  
  -- LN1011 - LNM10 Loan account details
  FUNCTION ln1011_account_details(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION ln1011_arrear_totals(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION ln1011_arrears_inquiry(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION ln1011_account_ledger_inquiry(p_account_no VARCHAR2) RETURN result_cursor;
  -- LN1011 - LNM10 Loan account details

  -- LN1020 - Loan account statement
  FUNCTION ln1020_account_summary(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor;
  FUNCTION ln1020_account_balance(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor;
  FUNCTION ln1020_payment_schedule(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor;
  FUNCTION ln1020_payments_made(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor;
  FUNCTION ln1020_posted_txn(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor;
  FUNCTION ln1020_not_posted_txn(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor;
  FUNCTION ln1020_mig_balance(p_account_no VARCHAR2)
  RETURN result_cursor;
  -- LN1020 - Loan account statement
  
  -- LN1021 - Group account statement
  FUNCTION ln1021_group_summary(p_group_id VARCHAR2,
                                p_group_cycle NUMBER)
  RETURN result_cursor;
  FUNCTION ln1021_group_members(p_group_id VARCHAR2,
                                p_group_cycle NUMBER)
  RETURN result_cursor;
  FUNCTION ln1021_group_mig_balance(p_group_id VARCHAR2,
                                    p_group_cycle NUMBER)
  RETURN result_cursor;
  FUNCTION ln1021_group_payment_schedule(p_group_id VARCHAR2,
                                         p_group_cycle NUMBER)
  RETURN result_cursor;
  FUNCTION ln1021_group_payments_made(p_group_id VARCHAR2,
                                      p_group_cycle NUMBER,
                                      p_date DATE,
                                      p_date_type VARCHAR2)
  RETURN result_cursor;
  FUNCTION ln1021_not_posted_txn(p_group_id VARCHAR2,
                                 p_group_cycle NUMBER,
                                 p_date DATE,
                                 p_date_type VARCHAR2)
  RETURN result_cursor;
  FUNCTION ln1021_group_balance(p_group_id VARCHAR2,
                                p_group_cycle NUMBER,
                                p_date DATE)
  RETURN result_cursor;
  -- LN1021 - Group account statement

  -- LN1030 - Loan disbursement detailed report
  -- LN1040 - Loan disbursement aggregated report
  FUNCTION ln1030(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_report_ccy VARCHAR2 := 'AZN')
  RETURN result_cursor;

  -- LN1031 - Loans pending branch disbursement report
  FUNCTION ln1031(p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1050 - Loan repayment detailed report
  -- LN1060 - Loan repayment aggregated report
  FUNCTION ln1050(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- LN1070 - Loan repayment projection detailed report
  FUNCTION ln1070(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1071 - Loan repayment projection detailed report (accounts)
  FUNCTION ln1071(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1072 - Repayment projection for future funds flow
  FUNCTION ln1072(p_date DATE) RETURN result_cursor;

  -- LN1090 - Loan portfolio outstanding detailed report
  FUNCTION ln1090(p_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_report_ccy VARCHAR2 := 'AZN',
                  p_fields VARCHAR2 := 'X')
  RETURN result_cursor;

  -- LN1091 - PO for autoloan
  FUNCTION ln1091(p_date DATE,
                  p_product_id NUMBER)
  RETURN result_cursor;

  -- LN1110 - Aging of arrears
  FUNCTION ln1110(p_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1120 - Pre-aggregated loan portfolio outstanding report
  FUNCTION ln1120(p_branch_id NUMBER,
                  p_product_id NUMBER)
  RETURN result_cursor;

  -- LN1130 - Non payment report
  FUNCTION ln1130(p_date DATE,
                  p_branch_id NUMBER,
                  p_account_no VARCHAR2 := NULL)
  RETURN result_cursor;

  -- LN1131 - Next payment report
  FUNCTION ln1131(p_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1140 - Disbursement monitoring report
  FUNCTION ln1140(p_account_no VARCHAR2,
                  p_group_id VARCHAR2,
                  p_group_cycle NUMBER)
  RETURN result_cursor;

  -- LN1150 - Repayment monitoring report
  FUNCTION ln1150(p_account_no VARCHAR2,
                  p_group_id VARCHAR2,
                  p_group_cycle NUMBER)
  RETURN result_cursor;

  -- LN1160 - List of accounts with suspended status
  FUNCTION ln1160 RETURN result_cursor;

  -- LN1170 - Fee payment report
  FUNCTION ln1170(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER)
  RETURN result_cursor;

  -- LN1180 - Legal department clients
  FUNCTION ln1180(p_date DATE,
                  p_branch_id NUMBER,
                  p_account_no VARCHAR2,
                  p_show_group_members CHAR := 'N')
  RETURN result_cursor;
  
  -- LN1200 - Write off loans
  FUNCTION ln1200(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER)
  RETURN result_cursor;
  
  -- LN1210 - Portfolio tendency
  FUNCTION ln1210(p_branch_id NUMBER,
                  p_date_list VARCHAR2,
                  p_ar_in_days_from NUMBER := 0,
                  p_ar_in_days_to NUMBER := 10000)
  RETURN result_cursor;
  
  -- LN1220 - Reserve user transactions
  FUNCTION ln1220(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- LN1230 - Rotation - Client transfer form
  FUNCTION ln1230(p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;

  -- LN1240 - Collateral information for active loans
  FUNCTION ln1240(p_branch_id NUMBER,
                  p_date DATE)
  RETURN data_types.result_cursor;

  -- LN1250 - Active loans with amounts greater than 20000 AZN
  FUNCTION ln1250 RETURN data_types.result_cursor;

  -- LN1260 - Active loans without disbursement
  FUNCTION ln1260(p_start_date DATE,
                  p_end_date DATE)
  RETURN data_types.result_cursor;

  -- LN1270 - Expired loans for archive department
  FUNCTION ln1270(p_branch_id NUMBER,
                  p_year VARCHAR2)
  RETURN data_types.result_cursor;
  
  -- portfolio outstanding by month
  FUNCTION get_po_by_month(p_start_date DATE,
                           p_end_date DATE)
  RETURN data_types.po_tab PIPELINED;
  
  -- portfolio outstanding by CO
  FUNCTION get_po_by_co(p_branch_id NUMBER,
                        p_date_list VARCHAR2,
                        p_ar_in_days_from NUMBER,
                        p_ar_in_days_to NUMBER)
  RETURN data_types.po_co_tab PIPELINED;
  
END;
/
CREATE OR REPLACE PACKAGE BODY reports_ln
IS

  -- LNU100 - Account statement for UAT
  FUNCTION get_uat_account_statement(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT t.tran_type,
                   t.tran_date,
                   t.post_date,
                   t.value_date,
                   --t.account_no,
                   DECODE(t.tran_type_id, 1, t.rpa_cash_amount) rpa_cash_amount,
                   DECODE(t.tran_type_id, 1, t.rpa_gl_amount) rpa_gl_amount,
                   DECODE(t.tran_type_id, 1, t.rpa_casa_amount) rpa_casa_amount,
                   DECODE(t.tran_type_id, 1, t.rpa_total_amount) rpa_total_amount,
                   DECODE(t.tran_type_id, 1, t.rpa_balance) rpa_balance_after_payment, --
                   --' -- ' sep2,
                   DECODE(t.tran_type_id, 0, NVL(t.fees_paid, 0)) fees_paid,
                   DECODE(t.tran_type_id, 0, NVL(t.penalty_paid, 0)) penalty_paid,
                   DECODE(t.tran_type_id, 0, NVL(t.interest_paid, 0)) interest_paid,
                   DECODE(t.tran_type_id, 0, NVL(t.principal_paid, 0)) principal_paid,
                   DECODE(t.tran_type_id, 0, NVL(t.total_paid, 0)) total_deduction,
                   DECODE(t.tran_type_id, 0, t.rpa_balance - t.paid_balance) rpa_balance_after_payment_app,
                   --' -- ' sep3,
                   DECODE(t.tran_type_id, 0, NVL(t.fees_due, 0)) fees_due,
                   DECODE(t.tran_type_id, 0, NVL(t.penalty_due, 0)) penalty_due,
                   DECODE(t.tran_type_id, 0, NVL(t.interest_due, 0)) interest_due,
                   DECODE(t.tran_type_id, 0, NVL(t.principal_due, 0)) principal_due,
                   DECODE(t.tran_type_id, 0, NVL(t.total_due, 0)) total_due,
                   --' -- ' sep3,
                   t.interest_outstanding,
                   t.principal_outstanding
            FROM
            (
            SELECT DECODE(t1.tran_type_id, 1, 'Payment to RPA', 'Appropriation') tran_type,
                   NVL(t1.tran_type_id, 0) tran_type_id,
                   NVL(t1.account_no, t2.account_no) account_no,
                   t1.tran_date,
                   t1.post_date,
                   NVL(t1.value_date, t2.due_date) value_date,
                   --' -- ' sep1,
                   t1.rpa_cash_amount,
                   t1.rpa_gl_amount,
                   t1.rpa_casa_amount,
                   t1.rpa_total_amount,
                   --' -- ' sep2,
                   t1.fees_paid,
                   t1.penalty_paid,
                   t1.interest_paid,
                   t1.principal_paid,
                   t1.total_paid,
                   --' -- ' sep3,
                   t2.fees_due,
                   t2.penalty_due,
                   t2.interest_due,
                   t2.principal_due,
                   t2.total_due,
                   --' -- ' sep4,
                   /*t3.total_interest,
                   t3.total_principal,*/
                   NVL((SELECT SUM(t.amt_txn_acy)
                        FROM ln_daily_txn_log_hist/*@host_link*/ t
                        WHERE t.cod_acct_no = t1.account_no
                              AND t.cod_txn_mnemonic IN(1403,1404,1405,1407, /*Cash*/ 1081,1082,1083,1084,1086,1088, /*GL*/ 1065,1066,1067,1068,1072,1410 /*Casa*/)
                              AND TRUNC(t.dat_value) <= TRUNC(t1.value_date)),
                       0) rpa_balance,
                   NVL((SELECT SUM(t.amt_txn_acy)
                        FROM ln_daily_txn_log_hist/*@host_link*/ t
                        WHERE t.flg_drcr = 'D'
                              AND t.cod_acct_no = t1.account_no
                              AND t.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3089,3091)
                              AND TRUNC(t.dat_value) <= TRUNC(t1.value_date)),
                        0) paid_balance,
                   DECODE(t1.tran_type_id, 0,
                          t3.total_interest - NVL((SELECT SUM(s.amt_interest)
                                                   FROM ln_acct_schedule_detls/*@host_link*/ s
                                                   WHERE s.cod_acct_no = t1.account_no
                                                         AND s.date_instal <= t1.value_date),
                                                  0)) interest_outstanding,
                   DECODE(t1.tran_type_id, 0,
                          t3.total_principal - NVL((SELECT SUM(s.amt_principal)
                                                    FROM ln_acct_schedule_detls/*@host_link*/ s
                                                    WHERE s.cod_acct_no = t1.account_no
                                                          AND s.date_instal <= t1.value_date),
                                                  0)) principal_outstanding
            FROM
            (
            SELECT tl.cod_acct_no account_no,
                   TRUNC(tl.dat_txn) tran_date,
                   TRUNC(tl.dat_post) post_date,
                   TRUNC(tl.dat_value) value_date,
                   (CASE WHEN tl.cod_txn_mnemonic IN(1081,1082,1083,1084,1086,1088, /*GL*/ 1403,1404,1405,1407, /*Cash*/ 1065,1066,1067,1068,1072,1410 /*Casa*/) THEN 1
                         WHEN tl.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097) THEN 0
                    END) tran_type_id,
                   ---------
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(1403,1404,1405,1407) THEN tl.amt_txn_acy ELSE 0 END) rpa_cash_amount,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(1081,1082,1083,1084,1086,1088) THEN tl.amt_txn_acy ELSE 0 END) rpa_gl_amount,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(1065,1066,1067,1068,1072,1410) THEN tl.amt_txn_acy ELSE 0 END) rpa_casa_amount,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(1403,1404,1405,1407, /*Cash*/ 1081,1082,1083,1084,1086,1088, /*GL*/ 1065,1066,1067,1068,1072,1410 /*Casa*/) THEN tl.amt_txn_acy ELSE 0 END) rpa_total_amount,
                   ---------
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(3085,3086) THEN tl.amt_txn_acy ELSE 0 END) fees_paid,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(3089,3091) THEN tl.amt_txn_acy ELSE 0 END) penalty_paid,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(3083,3084) THEN tl.amt_txn_acy ELSE 0 END) interest_paid,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(3014, 3082) THEN tl.amt_txn_acy ELSE 0 END) principal_paid,
                   SUM(CASE WHEN tl.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3089,3091) THEN tl.amt_txn_acy ELSE 0 END) total_paid
                   ---------
            FROM ln_daily_txn_log_hist/*@host_link*/ tl
            WHERE (tl.flg_drcr = 'D' AND tl.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097)
                   OR tl.cod_txn_mnemonic IN(1081,1082,1083,1084,1086,1088, /*GL*/ 1403,1404,1405,1407, /*Cash*/ 1065,1066,1067,1068,1072,1410 /*Casa*/))
                  AND tl.cod_acct_no = v_account_no
            GROUP BY tl.cod_acct_no,
                     TRUNC(tl.dat_txn),
                     TRUNC(tl.dat_post),
                     TRUNC(tl.dat_value),
                     (CASE WHEN tl.cod_txn_mnemonic IN(1081,1082,1083,1084,1086,1088, /*GL*/ 1403,1404,1405,1407, /*Cash*/ 1065,1066,1067,1068,1072,1410 /*Casa*/) THEN 1
                           WHEN tl.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097) THEN 0
                      END)
            ) t1 FULL OUTER JOIN
            (
            SELECT la.cod_acct_no account_no,
                   TRUNC(la.dat_arrears_due) due_date,
                   SUM(DECODE(la.cod_arrear_type,'F',la.amt_arrears_due,0)) fees_due,
                   SUM(DECODE(la.cod_arrear_type,'A',la.amt_arrears_due,0)) penalty_due,
                   SUM(DECODE(la.cod_arrear_type,'I',la.amt_arrears_due,0)) interest_due,
                   SUM(DECODE(la.cod_arrear_type,'C',la.amt_arrears_due,0)) principal_due,
                   SUM(la.amt_arrears_due) total_due
            FROM ln_arrears_table/*@host_link*/ la
            WHERE la.cod_acct_no = v_account_no
                  AND la.cod_arrear_type IN('F','A','I','C')
            GROUP BY la.cod_acct_no,
                  TRUNC(la.dat_arrears_due)
            HAVING SUM(la.amt_arrears_due) > 0
            ) t2 ON t1.value_date = t2.due_date,
            (
            SELECT SUM(s.amt_interest) total_interest,
                   SUM(s.amt_principal) total_principal
            FROM ln_acct_schedule_detls/*@host_link*/ s
            WHERE s.cod_acct_no = v_account_no
            --GROUP BY s.cod_acct_no
            ) t3
            WHERE NVL(t2.total_due, 1) > 0
            ) t
            /*WHERE t.tran_date != to_date('1900-01-01', 'yyyy-mm-dd')*/
                  --AND decode(t.tran_type_id, 0, nvl(t.total_due, 0)) > 0
            ORDER BY t.value_date,
                     t.tran_date,
                     t.tran_type_id DESC;

       RETURN c_result;
  END;
  -- LNU100 - Account statement for UAT

  -- LN1010 - Loan account information migrated from SIEM
  FUNCTION ln1010(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_migration_date DATE := report_methods.get_migration_date;
  BEGIN
       OPEN c_result FOR
            SELECT v_migration_date migration_date,
                   a.cod_acct_no account_no,
                   a.amt_face_value loan_amount,
                   pm.nam_ccy_short acy,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   bm.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   NVL(ab.amt_princ_balance, 0) principal_outstanding,
                   NVL(ab.amt_rpa_balance, 0) rpa_balance,
                   ibd.amt_last_int1_accr interest_accrued,
                   ast.txt_acct_status account_status,
                   NVL(t.pricipal_arrears, 0) pricipal_arrears,
                   NVL(t.interest_arrears, 0) interest_arrears,
                   NVL(t.penalty_arrears, 0) penalty_arrears
            FROM ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ba_acct_status ast,
                 conv_ln_acct_dtls a,
                 conv_ln_acct_balances ab,
                 conv_ln_acct_int_balance_dtls ibd,
                 conv_ci_custmast c,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT lat.cod_acct_no,
                         SUM(DECODE(lat.cod_arrear_type, 'C', lat.amt_arrears_due, 0)) pricipal_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'I', lat.amt_arrears_due, 0)) interest_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'A', lat.amt_arrears_due, 0)) penalty_arrears
                  FROM conv_ln_arrears_table lat
                  GROUP BY lat.cod_acct_no) t
            WHERE ab.cod_acct_no(+) = a.cod_acct_no
                  AND ibd.cod_acct_no(+) = a.cod_acct_no
                  AND t.cod_acct_no(+) = a.cod_acct_no
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND pm.cod_prod = a.cod_prod
                  AND ast.cod_acct_status = a.cod_acct_stat
                  AND ast.cod_module = 'LN'
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;
  -- LN1010 - Loan account information migrated from SIEM
  
  -- LN1011 - LNM10 Loan account details
  FUNCTION ln1011_account_details(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    --
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    --
    v_amount_paid_today NUMBER := 0;
    v_outstanding_balance NUMBER := 0;
    v_advance_amount NUMBER := 0;
    v_ioa_base_amount NUMBER := 0;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       SELECT DISTINCT a.amt_paid_today,
              ab.amt_princ_balance,
              ab.amt_rpa_balance,
              (t1.principal_arr_due * t2.principal_calc_base +
               t1.interest_arr_due * t2.interest_calc_base +
               t1.fee_arr_due * t2.fee_calc_base +
               t1.only_arr_due * t2.arrear_calc_base)
              INTO v_amount_paid_today,
                   v_outstanding_balance,
                   v_advance_amount,
                   v_ioa_base_amount
       FROM ln_acct_dtls@host_link a,
            ln_acct_balances@host_link ab,
            (SELECT lat.cod_acct_no cod_acct_no,
                    SUM(DECODE(INSTR('C', lat.cod_arrear_type), 0, 0, lat.amt_arrears_due)) principal_arr_due,
                    SUM(DECODE(INSTR('ITNU', lat.cod_arrear_type), 0, 0, lat.amt_arrears_due)) interest_arr_due,
                    SUM(DECODE(INSTR('FPSODEGM', lat.cod_arrear_type), 0, 0, lat.amt_arrears_due)) fee_arr_due,
                    SUM(DECODE(INSTR('AL', lat.cod_arrear_type), 0, 0, lat.amt_arrears_due)) only_arr_due
             FROM ln_arrears_table@host_link lat
             WHERE lat.amt_arrears_due > 0
                   AND lat.dat_ioa_grace_expiry < SYSDATE
                   AND lat.cod_acct_no = v_account_no
             GROUP BY lat.cod_acct_no) t1,
            (SELECT TRIM(acs.cod_acct_no) cod_acct_no,
                    DECODE(INSTR(ir.cod_int_calc_base, 'C'), 0, 0, 1) principal_calc_base,
                    DECODE(INSTR(ir.cod_int_calc_base, 'I'), 0, 0, 1) interest_calc_base,
                    DECODE(INSTR(ir.cod_int_calc_base, 'F'), 0, 0, 1) fee_calc_base,
                    DECODE(INSTR(ir.cod_int_calc_base, 'A'), 0, 0, 1) arrear_calc_base
             FROM ln_int_rules@host_link ir,
                  ln_acct_schedule@host_link acs,
                  ln_acct_attributes@host_link aa
             WHERE acs.cod_acct_no = aa.cod_acct_no
                   AND acs.dat_stage_start <= SYSDATE
                   AND DECODE(aa.flg_include_to_date,
                              'N',
                              acs.dat_stage_end - 1,
                              acs.dat_stage_end) >= SYSDATE
                   AND ir.cod_int_rule = acs.cod_ioa_rule
                   AND acs.flg_mnt_status = 'A'
                   AND ir.flg_mnt_status = 'A'
                   AND acs.cod_acct_no = v_account_no) t2
       WHERE a.cod_acct_no = ab.cod_acct_no
             AND a.cod_acct_no = t1.cod_acct_no(+)
             AND a.cod_acct_no = t2.cod_acct_no(+)
             --
             AND a.cod_acct_no = v_account_no;
       
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   c.cod_cust_id customer_id,
                   ap.app_aad_id application_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   a.dat_first_disb first_disb_date,
                   a.dat_of_maturity maturity_date,
                   ab.amt_net_disbursed disbursed_amount,
                   a.rat_var_current interest_rate,
                   pm.nam_ccy_short ccy,
                   v_amount_paid_today amount_paid_today,
                   (v_outstanding_balance - v_amount_paid_today - v_advance_amount) outstanding_balance,
                   v_advance_amount advance_amount,
                   NVL(ps.interest_base, 0)interest_base,
                   NVL(v_ioa_base_amount, 0) penalty_interest_base,
                   DECODE(ac.flg_accr_status, 'S', 'Suspended', 'Normal') accrual_status,
                   TRIM(ast.txt_acct_status) account_status
            FROM ln_acct_dtls a,
                 ci_custmast c,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ln_acct_balances ab,
								         ln_acct_attributes aa,
                 ba_acct_status ast,
                 ac_acct_crr_code aac,
                 ac_crr_codes ac,
                 or_applications ap,
								         sm_user_profile up,
                 (SELECT s.cod_acct_no account_no,
                         SUM(CASE WHEN s.date_instal > SYSDATE THEN s.amt_principal ELSE 0 END) interest_base
                  FROM ln_acct_schedule_detls s
                  WHERE s.cod_acct_no = TRIM(v_account_no)
                  GROUP BY s.cod_acct_no) ps
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND aa.cod_acct_off = up.cod_userno
                  AND a.cod_acct_no = ps.account_no(+)
                  AND a.cod_acct_no = aac.cod_acct_no(+)
                  AND a.cod_acct_no = ap.app_acc_nbr(+)
                  AND c.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND ast.cod_acct_status = a.cod_acct_stat
                  AND ast.cod_module = 'LN'
                  AND ac.cod_crr(+) = aac.cod_crr_to
                  AND ac.flg_accr_status(+) = 'S'
                  --
                  AND a.cod_acct_no = TRIM(v_account_no);

       RETURN c_result;
  END;
  
  FUNCTION ln1011_arrear_totals(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    --
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   b.principal_arrears,
                   (b.normal_arrears + bl.normal_arrears + bl.normal_arrears_pmi + bl.normal_arrears_ioa) normal_arrears,
                   --bl.normal_arrears_pmi,
                   --bl.normal_arrears_ioa,
                   (b.suspended_arrears + bl.suspended_arrears + bl.suspended_arrears_pmi + bl.suspended_arrears_ioa) suspended_arrears,
                   --bl.suspended_arrears_pmi,
                   --bl.suspended_arrears_ioa,
                   --(b.total_arrears + bl.total_arrears) - (b.normal_arrears + bl.normal_arrears + bl.normal_arrears_pmi + bl.normal_arrears_ioa) - b.principal_arrears suspended_arrears2,
                   (b.total_arrears + bl.total_arrears) total_arrears
            FROM ln_acct_dtls a,
                 (SELECT ab.cod_acct_no account_no,
                         ab.amt_arrears_princ principal_arrears,
                         (ab.amt_arrears_fees +
                          ab.amt_arrears_prem +
                          ab.amt_arrears_legal +
                          ab.amt_arrears_outgoing) normal_arrears,
                         (ab.amt_arrears_susp_fees +
                          ab.amt_arrears_susp_prem +
                          ab.amt_arrears_susp_legal +
                          ab.amt_arrears_susp_outgoing) suspended_arrears,
                         (ab.amt_arrears_princ +
                          ab.amt_arrears_fees +
                          ab.amt_arrears_prem +
                          ab.amt_arrears_legal +
                          ab.amt_arrears_outgoing +
                          ab.amt_arrears_susp_fees +
                          ab.amt_arrears_susp_prem +
                          ab.amt_arrears_susp_legal +
                          ab.amt_arrears_susp_outgoing) total_arrears
                 FROM ln_acct_balances ab
                 WHERE ab.cod_acct_no = v_account_no) b,
                (SELECT bd.cod_acct_no account_no,
                        SUM(DECODE(r.cod_int_typ, 0,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears,
                                                         2, bd.bal_int2_arrears,
                                                         3, bd.bal_int3_arrears,
                                                         4, bd.bal_int4_arrears,
                                                         5, bd.bal_int5_arrears,
                                                         6, bd.bal_int6_arrears,
                                                         7, bd.bal_int7_arrears,
                                                         8, bd.bal_int8_arrears,
                                                         9, bd.bal_int9_arrears,
                                                         10, bd.bal_int10_arrears,
                                                         11, bd.bal_int11_arrears,
                                                         12, bd.bal_int12_arrears,
                                                         13, bd.bal_int13_arrears,
                                                         14, bd.bal_int14_arrears,
                                                         15, bd.bal_int15_arrears,
                                                         16, bd.bal_int16_arrears),
                                   0)) normal_arrears,
                        SUM(DECODE(r.cod_int_typ, 1,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears,
                                                         2, bd.bal_int2_arrears,
                                                         3, bd.bal_int3_arrears,
                                                         4, bd.bal_int4_arrears,
                                                         5, bd.bal_int5_arrears,
                                                         6, bd.bal_int6_arrears,
                                                         7, bd.bal_int7_arrears,
                                                         8, bd.bal_int8_arrears,
                                                         9, bd.bal_int9_arrears,
                                                         10, bd.bal_int10_arrears,
                                                         11, bd.bal_int11_arrears,
                                                         12, bd.bal_int12_arrears,
                                                         13, bd.bal_int13_arrears,
                                                         14, bd.bal_int14_arrears,
                                                         15, bd.bal_int15_arrears,
                                                         16, bd.bal_int16_arrears),
                                   0)) normal_arrears_ioa,
                        SUM(DECODE(r.cod_int_typ, 4,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears,
                                                         2, bd.bal_int2_arrears,
                                                         3, bd.bal_int3_arrears,
                                                         4, bd.bal_int4_arrears,
                                                         5, bd.bal_int5_arrears,
                                                         6, bd.bal_int6_arrears,
                                                         7, bd.bal_int7_arrears,
                                                         8, bd.bal_int8_arrears,
                                                         9, bd.bal_int9_arrears,
                                                         10, bd.bal_int10_arrears,
                                                         11, bd.bal_int11_arrears,
                                                         12, bd.bal_int12_arrears,
                                                         13, bd.bal_int13_arrears,
                                                         14, bd.bal_int14_arrears,
                                                         15, bd.bal_int15_arrears,
                                                         16, bd.bal_int16_arrears),
                                   0)) normal_arrears_pmi,
                        SUM(DECODE(r.cod_int_typ, 0,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears_susp,
                                                         2, bd.bal_int2_arrears_susp,
                                                         3, bd.bal_int3_arrears_susp,
                                                         4, bd.bal_int4_arrears_susp,
                                                         5, bd.bal_int5_arrears_susp,
                                                         6, bd.bal_int6_arrears_susp,
                                                         7, bd.bal_int7_arrears_susp,
                                                         8, bd.bal_int8_arrears_susp,
                                                         9, bd.bal_int9_arrears_susp,
                                                         10, bd.bal_int10_arrears_susp,
                                                         11, bd.bal_int11_arrears_susp,
                                                         12, bd.bal_int12_arrears_susp,
                                                         13, bd.bal_int13_arrears_susp,
                                                         14, bd.bal_int14_arrears_susp,
                                                         15, bd.bal_int15_arrears_susp,
                                                         16, bd.bal_int16_arrears_susp),
                                   0)) suspended_arrears,
                        SUM(DECODE(r.cod_int_typ, 1,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears_susp,
                                                         2, bd.bal_int2_arrears_susp,
                                                         3, bd.bal_int3_arrears_susp,
                                                         4, bd.bal_int4_arrears_susp,
                                                         5, bd.bal_int5_arrears_susp,
                                                         6, bd.bal_int6_arrears_susp,
                                                         7, bd.bal_int7_arrears_susp,
                                                         8, bd.bal_int8_arrears_susp,
                                                         9, bd.bal_int9_arrears_susp,
                                                         10, bd.bal_int10_arrears_susp,
                                                         11, bd.bal_int11_arrears_susp,
                                                         12, bd.bal_int12_arrears_susp,
                                                         13, bd.bal_int13_arrears_susp,
                                                         14, bd.bal_int14_arrears_susp,
                                                         15, bd.bal_int15_arrears_susp,
                                                         16, bd.bal_int16_arrears_susp),
                                   0)) suspended_arrears_ioa,
                        SUM(DECODE(r.cod_int_typ, 4,
                                   DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears_susp,
                                                         2, bd.bal_int2_arrears_susp,
                                                         3, bd.bal_int3_arrears_susp,
                                                         4, bd.bal_int4_arrears_susp,
                                                         5, bd.bal_int5_arrears_susp,
                                                         6, bd.bal_int6_arrears_susp,
                                                         7, bd.bal_int7_arrears_susp,
                                                         8, bd.bal_int8_arrears_susp,
                                                         9, bd.bal_int9_arrears_susp,
                                                         10, bd.bal_int10_arrears_susp,
                                                         11, bd.bal_int11_arrears_susp,
                                                         12, bd.bal_int12_arrears_susp,
                                                         13, bd.bal_int13_arrears_susp,
                                                         14, bd.bal_int14_arrears_susp,
                                                         15, bd.bal_int15_arrears_susp,
                                                         16, bd.bal_int16_arrears_susp),
                                   0)) suspended_arrears_pmi,
                        (SUM(DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears,
                                                  2, bd.bal_int2_arrears,
                                                  3, bd.bal_int3_arrears,
                                                  4, bd.bal_int4_arrears,
                                                  5, bd.bal_int5_arrears,
                                                  6, bd.bal_int6_arrears,
                                                  7, bd.bal_int7_arrears,
                                                  8, bd.bal_int8_arrears,
                                                  9, bd.bal_int9_arrears,
                                                  10, bd.bal_int10_arrears,
                                                  11, bd.bal_int11_arrears,
                                                  12, bd.bal_int12_arrears,
                                                  13, bd.bal_int13_arrears,
                                                  14, bd.bal_int14_arrears,
                                                  15, bd.bal_int15_arrears,
                                                  16, bd.bal_int16_arrears)) +
                         SUM(DECODE(r.ctr_srl_no, 1, bd.bal_int1_arrears_susp,
                                                  2, bd.bal_int2_arrears_susp,
                                                  3, bd.bal_int3_arrears_susp,
                                                  4, bd.bal_int4_arrears_susp,
                                                  5, bd.bal_int5_arrears_susp,
                                                  6, bd.bal_int6_arrears_susp,
                                                  7, bd.bal_int7_arrears_susp,
                                                  8, bd.bal_int8_arrears_susp,
                                                  9, bd.bal_int9_arrears_susp,
                                                  10, bd.bal_int10_arrears_susp,
                                                  11, bd.bal_int11_arrears_susp,
                                                  12, bd.bal_int12_arrears_susp,
                                                  13, bd.bal_int13_arrears_susp,
                                                  14, bd.bal_int14_arrears_susp,
                                                  15, bd.bal_int15_arrears_susp,
                                                  16, bd.bal_int16_arrears_susp))) total_arrears
                  FROM ln_acct_int_balance_dtls bd,
                       mv_product_int_rules r
                  WHERE bd.cod_prod = r.cod_prod
                        AND bd.cod_acct_no = v_account_no
                  GROUP BY bd.cod_acct_no) bl
            WHERE a.cod_acct_no = b.account_no(+)
                  AND a.cod_acct_no = bl.account_no(+)
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;
  
  FUNCTION ln1011_arrears_inquiry(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    --
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT la.cod_acct_no account_no,
                   la.ref_billno_srl ref_bill_no,
                   la.cod_arrear_type arrear_type,
                   la.dat_arrears_due due_date,
                   la.amt_arrears_assessed amount_assessed,
                   la.amt_arrears_due amount_due,
                   DECODE(la.amt_arrears_due, 0, 0, TRUNC(SYSDATE) - la.dat_arrears_due) days_past_due,
                   la.dat_last_payment last_payment_date
            FROM ln_arrears_table la
            WHERE la.dat_arrear_cancelled IS NULL
                  AND la.cod_arrear_type NOT IN ('R', '1')
                  --
                  AND la.cod_acct_no = v_account_no
            ORDER BY la.ref_billno_srl,
                     la.dat_arrears_due,
                     la.dat_last_payment,
                     la.cod_arrear_type;

       RETURN c_result;
  END;
  
  FUNCTION ln1011_account_ledger_inquiry(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    --
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT al.dat_txn txn_date,
                   TRUNC(al.dat_value) value_date,
                   TRIM(al.txt_txn_desc) txn_desc,
                   al.cod_txn_mnemonic,
                   al.flg_drcr drcr,
                   al.amt_txn_acy,
                   /*(CASE WHEN al.cod_user_id IN('SYSOPER', 'SYSTEM', 'CONVTELLER', 'CONVSUPER')
                         THEN ''
                         ELSE al.cod_user_id
                    END) user_id,
                   (CASE WHEN al.cod_auth_id IN('SYSOPER', 'SYSTEM', 'CONVTELLER', 'CONVSUPER')
                         THEN ''
                         ELSE al.cod_auth_id
                    END) auth_id,*/
                   al.cod_user_id user_id,
                   al.cod_auth_id auth_id,
                   SUM(DECODE(al.flg_drcr, 'D', al.amt_txn_acy, -al.amt_txn_acy)) OVER (ORDER BY al.dat_post, al.dat_txn, al.dat_value, al.ctr_batch_no, al.ref_sys_tr_aud_no, al.ref_sub_seq_no) running_balance
            FROM ln_acct_ledg al
            WHERE al.cod_acct_no = v_account_no
            ORDER BY al.dat_post,
                     al.dat_txn,
                     al.dat_value,
                     al.ctr_batch_no,
                     al.ref_sys_tr_aud_no,
                     al.ref_sub_seq_no;

       RETURN c_result;
  END;
  -- LN1011 - LNM10 Loan account details
  
  -- LN1020 - Loan account statement
  -- LN1181 - Transfer form to legal department
  FUNCTION ln1020_account_summary(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   a.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   --a.cod_ccy currency_code,
                   trim(pm.nam_ccy_short) ccy,
                   aa.cod_acct_off co_user_no,
                   (TRIM(up.nam_user) || ' (' || TRIM(up.cod_user_id) || ')') co_user_name,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                   cl.customer_cycle,
                   TRIM(lp.lkc_desc) loan_purpose,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || TRIM(pm.nam_product)) product_name,
                   NVL(t.amount_borrowed, 0) amount_borrowed,
                   a.amt_face_value amount_sanctioned,
                   a.rat_var_current interest_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   TRUNC(a.dat_last_disb) last_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   TRIM(g.cod_group) group_id,
                   DECODE(g.cod_group, NULL, NULL, (TRIM(g.cod_group) || ' - ' || TRIM(g.nam_group))) group_name,
                   g.cod_group_cycle group_cycle,
                   TRIM(ast.txt_acct_status) account_status
            FROM ln_acct_dtls/*@host_link*/ a,
                 ln_prod_mast/*@host_link*/ pm,
                 ln_acct_attributes/*@host_link*/ aa,
                 ba_cc_brn_mast/*@host_link*/ bm,
                 or_applications/*@host_link*/ ap,
                 ci_custmast/*@host_link*/ c,
                 sm_user_profile/*@host_link*/ up,
                 ba_acct_status/*@host_link*/ ast,
                 or_lookups/*@host_link*/ lp,
                 mv_customer_cycle_list cl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT sd.cod_acct_no,
                         SUM(sd.amt_principal) amount_borrowed
                  FROM (SELECT sd1.cod_acct_no,
                               NULL effective_date,
                               sd1.amt_principal
                        FROM ln_acct_schedule_detls/*@host_link*/ sd1
                        WHERE sd1.cod_acct_no = v_account_no
                        UNION ALL
                        SELECT sd2.cod_acct_no,
                               sd2.dat_amd effective_date,
                               sd2.amt_principal
                        FROM ln_acct_schedule_detls_hist/*@host_link*/ sd2
                        WHERE sd2.cod_acct_no = v_account_no) sd,
                       (SELECT MIN(sdh.dat_amd) min_date
                        FROM ln_acct_schedule_detls_hist/*@host_link*/ sdh
                        WHERE sdh.cod_acct_no = v_account_no
                              AND sdh.dat_amd >= p_date) md,
                       (SELECT b.dat_process effective_date
                        FROM ba_bank_mast/*@host_link*/ b
                        /*WHERE flg_mnt_status = 'A'*/) bb
                  WHERE nvl(sd.effective_date, bb.effective_date) = LEAST(bb.effective_date, nvl(md.min_date, bb.effective_date))
                  GROUP BY sd.cod_acct_no) t
            WHERE a.cod_prod = pm.cod_prod
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND TRIM(ap.app_acc_nbr(+)) = a.cod_acct_no
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND t.cod_acct_no(+) = a.cod_acct_no
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND c.cod_cust_id = a.cod_cust_id
                  AND cl.cod_cust_id = a.cod_cust_id
                  AND up.cod_userno = aa.cod_acct_off
                  AND ast.cod_acct_status = a.cod_acct_stat
                  AND ast.cod_module = 'LN'
                  AND lp.lkc_code(+) = ap.app_purpose_cd
                  AND lp.lkc_lkt_type(+) = 'APP_PURPOSE_CD'
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;

  FUNCTION ln1020_account_balance(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   ab.amt_disbursed loan_amount,
                   ab.amt_rpa_balance rpa_balance,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                    END) principal_outstanding,
                   --ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_paid,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE NVL(ar.principal_due, 0)
                    END) principal_due,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE NVL(ar.interest_due, 0)
                    END) interest_due,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE NVL(ar.principal_arrears, 0)
                    END) principal_arrears,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE NVL(ar.interest_arrears, 0)
                    END) interest_arrears,
                   (CASE WHEN NVL(ar.principal_arrears, 0) > 0
                         THEN report_methods.get_arrears_in_days(p_date,
                                                                 a.cod_acct_no,
                                                                 ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                 NVL(rs.principal_due_by_schedule, 0))
                         ELSE 0
                    END) arrears_in_days,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE NVL(ar.penalty_billed, 0)
                    END) penalty_billed
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 conv_ln_acct_balances cab,
                 conv_ln_acct_int_balance_dtls caib,
                 (SELECT rs1.cod_acct_no cod_acct_no,
                         --SUM(rs1.amt_principal) loan_amt_by_schedule,
                         SUM(CASE WHEN rs1.date_instal < p_date THEN rs1.amt_principal ELSE 0 END) principal_due_by_schedule
                  FROM ln_acct_schedule_detls rs1
                  WHERE rs1.cod_acct_no = v_account_no
                  GROUP BY rs1.cod_acct_no) rs,
                 (SELECT th.cod_acct_no cod_acct_no,
                         SUM(CASE WHEN th.cod_arrear_type = 'C'
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_due,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_due,
                         SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N') AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) penalty_billed
                  FROM ln_arrear_txn_hist th
                  WHERE th.dat_value <= p_date
                        AND th.cod_acct_no = v_account_no
                  GROUP BY th.cod_acct_no) ar,
                 (SELECT dtl.cod_acct_no,
                         SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                  FROM rpt_ln_daily_txn_log_po dtl
                  WHERE dtl.dat_value <= p_date
                        AND dtl.cod_acct_no = v_account_no
                  GROUP BY dtl.cod_acct_no) tl
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = cab.cod_acct_no(+)
                  AND a.cod_acct_no = caib.cod_acct_no(+)
                  AND a.cod_acct_no = tl.cod_acct_no
                  AND a.cod_acct_no = rs.cod_acct_no(+)
                  AND a.cod_acct_no = ar.cod_acct_no(+);

       RETURN c_result;
  END;

  FUNCTION ln1020_payment_schedule(p_account_no VARCHAR2, p_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT effective_date,
                   payment_no,
                   payment_date,
                   principal,
                   interest
            FROM (SELECT (SELECT dat_process
                          FROM ba_bank_mast/*@host_link*/
                          /*WHERE flg_mnt_status = 'A'*/) effective_date,
                         s.ctr_instal payment_no,
                         TRUNC(s.date_instal) payment_date,
                         s.amt_principal principal,
                         s.amt_interest interest,
                         '' charges
                  FROM ln_acct_schedule_detls/*@host_link*/ s
                  WHERE TRIM(s.cod_acct_no) = v_account_no
                  UNION ALL
                  SELECT sh.dat_amd effective_date,
                         sh.ctr_instal payment_no,
                         TRUNC(sh.date_instal) payment_date,
                         sh.amt_principal principal,
                         sh.amt_interest interest,
                         '' charges
                  FROM ln_acct_schedule_detls_hist/*@host_link*/ sh
                  WHERE TRIM(sh.cod_acct_no) = v_account_no)
            WHERE effective_date <= (SELECT MIN(dt.effective_date)
                                     FROM (SELECT dat_process effective_date
                                           FROM ba_bank_mast/*@host_link*/
                                           --WHERE flg_mnt_status = 'A'
                                           UNION
                                           SELECT dat_amd
                                           FROM ln_acct_schedule_detls_hist/*@host_link*/
                                           WHERE TRIM(cod_acct_no) = v_account_no) dt
                                     WHERE dt.effective_date >= p_date)
            ORDER BY 1 DESC, 3, 2;

       RETURN c_result;
  END;

  FUNCTION ln1020_payments_made(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT th.cod_acct_no account_no,
                   TRUNC(th.dat_value) value_date,
                   SUM(CASE WHEN th.cod_arrear_type IN ('C','R')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) principal,
                   SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) interest,
                   SUM(decode(th.cod_arrear_type, 'F', th.amt_txn_acy, 0)) fee,
                   SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) penalty,
                   SUM(CASE WHEN th.cod_arrear_type NOT IN ('C','R','I','N','F','A','L')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) other,
                   SUM(th.amt_txn_acy) total
            FROM ln_arrear_txn_hist th
            WHERE th.cod_drcr = 'C'
                  --
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(th.dat_value),
                             'tran_date', TRUNC(th.dat_txn),
                             'post_date', TRUNC(th.dat_post)) <= p_date
                  AND th.cod_acct_no = v_account_no
            GROUP BY th.cod_acct_no,
                     TRUNC(th.dat_value)
            ORDER BY 2;

       RETURN c_result;
  END;

  FUNCTION ln1020_posted_txn(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT (CASE WHEN trn_code IN (1003,1011,1017,1019,1085,1087,1413)
                         THEN 'Disbursement transactions'
                         WHEN trn_code IN (3260,3261,3262,3263,3264,3265,4260,4262,4263)
                         THEN 'Interest accrual and income transactions'
                         WHEN trn_code IN (1403,1404,1405,1407,1410,1081,1082,1083,1084,1086,3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097)
                         THEN 'Payment and appropriation transactions'
                         WHEN trn_code IN (3400,4400)
                         THEN 'Loan provision transactions'
                         ELSE 'Other transactions'
                    END) tran_cat,
                   (CASE WHEN trn_code IN (1003,1011,1017,1019,1085,1087,1413)
                         THEN 1
                         WHEN trn_code IN (3260,3261,3262,3263,3264,3265,4260,4262,4263)
                         THEN 3
                         WHEN trn_code IN (1403,1404,1405,1407,1410,1081,1082,1083,1084,1086,3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097)
                         THEN 2
                         WHEN trn_code IN (3400,4400)
                         THEN 4
                         ELSE 5
                    END) tran_cat_ord,
                   ah.trn_ref_no reference,
                   ah.ac_entry_sr_no,
                   ah.trn_dt transaction_date,
                   ah.value_dt value_date,
                   ah.txn_init_date init_date,
                   (ah.trn_code || ': ' || ah.txt_txn_desc) transaction_desc,
                   ah.ac_no account_no,
                   ah.ac_ccy currency_code,
                   ah.drcr_ind debit_or_credit,
                   ah.fcy_amount amount_ccy,
                   ah.lcy_amount amount_lcy,
                   ah.related_customer,
                   ah.user_id,
                   ah.auth_id
            FROM actb_history/*@host_link*/ ah
            WHERE TRIM(ah.related_account) = v_account_no
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(ah.value_dt),
                             'tran_date', TRUNC(ah.txn_init_date),
                             'post_date', TRUNC(ah.trn_dt)) <= TRUNC(p_date)
            ORDER BY tran_cat_ord,
                     --ah.related_reference,
                     ah.fcy_amount,
                     ah.trn_ref_no/*,
                     ah.event_sr_no*/;

       RETURN c_result;
  END;

  FUNCTION ln1020_not_posted_txn(p_account_no VARCHAR2, p_date DATE, p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT rc.tim_sys entr_date,
                   rc.dat_val value_date,
                   rc.cod_tran tran_code,
                   rc.amt_tran_tcye amount_tcy,
                   trim(ct.nam_ccy_short) tcy,
                   rc.amt_tran_acye amount_acy,
                   trim(ca.nam_ccy_short) acy,
                   (rc.cod_tran || ': ' || rc.buf_stat_scr) tran_desc,
                   rc.user_id,
                   rc.id_auth auth_id,
                   (CASE WHEN rc.cod_tran_stat = 32 THEN 'Authorization requested'
                         WHEN rc.cod_tran_stat IN(33,97) THEN 'Cancelled'
                         WHEN rc.cod_tran_stat = 96 THEN 'Authorization granted'
                         WHEN rc.cod_tran_stat = 128 THEN 'Submitted to host'
                         ELSE 'Code: ' || rc.cod_tran_stat
                   END) tran_state
            FROM v_rec_txnlog rc,
                 ba_ccy_code ct,
                 ba_ccy_code ca
            WHERE NVL(rc.amt_tran_tcye, 0) != 0
                  AND ca.cod_ccy = rc.cod_acct_cur
                  AND ct.cod_ccy = rc.cod_tran_cur
                  --
                  AND rc.id_acct = v_account_no
                  AND DECODE(p_date_type,
                         'value_date', TRUNC(rc.dat_val),
                         'tran_date', TRUNC(rc.dat_sys),
                         'post_date', TRUNC(rc.dat_post)) = TRUNC(p_date)
            ORDER BY tran_state,
                     rc.tim_sys;

       RETURN c_result;
  END;
  
  FUNCTION ln1020_mig_balance(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_migration_date DATE := report_methods.get_migration_date;
  BEGIN
       OPEN c_result FOR
            SELECT v_migration_date migration_date,
                   NVL(ab.amt_princ_balance, 0) principal_outstanding,
                   NVL(ab.amt_rpa_balance, 0) rpa_balance,
                   NVL(t.pricipal_arrears, 0) pricipal_arrears,
                   NVL(t.interest_arrears, 0) interest_arrears,
                   NVL(t.penalty_arrears, 0) penalty_arrears,
                   (NVL(ab.amt_princ_balance, 0) +
                        NVL(t.interest_arrears, 0) +
                        NVL(t.penalty_arrears, 0) -
                        NVL(ab.amt_rpa_balance, 0)) balance_bf_by_conv
            FROM conv_ln_acct_dtls a,
                 conv_ln_acct_balances ab,
                 (SELECT lat.cod_acct_no cod_acct_no,
                         SUM(DECODE(lat.cod_arrear_type, 'C', lat.amt_arrears_due, 0)) pricipal_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'I', lat.amt_arrears_due, 0)) interest_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'A', lat.amt_arrears_due, 0)) penalty_arrears
                  FROM conv_ln_arrears_table lat
                  WHERE lat.cod_acct_no = v_account_no
                  GROUP BY lat.cod_acct_no) t
            WHERE ab.cod_acct_no(+) = a.cod_acct_no
                  AND TRIM(t.cod_acct_no(+)) = a.cod_acct_no
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;
  -- LN1020 - Loan account statement
  
  -- LN1021 - Group account statement
  FUNCTION ln1021_group_summary(p_group_id VARCHAR2,
                                p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(br.branch_id) || ' - ' || br.branch_name) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || TRIM(pm.nam_product)) product_name,
                   (TRIM(up.nam_user) || ' ( ' || TRIM(up.cod_user_id) || ' )') co_user_name,
                   TRIM(pm.nam_ccy_short) acy,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(t.term) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(t.term/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(t.term/65536) || ' (Monthly)'
                         ELSE '0'
                    END) term,
                   t.group_id,
                   t.group_name,
                   t.group_cycle,
                   t.sanctioned_amount,
                   t.disbursed_amount,
                   t.interest_rate,
                   t.first_disb_date,
                   t.last_disb_date,
                   t.number_of_clients
            FROM ln_prod_mast pm,
                 sm_user_profile up,
                 v_branches_and_regions br,
                 (SELECT TRIM(g.cod_group) group_id,
                         g.cod_group_cycle group_cycle,
                         MAX(g.nam_group) group_name,
                         MIN(a.cod_cc_brn) branch_id,
                         MIN(a.cod_prod) product_id,
                         MAX(aa.cod_acct_off) co_user_no,
                         SUM(a.amt_face_value) sanctioned_amount,
                         SUM(ab.amt_net_disbursed) disbursed_amount,
                         MIN(a.rat_var_current) interest_rate,
                         MIN(a.ctr_term_months) term,
                         MIN(a.dat_first_disb) first_disb_date,
                         MAX(a.dat_last_disb) last_disb_date,
                         COUNT(*) number_of_clients
                  FROM ln_acct_dtls a,
                       ln_acct_balances ab,
                       ln_acct_attributes aa,
                       (SELECT DISTINCT gd.cod_group,
                               gd.nam_group,
                               gd.cod_group_cycle,
                               gd.cod_acct_no
                        FROM ba_group_details gd
                        WHERE gd.flg_group_acc_attr = 'A'
                              --
                              AND gd.cod_group = TRIM(p_group_id)
                              AND gd.cod_group_cycle = p_group_cycle) g
                  WHERE a.cod_acct_no = g.cod_acct_no
                        AND a.cod_acct_no = ab.cod_acct_no
                        AND a.cod_acct_no = aa.cod_acct_no
                  GROUP BY TRIM(g.cod_group),
                           --TRIM(g.nam_group),
                           g.cod_group_cycle) t
            WHERE pm.cod_prod = t.product_id
                  AND br.branch_id = t.branch_id
                  AND up.cod_userno = t.co_user_no;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_group_members(p_group_id VARCHAR2,
                                p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   ab.amt_net_disbursed loan_amount,
                   TRIM(pm.nam_ccy_short) acy,
                   TRIM(ast.txt_acct_status) account_status,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                   --TRIM(c.cod_cust_natl_id) passport_id,
                   TRIM(cd.document_id) passport_id,
                   TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) customer_age,
                   (CASE WHEN c.txt_cust_sex = 'M' THEN 'Male'
                         WHEN c.txt_cust_sex = 'F' THEN 'Female'
                    END) gender,
                   DECODE(c.cod_cust_stat, 0,  'Regular', 1,  'IDP', 2,  'Refugee') refugee_status
            FROM ln_acct_dtls a,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ln_acct_balances ab,
                 ba_acct_status ast,
                 v_ci_docmast cd,
                 (SELECT DISTINCT /*gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,*/
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        --
                        AND gd.cod_group = TRIM(p_group_id)
                        AND gd.cod_group_cycle = p_group_cycle) g
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = g.cod_acct_no
                  AND a.cod_cust_id = c.cod_cust_id
                  AND a.cod_prod = pm.cod_prod
                  AND ast.cod_acct_status = a.cod_acct_stat
                  AND ast.cod_module = 'LN'
                  AND c.cod_cust_id = cd.cod_cust_id(+)
            ORDER BY 1;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_group_mig_balance(p_group_id VARCHAR2,
                                    p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_migration_date DATE := report_methods.get_migration_date;
  BEGIN
       OPEN c_result FOR
            SELECT v_migration_date migration_date,
                   SUM(NVL(ab.amt_princ_balance, 0)) principal_outstanding,
                   SUM(NVL(ab.amt_rpa_balance, 0)) rpa_balance,
                   SUM(NVL(t.pricipal_arrears, 0)) pricipal_arrears,
                   SUM(NVL(t.interest_arrears, 0)) interest_arrears,
                   SUM(NVL(t.penalty_arrears, 0)) penalty_arrears,
                   SUM(NVL(ab.amt_princ_balance, 0) +
                       NVL(t.interest_arrears, 0) +
                       NVL(t.penalty_arrears, 0) -
                       NVL(ab.amt_rpa_balance, 0)) balance_bf_by_conv
            FROM conv_ln_acct_dtls a,
                 conv_ln_acct_balances ab,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        --
                        AND gd.cod_group = TRIM(p_group_id)
                        AND gd.cod_group_cycle = p_group_cycle) g,
                 (SELECT lat.cod_acct_no,
                         SUM(DECODE(lat.cod_arrear_type, 'C', lat.amt_arrears_due, 0)) pricipal_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'I', lat.amt_arrears_due, 0)) interest_arrears,
                         SUM(DECODE(lat.cod_arrear_type, 'A', lat.amt_arrears_due, 0)) penalty_arrears
                  FROM conv_ln_arrears_table lat
                  GROUP BY lat.cod_acct_no) t
            WHERE ab.cod_acct_no(+) = a.cod_acct_no
                  AND t.cod_acct_no(+) = a.cod_acct_no
                  AND g.cod_acct_no = a.cod_acct_no;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_group_payment_schedule(p_group_id VARCHAR2,
                                         p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ROWNUM payment_no,
                   ps.*
            FROM (SELECT t.payment_date,
                         SUM(t.principal_balance) principal_balance,
                         SUM(t.principal_amount) principal_amount,
                         SUM(t.interest_amount) interest_amount,
                         SUM(t.payment_amount) payment_amount
                  FROM (SELECT DISTINCT sd.cod_acct_no,
                               TRUNC(sd.date_instal) payment_date,
                               sd.amt_princ_bal principal_balance,
                               sd.amt_principal principal_amount,
                               sd.amt_interest interest_amount,
                               (sd.amt_principal + sd.amt_interest) payment_amount
                        FROM ln_acct_schedule_detls sd,
                             ln_acct_dtls a,
                             (SELECT DISTINCT gd.cod_group,
                                     gd.nam_group,
                                     gd.cod_group_cycle,
                                     gd.cod_acct_no
                              FROM ba_group_details gd
                              WHERE gd.flg_group_acc_attr = 'A'
                                    --
                                    AND gd.cod_group = TRIM(p_group_id)
                                    AND gd.cod_group_cycle = p_group_cycle) g
                        WHERE sd.cod_acct_no = g.cod_acct_no
                              AND a.cod_acct_no = g.cod_acct_no) t
                  GROUP BY t.payment_date
                  ORDER BY 1) ps;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_group_payments_made(p_group_id VARCHAR2,
                                      p_group_cycle NUMBER,
                                      p_date DATE,
                                      p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT trim(g.cod_group) group_id,
                   g.cod_group_cycle group_cycle,
                   TRUNC(th.dat_value) value_date,
                   SUM(decode(th.cod_arrear_type, 'C', th.amt_txn_acy, 0)) principal,
                   SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) interest,
                   SUM(decode(th.cod_arrear_type, 'F', th.amt_txn_acy, 0)) fee,
                   SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) penalty,
                   SUM(CASE WHEN th.cod_arrear_type NOT IN ('C','I','N','F','A','L')
                            THEN th.amt_txn_acy
                            ELSE 0
                       END) other,
                   SUM(th.amt_txn_acy) total
            FROM ln_arrear_txn_hist th,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        --
                        AND gd.cod_group = TRIM(p_group_id)
                        AND gd.cod_group_cycle = p_group_cycle) g
            WHERE th.cod_acct_no = g.cod_acct_no
                  AND th.cod_drcr = 'C'
                  --
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(th.dat_value),
                             'tran_date', TRUNC(th.dat_txn),
                             'post_date', TRUNC(th.dat_post)) <= p_date
            GROUP BY trim(g.cod_group),
                     g.cod_group_cycle,
                     TRUNC(th.dat_value)
            ORDER BY 2;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_not_posted_txn(p_group_id VARCHAR2,
                                 p_group_cycle NUMBER,
                                 p_date DATE,
                                 p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT rc.tim_sys entr_date,
                   rc.dat_val value_date,
                   rc.cod_tran tran_code,
                   rc.amt_tran_tcye amount_tcy,
                   trim(ct.nam_ccy_short) tcy,
                   rc.amt_tran_acye amount_acy,
                   trim(ca.nam_ccy_short) acy,
                   (rc.cod_tran || ': ' || rc.buf_stat_scr) tran_desc,
                   rc.user_id,
                   rc.id_auth auth_id,
                   (CASE WHEN rc.cod_tran_stat = 32 THEN 'Authorization requested'
                         WHEN rc.cod_tran_stat IN(33,97) THEN 'Cancelled'
                         WHEN rc.cod_tran_stat = 96 THEN 'Authorization granted'
                         WHEN rc.cod_tran_stat = 128 THEN 'Submitted to host'
                         ELSE 'Code: ' || rc.cod_tran_stat
                   END) tran_state
            FROM v_rec_txnlog rc,
                 ba_ccy_code ct,
                 ba_ccy_code ca,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        --
                        AND gd.cod_group = TRIM(p_group_id)
                        AND gd.cod_group_cycle = p_group_cycle) g
            WHERE NVL(rc.amt_tran_tcye, 0) != 0
                  AND ca.cod_ccy = rc.cod_acct_cur
                  AND ct.cod_ccy = rc.cod_tran_cur
                  --
                  AND rc.id_acct = g.cod_acct_no
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(rc.dat_val),
                             'tran_date', TRUNC(rc.dat_sys),
                             'post_date', TRUNC(rc.dat_post)) = TRUNC(p_date)
            ORDER BY tran_state,
                     rc.tim_sys;

       RETURN c_result;
  END;
  
  FUNCTION ln1021_group_balance(p_group_id VARCHAR2,
                                p_group_cycle NUMBER,
                                p_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT --a.cod_acct_no account_no,
                   ab.amt_disbursed group_amount,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                         THEN 0
                         ELSE (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                    END) principal_outstanding,
                   --ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_paid,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                            THEN 0
                            ELSE NVL(ar.principal_due, 0)
                       END) principal_due,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                            THEN 0
                            ELSE NVL(ar.interest_due, 0)
                       END) interest_due,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                            THEN 0
                            ELSE NVL(ar.principal_arrears, 0)
                       END) principal_arrears,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                            THEN 0
                            ELSE NVL(ar.interest_arrears, 0)
                       END) interest_arrears,
                   (CASE WHEN NVL(ar.principal_arrears, 0) > 0
                            THEN report_methods.get_arrears_in_days(p_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule, 0))
                            ELSE 0
                       END) arrears_in_days,
                   (CASE WHEN a.cod_acct_stat IN (1,5,11)
                            THEN 0
                            ELSE NVL(ar.penalty_billed, 0)
                       END) penalty_billed
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 conv_ln_acct_balances cab,
                 conv_ln_acct_int_balance_dtls caib,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        --
                        AND gd.cod_group = TRIM(p_group_id)
                        AND gd.cod_group_cycle = p_group_cycle) g,
                 (SELECT rs1.cod_acct_no cod_acct_no,
                         SUM(CASE WHEN rs1.date_instal < p_date THEN rs1.amt_principal ELSE 0 END) principal_due_by_schedule
                  FROM ln_acct_schedule_detls rs1
                  GROUP BY rs1.cod_acct_no) rs,
                 (SELECT th.cod_acct_no cod_acct_no,
                         SUM(CASE WHEN th.cod_arrear_type = 'C'
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_due,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_due,
                         SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N') AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) penalty_billed
                  FROM ln_arrear_txn_hist th
                  WHERE th.dat_value <= p_date
                  GROUP BY th.cod_acct_no) ar,
                 (SELECT dtl.cod_acct_no,
                         SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                  FROM rpt_ln_daily_txn_log_po dtl
                  WHERE dtl.dat_value <= p_date
                  GROUP BY dtl.cod_acct_no) tl
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = cab.cod_acct_no(+)
                  AND a.cod_acct_no = caib.cod_acct_no(+)
                  AND a.cod_acct_no = tl.cod_acct_no
                  AND a.cod_acct_no = rs.cod_acct_no(+)
                  AND a.cod_acct_no = ar.cod_acct_no(+)
                  --
                  AND a.cod_acct_no = g.cod_acct_no;

       RETURN c_result;
  END;
  -- LN1021 - Group account statement

  -- LN1030 - Loan disbursement detailed report
  -- LN1040 - Loan disbursement aggregated report
  FUNCTION ln1030(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_report_ccy VARCHAR2 := 'AZN')
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT vbr.branch_id,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   vbr.region_id,
                   vbr.region_name,
                   up.user_name_and_id credit_officer,
                   up.cs_user_name_and_id credit_supervisor,
                   a.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   a.cod_acct_no account_no,
                   a.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   cl.customer_cycle,
                   c.txt_cust_sex gender_id,
                   (CASE WHEN c.txt_cust_sex = 'M' THEN 'Male'
                         WHEN c.txt_cust_sex = 'F' THEN 'Female'
                   END) gender,
                   TRUNC(c.dat_birth_cust) customer_birth_date,
                   TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) customer_age,
                   c.flg_cust_typ customer_type_id,
                   ct.txt_cust_typ customer_document_type,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   TRUNC(a.dat_first_disb) disb_date,
                   TRIM(lp.lkc_desc) loan_purpose,
                   a.rat_var_current loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months/30) 
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367/4)
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536)
                         ELSE '0'
                    END) loan_term_in_month,
                   a.dat_of_maturity maturity_date,
                   ab.amt_net_disbursed amount_acy,
                   pm.nam_ccy_short currency_code,
                   (ab.amt_net_disbursed * cy.currency_rate) amount_lcy,
                   (ab.amt_net_disbursed * DECODE(p_report_ccy, 'AZN', cy.currency_rate, cru.currency_rate)) amount_rcy,
                   TRIM(bt.txt_business) business_type,
                   DECODE(c.cod_mis_cust_code_3, 1, 'Urban', 2, 'Semi-urban', 3, 'Rural-client') customer_type
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 or_applications ap,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ln_acct_attributes aa,
                 ci_cust_types ct,
                 ci_business_types bt,
                 or_lookups lp,
                 mv_customer_cycle_list cl,
                 mv_currency_rates_for_usd cru,
                 v_branches_and_regions vbr,
                 v_user_profile up,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT cr.cod_ccy currency_no,
                         cr.rat_ccy_cb currency_rate
                  FROM ba_ccy_rate cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                        FROM ba_ccy_rate cc
                        WHERE cc.dat_tim_rate_eff <= p_end_date
                        GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) cy,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                              b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE ab.cod_acct_no = a.cod_acct_no
                  AND aa.cod_acct_no = a.cod_acct_no
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND ap.app_acc_nbr(+) = a.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND cl.cod_cust_id(+) = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND vbr.branch_id = up.user_branch_id
                  AND up.user_no = aa.cod_acct_off
                  AND ct.flg_cust_typ = c.flg_cust_typ
                  AND bt.cod_business_cat(+) = c.cod_business_cat
                  AND lp.lkc_code(+) = ap.app_purpose_cd
                  AND lp.lkc_lkt_type(+) = 'APP_PURPOSE_CD'
                  AND cy.currency_no = a.cod_ccy
                  AND cru.currency_no = a.cod_ccy
                  AND ab.amt_disbursed > 0
                  --
                  AND INSTR(NVL(br.branch_id_list, vbr.branch_id), vbr.branch_id, 1) > 0
                  AND a.dat_first_disb BETWEEN TRUNC(p_start_date) AND TRUNC(p_end_date)
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND aa.cod_acct_off = NVL(p_user_no, aa.cod_acct_off)
            ORDER BY 2, 5, 8, group_name,
                     a.dat_first_disb, 11;

       RETURN c_result;
  END;
  -- LN1030 - Loan disbursement detailed report
  -- LN1040 - Loan disbursement aggregated report

  -- LN1031 - Loans pending branch disbursement report
  FUNCTION ln1031(p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT a.dat_first_disb host_disb_date,
                   TRIM(pm.nam_product) product_name,
                   TRIM(g.cod_group) group_id,
                   TRIM(g.nam_group) group_name,
                   TRIM(a.cod_acct_no) account_no,
                   c.cod_cust_id customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   --report_methods.get_client_cycle(a.cod_cust_id, SYSDATE) client_cycle,
                   pm.nam_ccy_short account_currency,
                   a.amt_face_value loan_amount
              FROM ln_acct_dtls@host_link a,
                   ln_prod_mast@host_link pm,
                   ci_custmast@host_link c,
                   ba_bank_mast@host_link bb,
                   xf_ol_st_txnlog_current@host_link tc,
                   ln_acct_attributes@host_link aa,
                   (SELECT DISTINCT gd.cod_group,
                           gd.nam_group,
                           gd.cod_group_cycle,
                           gd.cod_acct_no
                    FROM ba_group_details@host_link gd
                    WHERE gd.flg_group_acc_attr = 'A') g
              WHERE a.cod_prod = pm.cod_prod
                    AND a.cod_cust_id = c.cod_cust_id
                    AND g.cod_acct_no(+) = a.cod_acct_no
                    AND TRUNC(a.dat_first_disb) = TRUNC(bb.dat_process)
                    AND bb.cod_bank = '999'
                    AND tc.cod_txn_mnemonic NOT IN (1003,1011,1413,1085)
                    AND a.cod_acct_no = tc.cod_acct_no
                    AND aa.cod_acct_no = a.cod_acct_no
                    --
                    AND INSTR(NVL(p_branch_id, TRIM(a.cod_cc_brn)), TRIM(a.cod_cc_brn)) > 0
                    AND aa.cod_acct_off = NVL(p_user_no, aa.cod_acct_off)
              ORDER BY a.cod_acct_no;

       RETURN c_result;
  END;
  -- LN1031 - Loans pending branch disbursement report

  -- LN1050 - Loan repayment detailed report
  -- LN1060 - Loan repayment aggregated report
  FUNCTION ln1050(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT tl.cod_cc_brn_txn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   tl.dat_txn tran_date,
                   TRUNC(tl.dat_value) value_date,
                   tl.cod_acct_no account_no,
                   (CASE WHEN tl.cod_txn_mnemonic IN(3014, 3082) THEN 'Principal'
                         WHEN tl.cod_txn_mnemonic IN(3083,3084) THEN 'Interest'
                         WHEN tl.cod_txn_mnemonic IN(3085,3086) THEN 'Fee'
                         WHEN tl.cod_txn_mnemonic IN(3089,3091) THEN 'Penalty'
                         WHEN tl.cod_txn_mnemonic IN(3087,3088,3092,3093,3094,3095,3096,3097) THEN 'Other'
                    END) tran_type,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || TRIM(pm.nam_product)) product_name,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                   TRIM(tl.ref_txn_no) tran_code,
                   tl.amt_txn_tcy tran_amount_tcy,
                   cc.nam_ccy_short tcy,
                   tl.amt_txn_lcy tran_amount_lcy,
                   'AZN' lcy,
                   trim(tl.txt_txn_desc) tran_desc,
                   TRIM(up.nam_user) teller,
                   TRIM(g.cod_group) group_id,
                   (TRIM(g.cod_group) || ' - ' || TRIM(g.nam_group)) group_name
            FROM ln_acct_dtls a,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ln_daily_txn_log_hist/*@host_link*/ tl,
                 ci_custmast/*@host_link*/ c,
                 sm_user_profile/*@host_link*/ up,
                 ba_ccy_code/*@host_link*/ cc,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                             b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_cc_brn = bm.cod_cc_brn
                  AND a.cod_prod = pm.cod_prod
                  AND tl.flg_drcr = 'D'
                  AND tl.cod_txn_mnemonic IN(3014, 3082,3083,3084,3085,3086,3089,3091,3087,3088,3092,3093,3094,3095,3096,3097)
                  AND a.cod_acct_no = tl.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND up.cod_userno = tl.cod_userno
                  AND cc.cod_ccy = tl.cod_ccy_txn
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  --
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(tl.dat_value),
                             'tran_date', TRUNC(tl.dat_txn),
                             'post_date', TRUNC(tl.dat_post)) BETWEEN TRUNC(p_start_date) AND TRUNC(p_end_date)
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  --AND a.cod_cc_brn = nvl(br.branch_id, a.cod_cc_brn)
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND tl.cod_userno = nvl(p_user_no, tl.cod_userno)
            ORDER BY 2, 5, 4;

       RETURN c_result;
  END;
  -- LN1050 - Loan repayment detailed report
  -- LN1060 - Loan repayment aggregated report

  -- LN1070 - Loan repayment projection detailed report
  -- LN1080 - Loan repayment projection aggregated report
  FUNCTION ln1070(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_cc_brn branch_id,
                   bm.nam_branch branch_name,
                   t.schedule_date,
                   t.payment_no,
                   a.cod_acct_no account_no,
                   TRIM(pm.cod_prod) product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   c.customer_id,
                   (TRIM(c.customer_id) || ' - ' || c.customer_name) customer_name,
                   (up.nam_user || ' (' || up.cod_user_id || ')') credit_officer,
                   g.cod_group group_id,
                   DECODE (g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   tt.currency_rate milli_bank_rate,
                   t.principal,
                   t.interest,
                   (t.principal + t.interest) total,
                   SUM(t.principal + t.interest) OVER(PARTITION BY g.cod_group, g.cod_group_cycle) group_total,
                   t.arrears,
                   ab.amt_rpa_balance rpa_balance,
                   (t.principal * tt.currency_rate) principal_loc,
                   (t.interest * tt.currency_rate) interest_loc,
                   ((t.principal + t.interest) * tt.currency_rate) total_loc,
                   (t.arrears * tt.currency_rate) arears_loc,
                   pm.nam_ccy_short currency_code,
                   'AZN' currency_code_loc,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   a.amt_face_value loan_amount,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) term,
                   cl.customer_cycle,
                   c.home_address,
                   c.home_phone,
                   c.mobile_phone,
                   c.business_address,
                   c.business_phone/*,
                   c.customer_employee*/
            FROM ln_acct_dtls a,
                 ln_prod_mast pm,
                 ba_cc_brn_mast bm,
                 ln_acct_attributes aa,
                 sm_user_profile up,
                 ln_acct_balances ab,
                 mv_customer_cycle_list cl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT cm.cod_cust_id customer_id,
                         cm.nam_cust_full customer_name,
                         TRIM(cm.nam_custadr_city || ', ' || cm.txt_custadr_add1) home_address,
                         TRIM(cm.ref_res_phone_country || ' ' || cm.ref_res_phone_area || ' ' || cm.ref_cust_phone) home_phone,
                         cm.ref_phone_mobile mobile_phone,
                         TRIM(cd.nam_empadr_cntry || ', ' || cd.nam_empadr_city || ', ' || cd.txt_empadr_add1) business_address,
                         TRIM(cd.ref_cust_emp_phone_country || ' ' || cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) business_phone
                  FROM ci_custmast cm,
                       ci_custdetl cd
                  WHERE cd.cod_cust_id(+) = cm.cod_cust_id) c,
                 (SELECT t1.cod_acct_no,
                         t1.ctr_instal payment_no,
                         t1.date_instal schedule_date,
                         t1.amt_principal principal,
                         t1.amt_interest interest,
                         NVL(t2.arrears, 0) arrears
                  FROM ln_acct_schedule_detls t1,
                      (SELECT tt1.amt_arrears_princ arrears,
                              tt2.cod_acct_no,
                              tt2.min_date_instal
                       FROM ln_acct_balances tt1,
                            (SELECT s.cod_acct_no,
                                    MIN(s.date_instal) min_date_instal
                             FROM ln_acct_schedule_detls s
                             GROUP BY s.cod_acct_no) tt2
                       WHERE tt1.cod_acct_no = tt2.cod_acct_no) t2
                 WHERE t1.date_instal BETWEEN p_start_date AND p_end_date
                       AND t1.cod_acct_no = t2.cod_acct_no(+)
                       AND t1.date_instal = t2.min_date_instal(+)) t,
                 (SELECT cr.cod_ccy currency_no,
                         cr.rat_ccy_cb currency_rate
                  FROM ba_ccy_rate cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                        FROM ba_ccy_rate cc
                        GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) tt,
               (SELECT NVL((SELECT TRIM(rs.setting_value)
                                  FROM rpt_report_settings rs
                                  WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                                b.branch_id) branch_id_list
                      FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM DUAL) b) br
            WHERE t.cod_acct_no = a.cod_acct_no
                  AND aa.cod_acct_no = a.cod_acct_no
                  AND ab.cod_acct_no = a.cod_acct_no
                  --AND bm.cod_cc_brn = a.cod_cc_brn
                  AND up.cod_cc_brn = bm.cod_cc_brn
                  AND c.customer_id = a.cod_cust_id
                  AND cl.cod_cust_id = a.cod_cust_id
                  AND tt.currency_no = a.cod_ccy
                  AND a.cod_prod = pm.cod_prod
                  AND up.cod_userno = aa.cod_acct_off
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND a.cod_acct_stat IN(2, 8)
                  --
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  --AND up.cod_cc_brn = nvl(br.branch_id, up.cod_cc_brn)
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND aa.cod_acct_off = NVL(p_user_no, aa.cod_acct_off)
            ORDER BY 3, 2, 12, 5, 4;

       RETURN c_result;
  END;
  -- LN1070 - Loan repayment projection detailed report
  -- LN1080 - Loan repayment projection aggregated report
  
  -- LN1071 - Loan repayment projection detailed report (accounts)
  FUNCTION ln1071(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT bm.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   a.cod_acct_no account_no,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   up.nam_user credit_officer,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   cr.rat_ccy_cb milli_bank_rate,
                   t1.principal,
                   t1.interest,
                   ab.amt_arrears_princ arrears,
                   ab.amt_rpa_balance rpa_balance,
                   (t1.principal * cr.rat_ccy_cb) principal_loc,
                   (t1.interest * cr.rat_ccy_cb) interest_loc,
                   (ab.amt_arrears_princ * cr.rat_ccy_cb) arears_loc,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   --'' total,
                   pm.nam_ccy_short currency_code,
                   'AZN' currency_code_loc,
                   (/*cm.nam_custadr_cntry || ', ' || */c.nam_custadr_city || ', ' || c.txt_custadr_add1) home_address,
                   (c.ref_res_phone_country || ' ' || c.ref_res_phone_area || ' ' || c.ref_cust_phone) home_phone,
                   (c.ref_phone_mobile) mobile_phone
            FROM ln_acct_dtls a,
                 ln_prod_mast pm,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 ba_cc_brn_mast bm,
                 ci_custmast c,
                 ba_ccy_rate cr,
                 sm_user_profile up,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT sd.cod_acct_no account_no,
                         SUM(sd.amt_principal) principal,
                         SUM(sd.amt_interest) interest
                  FROM ln_acct_schedule_detls sd
                  WHERE TRUNC(sd.date_instal) BETWEEN TRUNC(p_start_date) AND TRUNC(p_end_date)
                  GROUP BY sd.cod_acct_no) t1,
                 (SELECT cc.cod_ccy currency_code,
                         MAX(cc.dat_tim_rate_eff) max_date
                  FROM ba_ccy_rate cc
                  GROUP BY cc.cod_ccy) t2,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_prod = pm.cod_prod
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND up.cod_userno = aa.cod_acct_off
                  AND a.cod_acct_no = ab.cod_acct_no
                  AND t1.account_no/*(+)*/ = a.cod_acct_no
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND a.cod_cust_id = c.cod_cust_id
                  AND cr.cod_ccy = a.cod_ccy
                  AND cr.dat_tim_rate_eff = t2.max_date
                  AND cr.cod_ccy = t2.currency_code
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  --
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND aa.cod_acct_off = NVL(p_user_no, aa.cod_acct_off)
             ORDER BY 2, 3;

       RETURN c_result;
  END;
  -- LN1071 - Loan repayment projection detailed report (accounts)

  -- LN1072 - Repayment projection for future funds flow
  FUNCTION ln1072(p_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT br.region_name,
                   br.branch_name,
                   t.ccy,
                   t.product_name,
                   SUM(DECODE(t.month_count, 1, t.principal_due, 0)) principal_due_1,
                   SUM(DECODE(t.month_count, 2, t.principal_due, 0)) principal_due_2,
                   SUM(DECODE(t.month_count, 3, t.principal_due, 0)) principal_due_3,
                   SUM(DECODE(t.month_count, 4, t.principal_due, 0)) principal_due_4,
                   SUM(DECODE(t.month_count, 5, t.principal_due, 0)) principal_due_5,
                   SUM(DECODE(t.month_count, 6, t.principal_due, 0)) principal_due_6,
                   SUM(DECODE(t.month_count, 7, t.principal_due, 0)) principal_due_7,
                   SUM(DECODE(t.month_count, 8, t.principal_due, 0)) principal_due_8,
                   SUM(DECODE(t.month_count, 9, t.principal_due, 0)) principal_due_9,
                   SUM(DECODE(t.month_count, 10, t.principal_due, 0)) principal_due_10,
                   SUM(DECODE(t.month_count, 11, t.principal_due, 0)) principal_due_11,
                   SUM(DECODE(t.month_count, 12, t.principal_due, 0)) principal_due_12,
                   SUM(CASE WHEN t.month_count BETWEEN 13 AND 18 THEN t.principal_due ELSE 0 END) principal_due_13_18,
                   SUM(CASE WHEN t.month_count BETWEEN 19 AND 24 THEN t.principal_due ELSE 0 END) principal_due_19_24,
                   SUM(CASE WHEN t.month_count > 24 THEN t.principal_due ELSE 0 END) principal_due_over_24,
                   SUM(t.principal_due) principal_due_total
            FROM v_branches_and_regions br,
                 (SELECT r.*,
                         (r.principal_amount + DECODE(r.month_count, r.min_month_count, (r.principal_balance - r.principal_due_by_schedule), 0)) principal_due
                  FROM (SELECT a.cod_cc_brn branch_id,
                               --a.cod_acct_no,
                               pm.nam_ccy_short ccy,
                               NVL(ps.month_count, 1) month_count,
                               MIN(NVL(ps.month_count, 1)) OVER(PARTITION BY a.cod_acct_no) min_month_count,
                               (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                               NVL(SUM(ps.principal_amount), 0) principal_amount,
                               SUM(tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_balance,
                               SUM(NVL(SUM(ps.principal_amount), 0)) OVER(PARTITION BY a.cod_acct_no) principal_due_by_schedule
                        FROM ln_prod_mast pm,
                             ln_acct_dtls a,
                             ln_acct_balances ab,
                             conv_ln_acct_balances cab,
                             conv_ln_acct_int_balance_dtls caib,
                             (SELECT sd.cod_acct_no,
                                     CEIL(MONTHS_BETWEEN(sd.date_instal, p_date)) month_count,
                                     SUM(sd.amt_principal) principal_amount
                              FROM ln_acct_schedule_detls sd
                              WHERE sd.date_instal >= p_date
                              GROUP BY sd.cod_acct_no,
                                       CEIL(MONTHS_BETWEEN(sd.date_instal, p_date))
                              HAVING SUM(sd.amt_principal) > 0) ps,
                             (SELECT dtl.cod_acct_no,
                                     SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                              FROM rpt_ln_daily_txn_log_po dtl
                              WHERE dtl.dat_value <= p_date
                              GROUP BY dtl.cod_acct_no) tl
                        WHERE a.cod_acct_no = ab.cod_acct_no
                              AND a.cod_acct_no = tl.cod_acct_no
                              AND a.cod_acct_no = ps.cod_acct_no(+)
                              AND a.cod_acct_no = cab.cod_acct_no(+)
                              AND a.cod_acct_no = caib.cod_acct_no(+)
                              AND pm.cod_prod = a.cod_prod
                              --
                              AND ab.amt_disbursed > 0
                              AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                        GROUP BY a.cod_cc_brn,
                                 pm.nam_ccy_short,
                                 (TRIM(pm.cod_prod) || ' - ' || pm.nam_product),
                                 a.cod_acct_no,
                                 ps.month_count) r) t
            WHERE t.branch_id = br.branch_id
            GROUP BY br.region_name,
                     br.branch_name,
                     t.ccy,
                     t.product_name
            ORDER BY 1, 2, 3;

       RETURN c_result;
  END;
  -- LN1072 - Repayment projection for future funds flow

  -- LN1090 - Loan portfolio outstanding detailed report
  FUNCTION ln1090(p_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER,
                  p_report_ccy VARCHAR2 := 'AZN',
                  p_fields VARCHAR2 := 'X')
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT DISTINCT vbr.region_id,
                   vbr.region_name,
                   vbr.branch_id,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   a.dat_first_disb first_disb_date,
                   a.dat_last_disb last_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   c.cod_employee_id employee_id,
									          DECODE(vbr.branch_id,998,(CASE WHEN SYSDATE > NVL(hu.date_terminated, SYSDATE) THEN 'N' ELSE 'Y' END), NULL) employee_status,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'AL', p_fields), 'AL'),
                          0, NULL,
                          DECODE(hu.branch_id, NULL, NULL, hu.branch_name)) branch_name_emp,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'CO', p_fields), 'CO'),
                          0, NULL,
                          up.user_name_and_id) credit_officer,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'CS', p_fields), 'CS'),
                          0, NULL,
                          up.cs_user_name_and_id) credit_supervisor,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'GN', p_fields), 'GN'),
                          0, NULL,
                          g.cod_group) group_id,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'GN', p_fields), 'GN'),
                          0, NULL,
                          g.nam_group) group_name,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'CN', p_fields), 'CN'),
                          0, NULL,
                          c.cod_cust_id) customer_id,
                   DECODE(INSTR(decode(p_fields, 'RPA', 'CN', p_fields), 'CN'),
                          0, NULL,
                          c.nam_cust_full) customer_name,
                   g.cod_group_cycle group_cycle,
                   NVL(ccl.customer_cycle, 1) customer_cycle,
                   DECODE(c.txt_cust_sex, 'M', 'Male', 'F', 'Female') gender,
                   c.txt_cust_sex gender_id,
                   TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) customer_age,
                   bt.txt_business business_type,
                   DECODE(c.cod_mis_cust_code_3, 1, 'Urban', 2, 'Semi-urban', 3, 'Rural-client') customer_type,
                   DECODE(c.cod_cust_stat, 0,	'Regular', 1,	'IDP', 2,	'Refugee') refugee_status,
									          (CASE WHEN NVL(m.dat_last_mnt, p_date) < p_date + 1 THEN DECODE(m.cod_mis_txn_3, 20, 'Temp', 'Normal') ELSE 'Normal' END) is_temp,
                   a.cod_acct_no account_no,
                   TRIM(report_methods.get_loan_term_in_month(pm.cod_prod_term_typ, a.ctr_term_months)) loan_term_in_month,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2, ' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   a.rat_var_current interest_rate,
                   NVL(rs.int_amt_by_schedule, 0) interest_amount,
                   ab.amt_disbursed loan_amt,
                   ROUND(ab.amt_disbursed * cy.currency_rate, 2) loan_amt_lcy,
                   (ab.amt_net_disbursed * cru.currency_rate) amount_usd,
                   pm.nam_ccy_short acy,
                   --NVL(cab.amt_princ_balance, 0) migrated_principal_balance,
                   ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))), 6) principal_outstanding,
                   ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cy.currency_rate, 6) principal_outstanding_lcy,
                   ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                         * decode(p_report_ccy, 'AZN', cy.currency_rate, cru.currency_rate), 6) principal_outstanding_rcy,
                   ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_paid,
                   NVL(ar.principal_arrears,0) principal_arrears,
                   (NVL(ar.principal_arrears,0) * cy.currency_rate) principal_arrears_lcy,
                   (NVL(ar.principal_arrears,0) * decode(p_report_ccy, 'AZN', cy.currency_rate, cru.currency_rate)) principal_arrears_rcy,
                   (CASE WHEN NVL(ar.principal_arrears, 0) > 0
                         THEN report_methods.get_arrears_in_days(p_date,
                                                                  a.cod_acct_no,
                                                                  ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                  NVL(rs.principal_due_by_schedule,0))
                         ELSE 0
                    END) arrears_in_days,
                   (CASE WHEN NVL(ar.principal_arrears, 0) > 0
                         AND report_methods.get_arrears_in_days(p_date,
                                                                 a.cod_acct_no,
                                                                 ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                 NVL(rs.principal_due_by_schedule,0))
                             > 0 THEN (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                         ELSE 0
                    END) par_amt,
                   (CASE WHEN NVL(ar.principal_arrears,0) > 0
                         AND report_methods.get_arrears_in_days(p_date,
                                                                 a.cod_acct_no,
                                                                 ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                 NVL(rs.principal_due_by_schedule,0))
                             > 0 THEN ((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cy.currency_rate)
                         ELSE 0
                    END) par_amt_lcy,
                   (CASE WHEN NVL(ar.principal_arrears,0) > 0
                         AND report_methods.get_arrears_in_days(p_date,
                                                                 a.cod_acct_no,
                                                                 ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                 NVL(rs.principal_due_by_schedule,0))
                             > 0 THEN ((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) *
                                       DECODE(p_report_ccy, 'AZN', cy.currency_rate, cru.currency_rate))
                         ELSE 0
                    END) par_amt_rcy,
                   nvl(ar.interest_arrears, 0) interest_arrears,
                   nvl(rs.future_interest, 0) future_interest,
                   ab.amt_rpa_balance current_rpa_balance,
                   nvl(ar.penalty_billed, 0) penalty_billed,
                   report_methods.calculate_penalty(NVL(ar.last_penalty_billing_date, p_date),
                                                    p_date,
                                                    NVL(ar.principal_penalized, 0)) penalty_unbilled,
                   aa.flg_negotiated,
                   rs.payment_date monthly_installment_date,
                   rs.payment_amount monthly_installment,
                   DECODE(pm.fee_type,
                          1, 4,
                          2, 2.5,
                          3, (CASE WHEN ab.amt_disbursed < 5000 THEN 4 WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 2 ELSE 0 END),
                          4, 2,--(CASE WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 3 WHEN ab.amt_disbursed >= 10000 THEN 2 ELSE 0 END),
                          5.1, 3,
                          5.2, (CASE WHEN (a.ctr_term_months/65536) > 12 THEN 4 ELSE 3 END),
                          0) fee_percent,
                   DECODE(pm.fee_type,
                          1, 4,
                          2, 2.5,
                          3, (CASE WHEN ab.amt_disbursed < 5000 THEN 4 WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 2 ELSE 0 END),
                          4, 2,--(CASE WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 3 WHEN ab.amt_disbursed >= 10000 THEN 2 ELSE 0 END),
                          5.1, 3,
                          5.2, (CASE WHEN (a.ctr_term_months/65536) > 12 THEN 4 ELSE 3 END),
                          0) * (ab.amt_disbursed * 0.01) fee_amount,
                   ROUND(DECODE(pm.fee_type,
                                1, 4,
                                2, 2.5,
                                3, (CASE WHEN ab.amt_disbursed < 5000 THEN 4 WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 2 ELSE 0 END),
                                4, 2,--(CASE WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 3 WHEN ab.amt_disbursed >= 10000 THEN 2 ELSE 0 END),
                                5.1, 3,
                                5.2, (CASE WHEN (a.ctr_term_months/65536) > 12 THEN 4 ELSE 3 END),
                                0) *
                         (ab.amt_disbursed * 0.01) *
                         -- (FLOOR(MONTHS_BETWEEN(LEAST(a.dat_of_maturity, p_date), a.dat_first_disb)) / report_methods.get_loan_term_in_month(pm.cod_prod_term_typ, a.ctr_term_months)), 6) fee_accrued,
                         (FLOOR(LEAST((LEAST(a.dat_of_maturity, p_date) - a.dat_first_disb), report_methods.get_loan_term_in_days(pm.cod_prod_term_typ, a.ctr_term_months))) / report_methods.get_loan_term_in_days(pm.cod_prod_term_typ, a.ctr_term_months)), 6) fee_accrued,
                   DECODE(pm.fee_type,
                          1, 4,
                          2, 2.5,
                          3, (CASE WHEN ab.amt_disbursed < 5000 THEN 4 WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 2 ELSE 0 END),
                          4, 2,--(CASE WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 3 WHEN ab.amt_disbursed >= 10000 THEN 2 ELSE 0 END),
                          5.1, 3,
                          5.2, (CASE WHEN (a.ctr_term_months/65536) > 12 THEN 4 ELSE 3 END),
                          0) * (ab.amt_disbursed * 0.01 * cru.currency_rate) fee_amount_usd,
                   ROUND(DECODE(pm.fee_type,
                                1, 4,
                                2, 2.5,
                                3, (CASE WHEN ab.amt_disbursed < 5000 THEN 4 WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 2 ELSE 0 END),
                                4, 2,--(CASE WHEN ab.amt_disbursed BETWEEN 5000 AND 9999 THEN 3 WHEN ab.amt_disbursed >= 10000 THEN 2 ELSE 0 END),
                                5.1, 3,
                                5.2, (CASE WHEN (a.ctr_term_months/65536) > 12 THEN 4 ELSE 3 END),
                                0) *
                         (ab.amt_disbursed * 0.01 * cru.currency_rate) *
                         -- (FLOOR(MONTHS_BETWEEN(LEAST(a.dat_of_maturity, p_date), a.dat_first_disb)) / report_methods.get_loan_term_in_month(pm.cod_prod_term_typ, a.ctr_term_months)), 6) fee_accrued,
                         (FLOOR(LEAST((LEAST(a.dat_of_maturity, p_date) - a.dat_first_disb), report_methods.get_loan_term_in_days(pm.cod_prod_term_typ, a.ctr_term_months))) / report_methods.get_loan_term_in_days(pm.cod_prod_term_typ, a.ctr_term_months)), 6) fee_accrued_usd,
                   DECODE(lac.num_1, 1, 'Y', 'N') expedited_loan,
                   DECODE(lac.num_1, 2, 'Y', 'N') start_up_loan,
                   DECODE(pl.account_no, NULL, 'N', 'Y') sysprob_loan,
                   COUNT(a.cod_acct_no) OVER (ORDER BY NULL) loan_count,
                   COUNT(DISTINCT c.cod_cust_id) OVER (PARTITION BY NULL) customer_count
            FROM ln_acct_dtls a,
                 ci_custmast c,
                 ci_business_types bt,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 ac_acct_writeoff wo,
                 conv_ln_acct_balances cab,
                 conv_ln_acct_int_balance_dtls caib,
                 ba_cust_prod_txn_mis_xref m,
                 ln_acct_cbr_dtls lac,
                 rpt_system_problem_loans pl,
                 mv_hr_users hu, -- employee info
                 mv_currency_rates_for_usd cru,
                 mv_customer_cycle_list ccl,
                 v_branches_and_regions vbr,
                 v_user_profile up,
                 v_ln_prod_mast pm,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT lc.account_no,
                         lc.change_date,
                         lc.user_no_old co_user_no
                  FROM mv_co_loan_changes lc,
                       (SELECT clc.account_no,
                               MIN(clc.change_date) min_date
                        FROM mv_co_loan_changes clc
                        WHERE clc.change_date > p_date
                        GROUP BY clc.account_no) lmd
                  WHERE lc.account_no = lmd.account_no
                        AND lc.change_date = lmd.min_date) mlc,
                 (SELECT rs1.cod_acct_no cod_acct_no,
                         --SUM(rs1.amt_principal) loan_amt_by_schedule1,
                         SUM(rs1.amt_interest) int_amt_by_schedule,
                         SUM(CASE WHEN rs1.date_instal < p_date THEN rs1.amt_principal ELSE 0 END) principal_due_by_schedule,
                         SUM(CASE WHEN rs1.date_instal > p_date THEN rs1.amt_interest ELSE 0 END) future_interest,
                         MAX(CASE WHEN rs1.date_instal <= p_date THEN rs1.date_instal ELSE NULL END) payment_date,
                         MAX(CASE WHEN rs1.date_instal <= p_date THEN (rs1.amt_principal + rs1.amt_interest) ELSE 0 END) payment_amount
                  FROM ln_acct_schedule_detls rs1
                  GROUP BY rs1.cod_acct_no) rs,
                 (SELECT cr.cod_ccy currency_no,
                         cr.rat_ccy_cb currency_rate
                  FROM ba_ccy_rate cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                       FROM ba_ccy_rate cc
                       WHERE cc.dat_tim_rate_eff <= p_date
                       GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) cy,
                 (SELECT th.cod_acct_no,
                         SUM(CASE WHEN th.cod_arrear_type = 'C'
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_arrears,
                         SUM(CASE WHEN th.cod_arrear_type = 'C' AND TRUNC(th.dat_value) < TRUNC(p_date)
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_penalized,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('A', 'L')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) penalty_billed,
                         MAX(CASE WHEN /*th.cod_arrear_type = 'C'
                                       AND th.cod_drcr = 'D'
                                       AND */TRUNC(th.dat_value) < TRUNC(p_date)
                                  THEN TRUNC(th.dat_value)
                                  ELSE NULL
                             END) last_penalty_billing_date
                  FROM ln_arrear_txn_hist th
                  WHERE th.dat_value <= p_date
                  GROUP BY th.cod_acct_no) ar,
                 (SELECT dtl.cod_acct_no,
                         SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                  FROM rpt_ln_daily_txn_log_po dtl
                  WHERE dtl.dat_value <= p_date
                  GROUP BY dtl.cod_acct_no) tl,
                 (SELECT NVL((SELECT TRIM(rst.setting_value)
                              FROM rpt_report_settings rst
                              WHERE rst.setting_key = ('REGION_' || b.branch_id)),
                             b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = tl.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_acct_no = rs.cod_acct_no(+)
                  AND a.cod_acct_no = ar.cod_acct_no(+)
                  AND a.cod_acct_no = wo.cod_acct_no(+)
                  AND a.cod_acct_no = cab.cod_acct_no(+)
                  AND a.cod_acct_no = caib.cod_acct_no(+)
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_acct_no = lac.cod_acct_no(+)
                  AND a.cod_acct_no = mlc.account_no(+)
                  AND a.cod_acct_no = pl.account_no(+)
                  AND a.cod_cust_id = c.cod_cust_id
                  AND c.cod_cust_id = ccl.cod_cust_id(+)
                  AND bt.cod_business_cat(+) = c.cod_business_cat
                  AND a.cod_prod = pm.cod_prod
                  AND up.user_branch_id = vbr.branch_id
                  AND cy.currency_no = a.cod_ccy
                  AND cru.currency_no = a.cod_ccy
                  AND up.user_no = nvl(mlc.co_user_no, aa.cod_acct_off)
                  AND c.cod_employee_id = hu.employee_id(+)
                  AND m.cod_prod_mnem_cust(+) = a.cod_acct_no
                  AND m.cod_mis_txn_3(+) = 20
                  AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                  AND ab.amt_disbursed > 0
                  --
                  --AND NVL(a.dat_acct_close, p_date + 1) > p_date
                  --AND NVL(wo.dat_writeoff, p_date + 1) > p_date
                  --AND a.dat_first_disb <= p_date
                  AND INSTR(NVL(br.branch_id_list, vbr.branch_id), vbr.branch_id, 1) > 0
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND nvl(mlc.co_user_no, aa.cod_acct_off) = NVL(p_user_no, nvl(mlc.co_user_no, aa.cod_acct_off))
                  AND DECODE(p_fields,
                             'RPA', ((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) + nvl(rs.future_interest, 0)),
                             -1) <= DECODE(p_fields, 'RPA', ab.amt_rpa_balance, -1)
            ORDER BY 2, 4, 12, 15;

       RETURN c_result;
  END;
  -- LN1090 - Loan portfolio outstanding detailed report

  -- LN1091 - PO for autoloan
  FUNCTION ln1091(p_date DATE,
                  p_product_id NUMBER)
  RETURN result_cursor
  IS
  BEGIN
       RETURN ln1090(p_date, 998, p_product_id, NULL, 'AZN', ''); -- AL - auto loan
  END;

  -- LN1110 - Aging of arrears
  FUNCTION ln1110(p_date DATE,
                  p_branch_id NUMBER,
                  p_product_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   --a.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   TRUNC(a.dat_last_disb) last_disb_date,
                   (up.nam_user || ' (' || up.cod_user_id || ')') credit_officer,
                   a.cod_acct_no account_no,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   --trim(ct.txt_cust_typ) document_type_name,
                   (CASE WHEN c.txt_cust_sex = 'F' THEN 'Female'
                         WHEN c.txt_cust_sex = 'M' THEN 'Male'
                    END) gender,
                   --trunc(c.dat_birth_cust) customer_dob,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   a.amt_face_value loan_amount,
                   (a.amt_face_value * cy.currency_rate) loan_amount_lcy,
                   (a.amt_face_value * cru.currency_rate) amount_usd,
                   TRIM(pm.nam_ccy_short) acy,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   a.rat_var_current interest_rate,
                   --t.principal_balance,
                   t.principal_paid,
                   t.principal_outstanding,
                   t.principal_arrears,
                   t.princ_arrears_1_7,
                   t.princ_arrears_8_30,
                   t.princ_arrears_31_60,
                   t.princ_arrears_61_90,
                   t.princ_arrears_91_180,
                   t.princ_arrears_181_365,
                   t.princ_arrears_over_365
                   /*(t.principal_balance * cy.currency_rate) principal_balance_lcy,
                   (t.total_arrears* cy.currency_rate) total_arrears_lcy,
                   (t.arrears_1_7 * cy.currency_rate) arrears_1_7_lcy,
                   (t.arrears_8_30 * cy.currency_rate) arrears_8_30_lcy,
                   (t.arrears_31_60 * cy.currency_rate) arrears_31_60_lcy,
                   (t.arrears_61_90 * cy.currency_rate) arrears_61_90_lcy,
                   (t.arrears_91_180 * cy.currency_rate) arrears_91_180_lcy,
                   (t.arrears_181_365 * cy.currency_rate) arrears_181_365_lcy,
                   (t.arrears_over_365 * cy.currency_rate) arrears_over_365_lcy,*/
            FROM ln_acct_dtls a,
                 ln_prod_mast pm,
                 ba_cc_brn_mast bm,
                 ci_custmast c,
                 ln_acct_attributes aa,
                 sm_user_profile up,
                 mv_currency_rates_for_usd cru,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT rs.cod_acct_no,
                         /*(CASE WHEN NVL(ar.principal_arrears, 0) > 0
                               THEN report_methods.get_arrears_in_days(p_date,
                                                                        rs.cod_acct_no,
                                                                        (NVL(cn.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)),
                                                                        NVL(rs.principal_due_by_schedule, 0))
                               ELSE 0
                          END) arrears_in_days,*/
                         (NVL(cn.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)) principal_paid,
                         NVL(rs.principal_amount, 0) - (NVL(cn.principal_paid_bfr_migration, 0) +
                            NVL(ar.principal_paid_aft_migration, 0)) principal_outstanding,
                         ar.principal_arrears,
                         (CASE WHEN ar.princ_arrears_1_7 < 0
                               THEN (ar.princ_arrears_1_7 + ar.princ_arrears_8_30 +
                                     ar.princ_arrears_31_60 + ar.princ_arrears_61_90 +
                                     ar.princ_arrears_91_180 + ar.princ_arrears_181_365 +
                                     ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_1_7
                          END) princ_arrears_1_7,
                         (CASE WHEN ar.princ_arrears_1_7 < 0
                               THEN 0
                               WHEN ar.princ_arrears_8_30 < 0
                               THEN (ar.princ_arrears_8_30 + ar.princ_arrears_31_60 +
                                     ar.princ_arrears_61_90 + ar.princ_arrears_91_180 +
                                     ar.princ_arrears_181_365 + ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_8_30
                          END) princ_arrears_8_30,
                         (CASE WHEN ar.princ_arrears_1_7 < 0 OR ar.princ_arrears_8_30 < 0
                               THEN 0
                               WHEN ar.princ_arrears_31_60 < 0
                               THEN (ar.princ_arrears_31_60 + ar.princ_arrears_61_90 +
                                     ar.princ_arrears_91_180 + ar.princ_arrears_181_365 +
                                     ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_31_60
                          END) princ_arrears_31_60,
                         (CASE WHEN ar.princ_arrears_1_7 < 0 OR ar.princ_arrears_8_30 < 0
                                    OR ar.princ_arrears_31_60 < 0
                               THEN 0
                               WHEN ar.princ_arrears_61_90 < 0
                               THEN (ar.princ_arrears_61_90 + ar.princ_arrears_91_180 +
                                     ar.princ_arrears_181_365 + ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_61_90
                          END) princ_arrears_61_90,
                         (CASE WHEN ar.princ_arrears_1_7 < 0 OR ar.princ_arrears_8_30 < 0
                                    OR ar.princ_arrears_31_60 < 0 OR ar.princ_arrears_61_90 < 0
                               THEN 0
                               WHEN ar.princ_arrears_91_180 < 0
                               THEN (ar.princ_arrears_91_180 + ar.princ_arrears_181_365 +
                                     ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_91_180
                          END) princ_arrears_91_180,
                         (CASE WHEN ar.princ_arrears_1_7 < 0 OR ar.princ_arrears_8_30 < 0
                                    OR ar.princ_arrears_31_60 < 0 OR ar.princ_arrears_61_90 < 0
                                    OR ar.princ_arrears_91_180 < 0
                               THEN 0
                               WHEN ar.princ_arrears_181_365 < 0
                               THEN (ar.princ_arrears_181_365 + ar.princ_arrears_over_365)
                               ELSE ar.princ_arrears_181_365
                          END) princ_arrears_181_365,
                         (CASE WHEN ar.princ_arrears_1_7 < 0 OR ar.princ_arrears_8_30 < 0
                                    OR ar.princ_arrears_31_60 < 0 OR ar.princ_arrears_61_90 < 0
                                    OR ar.princ_arrears_91_180 < 0 OR ar.princ_arrears_181_365 < 0
                               THEN 0
                               ELSE ar.princ_arrears_over_365
                          END) princ_arrears_over_365
                  FROM (SELECT lab.cod_acct_no cod_acct_no,
                               SUM(NVL(lab.amt_princ_balance, 0)) migrated_principal_balance,
                               SUM(NVL(lab.amt_net_disbursed, 0) - NVL(lab.amt_princ_balance, 0)) principal_paid_bfr_migration,
                               SUM(NVL(ibd.bal_int1_chg, 0)) interest_paid_bfr_migration
                        FROM conv_ln_acct_balances lab,
                             conv_ln_acct_int_balance_dtls ibd
                        WHERE lab.cod_acct_no = ibd.cod_acct_no(+)
                        GROUP BY lab.cod_acct_no) cn,
                       (SELECT sd.cod_acct_no cod_acct_no,
                               SUM(sd.amt_principal) principal_amount,
                               SUM(sd.amt_interest) interest_amount,
                               SUM(CASE WHEN sd.date_instal < p_date
                                        THEN sd.amt_principal
                                        ELSE 0
                                   END) principal_due_by_schedule,
                               SUM(CASE WHEN sd.date_instal < p_date
                                        THEN sd.amt_interest
                                        ELSE 0
                                   END) interest_due_by_schedule
                        FROM ln_acct_schedule_detls sd
                        GROUP BY sd.cod_acct_no) rs,
                       (SELECT th.cod_acct_no cod_acct_no,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.cod_drcr = 'C'
                                        THEN th.amt_txn_acy
                                        ELSE 0
                                   END) principal_paid_aft_migration,
                               SUM(CASE WHEN th.cod_arrear_type = 'C'
                                        THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                        ELSE 0
                                   END) principal_arrears,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-7 AND p_date THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_1_7,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-30 AND p_date-8 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_8_30,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-60 AND p_date-31 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_31_60,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-90 AND p_date-61 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_61_90,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-180 AND p_date-91 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_91_180,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value BETWEEN p_date-365 AND p_date-181 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_181_365,
                               SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value < p_date-365 THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy) ELSE 0 END) princ_arrears_over_365
                        FROM ln_arrear_txn_hist th
                        WHERE th.dat_value <= p_date
                        GROUP BY th.cod_acct_no) ar
                  WHERE ar.cod_acct_no(+) = rs.cod_acct_no
                        AND cn.cod_acct_no(+) = rs.cod_acct_no
                        AND ar.principal_arrears > 0) t,
                 (SELECT cr.cod_ccy currency_no,
                         cr.rat_ccy_cb currency_rate
                  FROM ba_ccy_rate/*@host_link*/ cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                        FROM ba_ccy_rate/*@host_link*/ cc
                        WHERE cc.dat_tim_rate_eff <= p_date
                        GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) cy,
               (SELECT NVL((SELECT TRIM(rs.setting_value)
                                  FROM rpt_report_settings rs
                                  WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                                b.branch_id) branch_id_list
                      FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_acct_no = t.cod_acct_no
                  AND aa.cod_acct_no = t.cod_acct_no
                  AND g.cod_acct_no(+) = t.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND a.cod_prod = pm.cod_prod
                  --AND c.flg_cust_typ = ct.flg_cust_typ
                  AND cy.currency_no = a.cod_ccy
                  AND cru.currency_no = a.cod_ccy
                  AND up.cod_userno = aa.cod_acct_off
                  AND up.cod_cc_brn = bm.cod_cc_brn
                  --
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  --AND a.cod_cc_brn = nvl(br.branch_id, a.cod_cc_brn)
                  AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                  AND aa.cod_acct_off = NVL(p_user_no, aa.cod_acct_off)
             ORDER BY 1, 5, 11;

       RETURN c_result;
  END;
  -- LN1110 - Aging of arrears

  -- LN1120 - Pre-aggregated loan portfolio outstanding report
  FUNCTION ln1120(p_branch_id NUMBER,
                  p_product_id NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT /*(*/br.branch_name/* || '  (' || br.branch_type || ')')*/ branch_name_type,
                   t.district,
                   t.us_sur_rur "urban\semi-urban\rural",
                   (SELECT (TRIM(p.cod_prod) || ' - ' || TRIM(p.nam_product))
                    FROM ln_prod_mast P
                    WHERE p.cod_prod = t.product_id) product_name,
                   t.economic_activity,
                   t.fund,
                   t.credit_officer_id,
                   t.loan_amount_lcy,
                   t.maturity_date,
                   t.loan_rate,
                   t.gender,
                   t.customer_age,
                   t.loan_amount_lcy_avg,
                   t.loan_rate_avg,
                   t.arrears_in_days_sum,
                   t.disb_amount_lcy_sum,
                   t.principal_outstanding_sum,
                   t.loan_count
            FROM (SELECT a.cod_cc_brn branch_id,
                         TRIM(c.nam_custadr_state) district,
                         '' us_sur_rur,
                         a.cod_prod product_id,
                         '' economic_activity,
                         '' fund,
                         c.cod_officr_id credit_officer_id,
                         --a.amt_face_value loan_amount,
                         (CASE WHEN (a.amt_face_value * cr.rat_ccy_cb) < 501 THEN 'up to 500'
                               WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 501 AND 1000 THEN '501 - 1000'
                               WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 1001 AND 5000 THEN '1001 - 5000'
                               WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 5001 AND 10000 THEN '5001 - 10000'
                               WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 10001 AND 20000 THEN '10001 - 20000'
                               WHEN (a.amt_face_value * cr.rat_ccy_cb) > 20000 THEN 'over 20000'
                          END) loan_amount_lcy,
                         a.dat_of_maturity maturity_date,
                         (CASE WHEN a.rat_var_current < 21 THEN 'up to 20 %'
                               WHEN a.rat_var_current BETWEEN 21 AND 25 THEN '21 - 25 %'
                               WHEN a.rat_var_current BETWEEN 26 AND 30 THEN '26 - 30 %'
                               WHEN a.rat_var_current BETWEEN 31 AND 35 THEN '31 - 35 %'
                               WHEN a.rat_var_current BETWEEN 36 AND 40 THEN '36 - 40 %'
                               WHEN a.rat_var_current > 40 THEN 'over 40 %'
                          END) loan_rate,
                         (CASE WHEN c.txt_cust_sex = 'F' THEN 'Female'
                               WHEN c.txt_cust_sex = 'M' THEN 'Male'
                          END) gender,
                         (CASE WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) < 21 THEN 'up to 20'
                               WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 21 AND 30 THEN '21 - 30'
                               WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 31 AND 40 THEN '31 - 40'
                               WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 41 AND 50 THEN '41 - 50'
                               WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) > 50 THEN 'over 50'
                          END) customer_age,
                         AVG(a.amt_face_value * cr.rat_ccy_cb) loan_amount_lcy_avg,
                         AVG(a.rat_var_current) loan_rate_avg,
                         SUM(report_methods.get_arrears_in_days(SYSDATE, a.cod_acct_no, dt.principal_paid, a.amt_face_value - dt.principal_paid)) arrears_in_days_sum,
                         --a.amt_face_value disb_amount,
                         SUM(a.amt_face_value * cr.rat_ccy_cb) disb_amount_lcy_sum,
                         --nvl(dt.principal_paid, 0) principal_paid,
                         SUM((a.amt_face_value * cr.rat_ccy_cb) - NVL(dt.principal_paid, 0)) principal_outstanding_sum,
                         COUNT(*) loan_count/*,
                         cr.rat_ccy_cb,
                         a.cod_ccy,
                         a.dat_first_disb*/
                  FROM ci_custmast c,
                       ln_acct_dtls a,
                       ba_ccy_rate cr,
                       (SELECT tl.cod_acct_no account_no,
                               SUM(tl.amt_txn_lcy) principal_paid
                        FROM ln_daily_txn_log_hist tl
                        WHERE tl.cod_txn_mnemonic IN (1003,1011,1017,1019,1085,1087,1413, 3014)
                              AND tl.flg_drcr = 'D'
                        GROUP BY tl.cod_acct_no) dt
                  WHERE a.cod_cc_brn = NVL(p_branch_id, a.cod_cc_brn)
                        AND a.cod_prod = NVL(p_product_id, a.cod_prod)
                        AND TRIM(c.cod_cust_id) = TRIM(a.cod_cust_id)
                        AND a.cod_acct_no = TRIM(dt.account_no(+))
                        AND cr.cod_ccy = a.cod_ccy
                        AND TRUNC(cr.dat_tim_rate_eff) = (SELECT TRUNC(MAX(cc.dat_tim_rate_eff)) max_date
                                                          FROM ba_ccy_rate cc
                                                          WHERE cc.cod_ccy = a.cod_ccy
                                                                AND cc.dat_tim_rate_eff <= a.dat_first_disb)
                  GROUP BY a.cod_cc_brn,
                           TRIM(c.nam_custadr_state),
                           --'' "urban\semi-urban\rural",
                           a.cod_prod,
                           --'' economic_activity,
                           --'',
                           c.cod_officr_id,
                           --a.amt_face_value loan_amount,
                           (CASE WHEN (a.amt_face_value * cr.rat_ccy_cb) < 501 THEN 'up to 500'
                                 WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 501 AND 1000 THEN '501 - 1000'
                                 WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 1001 AND 5000 THEN '1001 - 5000'
                                 WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 5001 AND 10000 THEN '5001 - 10000'
                                 WHEN (a.amt_face_value * cr.rat_ccy_cb) BETWEEN 10001 AND 20000 THEN '10001 - 20000'
                                 WHEN (a.amt_face_value * cr.rat_ccy_cb) > 20000 THEN 'over 20000'
                            END),
                           a.dat_of_maturity,
                           (CASE WHEN a.rat_var_current < 21 THEN 'up to 20 %'
                                 WHEN a.rat_var_current BETWEEN 21 AND 25 THEN '21 - 25 %'
                                 WHEN a.rat_var_current BETWEEN 26 AND 30 THEN '26 - 30 %'
                                 WHEN a.rat_var_current BETWEEN 31 AND 35 THEN '31 - 35 %'
                                 WHEN a.rat_var_current BETWEEN 36 AND 40 THEN '36 - 40 %'
                                 WHEN a.rat_var_current > 40 THEN 'over 40 %'
                            END),
                           (CASE WHEN c.txt_cust_sex = 'F' THEN 'Female'
                                 WHEN c.txt_cust_sex = 'M' THEN 'Male'
                            END),
                           (CASE WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) < 21 THEN 'up to 20'
                                 WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 21 AND 30 THEN '21 - 30'
                                 WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 31 AND 40 THEN '31 - 40'
                                 WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) BETWEEN 41 AND 50 THEN '41 - 50'
                                 WHEN TRUNC((SYSDATE - c.dat_birth_cust) / 365.25) > 50 THEN 'over 50'
                            END)
                  ) t,
                 (SELECT (TRIM(b.cod_cc_brn) || ' - ' || TRIM(b.nam_branch)) branch_name,
                         (TRIM(bt.cod_brn_type) || ' - ' || TRIM(bt.cod_brn_name)) branch_type,
                         b.cod_cc_brn branch_id
                  FROM ba_cc_brn_mast b,
                       ba_brn_type_master bt
                  WHERE b.cod_brn_type = bt.cod_brn_type) br
            WHERE t.branch_id = br.branch_id;

       RETURN c_result;
  END;
  -- LN1120 - Pre-aggregated loan portfolio outstanding report

  -- LN1130 - Non payment report
  FUNCTION ln1130(p_date DATE,
                  p_branch_id NUMBER,
                  p_account_no VARCHAR2 := NULL)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_npr AS(SELECT ab.cod_acct_no,
                                 ab.amt_disbursed,
                                 NVL(rs.principal_due_by_schedule, 0) principal_due_by_schedule,
                                 (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_balance,
                                 (ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))) principal_paid,
                                 ar.principal_arrears,
                                 ar.interest_arrears,
                                 ar.penalty_billed,
                                 ar.last_due_forced_date,
                                 ar.last_repayment_date,
                                 ar.principal_penalized,
                                 ar.last_penalty_billing_date,
                                 report_methods.get_arrears_in_days(p_date,
                                                                    ab.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule, 0)) arrears_in_days
                          FROM ln_acct_balances ab,
                               conv_ln_acct_balances cab,
                               conv_ln_acct_int_balance_dtls caib,
                               (SELECT rs1.cod_acct_no,
                                      SUM(rs1.amt_principal) principal_due_by_schedule
                                FROM ln_acct_schedule_detls rs1
                                WHERE rs1.date_instal < p_date
                                GROUP BY rs1.cod_acct_no) rs,
                               (SELECT th.cod_acct_no,
                                       SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                THEN DECODE(th.cod_drcr, 'D', amt_txn_acy, -amt_txn_acy)
                                                ELSE 0
                                           END) principal_arrears,
                                       SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value < p_date
                                                THEN DECODE(th.cod_drcr, 'D', amt_txn_acy, -amt_txn_acy)
                                                ELSE 0
                                           END) principal_penalized,
                                       SUM(CASE WHEN th.cod_arrear_type IN ('I','N')
                                                THEN DECODE(th.cod_drcr, 'D', amt_txn_acy, -amt_txn_acy)
                                                ELSE 0
                                           END) interest_arrears,
                                       SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                                                THEN DECODE(th.cod_drcr, 'D', amt_txn_acy, -amt_txn_acy)
                                                ELSE 0
                                           END) penalty_billed,
                                       MAX(CASE WHEN /*th.cod_arrear_type = 'C'
                                                     AND th.cod_drcr = 'D'
                                                     AND */th.dat_value < p_date
                                                THEN th.dat_value
                                                ELSE NULL
                                           END) last_penalty_billing_date,
                                       MAX(CASE WHEN th.cod_drcr = 'D'
                                                     AND th.dat_value <= p_date
                                                THEN th.dat_value
                                                ELSE NULL
                                           END) last_due_forced_date,
                                       MAX(CASE WHEN th.cod_drcr = 'C'
                                                     AND th.dat_value <= p_date
                                                THEN th.dat_value
                                                ELSE NULL
                                           END) last_repayment_date
                                FROM ln_arrear_txn_hist th
                                WHERE th.dat_value <= p_date
                                GROUP BY th.cod_acct_no
                                HAVING SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                THEN DECODE(th.cod_drcr, 'D', amt_txn_acy, -amt_txn_acy)
                                                ELSE 0
                                           END) > 0) ar,
                               (SELECT dtl.cod_acct_no,
                                       SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                                FROM rpt_ln_daily_txn_log_po dtl
                                WHERE dtl.dat_value <= p_date
                                GROUP BY dtl.cod_acct_no) tl
                          WHERE tl.cod_acct_no = rs.cod_acct_no
                                AND tl.cod_acct_no = ar.cod_acct_no
                                AND tl.cod_acct_no = ab.cod_acct_no
                                AND tl.cod_acct_no = cab.cod_acct_no(+)
                                AND tl.cod_acct_no = caib.cod_acct_no(+)
                                --
                                AND ab.amt_disbursed > 0
                                AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                                --
                                AND report_methods.get_arrears_in_days(p_date,
                                                                       ab.cod_acct_no,
                                                                       ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                       NVL(rs.principal_due_by_schedule,0)) > 0)
            --
            SELECT DISTINCT bm.cod_cc_brn branch_id,
                   bm.nam_branch branch_name,
                   pm.cod_prod product_id,
                   pm.nam_product product_name,
                   c.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
									          (CASE WHEN NVL(m.dat_last_mnt, p_date) < p_date + 1 THEN DECODE(m.cod_mis_txn_3, 20, 'Temp', 'Normal') ELSE 'Normal' END) is_temp,
                   up.user_name_and_id credit_officer,
                   up.cs_user_name_and_id credit_supervisor,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   DECODE(p_account_no, NULL, NULL, lg.guarantor_name) guarantor_name,
                   DECODE(p_account_no, NULL, NULL, (lg.account_no || '-' || TRIM(lg.guarantor_id))) agreement_no,
                   a.cod_acct_no account_no,
                   pm.nam_ccy_short acy,
                   w.amt_disbursed loan_amt,
                   a.rat_var_current interest_rate,
                   (NVL(w.amt_disbursed, 0) * cru.currency_rate) amount_usd,
                   DECODE(ac.flg_accr_status, 'S', 'Suspended', 'Normal') account_status, -- accrual status
                   w.principal_balance principal_outstanding,
                   (w.principal_balance * cy.currency_rate) principal_outstanding_lcy,
                   w.principal_paid,
                   NVL(w.principal_arrears, 0) principal_arrears,
                   NVL(w.principal_penalized, 0) principal_penalized, -- for test
                   TRUNC((NVL(w.principal_arrears, 0) * cy.currency_rate), 2) principal_arrears_lcy,
                   DECODE(NVL(w.principal_arrears, 0), 0, 0, w.arrears_in_days) arrears_in_days,
                   DECODE(NVL(w.principal_arrears, 0), 0, to_date(NULL,'yyyymmdd'), TRUNC(p_date) - TRUNC(w.arrears_in_days)) date_of_missed,
                   w.interest_arrears,
                   w.penalty_billed,
                   (CASE WHEN a.cod_acct_stat IN (1,5/*,11*/)
                         THEN 0
                         ELSE report_methods.calculate_penalty(NVL(w.last_penalty_billing_date, NVL(sl.penalty_stopped_date, p_date)),
                                                               NVL(sl.penalty_stopped_date, p_date),
                                                               NVL(w.principal_penalized, 0))
                    END) penalty_unbilled,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   w.last_due_forced_date,
                   w.last_repayment_date,
                   aa.flg_legal,
                   aa.flg_negotiated,
                   DECODE(bc.cod_cust_id, NULL, 'N', 'Y') blacklisted,
                   sl.penalty_stopped_date penalty_stop_date,
                   DECODE(pl.account_no, NULL, 'N', 'Y') system_problem,
                   DECODE(lac.num_1, 1, 'Y', 'N') expedited_loan
            FROM w_npr w,
                 ln_acct_dtls a,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ln_acct_attributes aa,
                 ac_acct_writeoff wo,
                 ac_acct_crr_code aac,
                 ac_crr_codes ac,
                 ba_cust_prod_txn_mis_xref m,
                 cs_ho_blacklist_info bc,
                 ln_acct_cbr_dtls lac,
                 rpt_system_problem_loans pl,
                 mv_currency_rates_for_usd cru,
                 mv_loan_guarantors lg,
                 mv_penalty_stopped_loans sl,
                 v_user_profile up,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT lc.account_no,
                         lc.change_date,
                         lc.user_no_old co_user_no
                  FROM mv_co_loan_changes lc,
                       (SELECT clc.account_no,
                               MIN(clc.change_date) min_date
                        FROM mv_co_loan_changes clc
                        WHERE clc.change_date > p_date
                        GROUP BY clc.account_no) lmd
                  WHERE lc.account_no = lmd.account_no
                        AND lc.change_date = lmd.min_date) mlc,
                 (SELECT cr.cod_ccy currency_no,
                         cr.rat_ccy_cb currency_rate
                  FROM ba_ccy_rate cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                        FROM ba_ccy_rate cc
                        WHERE cc.dat_tim_rate_eff <= p_date
                        GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) cy,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                              b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_acct_no = w.cod_acct_no
                  AND a.cod_acct_no = wo.cod_acct_no(+)
                  AND a.cod_acct_no = aac.cod_acct_no(+)
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_acct_no = lac.cod_acct_no(+)
                  AND a.cod_acct_no = lg.account_no(+)
                  AND a.cod_acct_no = mlc.account_no(+)
                  AND a.cod_acct_no = sl.account_no(+)
                  AND a.cod_acct_no = pl.account_no(+)
                  AND a.cod_cust_id = c.cod_cust_id
                  AND a.cod_prod = pm.cod_prod
                  AND up.user_branch_id = bm.cod_cc_brn
                  AND cy.currency_no = a.cod_ccy
                  AND cru.currency_no = a.cod_ccy
                  AND up.user_no = NVL(mlc.co_user_no, aa.cod_acct_off)
                  AND ac.cod_crr(+) = aac.cod_crr_to
                  AND ac.flg_accr_status(+) = 'S'
                  AND m.cod_prod_mnem_cust(+) = a.cod_acct_no
                  AND m.cod_mis_txn_3(+) = 20
                  AND bc.cod_cust_id(+) = a.cod_cust_id
                  --
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  AND a.cod_acct_no = NVL(p_account_no, a.cod_acct_no)
            ORDER BY 8, 11, 2;

       RETURN c_result;
  END;
  -- LN1130 - Non payment report

  -- LN1131 - Next payment report
  FUNCTION ln1131(p_date DATE,
                  p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_nxp AS (SELECT --xt.cod_acct_no account_no,
                                  SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1) account_no,
                                  MAX(xt.dat_txn_str) last_payment_date,
                                  MAX(xt.dat_value_str) last_value_date,
                                  SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * xt.amt_txn_acy) acy_amount_paid,
                                  SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * xt.amt_txn_lcy) lcy_amount_paid
                           FROM xf_ol_st_txnlog_mmdd xt,
                                (SELECT --x.cod_acct_no,
                                        SUBSTR(x.txn_nrrtv, 1, INSTR(x.txn_nrrtv, ':', 1)-1) cod_acct_no,
                                        MAX(x.dat_value_str) max_date
                                 FROM xf_ol_st_txnlog_mmdd x
                                 WHERE x.ref_subseq_no = 1
                                       AND x.cod_txn_mnemonic IN(1081,1403,1405)
                                       AND x.dat_value_str BETWEEN p_date-10 AND p_date
                                       --AND x.cod_cc_brn = NVL(p_branch_id, x.cod_cc_brn)
                                 GROUP BY SUBSTR(x.txn_nrrtv, 1, INSTR(x.txn_nrrtv, ':', 1)-1)) t
                           WHERE SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1) = t.cod_acct_no
                                 AND xt.dat_value_str = t.max_date
                                 AND xt.ref_subseq_no = 1
                                 AND xt.cod_txn_mnemonic IN(1081,1403,1405)
                                 --AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                           GROUP BY SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1)
                           --
                           UNION
                           --
                           SELECT --TRIM(xt.cod_acct_no) account_no,
                                  TRIM(SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1)) account_no,
                                  MAX(xt.dat_txn_str) last_payment_date,
                                  MAX(xt.dat_value_str) last_value_date,
                                  SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * xt.amt_txn_acy) acy_amount_paid,
                                  SUM((CASE WHEN xt.cod_msg_typ < 400 THEN 1 ELSE -1 END) * xt.amt_txn_lcy) lcy_amount_paid
                           FROM xf_ol_st_txnlog_current@host_link xt
                           WHERE xt.ref_subseq_no = 1
                                 AND xt.cod_txn_mnemonic IN(1081,1403,1405)
                                 --AND xt.cod_cc_brn = NVL(p_branch_id, xt.cod_cc_brn)
                                 AND xt.dat_value_str <= p_date
                           GROUP BY TRIM(SUBSTR(xt.txn_nrrtv, 1, INSTR(xt.txn_nrrtv, ':', 1)-1)))
            --
            SELECT a.cod_acct_no account_no,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   (up.nam_user || ' ( ' || up.cod_user_id || ' )') credit_officer,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   TRIM(pm.nam_ccy_short) acy,
                   ps.amt_principal principal_amount,
                   ps.amt_interest interest_amount,
                   (ps.amt_principal + ps.amt_interest) total_amount,
                   ab.amt_rpa_balance rpa_balance,
                   w.last_payment_date,
                   w.last_value_date,
                   w.acy_amount_paid,
                   w.lcy_amount_paid
            FROM ln_acct_dtls a,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 ci_custmast c,
                 sm_user_profile up,
                 ln_prod_mast pm,
                 ba_cc_brn_mast bm,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT sd.cod_acct_no,
                         sd.amt_principal,
                         sd.amt_interest
                  FROM ln_acct_schedule_detls sd
                  WHERE sd.date_instal BETWEEN p_date - DECODE(to_char(p_date, 'd'), '2', 2, 0) AND p_date) ps,
                 w_nxp w
            WHERE a.cod_acct_no = ps.cod_acct_no
                  AND aa.cod_acct_no = ps.cod_acct_no
                  AND ab.cod_acct_no = ps.cod_acct_no
                  AND w.account_no(+) = ps.cod_acct_no
                  AND g.cod_acct_no(+) = ps.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND up.cod_userno = aa.cod_acct_off
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND a.cod_acct_stat IN(2, 8)
                  --
                  AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)
                  AND up.cod_userno = NVL(p_user_no, up.cod_userno)
            ORDER BY 4, 2, 3, 1, 5;

       RETURN c_result;
  END;
  -- LN1131 - Next payment report

  -- LN1140 - Disbursement monitoring report
  FUNCTION ln1140(p_account_no VARCHAR2,
                  p_group_id VARCHAR2,
                  p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_account_list VARCHAR2(32000) := '';
  BEGIN
       SELECT (wm_concat(gr.account_no) || ',' || v_account_no) INTO v_account_list
       FROM (SELECT DISTINCT TRIM(gd2.cod_acct_no) account_no
             FROM ba_group_details@host_link gd1,
                  ba_group_details@host_link gd2
             WHERE TRIM(gd1.cod_group) = TRIM(gd2.cod_group(+))
                   AND gd1.cod_group_cycle = gd2.cod_group_cycle(+)
                   AND gd1.flg_group_acc_attr(+) = 'A'
                   AND gd1.flg_mnt_status(+) = 'A'
                   AND gd2.flg_group_acc_attr(+) = 'A'
                   AND gd2.flg_mnt_status(+) = 'A'
                   AND (TRIM(gd1.cod_acct_no) = v_account_no
                        OR (TRIM(gd1.cod_group) = p_group_id
                            AND gd1.cod_group_cycle = p_group_cycle))) gr;

       OPEN c_result FOR
            SELECT DISTINCT a.cod_acct_no account_no,
                   TRIM(g.cod_group) group_id,
                   g.cod_group_cycle group_cycle,
                   TRIM(g.nam_group) group_name,
                   (TRIM(pm.cod_prod) || ' - ' || TRIM(pm.nam_product)) product_name,
                   a.amt_face_value loan_amount,
                   --a.rat_var_current loan_rate,
                   pm.nam_ccy_short acy,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                   --TRIM(c.cod_cust_natl_id) customer_ic,
                   TRIM(cld.field_value) customer_ic,
                   TRUNC(dl.dat_disb) host_disb_date,
                   dl.amt_disbursed host_disb_amount,
                   cc.nam_ccy_short branch_disb_ccy,
                   lb.amt_txn_acy branch_disb_amount
            FROM ln_acct_dtls@host_link a,
                 ci_custmast@host_link c,
                 ln_prod_mast@host_link pm,
                 ln_disb_log@host_link dl,
                 xf_ln_brn_xref@host_link lb,
                 ba_ccy_code@host_link cc,
                 udf_cust_log_details@host_link cld,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details@host_link gd
                  WHERE gd.flg_group_acc_attr = 'A'
                        AND gd.flg_mnt_status = 'A') g
            WHERE a.cod_acct_no = g.cod_acct_no(+)
                  AND pm.cod_prod = a.cod_prod
                  AND c.cod_cust_id = a.cod_cust_id
                  AND dl.cod_acct_no(+) = a.cod_acct_no
                  AND lb.cod_acct_no(+) = a.cod_acct_no
                  AND lb.flg_txn_status(+) = 'P'
                  AND cc.cod_ccy(+) = lb.cod_ccy_acc
                  --
                  AND to_char(c.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld.flg_mnt_status(+) = 'A'
                  AND cc.flg_mnt_status(+) = 'A'
                  AND a.flg_mnt_status = 'A'
                  AND c.flg_mnt_status = 'A'
                  AND pm.flg_mnt_status = 'A'
                  --
                  AND INSTR(v_account_list, TRIM(a.cod_acct_no), 1) > 0
            ORDER BY 1;

       RETURN c_result;
  END;
  -- LN1140 - Disbursement monitoring report

  -- LN1150 - Repayment monitoring report
  FUNCTION ln1150(p_account_no VARCHAR2,
                  p_group_id VARCHAR2,
                  p_group_cycle NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT a.cod_acct_no account_no,
                   TRIM(g.cod_group) group_id,
                   g.cod_group_cycle group_cycle,
                   TRIM(g.nam_group) group_name,
                   (TRIM(pm.cod_prod) || ' - ' || TRIM(pm.nam_product)) product_name,
                   a.amt_face_value loan_amount,
                   --a.rat_var_current loan_rate,
                   pm.nam_ccy_short acy,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                   TRIM(c.cod_cust_natl_id) customer_ic,
                   NVL(t.principal_arrears, 0) principal_arrears,
                   NVL(t.interest_arrears, 0) interest_arrears,
                   NVL(t.penalty_arrears, 0) penalty_arrears,
                   report_methods.calculate_penalty(t.penalty_date, SYSDATE, t.principal_arrears) calculated_penalty,
                   a.amt_paid_today amount_paid_today
            FROM ln_acct_dtls a,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 ci_custmast c,
                 ln_prod_mast pm,
                 (SELECT la.cod_acct_no account_no,
                         TRUNC(NVL(MAX(CASE WHEN la.cod_arrear_type IN('N','L')
                                            THEN la.dat_arrears_due
                                       END),
                                   SYSDATE/*p_date*/)) penalty_date,
                         SUM(DECODE(la.cod_arrear_type,'C',la.amt_arrears_due,0)) principal_arrears,
                         SUM(DECODE(la.cod_arrear_type,'I',la.amt_arrears_due,0)) interest_arrears,
                         --SUM(decode(la.cod_arrear_type,'F',la.amt_arrears_due,0)) fee_arrears,
                         SUM(DECODE(la.cod_arrear_type,'A',la.amt_arrears_due,0)) penalty_arrears--,
                         --SUM(decode(la.cod_arrear_type,'N',la.amt_arrears_due,0)) suspended_interest_arrears,
                         --SUM(decode(la.cod_arrear_type,'L',la.amt_arrears_due,0)) suspended_penalty_arrears,
                         --SUM(la.amt_arrears_due) total_arrears
                  FROM ln_arrears_table la
                  WHERE la.cod_arrear_type IN(/*'F',*/'A','I','C','N','L')
                        AND la.dat_arrear_cancelled IS NULL
                  GROUP BY la.cod_acct_no) t,
                 (SELECT DISTINCT NVL(g.cod_acct_no,d.account_no) account_no
                  FROM ba_group_details g,
                       (SELECT TRIM(p_account_no) account_no,
                               TRIM(p_group_id) group_id,
                               p_group_cycle group_cycle
                        FROM dual) d
                  WHERE TRIM(g.cod_group(+)) = d.group_id
                        AND g.cod_group_cycle(+) = d.group_cycle) p
            WHERE a.cod_acct_no = p.account_no
                  AND g.cod_acct_no(+) = p.account_no
                  AND pm.cod_prod = a.cod_prod
                  AND c.cod_cust_id = a.cod_cust_id
                  AND t.account_no(+) = p.account_no
            ORDER BY 1;

       RETURN c_result;
  END;
  -- LN1150 - Repayment monitoring report

  -- LN1160 - List of accounts with suspended status
  FUNCTION ln1160
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ac.cod_acct_no account_no,
                   bm.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   --ac.cod_crr_from,
                   --ac.cod_crr_to,
                   DECODE(cc.flg_accr_status, 'S', 'Suspended', 'Normal') account_status,
                   TRUNC(ac.dat_process) process_date,
                   TRUNC(a.dat_first_disb) first_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   a.amt_face_value loan_amount,
                   pm.nam_ccy_short acy,
                   a.rat_var_current loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term
            FROM ac_acct_crr_code ac,
                 ac_crr_codes cc,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ln_acct_dtls a,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g
            WHERE cc.cod_crr = ac.cod_crr_to
                  AND cc.flg_accr_status = 'S'
                  AND bm.cod_cc_brn = ac.cod_cc_brn
                  AND pm.cod_prod = ac.cod_prod
                  AND c.cod_cust_id = ac.cod_cust_id
                  AND a.cod_acct_no = ac.cod_acct_no
                  AND g.cod_acct_no(+) = a.cod_acct_no
            ORDER BY 1, 2, 4, 6;

       RETURN c_result;
  END;
  -- LN1160 - List of accounts with suspended status

  -- LN1170 - Fee payment report
  FUNCTION ln1170(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT bm.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   a.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   a.cod_acct_no account_no,
                   a.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   --TRIM(c.cod_cust_natl_id) customer_ic,
                   cd.document_id customer_ic,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   TRUNC(a.dat_first_disb) disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   --a.amt_face_value loan_amount,
                   ab.amt_disbursed disbursed_amount,
                   pm.nam_ccy_short acy,
                   a.rat_var_current loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   f.fee_assessed,
                   f.fee_paid
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ba_cc_brn_mast bm,
                 ci_cust_types ct,
                 v_ci_docmast cd,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT th.cod_acct_no account_no,
                         SUM(DECODE(th.cod_drcr, 'D', th.amt_txn_acy, 0)) fee_assessed,
                         SUM(DECODE(th.cod_drcr, 'C', th.amt_txn_acy, 0)) fee_paid
                  FROM ln_arrear_txn_hist th
                  WHERE th.cod_arrear_type = 'F'
                        AND th.dat_value BETWEEN TRUNC(p_start_date) AND TRUNC(p_end_date)
                  GROUP BY th.cod_acct_no) f,
               (SELECT NVL((SELECT TRIM(rs.setting_value)
                                  FROM rpt_report_settings rs
                                  WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                                b.branch_id) branch_id_list
                      FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE c.cod_cust_id = a.cod_cust_id
                  AND ab.cod_acct_no = a.cod_acct_no
                  AND f.account_no = a.cod_acct_no
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND ct.flg_cust_typ = c.flg_cust_typ
                  AND c.cod_cust_id = cd.cod_cust_id(+)
                  --
                  AND a.dat_first_disb <= TRUNC(p_end_date)
                  AND NVL(a.dat_acct_close, p_end_date + 1) > TRUNC(p_end_date)
                  AND INSTR(NVL(br.branch_id_list, bm.cod_cc_brn), bm.cod_cc_brn, 1) > 0
                  --AND a.cod_cc_brn = nvl(br.branch_id, a.cod_cc_brn)
            ORDER BY a.cod_cc_brn,
                     a.cod_prod,
                     g.cod_group,
                     a.dat_first_disb,
                     a.dat_of_maturity;

       RETURN c_result;
  END;
  -- LN1170 - Fee payment report

  -- LN1180 - Legal department clients
  -- LN1181 - Transfer form to legal department
  FUNCTION ln1180(p_date DATE,
                  p_branch_id NUMBER,
                  p_account_no VARCHAR2,
                  p_show_group_members CHAR := 'N')
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            WITH w_loans AS (SELECT (v_account_no || DECODE(p_show_group_members, 'Y', (',' || wm_concat(t.loan)), '')) loans
                             FROM (SELECT DISTINCT bgd.cod_acct_no loan
                                   FROM ba_group_details bgd,
                                        (SELECT DISTINCT gd.cod_group,
                                                gd.cod_group_cycle
                                         FROM ba_group_details gd
                                         WHERE gd.flg_group_acc_attr = 'A'
                                               AND gd.cod_acct_no = v_account_no) g
                                   WHERE bgd.cod_group = g.cod_group
                                         AND bgd.cod_group_cycle = g.cod_group_cycle
                                         AND bgd.cod_acct_no != v_account_no) t)
            SELECT bm.cod_cc_brn branch_id,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   (up.nam_user || ' (' || up.cod_user_id || ')') credit_officer,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   a.cod_acct_no account_no,
                   pm.nam_ccy_short acy,
                   ab.amt_disbursed loan_amount,
                   NVL(rs.interest_amount, 0) interest_amount,
                   (NVL(rs.principal_amount, 0) + NVL(rs.interest_amount, 0)) total_amount,
                   ab.amt_rpa_balance current_rpa_balance,
                   a.rat_var_current loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Gündəlik)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2,' (Həftəlik)', ' (2 həftədən bir)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Aylıq)'
                         ELSE '0'
                    END) loan_term_az,
                   a.dat_first_disb first_disb_date,
                   a.dat_last_disb last_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   ast.txt_acct_status account_status,
                   wo.dat_writeoff write_off_date,
                   ab.amt_disbursed - (NVL(conv.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)) principal_due,
                   (CASE WHEN a.cod_acct_stat IN (1,5)
                         THEN 0
                         ELSE NVL(rs.interest_amount, 0) - (NVL(conv.interest_paid_bfr_migration, 0) + NVL(ar.interest_paid_aft_migration, 0))
                    END) interest_due,
                   ((ab.amt_disbursed - (NVL(conv.principal_paid_bfr_migration, 0) +
                     NVL(ar.principal_paid_aft_migration, 0))) +
                     (CASE WHEN a.cod_acct_stat IN (1,5)
                          THEN 0
                          ELSE NVL(rs.interest_amount, 0) - (NVL(conv.interest_paid_bfr_migration, 0) + NVL(ar.interest_paid_aft_migration, 0))
                     END)) total_due,
                   (NVL(conv.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)) principal_paid,
                   (NVL(conv.interest_paid_bfr_migration, 0) + NVL(ar.interest_paid_aft_migration, 0)) interest_paid,
                   (NVL(conv.penalty_paid_bfr_migration, 0) + NVL(ar.penalty_paid_aft_migration, 0)) penalty_paid,
                   (NVL(conv.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0) +
                    NVL(conv.interest_paid_bfr_migration, 0) + NVL(ar.interest_paid_aft_migration, 0) +
                    NVL(conv.penalty_paid_bfr_migration, 0) + NVL(ar.penalty_paid_aft_migration, 0)) total_paid,
                   NVL(ar.principal_arrears, 0) principal_arrears,
                   NVL(ar.interest_arrears, 0) interest_arrears,
                   TRUNC(p_date) -
                   (CASE WHEN NVL(principal_arrears,0) > 0
                         THEN report_methods.get_arrears_in_days(p_date,
                                                                  a.cod_acct_no,
                                                                  (NVL(conv.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)),
                                                                  NVL(rs.principal_due_by_schedule, 0))
                         ELSE NULL
                    END) arrear_start_date,
                   ar.last_repayment_date,
                   (CASE WHEN NVL(principal_arrears,0) > 0
                         THEN report_methods.get_arrears_in_days(p_date,
                                                                  a.cod_acct_no,
                                                                  (NVL(conv.principal_paid_bfr_migration, 0) + NVL(ar.principal_paid_aft_migration, 0)),
                                                                  NVL(rs.principal_due_by_schedule, 0))
                         ELSE 0
                    END) arrears_in_days,
                   ar.penalty_billed,
                   (CASE WHEN a.cod_acct_stat IN (1,5)
                         THEN 0
                         ELSE report_methods.calculate_penalty(NVL(ar.last_penalty_billing_date, NVL(sl.penalty_stopped_date, p_date)),
                                                               NVL(sl.penalty_stopped_date, p_date),
                                                               NVL(principal_penalized, 0))
                    END) penalty_unbilled,
                   sl.penalty_stopped_date penalty_stop_date,
                   NVL(to_char(ar.change_date, 'dd/mm/yyyy'), aa.flg_legal) legal_status,
                   lrl.collector_name,
                   lrl.loan_status legal_info,
                   lrl.court_decision1,
                   lrl.court_decision2,
                   ar.total_paid_aft_mig_lgl,
                   ar.total_paid_last_month,
                   NVL(woa.wo_principal_amount, 0) wo_principal_amount,
                   NVL(woa.wo_interest_amount, 0) wo_interest_amount,
                   NVL(woa.wo_penalty_amount, 0) wo_penalty_amount,
                   NVL(woa.wo_paid_amount, 0) wo_paid_amount
            FROM ln_acct_dtls a,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ci_custmast c,
                 ln_acct_attributes aa,
                 sm_user_profile up,
                 ln_acct_balances ab,
                 ba_acct_status ast,
                 ac_acct_writeoff wo,
                 mv_penalty_stopped_loans sl,
                 mv_write_off_amounts woa,
                 v_ld_related_loans lrl,
                 w_loans wl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT lab.cod_acct_no,
                         SUM(NVL(lab.amt_princ_balance, 0)) migrated_principal_balance,
                         SUM(NVL(lab.amt_net_disbursed, 0) - NVL(lab.amt_princ_balance, 0)) principal_paid_bfr_migration,
                         SUM(NVL(ibd.bal_int1_chg, 0)) interest_paid_bfr_migration,
                         SUM(NVL(ibd.bal_int2_chg, 0)) penalty_paid_bfr_migration
                  FROM conv_ln_acct_balances lab,
                       conv_ln_acct_int_balance_dtls ibd
                  WHERE lab.cod_acct_no = ibd.cod_acct_no(+)
                  GROUP BY lab.cod_acct_no) conv,
                 (SELECT sd.cod_acct_no,
                         SUM(sd.amt_principal) principal_amount,
                         SUM(sd.amt_interest) interest_amount,
                         SUM(CASE WHEN sd.date_instal < p_date
                                  THEN sd.amt_principal
                                  ELSE 0
                             END) principal_due_by_schedule,
                         SUM(CASE WHEN sd.date_instal < p_date
                                  THEN sd.amt_interest
                                  ELSE 0
                             END) interest_due_by_schedule
                  FROM ln_acct_schedule_detls sd
                  GROUP BY sd.cod_acct_no) rs,
                 (SELECT th.cod_acct_no,
                         SUM(CASE WHEN th.cod_arrear_type IN('C','R')
                                  THEN DECODE(th.cod_drcr, 'C', th.amt_txn_acy, DECODE(th.cod_txn_mnemonic, 3121, -th.amt_txn_acy, 0))
                                  ELSE 0
                             END) principal_paid_aft_migration,
                         SUM(CASE WHEN th.cod_arrear_type IN('I', 'N') AND th.cod_drcr = 'C'
                                  THEN th.amt_txn_acy
                                  ELSE 0
                             END) interest_paid_aft_migration,
                         SUM(CASE WHEN th.cod_arrear_type IN ('A','L') AND th.cod_drcr = 'C'
                                  THEN th.amt_txn_acy
                                  ELSE 0
                             END) penalty_paid_aft_migration,
                         SUM(CASE WHEN th.cod_arrear_type = 'C' AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_arrears,
                         SUM(CASE WHEN th.cod_arrear_type IN ('I','N') AND th.dat_value < p_date
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) interest_arrears,
                         SUM(CASE WHEN th.cod_arrear_type = 'C' AND TRUNC(th.dat_value) < TRUNC(p_date)
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) principal_penalized,
                         SUM(CASE WHEN th.cod_arrear_type IN ('A','L')
                                  THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, -th.amt_txn_acy)
                                  ELSE 0
                             END) penalty_billed,
                         MAX(CASE WHEN /*th.cod_arrear_type = 'C'
                                       AND th.cod_drcr = 'D'
                                       AND */TRUNC(th.dat_value) < TRUNC(p_date)
                                  THEN TRUNC(th.dat_value)
                                  ELSE NULL
                             END) last_penalty_billing_date,
                         MAX(CASE WHEN th.cod_drcr = 'C'
                                       AND th.dat_value <= p_date
                                  THEN th.dat_value
                                  ELSE NULL
                             END) last_repayment_date,
                         MAX(ldl.change_date) change_date,
                         SUM(CASE WHEN th.cod_arrear_type IN ('C','I','N','A','L','R') AND th.dat_value >= ldl.change_date
                                  THEN DECODE(th.cod_drcr, 'C', th.amt_txn_acy, DECODE(th.cod_txn_mnemonic, 3121, -th.amt_txn_acy, 0))
                                  ELSE 0
                             END) total_paid_aft_mig_lgl,
                         SUM(CASE WHEN th.cod_arrear_type IN ('C','I','N','A','L','R') AND to_char(th.dat_value, 'yyyymm') = to_char(p_date, 'yyyymm')
                                  THEN DECODE(th.cod_drcr, 'C', th.amt_txn_acy, DECODE(th.cod_txn_mnemonic, 3121, -th.amt_txn_acy, 0))
                                  ELSE 0
                             END) total_paid_last_month
                  FROM ln_arrear_txn_hist th,
                       mv_legal_dep_loans ldl
                  WHERE th.cod_acct_no = ldl.account_no(+)
                        AND th.dat_value <= p_date
                  GROUP BY th.cod_acct_no) ar
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_acct_no = rs.cod_acct_no
                  AND a.cod_acct_no = conv.cod_acct_no(+)
                  AND a.cod_acct_no = ar.cod_acct_no(+)
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_acct_no = wo.cod_acct_no(+)
                  AND a.cod_acct_no = sl.account_no(+)
                  AND a.cod_acct_no = lrl.cod_acct_no(+)
                  AND a.cod_acct_no = woa.cod_acct_no(+)
                  AND a.cod_cust_id = c.cod_cust_id
                  AND a.cod_prod = pm.cod_prod
                  AND up.cod_cc_brn = bm.cod_cc_brn
                  AND up.cod_userno = aa.cod_acct_off
                  AND ast.cod_acct_status = a.cod_acct_stat
                  AND ast.cod_module = 'LN'
                  AND ab.amt_disbursed > 0
                  --
                  AND DECODE(p_show_group_members, 'Y', 'Y', aa.flg_legal) = 'Y'
                  AND a.dat_first_disb <= p_date
                  AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)
                  AND INSTR(NVL(wl.loans, a.cod_acct_no), a.cod_acct_no) > 0
            ORDER BY 2, 4, 7, 5;

       RETURN c_result;
  END;
  -- LN1180 - Legal department clients
  
  -- LN1200 - Write off loans
  FUNCTION ln1200(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT vbr.region_id,
                   vbr.region_name,
                   vbr.branch_id,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   pm.cod_prod product_id,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   a.dat_first_disb first_disb_date,
                   a.dat_last_disb last_disb_date,
                   TRUNC(a.dat_of_maturity) maturity_date,
                   up.user_name_and_id credit_officer,
                   g.cod_group group_id,
                   DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                   g.cod_group_cycle group_cycle,
                   c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   cl.customer_cycle,
                   --trim(c.cod_cust_natl_id) customer_ic,
                   cd.document_id customer_ic,
                   DECODE(m.cod_mis_txn_3, 20, 'Temp', 'Normal') is_temp,
                   a.cod_acct_no account_no,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months) || ' (Daily)'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367) || DECODE(pm.cod_prod_term_typ, 2, ' (Weekly)', ' (Biweekly)')
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536) || ' (Monthly)'
                         ELSE '0'
                    END) loan_term,
                   a.rat_var_current interest_rate,
                   ab.amt_disbursed disbursed_amount,
                   pm.nam_ccy_short acy,
                   wo.amt_writeoff_princ_bal wo_principal_balance,
                   wo.amt_writeoff_susp_int wo_susp_interest,
                   wo.amt_writeoff_susp_pen_int wo_susp_penalty_interest,
                   --wo.amt_writeoff_int wo_interest_balance,
                   wo.dat_writeoff wo_date,
                   us.user_name_and_id user_name,
                   au.user_name_and_id author_name,
                   bc.bc_reason1,
                   bc.bc_reason2
            FROM ln_acct_dtls a,
                 ci_custmast c,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 ac_acct_writeoff wo,
                 ba_cust_prod_txn_mis_xref m,
                 ln_prod_mast pm,
                 --cs_ho_blacklist_info bc,
                 v_ci_docmast cd,
                 v_user_profile up,
                 v_user_profile us,
                 v_user_profile au,
                 v_branches_and_regions vbr,
                 mv_customer_cycle_list cl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT bci.cod_cust_id,
                         MIN(bci.cod_agency) bc_reason1,
                         DECODE(COUNT(bci.cod_cust_id), 1, NULL, MAX(bci.cod_agency)) bc_reason2
                  FROM cs_ho_blacklist_info bci
                  GROUP BY bci.cod_cust_id) bc,
                 (SELECT NVL((SELECT TRIM(rst.setting_value)
                              FROM rpt_report_settings rst
                              WHERE rst.setting_key = ('REGION_' || b.branch_id)),
                             b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = wo.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no(+)
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_cust_id = c.cod_cust_id
                  AND a.cod_prod = pm.cod_prod
                  AND up.user_branch_id = vbr.branch_id
                  AND up.user_no = aa.cod_acct_off
                  AND cl.cod_cust_id = a.cod_cust_id
                  AND m.cod_prod_mnem_cust(+) = a.cod_acct_no
                  AND m.cod_mis_txn_3(+) = 20
                  AND c.cod_cust_id = cd.cod_cust_id(+)
                  AND c.cod_cust_id = bc.cod_cust_id(+)
                  AND us.user_id = wo.cod_user_id
                  AND au.user_id = wo.cod_auth_id
                  --
                  AND INSTR(NVL(br.branch_id_list, vbr.branch_id), vbr.branch_id, 1) > 0
                  AND wo.dat_writeoff BETWEEN p_start_date AND p_end_date
            ORDER BY 4, 10;

       RETURN c_result;
  END;
  -- LN1200 - Write off loans

  -- LN1210 - Portfolio tendency
  FUNCTION ln1210(p_branch_id NUMBER,
                  p_date_list VARCHAR2,
                  p_ar_in_days_from NUMBER := 0,
                  p_ar_in_days_to NUMBER := 10000)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_ar_in_days_from NUMBER(6) := (CASE WHEN p_ar_in_days_from = 0 THEN 1 ELSE p_ar_in_days_from END);
  BEGIN
       OPEN c_result FOR
            SELECT to_date(pc.q_date, 'yyyymmdd') query_date,
                   (br.region_id || ' - ' || br.region_name) region_name,
                   (br.branch_id || ' - ' || br.branch_name) branch_name,
                   (up.nam_user || ' ( ' || up.cod_user_id || ' )') user_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   COUNT(DISTINCT pc.user_no) OVER(PARTITION BY pc.q_date, br.branch_id) co_count_by_branch,
                   COUNT(DISTINCT pc.user_no) OVER(PARTITION BY pc.q_date, br.region_id) co_count_by_region,
                   COUNT(DISTINCT pc.user_no) OVER(PARTITION BY pc.q_date) co_count_total,
                   pc.pa_usd,
                   pc.pa_usd_rst,
                   pc.pa_usd_exp,
                   pc.pa_usd_st,
                   pc.po_usd,
                   pc.po_usd_rst,
                   pc.po_usd_exp,
                   pc.po_usd_st,
                   pc.po_usd_total,
                   pc.client_count_pa,
                   pc.client_count_pa_rst,
                   pc.client_count_pa_exp,
                   pc.client_count_pa_st,
                   pc.client_count,
                   pc.client_count_rst,
                   pc.client_count_exp,
                   pc.client_count_st,
                   pc.client_count_total,
                   pc.client_count_disb,
                   pc.client_count_disb_rst,
                   pc.client_count_disb_exp,
                   pc.client_count_disb_st,
                   pc.disb_amount,
                   pc.disb_amount_rst,
                   pc.disb_amount_exp,
                   pc.disb_amount_st
            FROM TABLE(reports_ln.get_po_by_co(p_branch_id, p_date_list, v_ar_in_days_from, p_ar_in_days_to)) pc,
                 ln_prod_mast pm,
                 sm_user_profile up,
                 v_branches_and_regions br
            WHERE pm.cod_prod = pc.product_id
                  AND up.cod_userno = pc.user_no
                  AND br.branch_id = up.cod_cc_brn
            ORDER BY 1, 2, 3, 4, 5;

       RETURN c_result;
  END;
  -- LN1210 - Portfolio tendency
  
  -- LN1220 - Reserve user transactions
  FUNCTION ln1220(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(xt.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                   (TRIM(up.nam_user) || ' ( ' || TRIM(up.cod_user_id) || ' )') user_name,
                   TRIM(xt.cod_acct_no) account_no,
                   (TRIM(xt.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)) txn_desc,
                   TRIM(xt.txn_nrrtv) txn_narrative,
                   xt.dat_txn_str txn_date,
                   xt.dat_post_str post_date,
                   xt.dat_value_str value_date,
                   xt.amt_txn_tcy amount_tcy,
                   cct.nam_ccy_short tcy,
                   xt.amt_txn_acy amount_acy,
                   cca.nam_ccy_short acy,
                   xt.amt_txn_lcy amount_lcy,
                   ccl.nam_ccy_short lcy,
                   xt.flg_drcr drcr
            FROM xf_ol_st_txnlog_mmdd@host_link xt,
                 ba_cc_brn_mast@host_link bm,
                 ba_txn_mnemonic@host_link tm,
                 sm_user_profile@host_link up,
                 ba_ccy_code@host_link cct,
                 ba_ccy_code@host_link cca,
                 ba_ccy_code@host_link ccl
            WHERE bm.cod_cc_brn = xt.cod_cc_brn
                  AND xt.cod_txn_mnemonic = tm.cod_txn_mnemonic(+)
                  AND up.cod_user_id = xt.cod_user_id
                  AND cct.cod_ccy = xt.cod_txn_ccy
                  AND cca.cod_ccy = xt.cod_acc_ccy
                  AND ccl.cod_ccy = xt.cod_other_acc_ccy
                  AND xt.cod_auth_id = 'FC_999_R'
                  --
                  AND xt.cod_cc_brn = nvl(p_branch_id, xt.cod_cc_brn)
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(xt.dat_value_str),
                             'tran_date', TRUNC(xt.dat_txn_str),
                             'post_date', TRUNC(xt.dat_post_str)) BETWEEN p_start_date AND p_end_date
            ORDER BY xt.dat_txn_str DESC, 1, 2;

       RETURN c_result;
  END;
  -- LN1220 - Reserve user transactions

  -- LN1230 - Rotation - Client transfer form
  FUNCTION ln1230(p_branch_id NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (up.nam_user || ' (' || up.cod_user_id || ')') co_name,
                   --c.cod_cust_id customer_id,
                   (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full) customer_name,
                   DECODE(g.cod_group, NULL, NULL, g.cod_group || ' - ' || g.nam_group) group_name,
                   a.cod_acct_no account_no,
                   COUNT(DISTINCT g.cod_group) OVER () group_count,
                   COUNT(DISTINCT DECODE(g.cod_group, NULL, NULL, a.cod_cust_id)) OVER () group_client_count,
                   COUNT(DECODE(g.cod_group, NULL, 1, NULL)) OVER () individual_loan_count,
                   COUNT(DISTINCT DECODE(g.cod_group, NULL, a.cod_cust_id, NULL)) OVER () individual_client_count,
                   COUNT(a.cod_cust_id) OVER () total_loan_count,
                   COUNT(DISTINCT a.cod_cust_id) OVER () total_client_count
            FROM ln_acct_dtls a,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 ci_custmast c,
                 sm_user_profile up,
                 ba_cc_brn_mast bm,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g
            WHERE aa.cod_acct_no = a.cod_acct_no
                  AND ab.cod_acct_no = a.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND g.cod_acct_no(+) = a.cod_acct_no
                  AND up.cod_userno = aa.cod_acct_off
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  --
                  AND ab.amt_princ_balance > 0
                  AND ab.amt_disbursed > 0
                  --
                  AND bm.cod_cc_brn = NVL(p_branch_id, bm.cod_cc_brn)
                  AND up.cod_userno = NVL(p_user_no, up.cod_userno)
             ORDER BY 1, 2, 5, 4;

       RETURN c_result;
  END;
  -- LN1230 - Rotation - Client transfer form

  -- LN1240 - Collateral information for active loans
  FUNCTION ln1240(p_branch_id NUMBER,
                  p_date DATE)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   a.cod_acct_no account_no,
                   c.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   --TRIM(ca.cod_collat_id) collateral_id,
                   --ch.cod_ccy currency_no,
                   a.dat_first_disb disb_date,
                   a.amt_face_value loan_amount,
                   pm.nam_ccy_short acy,
                   a.rat_var_current loan_rate,
                   MAX(t.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_outstanding,
                   DECODE(cc.cod_coll_type, 1, 'Property', 2, 'Transport', 5, 'Non-Standard') collateral_type,
                   cc.nam_coll collateral_name,
                   SUM(ch.amt_orig_value) original_value,
                   SUM(ch.amt_last_val) collateral_value,
                   SUM(ch.amt_market_val) market_value,
                   MAX(bt.txt_business) business_type
            FROM ba_lo_coll_acct_xref ca,
                 ba_coll_hdr ch,
                 ba_coll_codes cc,
                 ln_acct_dtls a,
                 ci_custmast c, -- borrower
                 --udf_cust_log_details cld_bw,
                 ln_prod_mast pm,
                 conv_ln_acct_balances cab,
                 conv_ln_acct_int_balance_dtls caib,
                 ln_acct_balances ab,
                 ln_acct_attributes aa,
                 ba_cc_brn_mast bm,
                 sm_user_profile up,
                 ci_business_types bt,
                 (SELECT dtl.cod_acct_no,
                         SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                  FROM rpt_ln_daily_txn_log_po dtl
                  WHERE dtl.dat_value <= p_date
                  GROUP BY dtl.cod_acct_no) t
            WHERE ca.cod_collat_id = ch.cod_collat_id(+)
                  AND cc.cod_coll(+) = ch.cod_coll
                  AND ca.cod_coll = ch.cod_coll(+)
                  AND a.cod_acct_no = ca.cod_acct_no(+)
                  AND a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_acct_no = t.cod_acct_no
                  AND a.cod_acct_no = cab.cod_acct_no(+)
                  AND a.cod_acct_no = caib.cod_acct_no(+)
                  AND a.cod_cust_id = c.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND up.cod_userno = aa.cod_acct_off
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  --AND to_char(a.cod_cust_id) = cld_bw.cod_cust_id(+)
                  AND c.cod_business_cat = bt.cod_business_cat(+)
                  --
                  AND ab.amt_disbursed > 0
                  AND (t.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                  --
                  AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)
            GROUP BY a.cod_acct_no,
                     --TRIM(ca.cod_collat_id),
                     (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch),
                     (TRIM(pm.cod_prod) || ' - ' || pm.nam_product),
                     cc.cod_coll_type,
                     cc.nam_coll,
                     ch.cod_ccy,
                     pm.nam_ccy_short,
                     a.dat_first_disb,
                     a.amt_face_value,
                     a.rat_var_current,
                     c.cod_cust_id,
                     c.nam_cust_full
            ORDER BY 1, 2, 5, 6;

       RETURN c_result;
  END;
  -- LN1240 - Collateral information for active loans

  -- LN1250 - Active loans with amounts greater than 20000 AZN
  FUNCTION ln1250
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   (up.nam_user || ' ( ' || up.cod_user_id || ' )') credit_officer,
                   c.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   cl.customer_cycle,
                   c.dat_birth_cust birth_date,
                   a.cod_acct_no account_no,
                   a.dat_first_disb disb_date,
                   ab.amt_disbursed loan_amount,
                   pm.nam_ccy_short acy,
                   (ab.amt_disbursed * cy.rat_ccy_cb) loan_amount_lcy,
                   ict.txt_ic_typ document_type,
                   cd.document_id,
                   cd.customer_birth_place
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 ln_acct_attributes aa,
                 ln_prod_mast pm,
                 ba_cc_brn_mast bm,
                 ci_custmast c,
                 ci_ic_types ict,
                 sm_user_profile up,
                 mv_customer_cycle_list cl,
                 v_ci_docmast cd,
                 (SELECT cr.cod_ccy,
                         cr.rat_ccy_cb
                  FROM ba_ccy_rate cr,
                       (SELECT cc.cod_ccy,
                               MAX(cc.dat_tim_rate_eff) max_date
                       FROM ba_ccy_rate cc
                       WHERE cc.dat_tim_rate_eff <= SYSDATE
                       GROUP BY cc.cod_ccy) t1
                  WHERE cr.cod_ccy = t1.cod_ccy
                        AND cr.dat_tim_rate_eff = t1.max_date) cy
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_prod = pm.cod_prod
                  AND a.cod_cust_id = c.cod_cust_id
                  AND aa.cod_acct_off = up.cod_userno
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND a.cod_ccy = cy.cod_ccy
                  AND a.cod_cust_id = cl.cod_cust_id(+)
                  AND a.cod_cust_id = cd.cod_cust_id(+)
                  AND cd.document_type = ict.flg_ic_typ(+)
                  --
                  AND up.cod_userno NOT IN(SELECT cbr.user_no FROM rpt_co_bank_relations cbr)
                  AND (ab.amt_disbursed * cy.rat_ccy_cb) > 20000
                  AND a.cod_acct_stat IN(6,8)
                  AND a.dat_first_disb >= ADD_MONTHS(TRUNC(SYSDATE), -12)
            ORDER BY disb_date, 1, 2, 4;
       
       RETURN c_result;
  END;
  -- LN1250 - Active loans with amounts greater than 20000 AZN

  -- LN1260 - Active loans without disbursement
  FUNCTION ln1260(p_start_date DATE,
                  p_end_date DATE)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   g.cod_group group_id,
                   g.nam_group group_name,
                   c.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   a.cod_acct_no account_no,
                   ab.amt_disbursed loan_amount,
                   ab.amt_rpa_balance rpa_balance,
                   ab.amt_princ_balance principal_balance,
                   pm.nam_ccy_short acy,
                   a.dat_acct_open open_date,
                   --a.dat_last_disb disb_date,
                   a.dat_of_maturity maturity_date,
                   a.cod_last_mnt_makerid maker_id
            FROM ln_acct_dtls a,
                 ln_acct_balances ab,
                 ci_custmast c,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g
            WHERE a.cod_acct_no = ab.cod_acct_no
                  AND a.cod_cust_id = c.cod_cust_id
                  AND a.cod_cc_brn = bm.cod_cc_brn
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_prod = pm.cod_prod
                  --
                  AND a.cod_acct_stat IN(6,8)
                  AND a.dat_first_disb IS NULL
                  --
                  AND a.dat_acct_open BETWEEN p_start_date AND p_end_date
            ORDER BY branch_name,
                     open_date,
                     group_name;
       
       RETURN c_result;
  END;
  -- LN1260 - Active loans without disbursement

  -- LN1270 - Expired loans for archive department
  FUNCTION ln1270(p_branch_id NUMBER,
                  p_year VARCHAR2)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   (TRIM(pm.cod_prod) || ' - ' || pm.nam_product) product_name,
                   (up.nam_user || ' ( ' || up.cod_user_id || ' )') credit_officer,
                   g.cod_group group_id,
                   g.nam_group group_name,
                   g.cod_group_cycle group_cycle,
                   c.cod_cust_id customer_id,
                   c.nam_cust_full customer_name,
                   cl.customer_cycle,
                   a.cod_acct_no account_no,
                   a.dat_first_disb disb_date,
                   a.dat_of_maturity maturity_date,
                   a.dat_acct_close close_date
            FROM ln_acct_dtls a,
                 ci_custmast c,
                 ba_cc_brn_mast bm,
                 ln_prod_mast pm,
                 ln_acct_attributes aa,
                 ln_acct_balances ab,
                 sm_user_profile up,
                 mv_customer_cycle_list cl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g
            WHERE a.cod_cust_id = c.cod_cust_id
                  AND bm.cod_cc_brn = up.cod_cc_brn
                  AND pm.cod_prod = a.cod_prod
                  AND a.cod_acct_no = aa.cod_acct_no
                  AND a.cod_acct_no = ab.cod_acct_no
                  AND up.cod_userno = aa.cod_acct_off
                  AND a.cod_acct_no = g.cod_acct_no(+)
                  AND a.cod_cust_id = cl.cod_cust_id(+)
                  --
                  AND ab.amt_disbursed > 0
                  --
                  AND TO_CHAR(GREATEST(a.dat_acct_close, a.dat_of_maturity), 'yyyy') = p_year
                  AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)
            ORDER BY GREATEST(a.dat_acct_close, a.dat_of_maturity),
                     credit_officer,
                     group_name,
                     customer_name;
       
       RETURN c_result;
  END;
  -- LN1270 - Expired loans for archive department
  
  -- portfolio outstanding by month
  FUNCTION get_po_by_month(p_start_date DATE,
                           p_end_date DATE)
  RETURN data_types.po_tab PIPELINED
  IS
    CURSOR c_dates IS
           SELECT ADD_MONTHS(to_date(to_char(p_start_date, 'yyyymm'), 'yyyymm') - 1, LEVEL) rep_month
           FROM dual
           CONNECT BY LEVEL < MONTHS_BETWEEN(ADD_MONTHS(p_end_date, 1), TO_DATE(TO_CHAR(p_start_date, 'yyyymm'), 'yyyymm') - 1);
    
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
                 AND up.cod_userno = NVL(mlc.co_user_no, aa.cod_acct_off)
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
               PIPE ROW(i);
           END LOOP;
           --dbms_output.put_line(to_char(dt.rep_month, 'yyyy.mm.dd'));
       END LOOP;
       
       RETURN;
  END;
  
  -- portfolio outstanding by CO
  FUNCTION get_po_by_co(p_branch_id NUMBER,
                        p_date_list VARCHAR2,
                        p_ar_in_days_from NUMBER,
                        p_ar_in_days_to NUMBER)
  RETURN data_types.po_co_tab PIPELINED
  IS
    CURSOR c_dates IS
           SELECT to_date(rep_date, 'yyyymmdd') rep_date,
                  to_date(rep_date_ds, 'yyyymmdd') rep_date_from
           FROM (SELECT LEVEL rnum,
                        REGEXP_REPLACE(REGEXP_SUBSTR(t.dt || ',',
                                                     '(.*?),',
                                                     1,
                                                     LEVEL),
                                       ',$',
                                       '') rep_date,
                        REGEXP_REPLACE(REGEXP_SUBSTR(t.dt || ',',
                                                     '(.*?),',
                                                     1,
                                                     DECODE(LEVEL, 1, NULL, LEVEL-1)),
                                       ',$',
                                       '') rep_date_ds
                 FROM (SELECT p_date_list dt
                       FROM dual) t
                 WHERE LEVEL <= 15
                 CONNECT BY REGEXP_INSTR(t.dt || ',',
                                         '(.*?),',
                                         1,
                                         LEVEL) > 0)
           ORDER BY 1;

    -- pa -> pricipal arrears
    -- po -> portfolio outstanding
    CURSOR c_po_for_co(v_date DATE, v_date_from DATE) IS
           SELECT TRIM(to_char(v_date, 'yyyymmdd')) q_date, -- query date
                  up.cod_userno user_no,
                  a.cod_prod product_id,
                  SUM(CASE WHEN spl.account_no IS NULL AND
                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                           THEN ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate, 6)
                           ELSE 0
                      END) pa_usd,
                  SUM(CASE WHEN spl.account_no IS NULL AND aa.flg_negotiated = 'Y' AND
                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                           THEN ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate, 6)
                           ELSE 0
                      END) pa_usd_rst,
                  SUM(CASE WHEN spl.account_no IS NULL AND lac.num_1 = 1 AND
                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                           THEN ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate, 6)
                           ELSE 0
                      END) pa_usd_exp,
                  SUM(CASE WHEN spl.account_no IS NULL AND lac.num_1 = 2 AND
                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                           THEN ROUND((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate, 6)
                           ELSE 0
                      END) pa_usd_st,
                  ROUND(SUM((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate), 6) po_usd,
                  ROUND(SUM(DECODE(aa.flg_negotiated, 'Y', (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))), 0) * cru.currency_rate), 6) po_usd_rst,
                  ROUND(SUM(DECODE(lac.num_1, 1, (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))), 0) * cru.currency_rate), 6) po_usd_exp,
                  ROUND(SUM(DECODE(lac.num_1, 2, (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))), 0) * cru.currency_rate), 6) po_usd_st,
                  SUM(ROUND(SUM((tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) * cru.currency_rate), 6)) OVER (ORDER BY NULL) po_usd_total,
                  SUM(COUNT(DISTINCT (CASE WHEN spl.account_no IS NULL AND
                                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                                           THEN a.cod_cust_id
                                           ELSE NULL
                                      END))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_pa,
                  SUM(COUNT(DISTINCT (CASE WHEN spl.account_no IS NULL AND aa.flg_negotiated = 'Y' AND
                                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                                           THEN a.cod_cust_id
                                           ELSE NULL
                                      END))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_pa_rst,
                  SUM(COUNT(DISTINCT (CASE WHEN spl.account_no IS NULL AND lac.num_1 = 1 AND
                                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                                           THEN a.cod_cust_id
                                           ELSE NULL
                                      END))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_pa_exp,
                  SUM(COUNT(DISTINCT (CASE WHEN spl.account_no IS NULL AND lac.num_1 = 2 AND
                                                report_methods.get_arrears_in_days(v_date,
                                                                    a.cod_acct_no,
                                                                    ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                    NVL(rs.principal_due_by_schedule,0)) BETWEEN p_ar_in_days_from AND p_ar_in_days_to
                                           THEN a.cod_cust_id
                                           ELSE NULL
                                      END))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_pa_st,
                  SUM(COUNT(DISTINCT a.cod_cust_id)) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count,
                  SUM(COUNT(DISTINCT DECODE(aa.flg_negotiated, 'Y', a.cod_cust_id, NULL))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_rst,
                  SUM(COUNT(DISTINCT DECODE(lac.num_1, 1, a.cod_cust_id, NULL))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_exp,
                  SUM(COUNT(DISTINCT DECODE(lac.num_1, 2, a.cod_cust_id, NULL))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_st,
                  SUM(COUNT(DISTINCT a.cod_cust_id)) OVER (ORDER BY NULL) client_count_total,
                  SUM(COUNT(DISTINCT DECODE(v_date_from, NULL, NULL, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN a.cod_cust_id ELSE NULL END)))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_disb,
                  SUM(COUNT(DISTINCT DECODE(v_date_from, NULL, NULL, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN DECODE(aa.flg_negotiated, 'Y', a.cod_cust_id, NULL) ELSE NULL END)))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_disb_rst,
                  SUM(COUNT(DISTINCT DECODE(v_date_from, NULL, NULL, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN DECODE(lac.num_1, 1, a.cod_cust_id, NULL) ELSE NULL END)))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_disb_exp,
                  SUM(COUNT(DISTINCT DECODE(v_date_from, NULL, NULL, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN DECODE(lac.num_1, 2, a.cod_cust_id, NULL) ELSE NULL END)))) OVER (PARTITION BY up.cod_userno, a.cod_prod) client_count_disb_st,
                  SUM(DECODE(v_date_from, NULL, 0, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN (a.amt_face_value * cru.currency_rate) ELSE 0 END))) disb_amount,
                  SUM(DECODE(v_date_from, NULL, 0, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN (DECODE(aa.flg_negotiated, 'Y', a.amt_face_value, 0) * cru.currency_rate) ELSE 0 END))) disb_amount_rst,
                  SUM(DECODE(v_date_from, NULL, 0, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN (DECODE(lac.num_1, 1, a.amt_face_value, 0) * cru.currency_rate) ELSE 0 END))) disb_amount_exp,
                  SUM(DECODE(v_date_from, NULL, 0, (CASE WHEN a.dat_first_disb BETWEEN v_date_from AND v_date-1 THEN (DECODE(lac.num_1, 2, a.amt_face_value, 0) * cru.currency_rate) ELSE 0 END))) disb_amount_st
           FROM ln_acct_dtls a,
                ln_acct_attributes aa,
                ln_acct_balances ab,
                ln_acct_cbr_dtls lac,
                conv_ln_acct_balances cab,
                conv_ln_acct_int_balance_dtls caib,
                ac_acct_writeoff wo,
                sm_user_profile up,
                rpt_system_problem_loans spl,
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
                --
                (SELECT rs1.cod_acct_no cod_acct_no,
                        --SUM(rs1.amt_principal) loan_amt_by_schedule,
                        SUM(CASE WHEN rs1.date_instal < v_date THEN rs1.amt_principal ELSE 0 END) principal_due_by_schedule
                 FROM ln_acct_schedule_detls rs1
                 GROUP BY rs1.cod_acct_no) rs,
                --
                (SELECT dtl.cod_acct_no,
                        SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                 FROM rpt_ln_daily_txn_log_po dtl
                 WHERE dtl.dat_value <= v_date
                 GROUP BY dtl.cod_acct_no) tl,
                (SELECT NVL((SELECT TRIM(rst.setting_value)
                             FROM rpt_report_settings rst
                             WHERE rst.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                 FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
           WHERE a.cod_acct_no = ab.cod_acct_no
                 AND a.cod_acct_no = aa.cod_acct_no
                 AND a.cod_acct_no = tl.cod_acct_no
                 AND a.cod_acct_no = rs.cod_acct_no(+)
                 AND a.cod_acct_no = cab.cod_acct_no(+)
                 AND a.cod_acct_no = caib.cod_acct_no(+)
                 AND a.cod_acct_no = wo.cod_acct_no(+)
                 AND a.cod_acct_no = lac.cod_acct_no(+)
                 AND a.cod_acct_no = mlc.account_no(+)
                 AND a.cod_acct_no = spl.account_no(+)
                 AND cru.currency_no = a.cod_ccy
                 AND up.cod_userno = nvl(mlc.co_user_no, aa.cod_acct_off)
                 AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                 AND ab.amt_disbursed > 0
                 --
                 --AND NVL(a.dat_acct_close, :v_date + 1) > :v_date
                 --AND NVL(wo.dat_writeoff, :v_date + 1) > :v_date
                 --AND a.dat_first_disb <= :v_date
                 AND INSTR(NVL(br.branch_id_list, up.cod_cc_brn), up.cod_cc_brn, 1) > 0
           GROUP BY up.cod_userno,
                    a.cod_prod
           ORDER BY up.cod_userno,
                    a.cod_prod;
  BEGIN
       FOR dt IN c_dates
       LOOP
           FOR i IN c_po_for_co(dt.rep_date, dt.rep_date_from)
           LOOP
               PIPE ROW(i);
           END LOOP;
           --dbms_output.put_line(to_char(dt.rep_month, 'yyyy.mm.dd'));
       END LOOP;
       
       RETURN;
  END;

END;
/
