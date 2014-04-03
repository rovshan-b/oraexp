CREATE OR REPLACE PACKAGE reports_ag
IS

  -- Author  : ELDANIZ
  -- Created : 11.08.2009 15:33:28
  -- Purpose : Agreements

  TYPE result_cursor IS REF CURSOR;

  -- LNA1030 - Group credit agreement
  -- LNA1040 - Group payment schedule
  -- LNA1041 - POA attachment for group
  -- LNA1042 - Detailed group payment schedule
  -- LNA1141 - Group disbursement authorization letter
  FUNCTION get_group_customers(p_group_id VARCHAR2,
                               p_cycle_no NUMBER,
                               p_customers VARCHAR2)
  RETURN result_cursor;

  -- LNA1050 - Loan payment schedule
  -- LNA1051 - POA attachment for loan
  FUNCTION get_loan_customer_info(p_account_no VARCHAR2) RETURN result_cursor;

  FUNCTION get_loan_ps(p_account_no VARCHAR2,
                       p_session_id VARCHAR2 := NULL)
  RETURN result_cursor;
  FUNCTION get_loan_ps_a(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION get_loan_ps_b(p_account_no VARCHAR2) RETURN result_cursor;
  -- auto loan sessions (LNA1035,LNA1060,LNA1091)
  FUNCTION get_loan_ps_auto(p_session_id VARCHAR2) RETURN result_cursor;

  -- LNA1040 - Group payment schedule
  FUNCTION get_group_ps(p_group_id VARCHAR2,
                        p_cycle_no NUMBER,
                        p_customers VARCHAR2/*,
                        p_sessions VARCHAR2*/)
  RETURN result_cursor;
  FUNCTION get_group_ps_a(p_group_id VARCHAR2,
                          p_cycle_no NUMBER,
                          p_customers VARCHAR2/*,
                          p_sessions VARCHAR2*/)
  RETURN result_cursor;
  FUNCTION get_group_ps_b(p_group_id VARCHAR2,
                          p_cycle_no NUMBER,
                          p_customers VARCHAR2)
  RETURN result_cursor;

  -- LNA1042 - Detailed group payment schedule
  FUNCTION get_group_ps_det(p_group_id VARCHAR2,
                            p_cycle_no NUMBER,
                            p_customers VARCHAR2,
                            p_split NUMBER := 0)
  RETURN result_cursor;
  FUNCTION get_group_ps_det_a(p_group_id VARCHAR2,
                              p_cycle_no NUMBER,
                              p_customers VARCHAR2,
                              p_split NUMBER := 0)
  RETURN result_cursor;
  FUNCTION get_group_ps_det_b(p_group_id VARCHAR2,
                              p_cycle_no NUMBER,
                              p_customers VARCHAR2,
                              p_split NUMBER := 0)
  RETURN result_cursor;
  -- LNA1042 - Detailed group payment schedule

  -- LNA1010 - Loan agreements
  FUNCTION lna1010_agreement_info(p_account_no VARCHAR2) RETURN result_cursor;  
  FUNCTION lna1010_loan_group_info(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION lna1010_loan_guarantors(p_account_no VARCHAR2) RETURN result_cursor;
  FUNCTION lna1010_loan_collaterals(p_account_no VARCHAR2, p_collateral_id VARCHAR2) RETURN result_cursor;
  -- LNA1010 - Loan agreements

  -- LNA1020 - Individual credit agreement
  FUNCTION lna1020(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor;

  -- LNA1030 - Group credit agreement
  FUNCTION lna1030(p_group_id VARCHAR2,
                   p_cycle_no NUMBER,
                   p_report_date DATE,
                   p_customers VARCHAR2)
  RETURN result_cursor;

  -- LNA1035 - Automobil credit agreement
  -- LNA1150 - Salary receivership agreement
  FUNCTION lna1035(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor;

  -- LNA1060 - Automobil collateral agreement
  FUNCTION lna1060(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor;

  -- LNA1070 - Mortgage collateral agreement
  FUNCTION lna1070(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor;
  -- LNA1071 - Mortgage collateral agreement (Joint)
  FUNCTION lna1071(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor;

  -- LNA1080 - General collateral agreement
  FUNCTION lna1080(p_collateral_id VARCHAR2,
                   p_collateral_type_id VARCHAR2,
                   p_account_no VARCHAR2,
                   p_report_date DATE)
  RETURN result_cursor;
  FUNCTION lna1080_collateral_attributes(p_collateral_id VARCHAR2,
                                         p_collateral_type_id VARCHAR2,
                                         p_account_no VARCHAR2)
  RETURN result_cursor;
  FUNCTION lna1080_collateral_accounts(p_collateral_id VARCHAR2) RETURN result_cursor;
  -- LNA1080 - General collateral agreement

  -- LNA1090 - Guarantee agreement
  -- LNA1091 - Automobil guarantee agreement
  -- LNA1151 - Guarantor salary receivership agreement
  FUNCTION lna1090(p_account_no VARCHAR2,
	                 p_customer_id NUMBER,
									 p_report_date DATE)
  RETURN result_cursor;
  
  -- LNA1100 - Payment order
  FUNCTION lna1100(p_branch_id NUMBER,
                   p_ccy NUMBER,
                   p_prop_disb_date DATE)
  RETURN result_cursor;

  -- LNA1101 - Bank payment order
  FUNCTION lna1101(p_ccy NUMBER,
                   p_em_bank_code VARCHAR2,
                   p_em_bank_account VARCHAR2,
                   p_bn_bank_code VARCHAR2,
                   p_bn_bank_account VARCHAR2,
                   p_amount NUMBER,
                   p_additional_info NUMBER :=0)
  RETURN result_cursor;
  
  -- LNA1120 - Commission fee receipt - 'CM'
  -- LNA1121 - Repayment receipt (KB) - 'CR'
  -- LNA1122 - Repayment receipt (IB) - 'CR'
  -- LNA1126 - Disbursement receipt - 'LA'
  FUNCTION lna1120(p_account_no VARCHAR2,
                   p_report_date DATE := trunc(SYSDATE),
                   p_report_type VARCHAR2 := 'CM')
  RETURN result_cursor;
  
  -- LNA1122 - Repayment receipts for subscription
  FUNCTION lna1125(p_user_no NUMBER) RETURN result_cursor;
  
  -- LNA1130 - Buy and Sell cash (Bank)
  -- LNA1131 - Buy and Sell cash (Vault)
  FUNCTION lna1130(p_branch_id NUMBER,
                   p_date DATE,
                   p_buy_sell VARCHAR2,
                   p_amount_and_tcy VARCHAR2)
  RETURN result_cursor;

  -- LNA1140 - Individual disbursement authorization letter
  FUNCTION lna1140(p_date DATE,
                   p_branch_id NUMBER,
                   p_account_no VARCHAR2,
                   p_bank_code VARCHAR2)
  RETURN result_cursor;

  -- LNA1141 - Group disbursement authorization letter
  FUNCTION lna1141(p_date DATE,
                   p_branch_id NUMBER,
                   p_group_id VARCHAR2,
                   p_group_cycle NUMBER,
                   p_customers VARCHAR2,
                   p_bank_code VARCHAR2)
  RETURN result_cursor;

  -- LNA1170 - Client registration form
  FUNCTION lna1170(p_customer_id NUMBER) RETURN data_types.result_cursor;

  -- LNA1180 - Credit line agreement
  FUNCTION lna1180(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor;

  -- LNA1190 - Loan restucture agreement
  FUNCTION lna1190(p_account_no VARCHAR2) RETURN result_cursor;

END;
/
CREATE OR REPLACE PACKAGE BODY reports_ag
IS

  -- LNA1030 - Group credit agreement
  -- LNA1040 - Group payment schedule
  -- LNA1041 - POA attachment for group
  -- LNA1042 - Detailed group payment schedule
  -- LNA1141 - Group disbursement authorization letter
  FUNCTION get_group_customers(p_group_id VARCHAR2,
                               p_cycle_no NUMBER,
                               p_customers VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_res AS (SELECT DISTINCT TRIM(gd.cod_group) group_id,
                                  gd.cod_group_cycle cycle_no,
                                  TRIM(gd.nam_group) group_name,
                                  TRIM(c.cod_cust_id) customer_id,
                                  TRIM(c.nam_cust_full) customer_name,
                                  --TRIM(c.cod_cust_natl_id) customer_ic,
                                  TRIM(cld.field_value) customer_ic,
                                  TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 ||
                                  DECODE(TRIM(c.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c.txt_permadr_add2)) ||
                                  DECODE(TRIM(c.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c.txt_permadr_add3))) customer_address,
                                  TRIM(gd.cod_acct_no) account_no,
                                  DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                                  TRIM(pm.nam_ccy_short) currency_code,
                                  a.rat_var_current loan_rate,
                                  TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                                            WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                                            WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                                            ELSE 0
                                       END) loan_term_value,
                                  TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                                            WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                                            WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                                            ELSE '0'
                                       END) loan_term_type,
                                  NVL((CASE WHEN dp.nam_plan LIKE '%IAL%' OR dp.nam_plan LIKE '%CAL%'
                                            THEN 2.5
                                            WHEN dp.nam_plan LIKE '%FEX%'
                                            THEN (CASE WHEN a.amt_face_value < DECODE(a.cod_ccy, 840, 5000, 4000) THEN 3
                                                       WHEN a.amt_face_value BETWEEN DECODE(a.cod_ccy, 840, 5000, 4000) AND DECODE(a.cod_ccy, 840, 9999, 7999) THEN 2
                                                  END)
                                            WHEN dp.nam_plan LIKE '%SME%'
                                            THEN 2/*(CASE WHEN a.amt_face_value BETWEEN 5000 AND 9999 THEN 3
                                                       WHEN a.amt_face_value >= 10000 THEN 2
                                                  END)*/
                                            WHEN dp.nam_plan LIKE '%SCG%' THEN 4
                                            WHEN dp.nam_plan LIKE '%RLP%'
                                            THEN (CASE WHEN pm.cod_prod_term_typ = 3 AND (a.ctr_term_months/65536) > 12
                                                       THEN 3
                                                       ELSE 2
                                                  END)
                                       END), 0) commission_rate,
                                  TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone) home_phone,
                                  TRIM(c.ref_phone_mobile) mobile_phone,
                                  TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) business_phone
                           FROM ci_custmast@host_link c,
                                ci_custdetl@host_link cd,
                                ba_group_details@host_link gd,
                                ln_acct_dtls@host_link a,
                                ln_acct_balances@host_link ab,
                                ln_prod_mast@host_link pm,
                                ba_sc_code@host_link sc,
                                ln_dedn_plan@host_link dp,
                                udf_cust_log_details@host_link cld
                           WHERE a.cod_cust_id = c.cod_cust_id
                                 AND a.cod_cust_id = cd.cod_cust_id
                                 AND a.cod_acct_no = gd.cod_acct_no
                                 AND a.cod_acct_no = ab.cod_acct_no
                                 AND pm.cod_prod = a.cod_prod
                                 AND dp.cod_plan = pm.cod_dedn_plan
                                 AND sc.cod_sc = dp.cod_deduction
                                 AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                                 --
                                 AND cld.cod_field_tag(+) = 'TXT_5'
                                 --
                                 AND cld.flg_mnt_status(+) = 'A'
                                 AND c.flg_mnt_status = 'A'
                                 AND gd.flg_mnt_status = 'A'
                                 AND a.flg_mnt_status = 'A'
                                 AND pm.flg_mnt_status = 'A'
                                 AND sc.flg_mnt_status = 'A'
                                 AND dp.flg_mnt_status = 'A'
                                 --
                                 --AND INSTR(p_customers, TRIM(c.cod_cust_id)) > 0
                                 AND gd.cod_group = TRIM(p_group_id)
                                 AND gd.cod_group_cycle = p_cycle_no)
                SELECT ROWNUM row_no,
                       t.*
                FROM (SELECT w.*
                      FROM w_res w
                      WHERE INSTR(p_customers, w.customer_id) > 0
                      ORDER BY 5) t;

       RETURN c_result;
  END;

  -- LNA1050 - Loan payment schedule
  -- LNA1051 - POA attachment for loan
  FUNCTION get_loan_customer_info(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

			    OPEN c_result FOR
            SELECT TRIM(a.cod_acct_no) account_no,
                   c.cod_cust_id customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   --TRIM(c.cod_cust_natl_id) customer_ic,
                   TRIM(cld.field_value) customer_ic,
                   TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 ||
                        DECODE(TRIM(c.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c.txt_permadr_add2)) ||
                        DECODE(TRIM(c.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c.txt_permadr_add3))) customer_address,
                   (DECODE(TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone), NULL, NULL, TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone) || ' (ev)') ||
                    DECODE(TRIM(c.ref_phone_mobile), NULL, NULL, ', ' || TRIM(c.ref_phone_mobile) || ' (mobil)') ||
                    DECODE(TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone), NULL, NULL, ', ' || TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) || ' (biznes)')) customer_phone,
                   a.amt_face_value loan_amount,
                   TRIM(pm.nam_ccy_short) currency_code,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                             ELSE 0
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type
            FROM ln_acct_dtls@host_link a,
                 ci_custmast@host_link c,
								         ci_custdetl@host_link cd,
                 ln_prod_mast@host_link pm,
                 udf_cust_log_details@host_link cld
            WHERE c.cod_cust_id = a.cod_cust_id
                  AND cd.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld.flg_mnt_status(+) = 'A'
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;

  FUNCTION get_loan_ps(p_account_no VARCHAR2,
                       p_session_id VARCHAR2 := NULL)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_count NUMBER := 0;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       SELECT COUNT(*) INTO v_count
       FROM ln_acct_schedule_detls@host_link sd
       WHERE sd.cod_acct_no = v_account_no;

       c_result := (CASE WHEN v_count > 0
                         THEN get_loan_ps_b(v_account_no)
                         ELSE (CASE WHEN p_session_id IS NULL
                                    THEN get_loan_ps_a(v_account_no)
                                    ELSE get_loan_ps_auto(p_session_id)
                               END)
                    END);

       RETURN c_result;
  END;

  FUNCTION get_loan_ps_a(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       OPEN c_result FOR
            SELECT ROWNUM payment_no,
                   ps.date_instal payment_date,
                   ps.amt_princ_bal principal_balance,
                   ps.amt_principal principal_amount,
                   ps.amt_interest interest_amount,
                   ps.amt_instal_outst payment_amount/*,
                   ps.session_id,
                   ps.cod_app_id application_id,
                   ps.cod_cust_id customer_id*/
            FROM rpt_ln_r501@host_link ps,
                 (SELECT --MAX(rl.session_id) max_id
                         TRIM(MAX(TO_NUMBER(SUBSTR(rl.session_id, 5)))) max_id
                  FROM rpt_ln_r501@host_link rl,
                       or_applications@host_link ap
                  WHERE rl.cod_app_id = ap.app_aad_id
                        --
                        AND TRIM(ap.app_acc_nbr) = v_account_no) t
            WHERE --ps.session_id = t.max_id
                  SUBSTR(ps.session_id, 5) = t.max_id
            ORDER BY 2, 1;

       RETURN c_result;
  END;

  FUNCTION get_loan_ps_b(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT /*s.ctr_instal*/ ROWNUM payment_no,
                   t.*
            FROM (SELECT s.date_instal payment_date,
                         s.amt_princ_bal principal_balance,
                         s.amt_principal principal_amount,
                         s.amt_interest interest_amount,
                         (s.amt_principal + s.amt_interest) payment_amount
                  FROM ln_acct_schedule_detls@host_link s
                  WHERE s.cod_acct_no = v_account_no
                  ORDER BY 1) t;

       RETURN c_result;
  END;

  -- auto loan sessions (LNA1035,LNA1060,LNA1091)
  FUNCTION get_loan_ps_auto(p_session_id VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ROWNUM payment_no,
                   ps.date_instal payment_date,
                   ps.amt_princ_bal principal_balance,
                   ps.amt_principal principal_amount,
                   ps.amt_interest interest_amount,
                   ps.amt_instal_outst payment_amount/*,
                   ps.session_id,
                   ps.cod_app_id application_id,
                   ps.cod_cust_id customer_id*/
            FROM rpt_ln_r501@host_link ps
            WHERE ps.session_id = p_session_id
            ORDER BY 2, 1;

       RETURN c_result;
  END;

  -- LNA1040 - Group payment schedule
  FUNCTION get_group_ps(p_group_id VARCHAR2,
                        p_cycle_no NUMBER,
                        p_customers VARCHAR2/*,
                        p_sessions VARCHAR2*/)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_count NUMBER := 0;
  BEGIN
       SELECT COUNT(*) INTO v_count
       FROM ln_acct_schedule_detls@host_link sd,
            ba_group_details@host_link gd
       WHERE sd.cod_acct_no = gd.cod_acct_no
             AND gd.flg_group_acc_attr = 'A'
             AND gd.ctr_stage_no = 1
             AND gd.cod_group = TRIM(p_group_id)
             AND gd.cod_group_cycle = p_cycle_no;

       c_result := (CASE WHEN v_count > 0
                         THEN get_group_ps_b(p_group_id, p_cycle_no, p_customers)
                         ELSE get_group_ps_a(p_group_id, p_cycle_no, p_customers/*, p_sessions*/)
                    END);

       RETURN c_result;
  END;

  FUNCTION get_group_ps_a(p_group_id VARCHAR2,
                          p_cycle_no NUMBER,
                          p_customers VARCHAR2/*,
                          p_sessions VARCHAR2*/)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       /*log_manager.add_log('TEST',
                           'get_group_payment_schedule_a',
                           'p_customers -> ' || p_customers || '; p_sessions -> ' || p_sessions,
                           SYSDATE,
                           SYSDATE,
                           'p_customers -> ' || p_customers || '; p_sessions -> ' || p_sessions,
                           1);*/
       
       OPEN c_result FOR
            SELECT ROWNUM payment_no,
                   p.*
            FROM (SELECT ps.date_instal payment_date,
                         SUM(ps.amt_princ_bal) principal_balance,
                         SUM(ps.amt_principal) principal_amount,
                         SUM(ps.amt_interest) interest_amount,
                         SUM(ps.amt_instal_outst) payment_amount/*,
                         ps.session_id,
                         ps.cod_app_id application_id,
                         ps.cod_cust_id customer_id*/
                  FROM rpt_ln_r501@host_link ps,
                       (SELECT rl.cod_app_id application_id,
                               --MAX(rl.session_id) max_id
                               TRIM(MAX(TO_NUMBER(SUBSTR(rl.session_id, 5)))) max_id
                               --DISTINCT rl.session_id
                        FROM rpt_ln_r501@host_link rl,
                             ba_group_details@host_link gd
                        WHERE rl.cod_app_id = gd.cod_application_id
                              --
                              AND INSTR(p_customers, TRIM(rl.cod_cust_id)) > 0
                              --AND instr(p_sessions, trim(rl.session_id)) > 0
                              --
                              AND gd.cod_group = TRIM(p_group_id)
                              AND gd.cod_group_cycle = p_cycle_no
                        GROUP BY rl.cod_app_id) t
                  WHERE ps.cod_app_id = t.application_id
                        AND SUBSTR(ps.session_id, 5) = t.max_id
                  GROUP BY ps.date_instal
                  ORDER BY 1) p;

       RETURN c_result;
  END;

  FUNCTION get_group_ps_b(p_group_id VARCHAR2,
                          p_cycle_no NUMBER,
                          p_customers VARCHAR2)
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
                  FROM (SELECT DISTINCT--trim(gd.cod_group) group_id,
                               sd.cod_acct_no,
                               sd.date_instal payment_date,
                               sd.amt_princ_bal principal_balance,
                               sd.amt_principal principal_amount,
                               sd.amt_interest interest_amount,
                               (sd.amt_principal + sd.amt_interest) payment_amount
                        FROM ln_acct_schedule_detls@host_link sd,
                             ba_group_details@host_link gd,
                             ln_acct_dtls@host_link a
                        WHERE sd.cod_acct_no = gd.cod_acct_no
                              AND a.cod_acct_no = gd.cod_acct_no
                              --
                              AND gd.cod_group = TRIM(p_group_id)
                              AND gd.cod_group_cycle = p_cycle_no
                              AND INSTR(p_customers, TRIM(a.cod_cust_id)) > 0
                        ) t
                  GROUP BY t.payment_date
                  ORDER BY 1) ps;

       RETURN c_result;
  END;
  -- LNA1040 - Group payment schedule

  -- LNA1042 - Detailed group payment schedule
  -- LNA1043 - Detailed group payment schedule (Splitted)
  FUNCTION get_group_ps_det(p_group_id VARCHAR2,
                            p_cycle_no NUMBER,
                            p_customers VARCHAR2,
                            p_split NUMBER := 0)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_count NUMBER := 0;
  BEGIN
       SELECT COUNT(*) INTO v_count
       FROM ln_acct_schedule_detls@host_link sd,
            ba_group_details@host_link gd
       WHERE sd.cod_acct_no = gd.cod_acct_no
             AND gd.flg_group_acc_attr = 'A'
             AND gd.ctr_stage_no = 1
             AND gd.cod_group = TRIM(p_group_id)
             AND gd.cod_group_cycle = p_cycle_no;

       c_result := (CASE WHEN v_count > 0
                         THEN get_group_ps_det_b(p_group_id, p_cycle_no, p_customers, p_split)
                         ELSE get_group_ps_det_a(p_group_id, p_cycle_no, p_customers, p_split)
                    END);

       RETURN c_result;
  END;

  FUNCTION get_group_ps_det_a(p_group_id VARCHAR2,
                              p_cycle_no NUMBER,
                              p_customers VARCHAR2,
                              p_split NUMBER := 0) -- 1 splitted, 0 - not splitted
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ps AS (SELECT ROWNUM payment_no,
                                 (DECODE(p_split, 0, p.account_no, p.customer_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                                 (DECODE(TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone), NULL, NULL, TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone) || ' (ev)') ||
                                  DECODE(TRIM(c.ref_phone_mobile), NULL, NULL, ', ' || TRIM(c.ref_phone_mobile) || ' (mobil)') ||
                                  DECODE(TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone), NULL, NULL, ', ' || TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) || ' (biznes)')) customer_phone,
                                 p.*
                          FROM (SELECT ps.cod_cust_id customer_id,
                                       t.group_name,
                                       TRIM(t.account_no) account_no,
                                       ps.date_instal payment_date,
                                       SUM(ps.amt_princ_bal) principal_balance,
                                       SUM(ps.amt_principal) principal_amount,
                                       SUM(ps.amt_interest) interest_amount,
                                       SUM(ps.amt_instal_outst) payment_amount
                                FROM rpt_ln_r501@host_link ps,
                                     (SELECT rl.cod_app_id application_id,
                                             gd.nam_group group_name,
                                             gd.cod_acct_no account_no,
                                             TRIM(MAX(TO_NUMBER(SUBSTR(rl.session_id, 5)))) max_id
                                      FROM rpt_ln_r501@host_link rl,
                                           ba_group_details@host_link gd
                                      WHERE rl.cod_app_id = gd.cod_application_id
                                            --
                                            --AND INSTR(p_customers, TRIM(rl.cod_cust_id)) > 0
                                            --AND instr(p_sessions, trim(rl.session_id)) > 0
                                            --
                                            AND gd.cod_group = TRIM(p_group_id)
                                            AND gd.cod_group_cycle = p_cycle_no
                                      GROUP BY rl.cod_app_id,
                                               gd.nam_group,
                                               gd.cod_acct_no) t
                                WHERE ps.cod_app_id = t.application_id
                                      AND SUBSTR(ps.session_id, 5) = t.max_id
                                GROUP BY ps.date_instal,
                                         ps.cod_cust_id,
                                         t.group_name,
                                         t.account_no
                                ORDER BY 1, 2) p,
                               ci_custmast@host_link c,
                               ci_custdetl@host_link cd
                          WHERE c.cod_cust_id = p.customer_id
                                AND cd.cod_cust_id = p.customer_id
                                AND c.flg_mnt_status = 'A')
            SELECT *
            FROM w_ps p
            WHERE INSTR(p_customers, TRIM(p.customer_id)) > 0;

       RETURN c_result;
  END;

  FUNCTION get_group_ps_det_b(p_group_id VARCHAR2,
                              p_cycle_no NUMBER,
                              p_customers VARCHAR2,
                              p_split NUMBER := 0)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ps AS (SELECT ROWNUM payment_no,
                                 (DECODE(p_split, 0, ps.account_no, ps.customer_id) || ' - ' || TRIM(c.nam_cust_full)) customer_name,
                                 (DECODE(TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone), NULL, NULL, TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone) || ' (ev)') ||
                                  DECODE(TRIM(c.ref_phone_mobile), NULL, NULL, ', ' || TRIM(c.ref_phone_mobile) || ' (mobil)') ||
                                  DECODE(TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone), NULL, NULL, ', ' || TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) || ' (biznes)')) customer_phone,
                                 ps.*
                          FROM (SELECT t.payment_date,
                                       t.group_name,
                                       t.customer_id,
                                       t.account_no,
                                       SUM(t.principal_balance) principal_balance,
                                       SUM(t.principal_amount) principal_amount,
                                       SUM(t.interest_amount) interest_amount,
                                       SUM(t.payment_amount) payment_amount
                                FROM (SELECT DISTINCT--trim(gd.cod_group) group_id,
                                             TRIM(gd.nam_group) group_name,
                                             TRIM(sd.cod_acct_no) account_no,
                                             a.cod_cust_id customer_id,
                                             sd.date_instal payment_date,
                                             sd.amt_princ_bal principal_balance,
                                             sd.amt_principal principal_amount,
                                             sd.amt_interest interest_amount,
                                             (sd.amt_principal + sd.amt_interest) payment_amount
                                      FROM ln_acct_schedule_detls@host_link sd,
                                           ba_group_details@host_link gd,
                                           ln_acct_dtls@host_link a
                                      WHERE sd.cod_acct_no = gd.cod_acct_no
                                            AND a.cod_acct_no = gd.cod_acct_no
                                            AND gd.cod_group = TRIM(p_group_id)
                                            AND gd.cod_group_cycle = p_cycle_no
                                            --AND INSTR(p_customers, TRIM(a.cod_cust_id)) > 0
                                      ) t
                                GROUP BY t.payment_date,
                                         t.group_name,
                                         t.customer_id,
                                         t.account_no
                                ORDER BY 1) ps,
                               ci_custmast@host_link c,
                               ci_custdetl@host_link cd
                          WHERE c.cod_cust_id = ps.customer_id
                                AND cd.cod_cust_id = ps.customer_id
                                AND c.flg_mnt_status = 'A')
            SELECT *
            FROM w_ps p
            WHERE INSTR(p_customers, TRIM(p.customer_id)) > 0;

       RETURN c_result;
  END;
  -- LNA1042 - Detailed group payment schedule
  -- LNA1043 - Detailed group payment schedule (Splitted)

  -- LNA1010 - Loan agreements
  FUNCTION lna1010_agreement_info(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT DISTINCT TRIM(a.cod_acct_no) account_no,
                   TRIM(CASE WHEN gd.cod_acct_no IS NULL
                             THEN (CASE WHEN a.cod_prod IN(441,442,443)
                                        THEN 'AUTO'
                                        ELSE 'IND'
                                   END)
                             ELSE 'GRP' END) loan_type,
                   TRIM(CASE WHEN gd.cod_acct_no IS NULL 
                             THEN (CASE WHEN a.cod_prod IN(441,442,443)
                                        THEN 'Automobil'
                                        ELSE 'Individual'
                                   END)
                             ELSE 'Group'
                        END) loan_type_desc,
                   TRIM(gd.cod_group) group_no,
                   gd.cod_group_cycle group_cycle_id
            FROM ln_acct_dtls@host_link a,
                 ba_group_details@host_link gd
            WHERE a.cod_acct_no = gd.cod_acct_no(+)
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;
  
  FUNCTION lna1010_loan_group_info(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT DISTINCT NVL(gd1.cod_branch_group, a.cod_cc_brn) branch_id,
                   TRIM(NVL(gd1.cod_acct_no, a.cod_acct_no)) account_no,
                   --(CASE WHEN gd1.cod_acct_no IS NULL THEN 'IND' ELSE 'GRP' END) loan_type,
                   --(CASE WHEN gd1.cod_acct_no IS NULL THEN 'Individual' ELSE 'Group' END) loan_type_desc,
                   TRIM(gd1.cod_group) group_id,
                   gd1.cod_group_cycle group_cycle,
                   TRIM(gd1.nam_group) group_name,
                   c.cod_cust_id customer_id,
                   TRIM(c.nam_cust_full) customer_name
            FROM ba_group_details@host_link gd1,
                 ba_group_details@host_link gd2,
                 ln_acct_dtls@host_link a,
                 ci_custmast@host_link c
            WHERE gd2.cod_acct_no(+) = a.cod_acct_no
                  AND gd1.cod_group(+) = gd2.cod_group
                  AND gd1.cod_group_cycle(+) = gd2.cod_group_cycle
                  AND c.cod_cust_id = NVL(gd1.cod_cust, a.cod_cust_id)
                  --
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;

  -- LNA1050 - Loan payment schedule
  FUNCTION lna1010_loan_guarantors(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT TRIM(cx.cod_acct_no) account_no,
                   c.cod_cust_id customer_id,
                   /*(trim(c.cod_cust_id) || ' - ' || */TRIM(c.nam_cust_full)/*)*/ customer_name,
                   (TRIM(cx.cod_acct_no) || '-' || TRIM(c.cod_cust_id)) agreement_no,
                   TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone) home_phone,
                   TRIM(c.ref_phone_mobile) mobile_phone,
                   TRIM(cd.ref_cust_emp_phone_area || ' ' || cd.ref_cust_emp_phone) business_phone
            FROM ch_acct_cust_xref@host_link cx,
                 ci_custmast@host_link c,
						           ci_custdetl@host_link cd
            WHERE c.cod_cust_id = cx.cod_cust
						            AND cd.cod_cust_id(+) = cx.cod_cust
                  AND cx.cod_acct_cust_rel = 'GUA'
                  --
                  AND cx.flg_mnt_status = 'A'
                  AND c.flg_mnt_status = 'A'
                  --
                  AND cx.cod_acct_no = v_account_no;

       RETURN c_result;
  END;

  FUNCTION lna1010_loan_collaterals(p_account_no VARCHAR2, p_collateral_id VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT DISTINCT (TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)) agreement_no,
                   TRIM(ca.cod_acct_no) account_no,
                   TRIM(ca.cod_collat_id) collateral_id,
                   ca.cod_coll collateral_code,
                   cc.cod_coll_type collateral_type_id,
                   TRIM(DECODE(cc.cod_coll_type, 1, 'PR', 2, 'TR', 'NS')) collateral_key,
                   TRIM(DECODE(cc.cod_coll_type, 1, 'Property', 2, 'Transport', 'Non-Standard')) collateral_type_name,
                   TRIM(cc.nam_coll) collateral_type_desc,
                   TRIM(ch.txt_deed_detl_1) collateral_desc,
                   ca.amt_coll_value collateral_price,
                   ch.amt_market_val market_price
            FROM ba_lo_coll_acct_xref@host_link ca,
                 ba_coll_hdr@host_link ch,
                 ba_coll_codes@host_link cc
            WHERE ca.cod_collat_id = ch.cod_collat_id
                  AND ch.cod_coll = cc.cod_coll
                  AND ca.cod_coll = cc.cod_coll
                  AND ca.cod_acct_no = v_account_no
                  AND ca.cod_collat_id = NVL(TRIM(p_collateral_id), ca.cod_collat_id)
            ORDER BY 4, 3;

       RETURN c_result;
  END;
  -- LNA1010 - Loan agreements

  -- LNA1020 - Individual credit agreement
  FUNCTION lna1020(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT DISTINCT TRIM(a.cod_acct_no) account_no,
                   TRIM(c.cod_cust_id) customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   --TRIM(c.cod_cust_natl_id) customer_ic,
                   TRIM(cld.field_value) customer_ic,
                   TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 ||
                   DECODE(TRIM(c.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c.txt_permadr_add2)) ||
                   DECODE(TRIM(c.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c.txt_permadr_add3))) customer_address,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRUNC(a.dat_first_disb) disb_date,
                   DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                   TRIM(pm.nam_ccy_short) currency_code,
                   TRIM(cc.nam_currency) currency_name,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                             ELSE 0
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type,
                   TRIM('360') number_of_days_in_year,
                   NVL((CASE WHEN dp.nam_plan LIKE '%IAL%' OR dp.nam_plan LIKE '%CAL%'
                             THEN 2.5
                             WHEN dp.nam_plan LIKE '%FEX%'
                             THEN (CASE WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) < DECODE(a.cod_ccy, 840, 5000, 4000)
                                        THEN (CASE WHEN bm.cod_cc_brn IN(304,306) THEN 2 ELSE 3 END)
                                                   WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) BETWEEN DECODE(a.cod_ccy, 840, 5000, 4000) AND DECODE(a.cod_ccy, 840, 9999, 7999) THEN 2
                                                  END)
                             WHEN dp.nam_plan LIKE '%SME%'
                             THEN 2/*(CASE WHEN a.amt_face_value BETWEEN 5000 AND 9999 THEN 3
                                        WHEN a.amt_face_value >= 10000 THEN 2
                                   END)*/
                             WHEN dp.nam_plan LIKE '%SCG%' THEN 4
                             WHEN dp.nam_plan LIKE '%RLP%'
                             THEN (CASE WHEN pm.cod_prod_term_typ = 3 AND (a.ctr_term_months/65536) > 12
                                        THEN 3
                                        ELSE 2
                                   END)
                        END), 0) commission_rate,
                   (CASE WHEN a.amt_face_value <= 2000 THEN 1 -- only guarantor
                         WHEN a.amt_face_value BETWEEN 2001 AND 9999 THEN 2 -- guarantor + collateral
                         WHEN a.amt_face_value >= 10000 THEN 3 -- ipoteka
                    END) agreement_option,
                   '1' penalty_rate,
                   DECODE(sd.cod_acct_no, NULL, 'A', 'B') state_point,
                   DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb) report_date
            FROM ci_custmast@host_link c,
                 ln_acct_dtls@host_link a,
                 ln_acct_balances@host_link ab,
                 ln_prod_mast@host_link pm,
                 rpt_ba_cc_brn_mast@host_link bm,
                 sm_user_profile@host_link up,
                 ln_dedn_plan@host_link dp,
                 ln_acct_schedule_detls@host_link sd,
                 udf_cust_log_details@host_link cld,
                 ba_ccy_code@host_link cc
            WHERE c.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND cc.cod_ccy = a.cod_ccy
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND up.cod_cc_brn = a.cod_cc_brn
                  AND ab.cod_acct_no = a.cod_acct_no
                  AND sd.cod_acct_no(+) = a.cod_acct_no
                  AND dp.cod_plan = pm.cod_dedn_plan
                  AND up.cod_branch_templ = 55
                  AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld.cod_field_tag(+) = 'TXT_5'
                  --
                  AND c.flg_mnt_status = 'A'
                  AND a.flg_mnt_status = 'A'
                  AND pm.flg_mnt_status = 'A'
                  AND dp.flg_mnt_status = 'A'
                  --AND bm.flg_mnt_status = 'A'
                  AND cld.flg_mnt_status(+) = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND a.cod_acct_no = v_account_no
                  AND ROWNUM = 1;

       RETURN c_result;
  END;
  -- LNA1020 - Individual credit agreement

  -- LNA1030 - Group credit agreement
  FUNCTION lna1030(p_group_id VARCHAR2,
                   p_cycle_no NUMBER,
                   p_report_date DATE,
                   p_customers VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       OPEN c_result FOR
            WITH gca AS (SELECT (t.group_id || '-' || TRIM(t.group_cycle_id)) agreement_no,
                                t.group_id,
                                t.group_cycle_id,
                                t.group_name,
                                t.product_id,
                                (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(bm.nam_branch)) branch_name,
                                TRIM(bm.nam_cc_city) branch_city,
                                TRIM(up.nam_user) branch_manager,
                                v_bank_address bank_address,
                                t.created_date,
                                t.loan_amount,
                                t.loan_rate,
                                TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (t.loan_term)
                                          WHEN pm.cod_prod_term_typ IN(2,4) THEN (t.loan_term/367)
                                          WHEN pm.cod_prod_term_typ = 3 THEN (t.loan_term/65536)
                                          ELSE 0
                                     END) loan_term_value,
                                TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                                          WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                                          WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                                          ELSE '0'
                                     END) loan_term_type,
                                TRIM(CASE WHEN pm.cod_dedn_plan IN(1,2) THEN 'SCG'
                                          WHEN pm.cod_dedn_plan IN(3,4) THEN 'RLP'
                                     END) product_type,
                                TRIM(CASE WHEN pm.cod_dedn_plan IN(1,2) THEN 'Həmrəy'
                                          WHEN pm.cod_dedn_plan IN(3,4) THEN 'Kənd'
                                     END) agreement_type,
                                TRIM(pm.nam_ccy_short) loan_currency,
                                t.loan_rate_error,
                                t.loan_term_error,
                                t.state_point,
                                '1' penalty_rate
                         FROM (SELECT g.group_id,
                                      g.group_cycle_id,
                                      g.group_name,
                                      g.branch_id,
                                      g.product_id,
                                      MAX(g.created_date) created_date,
                                      SUM(g.loan_amount) loan_amount,
                                      MAX(g.loan_rate) loan_rate,
                                      MAX(g.loan_term) loan_term,
                                      AVG(g.loan_currency) loan_currency,
                                      MAX(g.state_point) state_point,
                                      (CASE WHEN MAX(g.loan_rate) != MIN(g.loan_rate) THEN 1 ELSE 0 END) loan_rate_error,
                                      (CASE WHEN MAX(g.loan_term) != MIN(g.loan_term) THEN 1 ELSE 0 END) loan_term_error
                               FROM (SELECT DISTINCT TRIM(gd.cod_group) group_id,
                                            gd.cod_group_cycle group_cycle_id,
                                            TRIM(gd.nam_group) group_name,
                                            TRIM(gd.cod_acct_no) account_no,
                                            --gd.cod_branch_group branch_id,
                                            a.cod_cc_brn branch_id,
                                            gd.cod_prod product_id,
                                            TRUNC(a.dat_first_disb) created_date,
                                            --a.amt_face_value loan_amount,
                                            DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                                            a.rat_var_current loan_rate,
                                            a.ctr_term_months loan_term,
                                            TRIM(a.cod_ccy) loan_currency,
                                            TRIM(DECODE(sd.cod_acct_no, NULL, 'A', 'B')) state_point
                                     FROM ln_acct_dtls@host_link a,
                                          ln_acct_balances@host_link ab,
                                          ba_group_details@host_link gd,
                                          ln_acct_schedule_detls@host_link sd
                                     WHERE a.cod_acct_no = gd.cod_acct_no
                                           AND a.cod_acct_no = ab.cod_acct_no
                                           AND sd.cod_acct_no(+) = gd.cod_acct_no
                                           --
                                           AND INSTR(p_customers, TRIM(a.cod_cust_id)) > 0
                                           AND gd.cod_group = TRIM(p_group_id)
                                           AND gd.cod_group_cycle = p_cycle_no) g
                               GROUP BY g.group_id,
                                        g.group_cycle_id,
                                        g.group_name,
                                        g.branch_id,
                                        g.product_id) t,
                              ln_prod_mast@host_link pm,
                              rpt_ba_cc_brn_mast@host_link bm,
                              sm_user_profile@host_link up
                         WHERE pm.cod_prod = t.product_id
                               AND bm.cod_cc_brn = t.branch_id
                               AND up.cod_branch_templ = 55
                               AND up.cod_cc_brn = t.branch_id
                               --
                               AND up.flg_mnt_status = 'A'
                               AND pm.flg_mnt_status = 'A'
                               --
                               AND INSTR(v_branch_manager_list, up.cod_user_id) > 0)
            SELECT tb.*,
                   DECODE(tb.state_point, 'A', p_report_date, tb.created_date) report_date,
                   report_methods.calculate_service_charge(tb.product_id, tb.loan_term_value, tb.loan_amount) commission_rate
            FROM gca tb;

       RETURN c_result;
  END;
  -- LNA1030 - Group credit agreement

  -- LNA1035 - Automobil credit agreement
  -- LNA1150 - Salary receivership agreement
  FUNCTION lna1035(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
    v_agreement_no_automobil VARCHAR2(200) := '';
    v_agreement_no_guarantor VARCHAR2(200) := '';
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       SELECT MAX(TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id))
              INTO v_agreement_no_automobil
       FROM ba_lo_coll_acct_xref@host_link ca
       WHERE ca.cod_acct_no = v_account_no
             AND ROWNUM = 1;

       SELECT --WM_CONCAT((TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust)))
              LISTAGG(TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust), ', ') WITHIN GROUP (ORDER BY TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust))
              INTO v_agreement_no_guarantor
       FROM ch_acct_cust_xref@host_link cx
       WHERE cx.cod_acct_cust_rel = 'GUA'
             AND cx.cod_acct_no = v_account_no;

       OPEN c_result FOR
            SELECT DISTINCT TRIM(a.cod_acct_no) account_no,
                   TRIM(c.cod_cust_id) customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   TRIM(cld.field_value) customer_ic,
                   TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1) customer_address,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRUNC(a.dat_first_disb) disb_date,
                   a.amt_face_value loan_amount,
                   TRIM(pm.nam_ccy_short) currency_code,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                             ELSE 0
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type,
                   TRIM('360') number_of_days_in_year,
                   NVL((CASE WHEN dp.nam_plan LIKE '%IAL%' OR dp.nam_plan LIKE '%CAL%'
                             THEN 2.5
                             WHEN dp.nam_plan LIKE '%FEX%'
                             THEN (CASE WHEN a.amt_face_value < DECODE(a.cod_ccy, 840, 5000, 4000) THEN 3
                                        WHEN a.amt_face_value BETWEEN DECODE(a.cod_ccy, 840, 5000, 4000) AND DECODE(a.cod_ccy, 840, 9999, 7999) THEN 2
                                   END)
                             WHEN dp.nam_plan LIKE '%SME%'
                             THEN 2/*(CASE WHEN a.amt_face_value BETWEEN 5000 AND 9999 THEN 3
                                        WHEN a.amt_face_value >= 10000 THEN 2
                                   END)*/
                             WHEN dp.nam_plan LIKE '%SCG%' THEN 4
                             WHEN dp.nam_plan LIKE '%RLP%'
                             THEN (CASE WHEN pm.cod_prod_term_typ = 3 AND (a.ctr_term_months/65536) > 12
                                        THEN 3
                                        ELSE 2
                                   END)
                        END), 0) commission_rate,
                   (CASE WHEN a.amt_face_value <= 2000 THEN 1 -- only guarantor
                         WHEN a.amt_face_value BETWEEN 2001 AND 9999 THEN 2 -- guarantor + collateral
                         WHEN a.amt_face_value >= 10000 THEN 3 -- ipoteka
                    END) agreement_option,
                   '1' penalty_rate,
                   TRIM(DECODE(sd.cod_acct_no, NULL, 'A', 'B')) state_point,
                   DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb) report_date,
                   /*(SELECT TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)
                    FROM ba_lo_coll_acct_xref@host_link ca
                    WHERE TRIM(ca.cod_acct_no) = TRIM(a.cod_acct_no)
                          AND ROWNUM = 1) agreement_no_automobil,
                   (SELECT --WM_CONCAT((TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust)))
                           LISTAGG(TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust), ', ')
                           WITHIN GROUP (ORDER BY TRIM(cx.cod_acct_no) || '-' || TRIM(cx.cod_cust))
                    FROM ch_acct_cust_xref@host_link cx
                    WHERE cx.cod_acct_cust_rel = 'GUA'
                          AND TRIM(cx.cod_acct_no) = TRIM(a.cod_acct_no)) agreement_no_guarantor*/
                   v_agreement_no_automobil agreement_no_automobil,
                   v_agreement_no_guarantor agreement_no_guarantor
            FROM ci_custmast@host_link c,
                 ln_acct_dtls@host_link a,
                 ln_prod_mast@host_link pm,
                 rpt_ba_cc_brn_mast@host_link bm,
                 sm_user_profile@host_link up,
                 ba_sc_code@host_link sc,
                 ln_dedn_plan@host_link dp,
                 ln_acct_schedule_detls@host_link sd,
                 udf_cust_log_details@host_link cld
            WHERE c.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND up.cod_cc_brn = a.cod_cc_brn
                  AND sd.cod_acct_no(+) = a.cod_acct_no
                  AND dp.cod_plan(+) = pm.cod_dedn_plan
                  AND sc.cod_sc(+) = dp.cod_deduction
                  AND up.cod_branch_templ = 55
                  AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld.cod_field_tag(+) = 'TXT_5'
                  --
                  AND c.flg_mnt_status = 'A'
                  AND a.flg_mnt_status = 'A'
                  AND pm.flg_mnt_status = 'A'
                  AND sc.flg_mnt_status(+) = 'A'
                  AND dp.flg_mnt_status(+) = 'A'
                  --AND bm.flg_mnt_status = 'A'
                  AND cld.flg_mnt_status(+) = 'A'
                  --
                  AND pm.cod_prod IN(441,442,443)
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND a.cod_acct_no = v_account_no
                  AND ROWNUM = 1;

       RETURN c_result;
  END;
  -- LNA1035 - Automobil credit agreement

  -- LNA1060 - Automobil collateral agreement
  FUNCTION lna1060(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       OPEN c_result FOR
            SELECT DISTINCT (TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)) agreement_no,
                   TRIM(ca.cod_acct_no) account_no,
                   TRIM(ca.cod_collat_id) collateral_id,
                   bm.cod_cc_brn branch_id,
                   TRIM(bm.nam_branch) branch_name,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   ca.cod_coll collateral_code,
                   cc.cod_coll_type collateral_type_id,
                   TRIM(DECODE(cc.cod_coll_type, 1, 'Property', 2, 'Transport', 'Non-Standard')) collateral_type_name,
                   TRIM(cc.nam_coll) collateral_type_desc,
                   TRIM(ch.txt_deed_detl_1) collateral_desc,
                   TRUNC(DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb)) report_date,
                   ch.cod_ccy currency_no,
                   TRIM(pm.nam_ccy_short) currency_code,
                   ch.amt_orig_value original_value,
                   ch.amt_last_val collateral_value,
                   ch.amt_market_val market_value,
                   c_ow.cod_cust_id collateral_owner_id,
                   --(trim(c_ow.nam_cust_mid) || ' ' || trim(c_ow.nam_cust_first) || ' ' || trim(c_ow.nam_cust_last) || ' ' || decode(c_ow.txt_cust_sex, 'F', 'qızı', 'oğlu')) collateral_owner_name,
                   TRIM(c_ow.nam_cust_full) collateral_owner_name,
                   TRIM(/*c_ow.nam_custadr_cntry || ', ' || */c_ow.nam_permadr_city || ', ' || c_ow.txt_permadr_add1) collateral_owner_address,
                   --TRIM(c_ow.cod_cust_natl_id) collateral_owner_ic,
                   TRIM(cld_ow.field_value) collateral_owner_ic,
                   c_bw.cod_cust_id borrower_customer_id,
                   --(trim(c_bw.nam_cust_mid) || ' ' || trim(c_bw.nam_cust_first) || ' ' || trim(c_bw.nam_cust_last) || ' ' || decode(c_bw.txt_cust_sex, 'F', 'qızı', 'oğlu')) borrower_customer_name,
                   TRIM(c_bw.nam_cust_full) borrower_customer_name,
                   TRIM(/*c_bw.nam_custadr_cntry || ', ' || */c_bw.nam_permadr_city || ', ' || c_bw.txt_permadr_add1) borrower_customer_address,
                   --TRIM(c_bw.cod_cust_natl_id) borrower_customer_ic,
                   TRIM(cld_bw.field_value) borrower_customer_ic,
                   a.amt_face_value loan_amount,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                         WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                         ELSE 0
                    END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type,
                   cau.cod_chassis_no tr_chassis_no,
                   cau.cod_regn_no tr_registration_no,
                   cau.cod_engine_no tr_engine_no,
                   cau.cod_mfg_year tr_manufacture_date,
                   cau.cod_model tr_model_name,
                   cau.txt_notes1 tr_tech_passport_no,
                   cau.txt_notes2 tr_auto_desc,
                   (SELECT TRIM(um.field_value)
                    FROM udf_field_mast@host_link uf,
                         udf_mnt_log_details@host_link um
                    WHERE uf.cod_field_tag = um.cod_field_tag
                          AND um.ref_udf_no = cau.ref_udf_no
                          AND uf.nam_field = 'ADDRESS') tr_collateral_address,
                   (SELECT TRIM(aaa.atr_value)
                    FROM or_application_ast_attrs@host_link aaa,
                         or_application_assets@host_link aa
                    WHERE aaa.atr_aad_id = aa.ase_aad_id
                          AND aaa.atr_ase_id = aa.ase_id
                          --
                          AND aaa.flg_mnt_status = 'A'
                          AND aa.flg_mnt_status = 'A'
                          AND aaa.atr_aat_type = 'VEHICLE_ATTR_1'
                          --
                          AND aa.ase_identification_nbr = ch.cod_collat_id
                          AND aa.ase_asb_type = ch.cod_coll
                          AND ROWNUM = 1) tr_auto_color
            FROM ba_lo_coll_acct_xref@host_link ca,
                 ba_coll_hdr@host_link ch,
                 ba_coll_codes@host_link cc,
                 ln_acct_dtls@host_link a,
                 rpt_ba_cc_brn_mast@host_link bm,
                 ba_coll_automobile@host_link cau,
                 ci_custmast@host_link c_ow, -- owner
                 ci_custmast@host_link c_bw, -- borrower
                 udf_cust_log_details@host_link cld_ow,
                 udf_cust_log_details@host_link cld_bw,
                 ln_prod_mast@host_link pm,
                 ln_acct_schedule_detls@host_link sd,
                 sm_user_profile@host_link up
            WHERE ca.cod_collat_id = ch.cod_collat_id
                  AND ch.cod_coll = cc.cod_coll
                  AND ca.cod_coll = cc.cod_coll
                  AND a.cod_acct_no = ca.cod_acct_no
                  AND bm.cod_cc_brn = ca.cod_cc_brn
                  AND ca.cod_collat_id = cau.cod_collat_id
                  AND ca.cod_coll = cau.cod_coll
                  AND c_ow.cod_cust_id(+) = TO_NUMBER(ch.nam_lender)
                  AND a.cod_cust_id = c_bw.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND sd.cod_acct_no(+) = ca.cod_acct_no
                  AND up.cod_branch_templ = 55
                  AND up.cod_cc_brn = ca.cod_cc_brn
                  AND cld_ow.cod_cust_id(+) = ch.nam_lender --edited by Yalchin on 17 Aprel 2012
                  AND to_char(a.cod_cust_id) = cld_bw.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld_ow.cod_field_tag(+) = 'TXT_5'
                  AND cld_bw.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld_ow.flg_mnt_status(+) = 'A'
                  AND cld_bw.flg_mnt_status(+) = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND ca.cod_collat_id = TRIM(p_collateral_id)
            ORDER BY 4, 3;

       RETURN c_result;
  END;
  -- LNA1060 - Automobil collateral agreement

  -- LNA1070 - Mortgage collateral agreement
  -- LNA1072 - Mortgage collateral agreement (Credit Line)
  FUNCTION lna1070(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       OPEN c_result FOR
            WITH w_upr AS (SELECT cp.cod_collat_id,
                                  MAX(DECODE(uf.nam_field, 'REGISTRATION_BOOK_NO', TRIM(um.field_value), '')) pr_reg_book_no,
                                  MAX(DECODE(uf.nam_field, 'REGISTRATION_PAGE_NO', TRIM(um.field_value), '')) pr_reg_page_no,
                                  MAX(DECODE(uf.nam_field, 'ADDRESS', TRIM(um.field_value), '')) pr_collateral_address,
                                  MAX(CASE WHEN UPPER(caa.txt_desc_attribute) LIKE '%FLOORS%' THEN caa.txt_default_value END) pr_number_of_floors,
                                  MAX(CASE WHEN UPPER(caa.txt_desc_attribute) LIKE '%ROOMS%' THEN caa.txt_default_value END) pr_number_of_rooms
                           FROM udf_field_mast@host_link uf,
                                udf_mnt_log_details@host_link um,
                                ba_coll_prop@host_link cp,
                                ba_coll_attribute_addons@host_link caa
                           WHERE uf.cod_field_tag = um.cod_field_tag
                                 AND um.ref_udf_no = cp.ref_udf_no
                                 AND caa.ref_udf_no(+) = cp.ref_udf_no
                                 AND caa.cod_coll(+) = cp.cod_coll
                                 --
                                 AND uf.nam_field IN('REGISTRATION_BOOK_NO','REGISTRATION_PAGE_NO','ADDRESS')
                                 --
                                 AND cp.cod_collat_id = TRIM(p_collateral_id)
                           GROUP BY cp.cod_collat_id),
                 w_udf AS (SELECT ca.cod_collat_id,
                                  MAX(DECODE(uald.cod_field_tag, 'DAT_0', uald.field_value, NULL)) line_last_date,
                                  MAX(DECODE(uald.cod_field_tag, 'NUM_3', uald.field_value, NULL)) line_total_amount,
                                  MAX(DECODE(uald.cod_field_tag, 'NUM_4', uald.field_value, NULL)) duration_in_month
                           FROM udf_acct_log_details@host_link uald,
                                ba_lo_coll_acct_xref@host_link ca
                           WHERE ca.cod_acct_no = RPAD(uald.cod_acct_no, 16, ' ')
                                 --
                                 AND uald.cod_field_tag IN('DAT_0','NUM_3','NUM_4')
                                 AND uald.flg_mnt_status = 'A'
                                 --
                                 AND ca.cod_collat_id = TRIM(p_collateral_id)
                           GROUP BY ca.cod_collat_id)
            SELECT DISTINCT (TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)) agreement_no,
                   TRIM(ca.cod_acct_no) account_no,
                   TRIM(ca.cod_collat_id) collateral_id,
                   bm.cod_cc_brn branch_id,
                   bm.nam_branch branch_name,
                   bm.nam_cc_city branch_city,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   ca.cod_coll collateral_code,
                   cc.cod_coll_type collateral_type_id,
                   TRIM(DECODE(cc.cod_coll_type, 1, 'Property', 2, 'Transport', 'Non-Standard')) collateral_type_name,
                   TRIM(cc.nam_coll) collateral_type_desc,
                   TRIM(ch.txt_deed_detl_1) collateral_desc,
                   TRUNC(DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb)) report_date,
                   ch.cod_ccy currency_no,
                   TRIM(pm.nam_ccy_short) currency_code,
                   ch.amt_orig_value original_value,
                   ch.amt_last_val collateral_value,
                   ch.amt_market_val market_value,
                   c_ow.cod_cust_id collateral_owner_id,
                   TRIM(c_ow.nam_cust_full) collateral_owner_name,
                   TRIM(c_ow.nam_permadr_city || ', ' || c_ow.txt_permadr_add1) collateral_owner_address,
                   TRIM(cld_ow.field_value) collateral_owner_ic,
                   c_bw.cod_cust_id borrower_customer_id,
                   TRIM(c_bw.nam_cust_full) borrower_customer_name,
                   TRIM(c_bw.nam_permadr_city || ', ' || c_bw.txt_permadr_add1) borrower_customer_address,
                   TRIM(cld_bw.field_value) borrower_customer_ic,
                   a.amt_face_value loan_amount,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                             ELSE 0
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type,
                   cp.amt_cost_price pr_cost_price_value,
                   cp.amt_forced_sale pr_forced_sale_value,
                   cp.amt_quit_rent pr_quit_rent_value,
                   cp.cod_area_unit pr_area_unit,
                   cp.flt_area pr_area,
                   TRUNC(cp.dat_lease_expiry) pr_lease_expiry_date,
                   cp.txt_collat_id pr_reg_no,
                   cp.txt_desc1 pr_reg_record_no,
                   TRIM(cp.txt_desc2) pr_reg_date,
                   w1.pr_reg_book_no,
                   w1.pr_reg_page_no,
                   w1.pr_collateral_address,
                   NVL(w1.pr_number_of_floors, 0) pr_number_of_floors,
                   NVL(w1.pr_number_of_rooms, 0) pr_number_of_rooms,
                   w2.line_last_date,
                   w2.line_total_amount,
                   w2.duration_in_month
            FROM ba_lo_coll_acct_xref@host_link ca,
                 ba_coll_hdr@host_link ch,
                 ba_coll_codes@host_link cc,
                 ln_acct_dtls@host_link a,
                 rpt_ba_cc_brn_mast@host_link bm,
                 ba_coll_prop@host_link cp,
                 ci_custmast@host_link c_ow, -- owner
                 ci_custmast@host_link c_bw, -- borrower
                 udf_cust_log_details@host_link cld_ow,
                 udf_cust_log_details@host_link cld_bw,
                 ln_prod_mast@host_link pm,
                 ln_acct_schedule_detls@host_link sd,
                 sm_user_profile@host_link up,
                 w_upr w1,
                 w_udf w2
            WHERE ca.cod_collat_id = ch.cod_collat_id
                  AND ch.cod_coll = cc.cod_coll
                  AND ca.cod_coll = cc.cod_coll
                  AND a.cod_acct_no = ca.cod_acct_no
                  AND bm.cod_cc_brn = ca.cod_cc_brn
                  AND ca.cod_collat_id = cp.cod_collat_id
                  AND ca.cod_coll = cp.cod_coll
                  AND c_ow.cod_cust_id(+) = TO_NUMBER(ch.nam_lender)
                  AND a.cod_cust_id = c_bw.cod_cust_id
                  AND pm.cod_prod = ca.cod_prod
                  AND sd.cod_acct_no(+) = ca.cod_acct_no
                  AND up.cod_branch_templ = 55
                  AND up.cod_cc_brn = ca.cod_cc_brn
                  AND cld_ow.cod_cust_id(+) = ch.nam_lender --edited by Yalchin on 17 Aprel 2012
                  AND TO_CHAR(a.cod_cust_id) = cld_bw.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND ca.cod_collat_id = w1.cod_collat_id(+)
                  AND ca.cod_collat_id = w2.cod_collat_id(+)
                  --
                  AND cld_ow.cod_field_tag(+) = 'TXT_5'
                  AND cld_bw.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld_ow.flg_mnt_status(+) = 'A'
                  AND cld_bw.flg_mnt_status(+) = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND ca.cod_collat_id = TRIM(p_collateral_id)
            ORDER BY 4, 3;

       RETURN c_result;
  END;
  -- LNA1070 - Mortgage collateral agreement
  -- LNA1072 - Mortgage collateral agreement (Credit Line)

  -- LNA1071 - Mortgage collateral agreement (Joint)
  FUNCTION lna1071(p_collateral_id VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       OPEN c_result FOR
            WITH w_col_prop1 AS (SELECT --um.ref_udf_no,
                                       cp.cod_collat_id collateral_id,
                                       --MAX(DECODE(uf.nam_field, 'CUST_ID', TRIM(um.field_value))) pr_customer_name,
                                       MAX(DECODE(uf.nam_field, 'ADDRESS', TRIM(um.field_value))) pr_collateral_address,
                                       MAX(DECODE(uf.nam_field, 'REGISTRATION_BOOK_NO', TRIM(um.field_value))) pr_reg_book_no,
                                       MAX(DECODE(uf.nam_field, 'REGISTRATION_PAGE_NO', TRIM(um.field_value))) pr_reg_page_no
                                FROM udf_field_mast@host_link uf,
                                     udf_mnt_log_details@host_link um,
                                     ba_coll_prop@host_link cp
                                WHERE uf.cod_field_tag = um.cod_field_tag
                                      AND um.ref_udf_no = cp.ref_udf_no
                                      AND cp.cod_collat_id = p_collateral_id
                                GROUP BY --um.ref_udf_no
                                         cp.cod_collat_id),
                 w_col_prop2 AS (SELECT aa.ase_identification_nbr collateral_id,
                                        --aa.ase_asb_type,
                                        MAX(DECODE(aaa.atr_aat_type, 'HOME_ATTR_1', TRIM(aaa.atr_value))) pr_number_of_floors,
                                        MAX(DECODE(aaa.atr_aat_type, 'HOME_ATTR_2', TRIM(aaa.atr_value))) pr_number_of_rooms,
                                        MAX(DECODE(aaa.atr_aat_type, 'HOME_ATTR_9', TRIM(aaa.atr_value))) pr_area/*,
                                        MAX(DECODE(aaa.atr_aat_type, 'VEHICLE_ATTR_1', TRIM(aaa.atr_value))) tr_auto_color,
                                        MAX(DECODE(aaa.atr_aat_type, 'VEHICLE_ATTR_2', TRIM(aaa.atr_value))) tr_insurance_company,
                                        MAX(DECODE(aaa.atr_aat_type, 'VEHICLE_ATTR_3', TRIM(aaa.atr_value))) tr_insurance_policy_number,
                                        MAX(DECODE(aaa.atr_aat_type, 'VEHICLE_ATTR_5', TRIM(aaa.atr_value))) tr_insurance_expire_date,
                                        MAX(DECODE(aaa.atr_aat_type, 'VEHICLE_ATTR_8', TRIM(aaa.atr_value))) tr_insurance_start_date*/
                                 FROM or_application_ast_attrs@host_link aaa,
                                      or_application_assets@host_link aa
                                 WHERE aaa.atr_aad_id = aa.ase_aad_id
                                       AND aaa.atr_ase_id = aa.ase_id
                                       --
                                       AND aaa.flg_mnt_status = 'A'
                                       AND aa.flg_mnt_status = 'A'
                                       --
                                       --AND INSTR('HOME_ATTR,VEHICLE_ATTR', SUBSTR(aaa.atr_aat_type, 1, INSTR(aaa.atr_aat_type, '_', 1, 2)-1), 1) > 0
                                       AND INSTR('HOME_ATTR', SUBSTR(aaa.atr_aat_type, 1, INSTR(aaa.atr_aat_type, '_', 1, 2)-1), 1) > 0
                                       --
                                       AND aa.ase_identification_nbr = p_collateral_id
                                 GROUP BY aa.ase_identification_nbr),
                 w_ps AS (SELECT p_collateral_id collateral_id,
                                 COUNT(DISTINCT sd.cod_acct_no) ps_exists
                          FROM ln_acct_schedule_detls@host_link sd,
                               ba_lo_coll_acct_xref@host_link ca
                          WHERE sd.cod_acct_no = ca.cod_acct_no
                                AND ca.cod_collat_id = TRIM(p_collateral_id)),
                 w_coll_info1 AS (SELECT DISTINCT (TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)) agreement_no,
                                         TRIM(ca.cod_acct_no) account_no,
                                         TRIM(ca.cod_collat_id) collateral_id,
                                         bm.cod_cc_brn branch_id,
                                         bm.nam_branch branch_name,
                                         bm.nam_cc_city branch_city,
                                         TRIM(up.nam_user) branch_manager,
                                         v_bank_address bank_address,
                                         a.dat_first_disb disb_date,
                                         --TRUNC(DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb)) report_date,
                                         TRIM(pm.nam_ccy_short) currency_code,
                                         c_bw.cod_cust_id borrower_customer_id,
                                         TRIM(c_bw.nam_cust_full) borrower_customer_name,
                                         TRIM(c_bw.nam_permadr_city || ', ' || c_bw.txt_permadr_add1) borrower_customer_address,
                                         TRIM(cld_bw.field_value) borrower_customer_ic,
                                         DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                                         a.rat_var_current loan_rate,
                                         TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                                                   WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                                                   WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                                                   ELSE 0
                                              END) loan_term_value,
                                         TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                                                   WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                                                   WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                                                   ELSE '0'
                                              END) loan_term_type
                                  FROM ba_lo_coll_acct_xref@host_link ca,
                                       ln_acct_dtls@host_link a,
                                       ln_acct_balances@host_link ab,
                                       rpt_ba_cc_brn_mast@host_link bm,
                                       ci_custmast@host_link c_bw, -- borrower
                                       udf_cust_log_details@host_link cld_bw,
                                       ln_prod_mast@host_link pm,
                                       sm_user_profile@host_link up
                                  WHERE a.cod_acct_no = ca.cod_acct_no
                                        AND ab.cod_acct_no = ca.cod_acct_no
                                        AND bm.cod_cc_brn = ca.cod_cc_brn
                                        AND a.cod_cust_id = c_bw.cod_cust_id
                                        AND pm.cod_prod = ca.cod_prod
                                        AND up.cod_branch_templ = 55
                                        AND up.cod_cc_brn = ca.cod_cc_brn
                                        AND to_char(a.cod_cust_id) = cld_bw.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                                        --
                                        AND cld_bw.cod_field_tag(+) = 'TXT_5'
                                        AND cld_bw.flg_mnt_status(+) = 'A'
                                        --
                                        AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                                        AND ca.cod_collat_id = TRIM(p_collateral_id)),
                 w_coll_info2 AS (SELECT ch.cod_collat_id collateral_id,
                                         MAX(cc.cod_coll) collateral_code,
                                         MAX(cc.cod_coll_type) collateral_type_id,
                                         MAX(TRIM(DECODE(cc.cod_coll_type, 1, 'Property', 2, 'Transport', 'Non-Standard'))) collateral_type_name,
                                         MAX(TRIM(cc.nam_coll)) collateral_type_desc,
                                         MAX(TRIM(ch.txt_deed_detl_1)) collateral_desc,
                                         MAX(ch.cod_ccy) currency_no,
                                         MAX(ch.amt_orig_value) original_value,
                                         MAX(ch.amt_last_val) collateral_value,
                                         MAX(ch.amt_market_val) market_value,
                                         MAX(cp.amt_cost_price) pr_cost_price_value,
                                         MAX(cp.amt_forced_sale) pr_forced_sale_value,
                                         MAX(cp.amt_quit_rent) pr_quit_rent_value,
                                         MAX(cp.cod_area_unit) pr_area_unit,
                                         MAX(cp.flt_area) pr_area,
                                         MAX(TRUNC(cp.dat_lease_expiry)) pr_lease_expiry_date,
                                         MAX(cp.txt_collat_id) pr_reg_no,
                                         MAX(cp.txt_desc1) pr_reg_record_no,
                                         MAX(TRIM(cp.txt_desc2)) pr_reg_date,
                                         MAX(DECODE(cc.cod_coll, 191, NULL, c_ow.cod_cust_id)) collateral_owner_id,
                                         MAX(DECODE(cc.cod_coll, 191, c_ow.cod_cust_id, NULL)) collateral_owner_id2,
                                         MAX(DECODE(cc.cod_coll, 191, NULL, TRIM(c_ow.nam_cust_full))) collateral_owner_name,
                                         MAX(DECODE(cc.cod_coll, 191, TRIM(c_ow.nam_cust_full), NULL)) collateral_owner_name2,
                                         MAX(DECODE(cc.cod_coll, 191, NULL, TRIM(c_ow.nam_permadr_city || ', ' || c_ow.txt_permadr_add1))) collateral_owner_address,
                                         MAX(DECODE(cc.cod_coll, 191, TRIM(c_ow.nam_permadr_city || ', ' || c_ow.txt_permadr_add1), NULL)) collateral_owner_address2,
                                         MAX(DECODE(cc.cod_coll, 191, NULL, TRIM(cld_ow.field_value))) collateral_owner_ic,
                                         MAX(DECODE(cc.cod_coll, 191, TRIM(cld_ow.field_value), NULL)) collateral_owner_ic2
                                  FROM ba_coll_hdr@host_link ch,
                                       ba_coll_codes@host_link cc,
                                       ba_coll_prop@host_link cp,
                                       ci_custmast@host_link c_ow, -- owner
                                       udf_cust_log_details@host_link cld_ow
                                  WHERE ch.cod_collat_id = cp.cod_collat_id
                                        AND ch.cod_coll = cc.cod_coll
                                        AND ch.cod_coll = cp.cod_coll
                                        AND c_ow.cod_cust_id(+) = TO_NUMBER(ch.nam_lender)
                                        AND cld_ow.cod_cust_id(+) = ch.nam_lender --edited by Yalchin on 17 Aprel 2012
                                        --
                                        AND cld_ow.cod_field_tag(+) = 'TXT_5'
                                        AND cld_ow.flg_mnt_status(+) = 'A'
                                        --
                                        AND ch.cod_collat_id = TRIM(p_collateral_id)
                                  GROUP BY ch.cod_collat_id)
            ----------------------------------------------
            SELECT ci1.agreement_no,
                   ci1.account_no,
                   ci1.collateral_id,
                   ci1.branch_id,
                   ci1.branch_name,
                   ci1.branch_city,
                   ci1.branch_manager,
                   ci1.bank_address,
                   ci2.collateral_code,
                   ci2.collateral_type_name,
                   ci2.collateral_type_desc,
                   ci2.collateral_desc,
                   DECODE(ps.ps_exists, 0, p_report_date, ci1.disb_date) report_date,
                   ci2.currency_no,
                   ci1.currency_code,
                   ci2.original_value,
                   ci2.collateral_value,
                   ci2.market_value,
                   ci2.collateral_owner_id,
                   ci2.collateral_owner_id2,
                   ci2.collateral_owner_name,
                   ci2.collateral_owner_name2,
                   ci2.collateral_owner_address,
                   ci2.collateral_owner_address2,
                   ci2.collateral_owner_ic,
                   ci2.collateral_owner_ic2,
                   ci1.borrower_customer_id,
                   ci1.borrower_customer_name,
                   ci1.borrower_customer_address,
                   ci1.borrower_customer_ic,
                   ci1.loan_amount,
                   ci1.loan_rate,
                   ci1.loan_term_value,
                   ci1.loan_term_type,
                   ci2.pr_cost_price_value,
                   ci2.pr_forced_sale_value,
                   ci2.pr_quit_rent_value,
                   ci2.pr_area_unit,
                   ci2.pr_area,
                   ci2.pr_lease_expiry_date,
                   ci2.pr_reg_no,
                   ci2.pr_reg_record_no,
                   ci2.pr_reg_date,
                   cp1.pr_reg_book_no,
                   cp1.pr_reg_page_no,
                   cp1.pr_collateral_address,
                   cp2.pr_number_of_floors,
                   cp2.pr_number_of_rooms
            FROM w_coll_info1 ci1,
                 w_coll_info2 ci2,
                 w_col_prop1 cp1,
                 w_col_prop2 cp2,
                 w_ps ps
            WHERE ci1.collateral_id = ci2.collateral_id
                  AND cp1.collateral_id(+) = ci1.collateral_id
                  AND cp2.collateral_id(+) = ci1.collateral_id
                  AND ps.collateral_id = ci1.collateral_id;

       RETURN c_result;
  END;
  -- LNA1071 - Mortgage collateral agreement (Joint)

  -- LNA1080 - General collateral agreement
  FUNCTION lna1080(p_collateral_id VARCHAR2,
                   p_collateral_type_id VARCHAR2,
                   p_account_no VARCHAR2,
                   p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT DISTINCT (TRIM(ca.cod_acct_no) || '-' || TRIM(ca.cod_collat_id)) agreement_no,
                   TRIM(ca.cod_acct_no) account_no,
                   TRIM(ca.cod_collat_id) collateral_id,
                   bm.cod_cc_brn branch_id,
                   TRIM(bm.nam_branch) branch_name,
                   TRIM(bm.nam_cc_city) branch_city,
                   up.cod_user_id branch_manager_id,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   ca.cod_coll collateral_code,
                   cc.cod_coll_type collateral_type_id,
                   TRIM(DECODE(cc.cod_coll_type, 1, 'Property', cc.cod_coll_type, 2, 'Transport', 'Non-Standard')) collateral_type_name,
                   TRIM(cc.nam_coll) collateral_type_desc,
                   TRIM(ch.txt_deed_detl_1) collateral_desc,
                   TRUNC(DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb)) report_date,
                   ch.cod_ccy currency_no,
                   TRIM(pm.nam_ccy_short) currency_code,
                   ch.amt_orig_value original_value,
                   ch.amt_last_val collateral_value,
                   ch.amt_market_val market_value,
                   c_ow.cod_cust_id collateral_owner_id,
                   TRIM(c_ow.nam_cust_full) collateral_owner_name,
                   --TRIM(c_ow.nam_permadr_city || ', ' || c_ow.txt_permadr_add1) collateral_owner_address,
                   TRIM(c_ow.nam_permadr_city || ', ' || TRIM(c_ow.txt_permadr_add1) ||
                   DECODE(TRIM(c_ow.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c_ow.txt_permadr_add2)) ||
                   DECODE(TRIM(c_ow.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c_ow.txt_permadr_add3))) collateral_owner_address,
                   --TRIM(c_ow.cod_cust_natl_id) collateral_owner_ic,
                   TRIM(cld_ow.field_value) collateral_owner_ic,
                   c_bw.cod_cust_id borrower_customer_id,
                   TRIM(c_bw.nam_cust_full) borrower_customer_name,
                   --TRIM(c_bw.nam_permadr_city || ', ' || c_bw.txt_permadr_add1) borrower_customer_address,
                   TRIM(c_bw.nam_permadr_city || ', ' || TRIM(c_bw.txt_permadr_add1) ||
                   DECODE(TRIM(c_bw.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c_bw.txt_permadr_add2)) ||
                   DECODE(TRIM(c_bw.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c_bw.txt_permadr_add3))) borrower_customer_address,
                   --TRIM(c_bw.cod_cust_natl_id) borrower_customer_ic,
                   TRIM(cld_bw.field_value) borrower_customer_ic,
                   a.amt_face_value loan_amount,
                   a.rat_var_current loan_rate,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                             ELSE 0
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                             ELSE '0'
                        END) loan_term_type,
                   (SELECT TRIM(um.field_value)
                    FROM udf_field_mast@host_link uf,
                         udf_mnt_log_details@host_link um
                    WHERE uf.cod_field_tag = um.cod_field_tag
                          AND um.ref_udf_no = ns.ref_udf_no
                          AND uf.nam_field = 'ADDRESS'
													AND um.flg_mnt_status = 'A') collateral_address
            FROM ba_lo_coll_acct_xref@host_link ca,
                 ba_coll_hdr@host_link ch,
                 ba_coll_codes@host_link cc,
                 ln_acct_dtls@host_link a,
                 rpt_ba_cc_brn_mast@host_link bm,
                 ci_custmast@host_link c_ow, -- owner
                 ci_custmast@host_link c_bw, -- borrower
                 udf_cust_log_details@host_link cld_ow,
                 udf_cust_log_details@host_link cld_bw,
                 ln_prod_mast@host_link pm,
                 ba_coll_ns@host_link ns,
                 ln_acct_schedule_detls@host_link sd,
                 sm_user_profile@host_link up
            WHERE ca.cod_collat_id = ch.cod_collat_id
                  AND cc.cod_coll = ch.cod_coll
                  AND ca.cod_coll = ch.cod_coll
                  AND a.cod_acct_no = ca.cod_acct_no
                  AND bm.cod_cc_brn = ch.cod_coll_homebrn
                  AND c_ow.cod_cust_id(+) = TO_NUMBER(ch.nam_lender)
                  AND a.cod_cust_id = c_bw.cod_cust_id
                  AND sd.cod_acct_no(+) = ca.cod_acct_no
                  AND pm.cod_prod = ca.cod_prod
                  AND ca.cod_collat_id = ns.cod_collat_id
                  AND ch.cod_coll = ns.cod_coll
                  AND up.cod_branch_templ = 55
                  AND up.cod_cc_brn = ca.cod_cc_brn
                  AND cld_ow.cod_cust_id(+) = ch.nam_lender --edited by Yalchin on 17 Aprel 2012
                  AND to_char(a.cod_cust_id) = cld_bw.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld_ow.cod_field_tag(+) = 'TXT_5'
                  AND cld_bw.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld_ow.flg_mnt_status(+) = 'A'
                  AND cld_bw.flg_mnt_status(+) = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND ca.cod_collat_id = TRIM(p_collateral_id)
                  AND ca.cod_acct_no = v_account_no
                  AND ch.cod_coll = p_collateral_type_id
            ORDER BY 4, 3;

       RETURN c_result;
  END;

  FUNCTION lna1080_collateral_attributes(p_collateral_id VARCHAR2,
                                         p_collateral_type_id VARCHAR2,
                                         p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT aaa.atr_aad_id application_id,
                   TRIM(ca.cod_acct_no) account_no,
                   TRIM(ca.cod_collat_id) collateral_id,
                   aaa.atr_aat_type attribute_type,
                   aaa.atr_att_desc attribute_desc,
                   aaa.atr_amt attribute_amount,
                   aaa.atr_value attribute_value,
                   c.nam_ccy_short ccy/*,
                   ca.**/
            FROM or_application_ast_attrs@host_link aaa,
                 or_application_assets@host_link aa,
                 ba_lo_coll_acct_xref@host_link ca,
                 ba_coll_codes@host_link cc,
                 ba_ccy_code@host_link c
            WHERE aaa.atr_aad_id = aa.ase_aad_id
                  AND aaa.atr_ase_id = aa.ase_id
                  AND aa.ase_identification_nbr = ca.cod_collat_id
                  AND ca.cod_coll = aa.ase_asb_type
                  AND cc.cod_coll = ca.cod_coll
                  AND c.cod_ccy = cc.cod_ccy
                  AND aa.ase_ast_type = 5 --
                  --
                  AND aaa.flg_mnt_status = 'A'
                  AND aa.flg_mnt_status = 'A'
                  AND cc.flg_mnt_status = 'A'
                  AND c.flg_mnt_status = 'A'
                  AND (aaa.atr_amt > 0 OR aaa.atr_value > '0')
                  --
                  AND ca.cod_collat_id = TRIM(p_collateral_id)
                  AND ca.cod_coll = p_collateral_type_id
                  AND ca.cod_acct_no = v_account_no;

       RETURN c_result;
  END;

  FUNCTION lna1080_collateral_accounts(p_collateral_id VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT DISTINCT TRIM(ca.cod_acct_no) account_no
            FROM ba_lo_coll_acct_xref@host_link ca
            WHERE ca.cod_collat_id = TRIM(p_collateral_id)
            ORDER BY 1;

       RETURN c_result;
  END;
  -- LNA1080 - General collateral agreement

  -- LNA1090 - Guarantee agreement
  -- LNA1091 - Automobil guarantee agreement
  -- LNA1151 - Guarantor salary receivership agreement
	-- LNA1161 - Guarantor salary deduction request form
  FUNCTION lna1090(p_account_no VARCHAR2,
	                 p_customer_id NUMBER,
									 p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT (TRIM(a.cod_acct_no) || '-' || TRIM(c_g.cod_cust_id)) agreement_no,
                   TRIM(a.cod_acct_no) account_no,
                   (TRIM(bm.cod_cc_brn) || ' ' || TRIM(bm.nam_branch)) branch_name,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   c_g.cod_cust_id guarantor_id,
                   TRIM(c_g.nam_cust_full) guarantor_name,
                   TRIM(cld_g.field_value) guarantor_ic,
                   TRIM(c_g.nam_custadr_city) guarantor_city,
                   TRIM(c_g.nam_permadr_city || ', ' || c_g.txt_permadr_add1 || ', ' || c_g.txt_permadr_add2) guarantor_address,
                   DECODE(c_g.txt_cust_sex, 'M', 'Male', 'F', 'Female') guarantor_gender,
                   c_g.cod_cust_id borrower_id,
                   TRIM(c_bw.nam_cust_full) borrower_name,
                   TRIM(cld_bw.field_value) borrower_ic,
                   TRIM(c_bw.nam_custadr_city) borrower_city,
                   TRIM(c_bw.nam_permadr_city || ', ' || c_bw.txt_permadr_add1 || ', ' || c_bw.txt_permadr_add2) borrower_address,
                   TRUNC(NVL(NVL(a.dat_first_disb, asd.dat_proposed_disb), p_report_date)) first_disb_date,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                             WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                        END) loan_term_value,
                   TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                             WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'--(CASE WHEN pm.cod_prod_term_typ = 2 THEN 'həftə' ELSE 'Biweekly' END)
                             WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                        END) loan_term_type,
                   --a.amt_face_value loan_amount,
                   DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                   TRIM(pm.nam_ccy_short) loan_currency,
                   a.rat_var_current loan_rate
            FROM ln_acct_dtls@host_link a,
						           ln_acct_balances@host_link ab,
                 ch_acct_cust_xref@host_link cx,
                 ci_custmast@host_link c_g,
                 ci_custmast@host_link c_bw,
                 ln_prod_mast@host_link pm,
                 rpt_ba_cc_brn_mast@host_link bm,
                 or_app_schedule_dtls@host_link asd,
                 sm_user_profile@host_link up,
                 udf_cust_log_details@host_link cld_g,
                 udf_cust_log_details@host_link cld_bw
            WHERE cx.cod_acct_no = a.cod_acct_no
						            AND ab.cod_acct_no = a.cod_acct_no
                  AND c_bw.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND c_g.cod_cust_id = cx.cod_cust
                  AND cx.cod_acct_cust_rel = 'GUA'
                  AND up.cod_cc_brn = a.cod_cc_brn
                  AND up.cod_branch_templ = 55
                  AND asd.cod_acct_no(+) = a.cod_acct_no
                  AND asd.ctr_stage_no(+) = 1
                  AND to_char(cx.cod_cust) = cld_g.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  AND to_char(a.cod_cust_id) = cld_bw.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld_g.cod_field_tag(+) = 'TXT_5'
                  AND cld_bw.cod_field_tag(+) = 'TXT_5'
                  --
                  AND cld_g.flg_mnt_status(+) = 'A'
                  AND cld_bw.flg_mnt_status(+) = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND a.cod_acct_no = v_account_no
                  AND c_g.cod_cust_id = p_customer_id;

       RETURN c_result;
  END;
  -- LNA1090 - Guarantee agreement
  -- LNA1091 - Automobil guarantee agreement

  -- LNA1100 - Client payment order
  FUNCTION lna1100(p_branch_id NUMBER,
                   p_ccy NUMBER,
                   p_prop_disb_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT DISTINCT ap.app_aad_id application_id,
                   c.cod_cust_id customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   TRIM(cld.field_value) customer_ic,
                   t.group_id,
                   t.group_cycle,
                   t.group_name,
                   t.account_no,
                   NVL(t.dat_proposed_disb, ap.app_dt) application_date,
                   ap.app_req_adv_amt requested_amount,
                   ap.app_prd_ccy ccy,
                   TRIM(cc.nam_ccy_short) ccy_short_name,
                   TRIM(cc.nam_currency) ccy_name--,
                   /*ap.app_req_rate requested_rate,
                   ap.app_req_term requested_term,
                   trim(ap.app_acc_nbr) account_no,
                   trunc(ad.adc_decision_dt) decision_date,
                   ad.adc_maturity_mrate approved_amount,
                   ad.adc_rate approved_rate,
                   ad.adc_term approved_term,
                   trunc(ap.dat_last_mnt) prop_disb_date,*/
            FROM or_applications@host_link ap,
                 or_applicants@host_link oa,
                 --or_application_decisions@host_link ad,
                 ci_custmast@host_link c,
                 ba_ccy_code@host_link cc,
                 udf_cust_log_details@host_link cld,
                 (SELECT DISTINCT asd.cod_application_id,
                         TRIM(asd.cod_group) group_id,
                         TRIM(asd.cod_group_cycle) group_cycle,
                         TRIM(asd.nam_group) group_name,
                         TRIM(asd.cod_acct_no) account_no,
                         asd.dat_proposed_disb
                  FROM or_app_schedule_dtls@host_link asd
                  WHERE asd.flg_mnt_status = 'A') t
            WHERE oa.apl_aad_id = ap.app_aad_id
                  AND oa.apl_relation_type_cd LIKE 'PRIM%'
                  --AND ad.adc_aad_id(+) = ap.app_aad_id
                  AND c.cod_cust_id = oa.apl_cus_id
                  AND t.cod_application_id(+) = ap.app_aad_id
                  AND cc.cod_ccy = ap.app_prd_ccy
                  --
                  AND DECODE(ap.app_status_cd,
                             'APPROVED', (CASE WHEN ap.app_sub_status_cd IN ('PRE_APPROVED', 'UNDEFINED', 'BLANK')
                                               THEN 1
                                               ELSE 0
                                          END),
                             'NEW', (CASE WHEN ap.app_sub_status_cd LIKE 'TO_CCL%'
                                          THEN 1
                                          ELSE 0
                                     END),
                             0) > 0
                  AND to_char(c.cod_cust_id) = cld.cod_cust_id(+)  --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND cld.cod_field_tag(+) = 'TXT_5'
                  AND cld.flg_mnt_status(+) = 'A'
                  AND ap.flg_mnt_status = 'A'
                  AND oa.flg_mnt_status = 'A'
                  AND c.flg_mnt_status = 'A'
                  AND ap.flg_repayment_mode = 3 -- 0 - Cash; 1 - Cheque; 2 - Account Transfer; 3 - GL
                  --
                  AND TRUNC(NVL(t.dat_proposed_disb, ap.app_dt)) = p_prop_disb_date
                  AND ap.app_pcb_branch = p_branch_id
                  AND ap.app_prd_ccy = p_ccy
            ORDER BY t.group_name,
                     TRIM(c.nam_cust_full),
                     t.account_no;

       RETURN c_result;
  END;
  -- LNA1100 - Client payment order

  -- LNA1101 - Bank payment order
  FUNCTION lna1101(p_ccy NUMBER,
                   p_em_bank_code VARCHAR2,
                   p_em_bank_account VARCHAR2,
                   p_bn_bank_code VARCHAR2,
                   p_bn_bank_account VARCHAR2,
                   p_amount NUMBER,
                   p_additional_info NUMBER :=0)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_ccy VARCHAR2(5) := 'USD';
  BEGIN
       SELECT cc.nam_ccy_short INTO v_ccy
       FROM ba_ccy_code cc
       WHERE cc.cod_ccy = p_ccy;
       
       OPEN c_result FOR
            SELECT e.bank_id em_bank_id,
                   (e.bank_name || ' ' || e.bank_branch_name) em_bank_name,
                   e.bank_branch_name em_bank_branch,
                   e.bank_voen em_bank_voen,
                   e.bank_code em_bank_code,
                   e.bank_corresp_acc em_bank_corresp_acc,
                   e.bank_swift em_bank_swift,
                   e.bank_acy em_bank_acy,
                   e.bank_account em_bank_account,
                   b.bank_id bn_bank_id,
                   (b.bank_name || ' ' || b.bank_branch_name) bn_bank_name,
                   b.bank_branch_name bn_bank_branch,
                   b.bank_voen bn_bank_voen,
                   b.bank_code bn_bank_code,
                   b.bank_corresp_acc bn_bank_corresp_acc,
                   b.bank_swift bn_bank_swift,
                   b.bank_acy bn_bank_acy,
                   b.bank_account bn_bank_account,
                   p_amount amount,
                   report_methods.get_words_from_amount(cc.cod_ccy, cc.nam_currency, p_amount) amount_in_words,
                   (CASE WHEN e.commission_amount > 0
                         THEN (' ( ' || TRIM(to_char(e.commission_amount, '99999999999990.99')) || ' ' || v_ccy || ' )')
                         ELSE ''
                    END) commission_amount,
                   e.emittent_client,
                   e.emittent_voen,
                   DECODE(b.voin_pointer, 0, e.emittent_client, (b.bank_name || ' ' || b.bank_branch_name)) beneficiar_client,
                   DECODE(b.voin_pointer, 0, e.emittent_voen, b.bank_voen) beneficiar_voen
            FROM ba_ccy_code cc,
                 (SELECT ROWNUM rnum,
                         be.bank_id,
                         be.bank_name,
                         be.bank_branch_name,
                         be.bank_voen,
                         be.bank_code,
                         be.bank_corresp_acc,
                         be.bank_swift,
                         be.bank_acy,
                         be.bank_account,
                         --(p_amount - (p_amount / DECODE(p_additional_info, 0, 1, 1, 1.0035, 2, 1.0042, 3, 1.005, 4, 1.006))) commission_amount,
                         (p_amount - (p_amount / (1 + (p_additional_info/10000)))) commission_amount,
                         '"FINCA Azerbaijan" QBKT' emittent_client,
                         '1400494391' emittent_voen
                  FROM rpt_other_banks be
                  WHERE be.emittent = 1
                        AND be.bank_acy = v_ccy
                        AND be.bank_code = p_em_bank_code
                        AND be.bank_account = p_em_bank_account
                        AND ROWNUM = 1) e,
                 (SELECT ROWNUM rnum,
                         bb.bank_id,
                         bb.bank_name,
                         bb.bank_branch_name,
                         bb.bank_voen,
                         bb.bank_code,
                         bb.bank_corresp_acc,
                         bb.bank_swift,
                         bb.bank_acy,
                         bb.bank_account,
                         bb.voin_pointer
                  FROM rpt_other_banks bb
                  WHERE bb.bank_acy = v_ccy
                        AND bb.bank_code = p_bn_bank_code
                        AND bb.bank_account = p_bn_bank_account
                        AND ROWNUM = 1) b
            WHERE e.rnum = b.rnum
                  AND cc.nam_ccy_short = v_ccy;

       RETURN c_result;
  END;
  -- LNA1101 - Bank payment order
  
  -- LNA1120 - Commission fee receipt - 'CM'
  -- LNA1121 - Repayment receipt (KB) - 'CR'
  -- LNA1122 - Repayment receipt (IB) - 'CR'
  -- LNA1126 - Disbursement receipt - 'LA'
  FUNCTION lna1120(p_account_no VARCHAR2,
                   p_report_date DATE := trunc(SYSDATE),
                   p_report_type VARCHAR2 := 'CM')
  RETURN result_cursor
  IS -- CM (CR) - Commission (Repayment) receipt; DS - Disbursement receipt
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_account_list VARCHAR2(32000) := '';
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       SELECT (WM_CONCAT(gr.account_no) || ',' || v_account_no) INTO v_account_list
       FROM (SELECT DISTINCT gd2.cod_acct_no account_no
             FROM ba_group_details@host_link gd1,
                  ba_group_details@host_link gd2
             WHERE gd1.cod_group = gd2.cod_group(+)
                   AND gd1.cod_group_cycle = gd2.cod_group_cycle(+)
                   AND gd1.flg_group_acc_attr(+) = 'A'
                   AND gd1.flg_mnt_status(+) = 'A'
                   AND gd2.flg_group_acc_attr(+) = 'A'
                   AND gd2.flg_mnt_status(+) = 'A'
                   AND gd1.cod_acct_no = v_account_no) gr;

       OPEN c_result FOR
            WITH w_res AS (SELECT DISTINCT TRIM(a.cod_acct_no) cod_acct_no,
                                  TRIM(g.cod_group) cod_group,
                                  TRIM(c.nam_cust_full) nam_cust_full,
                                  TRIM(g.nam_group) nam_group,
                                  TRIM(cld.field_value) field_value,
                                  a.cod_cc_brn,
                                  aa.cod_acct_off,
                                  a.dat_first_disb,
                                  cc.cod_ccy,
                                  TRIM(cc.nam_ccy_short) nam_ccy_short,
                                  TRIM(cc.nam_currency) nam_currency,
                                  (NVL((CASE WHEN dp.nam_plan LIKE '%IAL%' OR dp.nam_plan LIKE '%CAL%'
                                             THEN 2.5
                                             WHEN dp.nam_plan LIKE '%FEX%'
                                             THEN (CASE WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) < DECODE(a.cod_ccy, 840, 5000, 4000) THEN 3
                                                        WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) BETWEEN DECODE(a.cod_ccy, 840, 5000, 4000) AND DECODE(a.cod_ccy, 840, 9999, 7999) THEN 2
                                                   END)
                                             WHEN dp.nam_plan LIKE '%SME%'
                                             THEN 2/*(CASE WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) BETWEEN 5000 AND 9999 THEN 3
                                                        WHEN DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) >= 10000 THEN 2
                                                   END)*/
                                             WHEN dp.nam_plan LIKE '%SCG%' THEN 4
                                             WHEN dp.nam_plan LIKE '%RLP%'
                                             THEN (CASE WHEN pm.cod_prod_term_typ = 3 AND (a.ctr_term_months/65536) > 12
                                                        THEN 3
                                                        ELSE 2
                                                   END)
                                        END), 0) * DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) / 100) commission_amount,
                                  DECODE(sd.date_instal, p_report_date, (sd.amt_principal + sd.amt_interest), 0) credit_amount,
                                  DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount
                           FROM ci_custmast@host_link c,
                                ln_acct_dtls@host_link a,
                                ln_acct_attributes@host_link aa,
                                ln_acct_balances@host_link ab,
                                ln_prod_mast@host_link pm,
                                ln_dedn_plan@host_link dp,
                                ln_acct_schedule_detls@host_link sd,
                                ba_ccy_code@host_link cc,
                                udf_cust_log_details@host_link cld,
                                (SELECT DISTINCT gd.cod_group,
                                        gd.nam_group,
                                        gd.cod_group_cycle,
                                        gd.cod_acct_no
                                 FROM ba_group_details@host_link gd
                                 WHERE gd.flg_group_acc_attr = 'A'
                                       AND gd.flg_mnt_status = 'A') g,
                                (SELECT REGEXP_REPLACE(REGEXP_SUBSTR(t.account_list || ',',
                                                                     '(.*?),',
                                                                     1,
                                                                     LEVEL),
                                                       ',$',
                                                       '') account_no
                                 FROM (SELECT v_account_list account_list FROM dual@host_link) t
                                 CONNECT BY REGEXP_INSTR(t.account_list || ',',
                                                         '(.*?),',
                                                         1,
                                                         LEVEL) > 0) l
                           WHERE c.cod_cust_id = a.cod_cust_id
                                 AND pm.cod_prod = a.cod_prod
                                 --AND bm.cod_cc_brn = a.cod_cc_brn
                                 AND aa.cod_acct_no = a.cod_acct_no
                                 AND ab.cod_acct_no = a.cod_acct_no
                                 AND dp.cod_plan = pm.cod_dedn_plan
                                 AND cc.cod_ccy = a.cod_ccy
                                 AND a.cod_acct_no = sd.cod_acct_no(+)
                                 AND a.cod_acct_no = g.cod_acct_no(+)
                                 AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                                 AND a.cod_acct_no = l.account_no
                                 --
                                 AND cld.cod_field_tag(+) = 'TXT_5'
                                 --
                                 AND c.flg_mnt_status = 'A'
                                 AND a.flg_mnt_status = 'A'
                                 AND aa.flg_mnt_status = 'A'
                                 AND pm.flg_mnt_status = 'A'
                                 AND dp.flg_mnt_status = 'A'
                                 AND cld.flg_mnt_status(+) = 'A'
                                 --
                                 AND sd.date_instal(+) = p_report_date
                                 --AND INSTR(v_account_list, a.cod_acct_no, 1) > 0
                                 )
            SELECT t.*,
                   p_report_date report_date,
                   ('KOD ' || ob.bank_code) bank_code,
                   ob.bank_voen,
                   ob.bank_account,
                   (ob.bank_name || ' ' || ob.bank_branch_name) bank_branch_name,
                   DECODE(ob.bank_id, 2, DECODE(t.ccy_no, 944, '10010', '10110')) cash_account,
                   report_methods.get_words_from_amount(t.ccy_no, t.currency_name, t.commission_amount) amount_in_words_cm,
                   report_methods.get_words_from_amount(t.ccy_no, t.currency_name, t.credit_amount) amount_in_words_cr,
                   report_methods.get_words_from_amount(t.ccy_no, t.currency_name, t.loan_amount) amount_in_words_la
            FROM (SELECT DISTINCT --MAX(DECODE(g.cod_group, NULL, TRIM(c.nam_cust_full), (TRIM(g.cod_group) || ' - ' || TRIM(g.nam_group)))) client_name,
                         DECODE(DECODE(p_report_type, 'DS', NULL, r.cod_group), NULL, r.cod_acct_no, r.cod_group) gr_acc_no,
                         DECODE(DECODE(p_report_type, 'DS', NULL, r.cod_group), NULL, r.nam_cust_full, r.nam_group) client_name,
                         MAX(r.field_value) customer_ic,
                         AVG(r.cod_cc_brn) branch_id,
                         MAX(r.cod_acct_off) co_user_no,
                         NVL(MIN(r.dat_first_disb), p_report_date) disb_date,
                         AVG(r.cod_ccy) ccy_no,
                         MAX(r.nam_ccy_short) ccy_code,
                         MAX(r.nam_currency) currency_name,
                         SUM(r.commission_amount) commission_amount,
                         SUM(r.credit_amount) credit_amount,
                         SUM(r.loan_amount) loan_amount
                  FROM w_res r
                  GROUP BY DECODE(DECODE(p_report_type, 'DS', NULL, r.cod_group), NULL, r.cod_acct_no, r.cod_group),
                           DECODE(DECODE(p_report_type, 'DS', NULL, r.cod_group), NULL, r.nam_cust_full, r.nam_group)) t,
                 (SELECT DISTINCT cb.branch_id,
                         cb.user_no,
                         cb.bank_id,
                         cb.bank_name,
                         cb.bank_branch_name,
                         cb.bank_code,
                         cb.bank_voen,
                         cb.bank_acy,
                         cb.bank_account
                  FROM v_co_banks cb) ob
            WHERE ob.branch_id = t.branch_id
                  AND ob.bank_acy = t.ccy_code
                  AND ob.user_no(+) = t.co_user_no;

       RETURN c_result;
  END;
  -- LNA1120 - Commission fee receipt - 'CM'
  -- LNA1121 - Repayment receipt (KB) - 'CR'
  -- LNA1122 - Repayment receipt (IB) - 'CR'
  -- LNA1126 - Disbursement receipt - 'LA'
  
  -- LNA1125 - Repayment receipts for subscription
  FUNCTION lna1125(p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT s.page_number,
                   s.co_user_no1,
                   s.co_user_name1,
                   s.gr_acc_no1,
                   s.client_name1,
                   s.bank_code1,
                   s.bank_account1,
                   s.bank_branch_name1,
                   s.repayment_date1,
                   s.disb_date1,
                   s.currency_no1,
                   s.currency_code1,
                   s.currency_name1,
                   s.credit_amount1,
                   report_methods.get_words_from_amount(s.currency_no1, s.currency_name1, s.credit_amount1) amount_in_words_cr1,
                   s.co_user_no2,
                   s.co_user_name2,
                   s.gr_acc_no2,
                   s.client_name2,
                   s.bank_code2,
                   s.bank_account2,
                   s.bank_branch_name2,
                   s.repayment_date2,
                   s.disb_date2,
                   s.currency_no2,
                   s.currency_code2,
                   s.currency_name2,
                   s.credit_amount2,
                   report_methods.get_words_from_amount(s.currency_no2, s.currency_name2, s.credit_amount2) amount_in_words_cr2
            FROM (SELECT ROUND(ROWNUM/2) page_number,
                         --
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.co_user_no)) co_user_no1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.co_user_name)) co_user_name1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.gr_acc_no)) gr_acc_no1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.client_name)) client_name1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.bank_code)) bank_code1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.bank_account)) bank_account1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.bank_branch_name)) bank_branch_name1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.repayment_date)) repayment_date1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.disb_date)) disb_date1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.currency_no)) currency_no1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.currency_code)) currency_code1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.currency_name)) currency_name1,
                         MAX(DECODE(MOD(ROWNUM, 2), 1, t.credit_amount)) credit_amount1,
                         --
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.co_user_no)) co_user_no2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.co_user_name)) co_user_name2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.gr_acc_no)) gr_acc_no2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.client_name)) client_name2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.bank_code)) bank_code2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.bank_account)) bank_account2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.bank_branch_name)) bank_branch_name2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.repayment_date)) repayment_date2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.disb_date)) disb_date2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.currency_no)) currency_no2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.currency_code)) currency_code2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.currency_name)) currency_name2,
                         MAX(DECODE(MOD(ROWNUM, 2), 0, t.credit_amount)) credit_amount2
                  FROM (SELECT DISTINCT rr.co_user_no,
                               rr.co_user_name,
                               --DECODE(cbr.bank_id, 1, 'IB', 2, 'KB', 3, 'DB', 4, 'MB', 5, 'SB', 6, 'UB') co_bank_code,
                               rr.gr_acc_no,
                               rr.client_name,
                               rr.repayment_date,
                               rr.disb_date,
                               rr.currency_no,
                               rr.currency_code,
                               rr.currency_name,
                               rr.credit_amount,
                               (bb.bank_name || ' ' || bb.bank_branch_name) bank_branch_name,
                               bb.bank_code,
                               bb.bank_account
                        FROM rpt_co_bank_relations cbr,
                             rpt_other_banks bb,
                             v_subs_repayment_receipts rr
                        WHERE cbr.user_no = rr.co_user_no
                              AND cbr.bank_id = bb.bank_id
                              AND cbr.bank_branch_id = bb.bank_branch_id
                              AND bb.bank_acy = rr.currency_code
                              --
                              AND rr.co_user_no = p_user_no
                        ORDER BY rr.repayment_date,
                                 rr.co_user_no,
                                 10) t
                  GROUP BY ROUND(ROWNUM/2)) s
            ORDER BY 1;

       RETURN c_result;
  END;
  -- LNA1125 - Repayment receipts for subscription
  
  -- LNA1130 - Buy and Sell cash (Bank)
  -- LNA1131 - Buy and Sell cash (Vault)
  FUNCTION lna1130(p_branch_id NUMBER,
                   p_date DATE,
                   p_buy_sell VARCHAR2,
                   p_amount_and_tcy VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT DISTINCT DECODE(rt.cod_tran, '9007', 'BCV', '9008', 'SCV', '9009', 'BCB', '9010', 'SCB') txn_type,
                   -- BCV - buy cash from vault
                   -- SCV - sell cash to vault
                   -- BCB - buy cash from bank
                   -- SCB - sell cash to bank
                   rt.dat_sys txn_date,
                   rt.amt_tran_tcye txn_amount,
                   DECODE(rt.cod_tran_cur, 840, 'USD', 944, 'AZN', 978, 'EUR') tcy,
                   rt.user_id,
                   rt.id_auth auth_id,
                   rt.id_tran_seq txn_no,
                   report_methods.get_words_from_amount(rt.cod_tran_cur, DECODE(rt.cod_tran_cur, 840, 'USD', 944, 'AZN', 978, 'EUR'), rt.amt_tran_tcye) amount_in_words/*,
                   rt.**/
            FROM v_rec_txnlog rt
            WHERE rt.cod_tran IN ('9007','9008','9009','9010')
                  AND rt.cod_tran_stat IN (128, 4224, 768)
                  --
                  AND rt.dat_sys >= p_date
                  AND rt.cod_brn = p_branch_id
                  AND DECODE(rt.cod_tran, '9007', 'BCV', '9008', 'SCV', '9009', 'BCB', '9010', 'SCB') = p_buy_sell
                  AND (TRIM(rt.amt_tran_tcye) || '_' || TRIM(rt.cod_tran_cur) || '_' || rt.user_id|| '_' || TRIM(rt.id_tran_seq)) = p_amount_and_tcy;

       RETURN c_result;
  END;
  -- CI1030 - Inactive customers

  -- LNA1140 - Individual disbursement authorization letter
  FUNCTION lna1140(p_date DATE,
                   p_branch_id NUMBER,
                   p_account_no VARCHAR2,
                   p_bank_code VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_agreement_no VARCHAR2(20) := '';
    v_customer_name VARCHAR2(50) := '';
    v_customer_ic VARCHAR2(50) := '';
    v_ccy VARCHAR2(15) := 0;
    v_disb_date DATE := NULL;
    v_loan_amount NUMBER := 0;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       SELECT TRIM(a.cod_acct_no),
              TRIM(c.nam_cust_full),
              TRIM(cld.field_value),
              TRIM(cc.nam_currency),
              NVL(MAX(a.dat_first_disb), p_date),
              NVL(SUM(asd.amt_principal), MAX(ps.principal_amount))
              INTO v_agreement_no,
                   v_customer_name,
                   v_customer_ic,
                   v_ccy,
                   v_disb_date,
                   v_loan_amount
       FROM ln_acct_dtls@host_link a,
            ci_custmast@host_link c,
            ba_ccy_code@host_link cc,
            ln_acct_schedule_detls@host_link asd,
            udf_cust_log_details@host_link cld,
            (SELECT RPAD(t.account_no, 16, ' ') account_no,
                    SUM(r.amt_principal) principal_amount
             FROM rpt_ln_r501@host_link r,
                  (SELECT ap.app_acc_nbr account_no,
                          TRIM(MAX(TO_NUMBER(SUBSTR(rl.session_id, 5)))) max_id
                   FROM rpt_ln_r501@host_link rl,
                        or_applications@host_link ap
                   WHERE rl.cod_app_id = ap.app_aad_id
                         --
                         AND TRIM(ap.app_acc_nbr) = TRIM(v_account_no)
                   GROUP BY ap.app_acc_nbr) t
             WHERE SUBSTR(r.session_id, 5) = t.max_id
             GROUP BY RPAD(t.account_no, 16, ' ')) ps
       WHERE c.cod_cust_id = a.cod_cust_id
             AND a.cod_acct_no = asd.cod_acct_no(+)
             AND a.cod_acct_no = ps.account_no(+)
             AND cc.cod_ccy = a.cod_ccy
             AND to_char(a.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
             --
             AND a.flg_mnt_status = 'A'
             AND cc.flg_mnt_status = 'A'
             AND cld.cod_field_tag(+) = 'TXT_5'
             AND cld.flg_mnt_status(+) = 'A'
             --
             AND a.cod_acct_no = v_account_no
       GROUP BY TRIM(a.cod_acct_no),
                TRIM(c.nam_cust_full),
                TRIM(cld.field_value),
                TRIM(cc.nam_currency);

       OPEN c_result FOR
            SELECT --up.cod_cc_brn,
                   --up.cod_user_id bm_user_id,
                   up.nam_user bm_user_name,
                   obm.other_bm_name,
                   obm.other_bm_gender,
                   v_agreement_no agreement_no,
                   v_customer_name customer_name,
                   v_customer_ic customer_ic,
                   v_disb_date report_date,
                   v_loan_amount loan_amount,
                   v_ccy ccy,
                   report_methods.get_words_from_number(v_loan_amount) amount_in_words
            FROM sm_user_profile up,
                 (SELECT DISTINCT bb.bm_name other_bm_name,
                         bb.bm_gender other_bm_gender
                  FROM rpt_other_banks bb
                  WHERE bb.bm_name IS NOT NULL
                        AND bb.bank_code = p_bank_code) obm
            WHERE INSTR(v_branch_manager_list, up.cod_user_id, 1) > 0
                  AND up.cod_cc_brn = p_branch_id;

       RETURN c_result;
  END;
  -- LNA1140 - Individual disbursement authorization letter

  -- LNA1141 - Group disbursement authorization letter
  FUNCTION lna1141(p_date DATE,
                   p_branch_id NUMBER,
                   p_group_id VARCHAR2,
                   p_group_cycle NUMBER,
                   p_customers VARCHAR2,
                   p_bank_code VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_agreement_no VARCHAR2(20) := '';
    v_ccy VARCHAR2(15) := 0;
    v_disb_date DATE := NULL;
    v_loan_amount NUMBER := 0;
    v_group_type VARCHAR2(15) := '';
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       SELECT (g.cod_group || '-' || TRIM(g.cod_group_cycle)),
              TRIM(cc.nam_currency),
              NVL(MAX(a.dat_first_disb), p_date),
              NVL(SUM(asd.amt_principal), TRUNC(SUM(DISTINCT g.principal_amount))),
              TRIM(CASE WHEN MAX(g.cod_prod) BETWEEN 100 AND 105 THEN 'S' ELSE 'R' END) -- S - Solidarity (Hemrey), R - Rural (Kend)
              INTO v_agreement_no,
                   v_ccy,
                   v_disb_date,
                   v_loan_amount,
                   v_group_type
       FROM ln_acct_dtls@host_link a,
            ln_acct_schedule_detls@host_link asd,
            ba_ccy_code@host_link cc,
            (SELECT DISTINCT gd.cod_acct_no,
                    gd.cod_group,
                    gd.cod_group_cycle,
                    gd.nam_group,
                    gd.cod_prod,
                    (ps.principal_amount + (gd.cod_application_id/100000000)) principal_amount
             FROM ba_group_details@host_link gd,
                  (SELECT r.cod_app_id,
                          SUM(r.amt_principal) principal_amount
                   FROM rpt_ln_r501@host_link r,
                        (SELECT rl.cod_app_id,
                                TRIM(MAX(TO_NUMBER(SUBSTR(rl.session_id, 5)))) max_id
                         FROM rpt_ln_r501@host_link rl
                         GROUP BY rl.cod_app_id) t
                   WHERE r.cod_app_id = t.cod_app_id
                         AND SUBSTR(r.session_id, 5) = t.max_id
                   GROUP BY r.cod_app_id) ps
             WHERE gd.flg_group_acc_attr = 'A'
                   AND gd.cod_application_id = ps.cod_app_id(+)
                   --
                   AND gd.cod_group = TRIM(p_group_id)
                   AND gd.cod_group_cycle = p_group_cycle) g
       WHERE a.cod_acct_no = asd.cod_acct_no(+)
             AND a.cod_acct_no = g.cod_acct_no
             AND cc.cod_ccy = a.cod_ccy
             --
             AND a.flg_mnt_status = 'A'
             AND cc.flg_mnt_status = 'A'
             --
             AND INSTR(p_customers, TRIM(a.cod_cust_id)) > 0
       GROUP BY (g.cod_group || '-' || TRIM(g.cod_group_cycle)),
                cc.nam_currency;

       OPEN c_result FOR
            SELECT --up.cod_cc_brn,
                   --up.cod_user_id bm_user_id,
                   up.nam_user bm_user_name,
                   obm.other_bm_name,
                   obm.other_bm_gender,
                   v_agreement_no agreement_no,
                   v_disb_date report_date,
                   v_loan_amount loan_amount,
                   v_ccy ccy,
                   v_group_type group_type,
                   report_methods.get_words_from_number(v_loan_amount) amount_in_words
            FROM sm_user_profile up,
                 (SELECT DISTINCT bb.bm_name other_bm_name,
                         bb.bm_gender other_bm_gender
                  FROM rpt_other_banks bb
                  WHERE bb.bm_name IS NOT NULL
                        AND bb.bank_code = p_bank_code) obm
            WHERE INSTR(v_branch_manager_list, up.cod_user_id, 1) > 0
                  AND up.cod_cc_brn = p_branch_id;

       RETURN c_result;
  END;
  -- LNA1141 - Group disbursement authorization letter

  -- LNA1170 - Client registration form
  FUNCTION lna1170(p_customer_id NUMBER)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT c.cod_cust_id customer_id,
                   MAX(c.nam_cust_full) customer_name,
                   MAX(TRUNC(c.dat_birth_cust)) customer_dob,
                   MAX(TRIM(c.nam_custadr_city || ', ' || c.txt_custadr_add1 || DECODE(c.txt_custadr_add2, NULL, NULL, ', ' || c.txt_custadr_add2) || DECODE(c.txt_custadr_add3, NULL, NULL, ', ' || c.txt_custadr_add3))) mailing_address,
                   MAX(TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 || DECODE(c.txt_permadr_add2, NULL, NULL, ', ' || c.txt_permadr_add2) || DECODE(c.txt_permadr_add3, NULL, NULL, ', ' || c.txt_permadr_add3))) registration_address,
                   MAX(TRIM(cd.nam_empadr_city || ', ' || cd.txt_empadr_add1)) business_address,
                   MAX(TRIM(c.ref_res_phone_area || ' ' || c.ref_cust_phone)) home_phone,
                   MAX(TRIM(c.ref_phone_mobile)) mobile_phone,
                   MAX(TRIM(c.txt_custadr_zip)) zip_code,
                   MAX(cd.ctr_yrs_job) business_experience,
                   MAX(TRIM(cd.nam_cust_emp)) activity_type,
                   MAX(TRIM(cd.nam_spouse_cust)) customer_spouse_name,
                   MAX(c.ctr_cust_spouses) customer_spouses,
                   MAX(oa.apl_no_of_dependents) number_of_dependents,
                   --oaa.apa_ownership_cd,
                   MAX(oaa.apa_stated_years) stated_years,
                   MAX(DECODE(mx.cod_mis_cust_2,
                              1, 2,
                              2, 5,
                              3, 7,
                              4, 3,
                              5, 6,
                              7, 4,
                              10, 1,
                              12, 8)) information_source_id,
                   MAX(DECODE(c.txt_cust_educn,
                          '1', 'Orta',
                          '2', 'Ali',
                          '3', 'Ibtidai',
                          '4', 'Texniki',
                          '5', 'Hərbi universitet',
                          '6', 'Hərbi məktəb')) education,
                   MAX(cp.txt_desc1) pr_reg_record_no,
                   MAX(cp.txt_desc2) pr_reg_date,
                   MAX(DECODE(cld.cod_field_tag, 'TXT_5', cld.field_value))customer_ic,
                   MAX(DECODE(cld.cod_field_tag, 'TXT_6', cld.field_value)) customer_birth_place
                   --,oa.*
            FROM ci_custmast@host_link c,
                 ci_custdetl@host_link cd,
                 ba_coll_hdr@host_link ch,
                 ba_coll_prop@host_link cp,
                 or_applicants@host_link oa,
                 or_applicant_address@host_link oaa,
                 ba_cust_prod_txn_mis_xref@host_link mx,
                 udf_cust_log_details@host_link cld
            WHERE c.cod_cust_id = cd.cod_cust_id
                  AND c.cod_cust_id = oa.apl_cus_id(+)
                  AND TRIM(c.cod_cust_id) = ch.nam_lender(+)
                  AND ch.cod_collat_id = cp.cod_collat_id(+)
                  AND ch.cod_coll = cp.cod_coll(+)
                  AND oa.apl_aad_id = oaa.apa_aad_id(+)
                  AND oa.apl_id = oaa.apa_apl_id(+)
                  AND RPAD(TRIM(c.cod_cust_id), 16) = mx.cod_prod_mnem_cust(+)
                  AND to_char(cd.cod_cust_id) = cld.cod_cust_id(+) --to_char added by Yalchin on 17 Aprel 2012
                  --
                  AND NVL(cld.cod_field_tag, 'TXT_5') IN('TXT_5', 'TXT_6')
                  AND mx.flg_mis_type = 'C'
                  --
                  AND c.cod_cust_id = p_customer_id
            GROUP BY c.cod_cust_id;

       RETURN c_result;
  END;
  -- LNA1170 - Client registration form

  -- LNA1180 - Credit line agreement
  -- LNA1181 - Payment schedule for credit line agreement
  FUNCTION lna1180(p_account_no VARCHAR2, p_report_date DATE)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       OPEN c_result FOR
            WITH w_udf AS (SELECT uald.cod_acct_no,
                                  MAX(DECODE(uald.cod_field_tag, 'DAT_0', uald.field_value, NULL)) line_last_date,
                                  MAX(DECODE(uald.cod_field_tag, 'NUM_3', uald.field_value, NULL)) line_total_amount,
                                  MAX(DECODE(uald.cod_field_tag, 'NUM_4', uald.field_value, NULL)) duration_in_month
                           FROM udf_acct_log_details@host_link uald
                           WHERE uald.cod_field_tag IN('DAT_0','NUM_3','NUM_4')
                                 AND uald.flg_mnt_status = 'A'
                                 AND uald.cod_acct_no = v_account_no
                           GROUP BY uald.cod_acct_no),
                 w_cla AS (SELECT DISTINCT TRIM(a.cod_acct_no) account_no,
                                  TRIM(c.cod_cust_id) customer_id,
                                  TRIM(c.nam_cust_full) customer_name,
                                  TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 ||
                                       DECODE(TRIM(c.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c.txt_permadr_add2)) ||
                                       DECODE(TRIM(c.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c.txt_permadr_add3))) customer_address,
                                  TRIM(up.nam_user) branch_manager,
                                  v_bank_address bank_address,
                                  TRIM(bm.nam_cc_city) branch_city,
                                  TRUNC(a.dat_first_disb) disb_date,
                                  DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                                  TRIM(pm.nam_ccy_short) currency_code,
                                  a.rat_var_current loan_rate,
                                  TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN (a.ctr_term_months)
                                            WHEN pm.cod_prod_term_typ IN(2,4) THEN (a.ctr_term_months/367)
                                            WHEN pm.cod_prod_term_typ = 3 THEN (a.ctr_term_months/65536)
                                            ELSE 0
                                       END) loan_term_value,
                                  TRIM(CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                                            WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                                            WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                                            ELSE '0'
                                       END) loan_term_type,
                                  DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb) report_date
                           FROM ci_custmast@host_link c,
                                ln_acct_dtls@host_link a,
                                ln_acct_balances@host_link ab,
                                ln_prod_mast@host_link pm,
                                ba_cc_brn_mast@host_link bm,
                                sm_user_profile@host_link up,
                                ln_acct_schedule_detls@host_link sd
                           WHERE c.cod_cust_id = a.cod_cust_id
                                 AND pm.cod_prod = a.cod_prod
                                 AND bm.cod_cc_brn = a.cod_cc_brn
                                 AND up.cod_cc_brn = a.cod_cc_brn
                                 AND ab.cod_acct_no = a.cod_acct_no
                                 AND sd.cod_acct_no(+) = a.cod_acct_no
                                 AND up.cod_branch_templ = 55
                                 --
                                 AND c.flg_mnt_status = 'A'
                                 AND a.flg_mnt_status = 'A'
                                 AND pm.flg_mnt_status = 'A'
                                 --
                                 AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                                 AND a.cod_acct_no = RPAD(v_account_no, 16, ' ')
                                 AND ROWNUM = 1)
            SELECT c.*,
                   u.line_last_date,
                   u.line_total_amount,
                   u.duration_in_month
            FROM w_cla c,
                 w_udf u
            WHERE c.account_no = u.cod_acct_no;
            /*SELECT DISTINCT TRIM(a.cod_acct_no) account_no,
                   TRIM(c.cod_cust_id) customer_id,
                   TRIM(c.nam_cust_full) customer_name,
                   TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 ||
                        DECODE(TRIM(c.txt_permadr_add2), NULL, NULL, ', ' || TRIM(c.txt_permadr_add2)) ||
                        DECODE(TRIM(c.txt_permadr_add3), NULL, NULL, ', ' || TRIM(c.txt_permadr_add3))) customer_address,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRUNC(a.dat_first_disb) disb_date,
                   DECODE(ab.amt_disbursed, 0, a.amt_face_value, ab.amt_disbursed) loan_amount,
                   TRIM(pm.nam_ccy_short) currency_code,
                   a.rat_var_current loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months)
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367)
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536)
                         ELSE 0
                    END) loan_term_value,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                         WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                         ELSE '0'
                    END) loan_term_type,
                   acd.cod_1 line_last_date,
                   acd.num_1 line_total_amount,
                   acd.num_2 duration_in_month,
                   DECODE(sd.cod_acct_no, NULL, p_report_date, a.dat_first_disb) report_date
            FROM ci_custmast@host_link c,
                 ln_acct_dtls@host_link a,
                 ln_acct_balances@host_link ab,
                 ln_prod_mast@host_link pm,
                 \*rpt_*\ba_cc_brn_mast@host_link bm,
                 sm_user_profile@host_link up,
                 ln_acct_schedule_detls@host_link sd,
                 ln_acct_cbr_dtls@host_link acd
            WHERE c.cod_cust_id = a.cod_cust_id
                  AND pm.cod_prod = a.cod_prod
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND up.cod_cc_brn = a.cod_cc_brn
                  AND TRIM(ab.cod_acct_no) = TRIM(a.cod_acct_no)
                  AND TRIM(acd.cod_acct_no) = TRIM(a.cod_acct_no)
                  AND TRIM(sd.cod_acct_no(+)) = TRIM(a.cod_acct_no)
                  AND up.cod_branch_templ = 55
                  --
                  AND c.flg_mnt_status = 'A'
                  AND a.flg_mnt_status = 'A'
                  AND pm.flg_mnt_status = 'A'
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND TRIM(a.cod_acct_no) = v_account_no
                  AND ROWNUM = 1;*/

       RETURN c_result;
  END;
  -- LNA1180 - Credit line agreement
  -- LNA1181 - Payment schedule for credit line agreement

  -- LNA1190 - Loan restucture agreement
  FUNCTION lna1190(p_account_no VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account_no);
    v_bank_address VARCHAR2(100) := report_methods.get_bank_address;
    v_branch_manager_list VARCHAR2(400) := report_methods.get_branch_manager_list;
  BEGIN
       v_account_no := RPAD(v_account_no, 16, ' ');

       OPEN c_result FOR
            SELECT TRIM(a.cod_acct_no) account_no,
                   TRIM(c.nam_cust_full) customer_name,
                   TRIM(bm.nam_cc_city) branch_city,
                   TRIM(up.nam_user) branch_manager,
                   v_bank_address bank_address,
                   TRIM(c.nam_permadr_city || ', ' || c.txt_permadr_add1 || ', ' || c.txt_permadr_add2) customer_address,
                   TRUNC(SYSDATE) report_date,
                   ab.amt_disbursed loan_amount,
                   TRUNC(a.dat_first_disb) disb_date,
                   TRIM(pm.nam_ccy_short) acy,
                   --a.rat_var_current loan_rate,
                   s.ps_loan_rate loan_rate,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN TRIM(a.ctr_term_months)
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN TRIM(a.ctr_term_months/367)
                         WHEN pm.cod_prod_term_typ = 3 THEN TRIM(a.ctr_term_months/65536)
                         ELSE '0'
                    END) loan_term_value,
                   (CASE WHEN pm.cod_prod_term_typ = 1 THEN 'gün'
                         WHEN pm.cod_prod_term_typ IN(2,4) THEN 'həftə'
                         WHEN pm.cod_prod_term_typ = 3 THEN 'ay'
                         ELSE '0'
                    END) loan_term_type,
                   ab.amt_princ_balance principal_balance,
                   (s.ps_interest_amount - (bd.bal_int1_chg + bd.bal_int1_chg_susp)) interest_balance,
                   (bd.amt_last_int2_chg + bd.amt_last_int2_chg_susp) penalty_amount,
                   (ab.amt_princ_balance + (s.ps_interest_amount - (bd.bal_int1_chg + bd.bal_int1_chg_susp)) + (bd.amt_last_int2_chg + bd.amt_last_int2_chg_susp)) total_amount,
                   s.ps_start_date/*,
                   s.ps_interest_amount*/
            FROM ln_acct_dtls@host_link a,
                 ln_acct_balances@host_link ab,
                 ln_acct_int_balance_dtls@host_link bd,
                 ci_custmast@host_link c,
                 ba_cc_brn_mast@host_link bm,
                 ln_prod_mast@host_link pm,
                 sm_user_profile@host_link up,
                 (SELECT sd.cod_acct_no cod_acct_no,
                         MAX(sd.rat_int) ps_loan_rate,
                         MIN(sd.dat_stage_start) ps_start_date,
                         SUM(sd.amt_interest) ps_interest_amount
                  FROM ln_acct_schedule_detls@host_link sd
                  WHERE sd.cod_acct_no = v_account_no
                  GROUP BY sd.cod_acct_no) s
            WHERE ab.cod_acct_no = a.cod_acct_no
                  AND bd.cod_acct_no = a.cod_acct_no
                  AND s.cod_acct_no = a.cod_acct_no
                  AND c.cod_cust_id = a.cod_cust_id
                  AND bm.cod_cc_brn = a.cod_cc_brn
                  AND pm.cod_prod = a.cod_prod
                  AND up.cod_cc_brn = bm.cod_cc_brn
                  AND up.cod_branch_templ = 55
                  --
                  AND INSTR(v_branch_manager_list, up.cod_user_id) > 0
                  AND a.cod_acct_no = v_account_no;

       RETURN c_result;
  END;
  -- LNA1190 - Loan restucture agreement

END;
/
