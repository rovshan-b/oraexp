CREATE OR REPLACE PACKAGE data_rptdb01
IS

  -- Author  : ELDANIZ
  -- Created : 19.01.2009 16:26:28
  -- Purpose :

  PROCEDURE refresh_all_tables;
  PROCEDURE refresh_flex_tables;
  PROCEDURE refresh_simple_tables;
  PROCEDURE refresh_history_tables;
  PROCEDURE refresh_rpt_tables;
  ------
  PROCEDURE refresh_aah;
  PROCEDURE refresh_ah;
  PROCEDURE refresh_dtl;
  PROCEDURE refresh_ath;
  PROCEDURE refresh_asd;
  PROCEDURE refresh_art;
  PROCEDURE refresh_al;
  PROCEDURE refresh_ci;
  PROCEDURE refresh_rth;
  PROCEDURE refresh_crl;
  PROCEDURE refresh_cfl;
  PROCEDURE refresh_rt; -- rec_txnlog
  PROCEDURE refresh_xt;
  PROCEDURE refresh_aah_for_date(p_date DATE);
  PROCEDURE refresh_ah_for_date(p_date DATE);
  PROCEDURE refresh_dtl_for_date(p_date DATE);
  PROCEDURE refresh_ath_for_date(p_date DATE);
  PROCEDURE refresh_asd_for_date(p_date DATE);
  PROCEDURE refresh_art_for_date(p_date DATE);
  PROCEDURE refresh_al_for_date(p_date DATE);
  PROCEDURE refresh_ci_for_date(p_date DATE);
  PROCEDURE refresh_rth_for_date(p_date DATE);
  PROCEDURE refresh_crl_for_date(p_date DATE);
  PROCEDURE refresh_cfl_for_date(p_max_change_no NUMBER,
                                 p_table_name VARCHAR2,
                                 p_field_list VARCHAR2);
  PROCEDURE refresh_rt_for_date(p_date DATE);
  PROCEDURE refresh_xt_for_date(p_date DATE);
  --
  PROCEDURE refresh_actb_accbal_hist;
  PROCEDURE refresh_ac_acct_crr_code;
  PROCEDURE refresh_ac_acct_writeoff;
  PROCEDURE refresh_ac_crr_codes;
  PROCEDURE refresh_ba_acct_status;
  PROCEDURE refresh_ba_bank_mast;
  PROCEDURE refresh_ba_brn_type_master;
  PROCEDURE refresh_ba_ccy_code;
  PROCEDURE refresh_ba_ccy_rate;
  PROCEDURE refresh_ba_cc_brn_mast;
  PROCEDURE refresh_ba_coll_auto;
  PROCEDURE refresh_ba_coll_codes;
  PROCEDURE refresh_ba_coll_hdr;
  PROCEDURE refresh_ba_coll_prop;
  PROCEDURE refresh_ba_group_details;
  PROCEDURE refresh_ba_lo_coll_acct_xr;
  PROCEDURE refresh_ba_sc_code;
  PROCEDURE refresh_ba_txn_mis_xref;
  PROCEDURE refresh_ba_txn_mnemonic;
  PROCEDURE refresh_ch_acct_cust_xref;
  PROCEDURE refresh_ci_business_types;
  PROCEDURE refresh_ci_custdetl;
  PROCEDURE refresh_ci_custmast;
  PROCEDURE refresh_ci_custrel;
  PROCEDURE refresh_ci_cust_types;
  PROCEDURE refresh_ci_ic_types;
  PROCEDURE refresh_ci_relmast;
  PROCEDURE refresh_conv_ci_custmast;
  PROCEDURE refresh_cs_ho_blacklist;
  PROCEDURE refresh_cs_prop_log;
  PROCEDURE refresh_gltm_glmaster;
  PROCEDURE refresh_gl_detemp;
  PROCEDURE refresh_gl_master;
  PROCEDURE refresh_ln_acct_attributes;
  PROCEDURE refresh_ln_acct_balances;
  PROCEDURE refresh_ln_acct_cbr_dtls;
  PROCEDURE refresh_ln_acct_dtls;
  PROCEDURE refresh_ln_acct_int_bal_dt;
  PROCEDURE refresh_ln_acct_sched_dtls;
  PROCEDURE refresh_ln_acct_sdh;
  PROCEDURE refresh_ln_arrears_table;
  PROCEDURE refresh_ln_arrear_txn_hist;
  PROCEDURE refresh_ln_dedn_plan;
  PROCEDURE refresh_ln_prod_mast;
  PROCEDURE refresh_ln_sched_types;
  PROCEDURE refresh_or_applicants;
  PROCEDURE refresh_or_applicants_md;
  PROCEDURE refresh_or_applications;
  PROCEDURE refresh_or_applications_md;
  PROCEDURE refresh_or_app_assets;
  PROCEDURE refresh_or_app_assets_md;
  PROCEDURE refresh_or_lookups;
  PROCEDURE refresh_sm_temp_profile;
  PROCEDURE refresh_sm_user_profile;
  PROCEDURE refresh_udf_acct_log_dtls;
  PROCEDURE refresh_udf_cust_log_dtls;
  PROCEDURE refresh_udf_lov_mast;

  --rpt tables
  --------------------------------------------------------------------------------------------

  PROCEDURE refresh_rpt_add_txn;
  PROCEDURE refresh_rpt_other_banks;
  PROCEDURE refresh_rpt_bme;
  PROCEDURE refresh_rpt_co_bnk_rel;
  PROCEDURE refresh_rpt_co_cs_rel;
  PROCEDURE refresh_rpt_co_levels;
  PROCEDURE refresh_rpt_gl_rpt_ln;
  PROCEDURE refresh_rpt_report_col;
  PROCEDURE refresh_rpt_report_col_rel;
  PROCEDURE refresh_rpt_reports;
  PROCEDURE refresh_rpt_settings;
  PROCEDURE refresh_rpt_siem_prst_shrt;
  PROCEDURE refresh_rpt_sys_problem_lns;
  PROCEDURE refresh_rpt_tree_items;
  PROCEDURE refresh_rpt_logs;

	-------------------- MASK DATA FOR OTHER FINCA HUB ---------------
	PROCEDURE mask_sensitive_tables;
	PROCEDURE restore_masked_tables;

END;
/
CREATE OR REPLACE PACKAGE BODY data_rptdb01
IS

  PROCEDURE refresh_all_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_all_tables';
  BEGIN
       v_started := SYSDATE;

       refresh_rpt_tables;
       refresh_simple_tables;
       refresh_flex_tables;
       refresh_history_tables;

       log_manager.add_log('REF_ALL',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR ALL TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_ALL_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_flex_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_flex_tables';
  BEGIN
       v_started := SYSDATE;

       refresh_ac_acct_crr_code;
       --refresh_ac_acct_writeoff;
       --refresh_ac_crr_codes;
       --refresh_ba_acct_status;
       --refresh_ba_brn_type_master;
       --refresh_ba_ccy_code;
       --refresh_ba_ccy_rate;
       refresh_ba_coll_auto;
       --refresh_ba_coll_codes;
       refresh_ba_coll_hdr;
       --refresh_ba_coll_prop;
       refresh_ba_group_details;
       refresh_ba_lo_coll_acct_xr;
       --refresh_ba_sc_code;
       refresh_ba_txn_mis_xref;
       --refresh_ba_txn_mnemonic;
       refresh_ch_acct_cust_xref;
       --refresh_ci_business_types;
       refresh_ci_custdetl;
       refresh_ci_custmast;
       refresh_ci_custrel;
       --refresh_ci_cust_types;
       --refresh_ci_ic_types;
       --refresh_ci_relmast;
       --refresh_cs_ho_blacklist;
       refresh_cs_prop_log;
       --refresh_gltm_glmaster;
       refresh_gl_detemp;
       refresh_gl_master;
       refresh_ln_acct_attributes;
       refresh_ln_acct_balances;
       --refresh_ln_acct_cbr_dtls;
       refresh_ln_acct_dtls;
       refresh_ln_acct_int_bal_dt;
       refresh_ln_acct_sdh;
       --refresh_ln_dedn_plan;
       --refresh_ln_prod_mast;
       --refresh_ln_sched_types;
       refresh_or_applicants;
       refresh_or_applicants_md;
       refresh_or_applications;
       refresh_or_applications_md;
       refresh_or_app_assets;
       refresh_or_app_assets_md;
       --refresh_or_lookups;
       --refresh_sm_temp_profile;
       --refresh_sm_user_profile;
       refresh_udf_acct_log_dtls;
       refresh_udf_cust_log_dtls;
       refresh_udf_lov_mast;
       --
       refresh_ba_bank_mast;
       --
       data_transfer.refresh_mv_tables(0);

       log_manager.add_log('REF_FLEX',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR FLEX TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_FLEX_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_simple_tables
  IS
    c_result data_types.result_cursor;
    v_query CLOB;
    v_proc_name_exec VARCHAR2(50);
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_simple_tables';
  BEGIN
       v_started := SYSDATE;
       v_query := 'WITH ';

       FOR i IN(SELECT ROWNUM rnum,
                       MAX(ROWNUM) OVER(ORDER BY NULL) max_rownum,
                       REGEXP_REPLACE(REGEXP_SUBSTR(t.hosts || ',',
                                                    '(.*?),',
                                                    1,
                                                    LEVEL),
                                      ',$',
                                      '') host_name,
                       REGEXP_REPLACE(REGEXP_SUBSTR(t.cursors || ',',
                                                    '(.*?),',
                                                    1,
                                                    LEVEL),
                                      ',$',
                                      '') cursor_name,
                       REGEXP_REPLACE(REGEXP_SUBSTR(t.db_links || ',',
                                                    '(.*?),',
                                                    1,
                                                    LEVEL),
                                      ',$',
                                      '') db_link,
                       REGEXP_REPLACE(REGEXP_SUBSTR(t.spaces || ',',
                                                    '(.*?),',
                                                    1,
                                                    LEVEL),
                                      ',$',
                                      '') prev_space
                FROM (SELECT SUBSTR(rs.setting_value, 0, INSTR(rs.setting_value, ';', 1)-1) hosts,
                             SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1)+1, INSTR(rs.setting_value, ';', 1, 2)-INSTR(rs.setting_value, ';', 1)-1) cursors,
                             SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1, 2)+1, INSTR(rs.setting_value, ';', 1, 3)-INSTR(rs.setting_value, ';', 1, 2)-1) spaces,
                             --SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1, 3)+1, LENGTH(rs.setting_value)-INSTR(rs.setting_value, ';', 1, 3)) db_links,
                             ',@rptdb01_link' db_links
                      FROM rpt_report_settings rs
                      WHERE rs.setting_key = 'DB_SIMPLE_TABLE_REFRESH_PARAMS') t
                CONNECT BY REGEXP_INSTR(t.hosts || ',',
                                        '(.*?),',
                                        1,
                                        LEVEL) > 0)
       LOOP
           BEGIN
                v_query := v_query || i.cursor_name || ' AS (';

                FOR j IN(SELECT ROWNUM rnum,
                                MAX(ROWNUM) OVER(ORDER BY NULL) max_rownum,
                                REGEXP_REPLACE(REGEXP_SUBSTR(t.table_list || ',',
                                                             '(.*?),',
                                                             1,
                                                             LEVEL),
                                               ',$',
                                               '') table_name,
                                REGEXP_REPLACE(REGEXP_SUBSTR(t.proc_list || ',',
                                                             '(.*?),',
                                                             1,
                                                             LEVEL),
                                               ',$',
                                               '') proc_name
                         FROM (SELECT SUBSTR(rs.setting_value, 0, INSTR(rs.setting_value, ';', 1)-1) table_list,
                                      SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1)+1, INSTR(rs.setting_value, ';', 1, 2)-INSTR(rs.setting_value, ';', 1)-1) proc_list
                               FROM rpt_report_settings rs
                               WHERE rs.setting_key = 'DB_SIMPLE_TABLES') t
                         CONNECT BY REGEXP_INSTR(t.table_list || ',',
                                                 '(.*?),',
                                                 1,
                                                 LEVEL) > 0)
                LOOP
                    v_query := v_query
                               || (CASE WHEN j.rnum = 1 THEN '' ELSE i.prev_space END)
                               || 'SELECT '''
                               || i.host_name
                               || ''' host_name, '''
                               || j.table_name
                               || ''' table_name,'''
                               || j.proc_name
                               || ''' proc_name, MAX(t.dat_last_mnt) max_dt, COUNT(*) row_count FROM '
                               || j.table_name
                               || i.db_link
                               || ' t';
                    v_query := v_query
                               || (CASE WHEN j.rnum != j.max_rownum THEN CHR(10) || i.prev_space || 'UNION' || CHR(10) ELSE '' END);
                END LOOP;
                v_query := v_query || ')' || (CASE WHEN i.rnum != i.max_rownum THEN ',' ELSE '' END) || CHR(10);
           END;
       END LOOP;
       v_query := v_query
                  || 'SELECT --DECODE(r.max_dt, h.max_dt, DECODE(r.row_count, h.row_count, 0, 1), 1) diff,' || CHR(10)
                  || '       r.proc_name' || CHR(10)
                  || '       --,r.*, h.*' || CHR(10)
                  || 'FROM w_rpt r,' || CHR(10)
                  || '     w_host h' || CHR(10)
                  || 'WHERE r.table_name = h.table_name' || CHR(10)
                  || '      AND DECODE(r.max_dt, h.max_dt, DECODE(r.row_count, h.row_count, 0, 1), 1) > 0';

       --report_methods.custom_output(v_query);

       OPEN c_result FOR v_query;

       v_query := 'BEGIN' || CHR(10);
       LOOP
           FETCH c_result INTO v_proc_name_exec;
           EXIT WHEN c_result%NOTFOUND;

           v_query := v_query
                      || '     '
                      || LOWER($$PLSQL_UNIT)
                      || '.'
                      || v_proc_name_exec
                      || ';'
                      || CHR(10);
       END LOOP;
       CLOSE c_result;
       v_query := v_query || 'END;';

       IF INSTR(v_query, LOWER($$PLSQL_UNIT), 1) > 0
       THEN
           EXECUTE IMMEDIATE v_query;
           --report_methods.custom_output(v_query);
       END IF;

       log_manager.add_log('REF_SIMPLE',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR MAIN SIMPLE TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_SIMPLE_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_history_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_history_tables';
  BEGIN
       v_started := SYSDATE;

       refresh_aah;
       refresh_ah;
       refresh_dtl;
       refresh_ath;
       refresh_asd;
       refresh_art;
       refresh_al;
       refresh_crl;
       refresh_cfl;
       refresh_xt;
       --data_collector.refresh_dtl_po;
       --data_collector.refresh_rpt_gl_totals;
       --refresh_ci;
       --
       data_transfer.refresh_mv_tables(1);

       log_manager.add_log('REF_HIST',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR HISTORY TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_HIST_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_tables';
  BEGIN
       v_started := SYSDATE;

       refresh_rpt_logs;
       refresh_rpt_add_txn;
       refresh_rpt_other_banks;
       refresh_rpt_bme;
       refresh_rpt_co_bnk_rel;
       refresh_rpt_co_cs_rel;
       refresh_rpt_co_levels;
       refresh_rpt_tree_items;
       refresh_rpt_gl_rpt_ln;
       refresh_rpt_report_col;
       refresh_rpt_report_col_rel;
       refresh_rpt_reports;
       refresh_rpt_settings;
       --refresh_rpt_siem_prst_shrt;
       refresh_rpt_sys_problem_lns;

       log_manager.add_log('REF_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR REPORT TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_aah
  IS
  BEGIN
       FOR i IN (SELECT t.bkg_date/*,
                        SUM(DECODE(t.table_name, 'actb_accbal_history@rptdb01_link', t.cnt, 0)) "actb_accbal_history@rptdb01_link",
                        SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0)) "actb_accbal_history",
                        (SUM(DECODE(t.table_name, 'actb_accbal_history@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0))) diff*/
                 FROM (SELECT 'actb_accbal_history@rptdb01_link' table_name,
                              TRUNC(ah.bkg_date) bkg_date,
                              COUNT(*) cnt
                       FROM actb_accbal_history@rptdb01_link ah
                       WHERE ah.bkg_date >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(ah.bkg_date)
                       UNION ALL
                       SELECT 'actb_accbal_history' table_name,
                              TRUNC(ah.bkg_date) bkg_date,
                              COUNT(*) cnt
                       FROM actb_accbal_history ah
                       WHERE ah.bkg_date >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(ah.bkg_date)) t
                 GROUP BY t.bkg_date
                 HAVING (SUM(DECODE(t.table_name, 'actb_accbal_history@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_aah_for_date(i.bkg_date);
       END LOOP;
  END;

  PROCEDURE refresh_ah
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'actb_history@rptdb01_link', t.cnt, 0)) "actb_history@rptdb01_link",
                        SUM(decode(t.table_name, 'actb_history', t.cnt, 0)) "actb_history",
                        (SUM(decode(t.table_name, 'actb_history@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'actb_history', t.cnt, 0))) diff*/
                 FROM (SELECT 'actb_history' table_name,
                              trunc(ah.trn_dt) txn_date,
                              COUNT(*) cnt
                       FROM actb_history ah
                       WHERE ah.trn_dt >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY trunc(ah.trn_dt)
                       UNION ALL
                       SELECT 'actb_history@rptdb01_link' table_name,
                              trunc(ah.trn_dt) txn_date,
                              COUNT(*) cnt
                       FROM actb_history@rptdb01_link ah
                       WHERE ah.trn_dt >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY trunc(ah.trn_dt)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'actb_history@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'actb_history', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_ah_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_dtl
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*, -- new
                        SUM(decode(t.table_name, 'ln_daily_txn_log_hist@rptdb01_link', t.cnt, 0)) "ln_daily_txn_log@rptdb01_link",
                        SUM(decode(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0)) "ln_daily_txn_log_hist",
                        (SUM(decode(t.table_name, 'ln_daily_txn_log_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_daily_txn_log_hist@rptdb01_link' table_name,
                              trunc(tl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_daily_txn_log_hist@rptdb01_link tl
                       WHERE tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(tl.dat_txn)
                       UNION ALL
                       SELECT t1.table_name,
                              t1.txn_date,
                              (t1.cnt - NVL(t2.txn_diff_count, 0)) cnt
                       FROM (SELECT 'ln_daily_txn_log_hist' table_name,
                                    TRUNC(tl.dat_txn) txn_date,
                                    COUNT(*) cnt
                             FROM ln_daily_txn_log_hist tl
                             WHERE tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                             GROUP BY TRUNC(tl.dat_txn)) t1,
                            (SELECT rat.table_name,
                                    rat.for_date,
                                    COUNT(*) txn_count,
                                    SUM(DECODE(rat.txn_type, 'I', 1, 'D', -1, 'U', 0, 0)) txn_diff_count
                             FROM rpt_additional_txn rat
                             GROUP BY rat.table_name,
                                      rat.for_date) t2
                       WHERE t1.table_name = t2.table_name(+)
                             AND t1.txn_date = t2.for_date(+)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'ln_daily_txn_log_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_dtl_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_ath
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'ln_arrear_txn_hist@rptdb01_link', t.cnt, 0)) "ln_arrear_txn_hist@rptdb01_link",
                        SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0)) "ln_arrear_txn_hist",
                        (SUM(decode(t.table_name, 'ln_arrear_txn_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_arrear_txn_hist@rptdb01_link' table_name,
                              trunc(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_arrear_txn_hist@rptdb01_link th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(th.dat_txn)
                       UNION ALL
                       SELECT 'ln_arrear_txn_hist' table_name,
                              trunc(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_arrear_txn_hist th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(th.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'ln_arrear_txn_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_ath_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_asd
  IS
  BEGIN
       FOR i IN (SELECT t.stage_start_date/*,
                        SUM(decode(t.table_name, 'ln_acct_schedule_detls@rptdb01_link', t.cnt, 0)) "ln_acct_sched_dtls@rptdb01_link",
                        SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0)) "ln_acct_schedule_detls",
                        (SUM(decode(t.table_name, 'ln_acct_schedule_detls@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_acct_schedule_detls@rptdb01_link' table_name,
                              trunc(s.dat_stage_start) stage_start_date,
                              COUNT(*) cnt
                       FROM ln_acct_schedule_detls@rptdb01_link s
                       WHERE s.dat_stage_start >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(s.dat_stage_start)
                       UNION ALL
                       SELECT 'ln_acct_schedule_detls' table_name,
                              trunc(s.dat_stage_start) stage_start_date,
                              COUNT(*) cnt
                       FROM ln_acct_schedule_detls s
                       WHERE s.dat_stage_start >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(s.dat_stage_start)) t
                 GROUP BY t.stage_start_date
                 HAVING (SUM(decode(t.table_name, 'ln_acct_schedule_detls@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_asd_for_date(i.stage_start_date);
       END LOOP;
  END;

  PROCEDURE refresh_art
  IS
  BEGIN
       FOR i IN (SELECT t.arr_due_date/*,
                        SUM(decode(t.table_name, 'ln_arrears_table@rptdb01_link', t.cnt, 0)) "ln_arrears_table@rptdb01_link",
                        SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0)) "ln_arrears_table",
                        (SUM(decode(t.table_name, 'ln_arrears_table@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_arrears_table@rptdb01_link' table_name,
                              trunc(lat.dat_arrears_due) arr_due_date,
                              COUNT(*) cnt
                       FROM ln_arrears_table@rptdb01_link lat
                       WHERE lat.dat_arrears_due >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -13)
                       GROUP BY trunc(lat.dat_arrears_due)
                       UNION ALL
                       SELECT 'ln_arrears_table' table_name,
                              trunc(lat.dat_arrears_due) arr_due_date,
                              COUNT(*) cnt
                       FROM ln_arrears_table lat
                       WHERE lat.dat_arrears_due >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -13)
                       GROUP BY trunc(lat.dat_arrears_due)) t
                 GROUP BY t.arr_due_date
                 HAVING (SUM(decode(t.table_name, 'ln_arrears_table@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_art_for_date(i.arr_due_date);
       END LOOP;
  END;

  PROCEDURE refresh_al
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'ln_acct_ledg@rptdb01_link', t.cnt, 0)) "ln_arrears_table@rptdb01_link",
                        SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0)) "ln_arrears_table",
                        (SUM(decode(t.table_name, 'ln_acct_ledg@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_acct_ledg@rptdb01_link' table_name,
                              trunc(al.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_acct_ledg@rptdb01_link al
                       WHERE al.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(al.dat_txn)
                       UNION ALL
                       SELECT 'ln_acct_ledg' table_name,
                              trunc(al.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_acct_ledg al
                       WHERE al.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY trunc(al.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'ln_acct_ledg@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_al_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_ci
  IS
  BEGIN
       FOR i IN (SELECT t.mnt_date/*,
                        SUM(decode(t.table_name, 'ci_custmast@rptdb01_link', t.cnt, 0)) "ci_custmast@rptdb01_link",
                        SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0)) "ci_custmast",
                        (SUM(decode(t.table_name, 'ci_custmast@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0))) diff*/
                 FROM (SELECT 'ci_custmast@rptdb01_link' table_name,
                              trunc(c.dat_last_mnt) mnt_date,
                              COUNT(*) cnt
                       FROM ci_custmast@rptdb01_link c
                       GROUP BY trunc(c.dat_last_mnt)
                       UNION ALL
                       SELECT 'ci_custmast' table_name,
                              trunc(c.dat_last_mnt) mnt_date,
                              COUNT(*) cnt
                       FROM ci_custmast c
                       GROUP BY trunc(c.dat_last_mnt)) t
                 GROUP BY t.mnt_date
                 HAVING (SUM(decode(t.table_name, 'ci_custmast@rptdb01_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_ci_for_date(i.mnt_date);
       END LOOP;
  END;

  PROCEDURE refresh_rth
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@rptdb01_link', t.cnt, 0)) "ln_rpa_txnlog_hist@rptdb01_link",
                        SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0)) "ln_rpa_txnlog_hist",
                        (SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_rpa_txnlog_hist@rptdb01_link' table_name,
                              TRUNC(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_rpa_txnlog_hist@rptdb01_link th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(th.dat_txn)
                       UNION ALL
                       SELECT 'ln_rpa_txnlog_hist' table_name,
                              TRUNC(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_rpa_txnlog_hist th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(th.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_rth_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_crl
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'ba_change_record_log@rptdb01_link', t.cnt, 0)) "ba_change_record_log@rptdb01_link",
                        SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0)) "ba_change_record_log",
                        (SUM(DECODE(t.table_name, 'ba_change_record_log@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0))) diff*/
                 FROM (SELECT 'ba_change_record_log@rptdb01_link' table_name,
                              TRUNC(crl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ba_change_record_log@rptdb01_link crl
                       WHERE crl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(crl.dat_txn)
                       UNION ALL
                       SELECT 'ba_change_record_log' table_name,
                              TRUNC(rl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ba_change_record_log rl
                       WHERE rl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(rl.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'ba_change_record_log@rptdb01_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_crl_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_cfl
  IS
    v_max_change_no NUMBER := 0;
  BEGIN
       SELECT MAX(crl.ctr_change_no) INTO v_max_change_no
       FROM ba_change_record_log crl
       WHERE crl.dat_txn < ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1);

       FOR i IN (SELECT cf.table_name,
                        WM_CONCAT(cf.field_name) field_list/*,
                        SUM(cf."@rptdb01_link") "@rptdb01_link",
                        SUM(cf."reporting") "reporting",
                        SUM(cf.diff) diff*/
                 FROM (SELECT t.table_name,
                              t.field_name,
                              SUM(DECODE(t.host, '@rptdb01_link', t.cnt, 0)) "@rptdb01_link",
                              SUM(DECODE(t.host, 'reporting', t.cnt, 0)) "reporting",
                              (SUM(DECODE(t.host, '@rptdb01_link', t.cnt, 0)) -
                               SUM(DECODE(t.host, 'reporting', t.cnt, 0))) diff
                       FROM (SELECT '@rptdb01_link' host,
                                    cfl.name_table table_name,
                                    cfl.cod_field_name field_name,
                                    COUNT(*) cnt
                             FROM ba_change_field_log@rptdb01_link cfl
                             WHERE cfl.ctr_change_no > v_max_change_no
                             GROUP BY cfl.name_table,
                                      cfl.cod_field_name
                             UNION ALL
                             SELECT 'reporting' host,
                                    fl.name_table table_name,
                                    fl.cod_field_name field_name,
                                    COUNT(*) cnt
                             FROM ba_change_field_log fl
                             WHERE fl.ctr_change_no > v_max_change_no
                             GROUP BY fl.name_table,
                                      fl.cod_field_name) t
                       GROUP BY t.table_name,
                                t.field_name
                       HAVING (SUM(DECODE(t.host, '@rptdb01_link', t.cnt, 0)) -
                               SUM(DECODE(t.host, 'reporting', t.cnt, 0))) != 0) cf
                 GROUP BY cf.table_name
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_cfl_for_date(v_max_change_no, i.table_name, i.field_list);
       END LOOP;
  END;

  PROCEDURE refresh_rt -- rec_txnlog
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date,
                        SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) "v_rec_txnlog",
                        SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0)) "rpt_rec_txnlog",
                        (SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0))) diff
                 FROM (SELECT 'v_rec_txnlog' table_name,
                              TRUNC(vrt.tim_sys) txn_date,
                              COUNT(*) cnt
                       FROM rpt_rec_txnlog@rptdb01_link vrt
                       WHERE TRUNC(vrt.tim_sys) < TRUNC(SYSDATE)
                             AND vrt.tim_sys >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(vrt.tim_sys)
                       UNION ALL
                       SELECT 'rpt_rec_txnlog' table_name,
                              TRUNC(rt.tim_sys) txn_date,
                              COUNT(*) cnt
                       FROM rpt_rec_txnlog rt
                       WHERE TRUNC(rt.tim_sys) < TRUNC(SYSDATE)
                             AND rt.tim_sys >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -6)
                       GROUP BY TRUNC(rt.tim_sys)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_rptdb01.refresh_rt_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_xt
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'host', t.cnt, 0)) host,
                        SUM(DECODE(t.table_name, 'rpt', t.cnt, 0)) rpt,
                        (SUM(DECODE(t.table_name, 'host', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'rpt', t.cnt, 0))) diff*/
                 FROM (SELECT 'host' table_name,
                              TRUNC(xt.dat_log) txn_date,
                              COUNT(*) cnt
                       FROM xf_ol_st_txnlog_mmdd@rptdb01_link xt
                       WHERE xt.dat_log >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(xt.dat_log)
                       UNION ALL
                       SELECT 'rpt' table_name,
                              TRUNC(xt.dat_log) txn_date,
                              COUNT(*) cnt
                       FROM xf_ol_st_txnlog_mmdd xt
                       WHERE xt.dat_log >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY TRUNC(xt.dat_log)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'host', t.cnt, 0)) != SUM(DECODE(t.table_name, 'rpt', t.cnt, 0)))
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_xt_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_aah_for_date(p_date DATE)
  IS
    -- actb_history for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_aah_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM actb_accbal_history aah
       WHERE trunc(aah.bkg_date) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO actb_accbal_history
       SELECT aah.branch_code,
              TRIM(aah.account) account,
              aah.bkg_date,
              aah.acy_opening_bal,
              aah.lcy_opening_bal,
              aah.acy_closing_bal,
              aah.lcy_closing_bal,
              aah.acy_dr_tur,
              aah.lcy_dr_tur,
              aah.acy_cr_tur,
              aah.lcy_cr_tur,
              aah.acc_ccy
       FROM actb_accbal_history@rptdb01_link aah
       WHERE trunc(aah.bkg_date) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_ah_for_date(p_date DATE)
  IS
    -- actb_history for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ah_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM actb_history ah
       WHERE trunc(ah.trn_dt) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO actb_history
       SELECT ah.trn_code,
              TRIM(ah.trn_ref_no) trn_ref_no,
              ah.ac_entry_sr_no,
              ah.trn_dt,
              ah.value_dt,
              ah.txn_init_date,
              TRIM(ah.txt_txn_desc) txt_txn_desc,
              ah.ac_branch,
              TRIM(ah.ac_no) ac_no,
              TRIM(ah.ac_ccy) ac_ccy,
              ah.drcr_ind,
              ah.fcy_amount,
              ah.lcy_amount,
              TRIM(ah.related_customer) related_customer,
              TRIM(ah.related_account) related_account,
              TRIM(ah.financial_cycle) financial_cycle,
              TRIM(ah.period_code) period_code,
              TRIM(ah.instrument_code) instrument_code,
              ah.batch_no,
              ah.curr_no,
              ah.user_id,
              ah.auth_id,
              ah.cust_gl
       FROM actb_history@rptdb01_link ah
       WHERE trunc(ah.trn_dt) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_dtl_for_date(p_date DATE)
  IS
    -- ln_daily_txn_log_hist for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_dtl_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_daily_txn_log_hist dtl
       WHERE trunc(dtl.dat_txn) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_daily_txn_log_hist
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
       FROM ln_daily_txn_log_hist@rptdb01_link tl
       WHERE trunc(tl.dat_txn) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_ath_for_date(p_date DATE)
  IS
    -- ln_arrear_txn_hist for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ath_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrear_txn_hist th
       WHERE trunc(th.dat_txn) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_arrear_txn_hist
       SELECT TRIM(th.cod_acct_no) cod_acct_no,
              th.ref_billno_srl,
              th.cod_arrear_type,
              th.dat_post,
              th.dat_value,
              th.dat_txn,
              th.dat_process,
              th.cod_acc_ccy,
              th.amt_txn_acy,
              th.cod_drcr,
              th.cod_txn_mnemonic
       FROM ln_arrear_txn_hist@rptdb01_link th
       WHERE trunc(th.dat_txn) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_asd_for_date(p_date DATE)
  IS
    -- ln_acct_schedule_detls for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_asd_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_schedule_detls s
       WHERE trunc(s.dat_stage_start) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls
       SELECT TRIM(s.cod_acct_no) cod_acct_no,
              s.dat_stage_start,
              s.ctr_instal,
              s.date_instal,
							s.rat_int,
              s.amt_principal,
              s.amt_interest,
              s.amt_princ_bal
       FROM ln_acct_schedule_detls@rptdb01_link s
       WHERE trunc(s.dat_stage_start) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_art_for_date(p_date DATE)
  IS
    -- ln_arrears_table for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_art_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrears_table la
       WHERE trunc(la.dat_arrears_due) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_arrears_table
       SELECT TRIM(la.cod_acct_no) cod_acct_no,
              la.ref_billno_srl,
              la.cod_arrear_type,
              la.cod_charge_ccy,
              la.amt_arrears_assessed,
              la.amt_arrears_due,
              la.dat_arrears_due,
              la.dat_last_payment,
              la.dat_arrear_cancelled
       FROM ln_arrears_table@rptdb01_link la
       WHERE trunc(la.dat_arrears_due) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_al_for_date(p_date DATE)
  IS
    -- ln_acct_ledg for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_al_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_ledg al
       WHERE trunc(al.dat_txn) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_ledg
       SELECT al.dat_txn,
              al.dat_value,
              al.dat_post,
              al.cod_cc_brn_txn,
              TRIM(al.cod_acct_no) cod_acct_no,
              al.ctr_batch_no,
              al.cod_txn_mnemonic,
              al.cod_txn_ccy,
              al.amt_txn_acy,
              al.amt_txn_tcy,
              al.amt_txn_lcy,
              al.flg_drcr,
              al.txt_txn_desc,
              al.cod_user_id,
              al.cod_auth_id,
              al.ref_sys_tr_aud_no,
              al.ref_sub_seq_no
       FROM ln_acct_ledg@rptdb01_link al
       WHERE trunc(al.dat_txn) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_ci_for_date(p_date DATE)
  IS
    -- ci_custmast for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custmast c
       WHERE trunc(c.dat_last_mnt) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_custmast
       SELECT c.cod_cust_id,
              TRIM(c.cod_officr_id) cod_officr_id,
              c.cod_cc_homebrn,
              c.cod_cust_natl_id,
              c.cod_business_cat,
              c.cod_cust_stat,
              c.cod_cust_marstat,
              TRIM(c.cod_employee_id) cod_employee_id,
              TRIM(c.nam_cust_full) nam_cust_full,
              TRIM(c.nam_cust_first) nam_cust_first,
              TRIM(c.nam_cust_mid) nam_cust_mid,
              TRIM(c.nam_cust_last) nam_cust_last,
              TRIM(c.nam_cust_shrt) nam_cust_shrt,
              TRIM(c.nam_custadr_cntry) nam_custadr_cntry,
              TRIM(c.nam_custadr_city) nam_custadr_city,
              TRIM(c.nam_custadr_state) nam_custadr_state,
              TRIM(c.txt_custadr_add1) txt_custadr_add1,
              TRIM(c.txt_custadr_add2) txt_custadr_add2,
              TRIM(c.txt_custadr_add3) txt_custadr_add3,
              TRIM(c.nam_permadr_cntry) nam_permadr_cntry,
              TRIM(c.nam_permadr_city) nam_permadr_city,
              TRIM(c.nam_permadr_state) nam_permadr_state,
              TRIM(c.txt_permadr_add1) txt_permadr_add1,
              TRIM(c.txt_permadr_add2) txt_permadr_add2,
              TRIM(c.txt_permadr_add3) txt_permadr_add3,
              TRIM(c.txt_permadr_zip) txt_permadr_zip,
              c.ref_res_phone_country,
              c.ref_res_phone_area,
              c.ref_cust_phone,
              c.ref_cust_phone_off,
              c.ref_cust_telex,
              c.ref_phone_mobile,
              c.ref_office_phone_area,
              c.flg_cust_typ,
              c.flg_ic_typ,
              c.txt_cust_sex,
              TRIM(c.txt_cust_desgn) txt_cust_desgn,
              TRIM(c.txt_cust_educn) txt_cust_educn,
              c.dat_birth_cust,
              c.dat_cust_open,
              TRIM(c.cod_last_mnt_makerid) cod_last_mnt_makerid,
              TRIM(c.cod_mis_cust_code_3) cod_mis_cust_code_3,
              c.ctr_cust_spouses,
              c.dat_last_mnt
       FROM ci_custmast@rptdb01_link c
       WHERE trunc(c.dat_last_mnt) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_rth_for_date(p_date DATE)
  IS
    -- ln_rpa_txnlog_hist for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rth_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_rpa_txnlog_hist th
       WHERE trunc(th.dat_txn) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_rpa_txnlog_hist
       SELECT TRIM(th.cod_acct_no) cod_acct_no,
              th.cod_int_type,
              th.flg_drcr,
              th.cod_txn_ccy,
              th.amt_txn_acy,
              th.dat_process,
              th.dat_post,
              th.dat_value,
              th.dat_txn,
              th.cod_txn_mnemonic,
              TRIM(th.txt_txn_desc) txt_txn_desc,
              th.ref_txn_no,
              th.ref_usr_no
       FROM ln_rpa_txnlog_hist@rptdb01_link th
       WHERE trunc(th.dat_txn) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_crl_for_date(p_date DATE)
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_crl_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_change_record_log crl
       WHERE trunc(crl.dat_txn) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_change_record_log
       SELECT crl.ctr_change_no,
              crl.cod_task,
              crl.cod_cc_brn,
              crl.dat_post,
              crl.dat_txn,
              crl.cod_txn_typ,
              crl.cod_user_id,
              crl.cod_auth_id,
              crl.cod_mnt_action,
              crl.flg_status,
              TRIM(crl.flg_status_reason) flg_status_reason,
              TRIM(crl.cod_acct_no) cod_acct_no,
              crl.cod_cust
       FROM ba_change_record_log@rptdb01_link crl
       WHERE TRUNC(crl.dat_txn) = TRUNC(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_cfl_for_date(p_max_change_no NUMBER,
                                 p_table_name VARCHAR2,
                                 p_field_list VARCHAR2)
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_cfl_for_date';
    v_params VARCHAR2(100) := 'p_max_change_no = ' || TRIM(p_max_change_no) ||
                              '; p_table_name = ' || p_table_name/* ||
                              '; p_field_list = ' || p_field_list*/;
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_change_field_log cfl
       WHERE cfl.ctr_change_no > p_max_change_no
             AND cfl.name_table = p_table_name
             AND INSTR(p_field_list, cfl.cod_field_name) > 0;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_change_field_log
       SELECT cfl.ctr_change_no,
              cfl.ctr_table_no,
              cfl.name_table,
              cfl.row_no,
              cfl.cod_field_name,
              TRIM(cfl.old_field_value) old_field_value,
              TRIM(cfl.new_field_value) new_field_value,
              cfl.key_flg
       FROM ba_change_field_log@rptdb01_link cfl
       WHERE cfl.ctr_change_no > p_max_change_no
             AND cfl.name_table = p_table_name
             AND INSTR(p_field_list, cfl.cod_field_name) > 0;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_rt_for_date(p_date DATE)
  IS
    -- rpt_rec_txnlog for date
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rt_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_rec_txnlog rt
       WHERE trunc(rt.tim_sys) = trunc(p_date);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_rec_txnlog
       SELECT rt.cod_brn,
              rt.dat_post,
              rt.user_no,
              rt.user_id,
              rt.id_tran_seq,
              rt.flg_upd_tot_stat,
              TRIM(rt.cod_tran) cod_tran,
              rt.cod_tran_stat,
              rt.dat_sys,
              rt.tim_sys,
              TRIM(rt.id_acct) id_acct,
              rt.cod_cc_brn,
              TRIM(rt.id_from_acct) id_from_acct,
              rt.cod_from_cc_brn,
              TRIM(rt.id_to_acct) id_to_acct,
              rt.cod_to_cc_brn,
              rt.dat_val,
              rt.cod_tran_cur,
              rt.cod_acct_cur,
              rt.rat_tran_cur,
              rt.rat_acct_cur,
              rt.amt_tran_tcye,
              rt.amt_tran_acye,
              rt.amt_tran_lcye,
              rt.amt_sc1_lcye,
              rt.amt_sc1_tcye,
              rt.amt_sc1_acye,
              rt.flg_sc1_cur,
              rt.amt_sc2_lcye,
              rt.amt_sc2_tcye,
              rt.amt_sc2_acye,
              rt.flg_sc2_cur,
              rt.amt_sc3_lcye,
              rt.amt_sc3_tcye,
              rt.amt_sc3_acye,
              rt.flg_sc3_cur,
              rt.amt_sc4_lcye,
              rt.amt_sc4_tcye,
              rt.amt_sc4_acye,
              rt.flg_sc4_cur,
              rt.amt_sc5_lcye,
              rt.amt_sc5_tcye,
              rt.amt_sc5_acye,
              rt.flg_sc5_cur,
              rt.flg_ccy_rat,
              rt.id_tran_seq_old,
              rt.buf_stat_scr,
              rt.buf_tran_scr1,
              rt.buf_tran_scr2,
              rt.buf_tran_scr3,
              rt.buf_tran_scr4,
              rt.cod_insr_typ,
              rt.cod_fnd_typ,
              rt.id_rte_trns,
              rt.id_insr,
              rt.dat_insr,
              rt.dat_val_insr,
              rt.cod_insr_cur,
              rt.amt_insr,
              rt.flg_txn_status,
              rt.cod_msg_typ,
              rt.flg_vouch_print,
              rt.id_auth,
              rt.auth_no,
              rt.cod_issuer,
              rt.cod_txn_lit,
              rt.cod_cust_id,
              rt.flg_pin_pad,
              rt.ref_card_no,
              rt.session_id
       FROM rpt_rec_txnlog@rptdb01_link rt
       WHERE trunc(rt.tim_sys) = trunc(p_date);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

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

  PROCEDURE refresh_xt_for_date(p_date DATE)
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_xt_for_date';
    v_params VARCHAR2(100) := 'p_date = ' || to_char(p_date, 'yyyy.mm.dd');
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM xf_ol_st_txnlog_mmdd xt
       WHERE xt.dat_log = TRUNC(p_date);
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO xf_ol_st_txnlog_mmdd
       SELECT xt.cod_cc_brn,
              xt.cod_prod,
              xt.cod_userno,
              TRIM(xt.cod_acct_no) cod_acct_no,
              xt.cod_txn_ccy,
              xt.cod_acc_ccy,
              xt.cod_txn_mnemonic,
              xt.cod_msg_typ,
              xt.cod_user_id,
              xt.cod_auth_id,
              xt.cod_cust,
              xt.ctr_batch_no,
              xt.dat_txn_str,
              xt.dat_value_str,
              xt.dat_post_str,
              xt.dat_log,
              xt.amt_txn_tcy,
              xt.amt_txn_acy,
              xt.amt_txn_lcy,
              xt.rat_conv_tclcy,
              xt.rat_conv_aclcy,
              TRIM(xt.txn_nrrtv) txn_nrrtv,
              xt.flg_drcr,
              TRIM(xt.ref_sys_tr_aud_no) ref_sys_tr_aud_no,
              TRIM(xt.ref_instr_no_stno) ref_instr_no_stno,
              TRIM(xt.ref_txn_no) ref_txn_no,
              TRIM(xt.ref_usr_no) ref_usr_no,
              xt.ref_subseq_no
       FROM xf_ol_st_txnlog_mmdd@rptdb01_link xt
       WHERE xt.dat_log = TRUNC(p_date);
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

  PROCEDURE refresh_actb_accbal_hist
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_actb_accbal_hist';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM actb_accbal_history;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO actb_accbal_history
       SELECT aah.branch_code,
              TRIM(aah.account) account,
              aah.bkg_date,
              aah.acy_opening_bal,
              aah.lcy_opening_bal,
              aah.acy_closing_bal,
              aah.lcy_closing_bal,
              aah.acy_dr_tur,
              aah.lcy_dr_tur,
              aah.acy_cr_tur,
              aah.lcy_cr_tur,
              aah.acc_ccy
       FROM actb_accbal_history@rptdb01_link aah;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ac_acct_crr_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_acct_crr_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_acct_crr_code;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ac_acct_crr_code
       SELECT TRIM(aac.cod_acct_no) cod_acct_no,
              aac.cod_cc_brn,
              aac.cod_prod,
              aac.cod_crr_cust,
              aac.cod_cust_id,
              aac.cod_crr_from,
              aac.cod_crr_to,
              aac.dat_process,
              aac.dat_last_mnt
       FROM ac_acct_crr_code@rptdb01_link aac;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ac_acct_writeoff
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_acct_writeoff';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_acct_writeoff;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ac_acct_writeoff
       SELECT TRIM(wo.cod_acct_no) cod_acct_no,
              wo.amt_writeoff_princ_bal,
              wo.amt_writeoff_int,
							wo.amt_writeoff_susp_int,
							wo.amt_writeoff_susp_pen_int,
              wo.flg_writeoff_type,
              wo.ctr_batch_no,
              wo.cod_user_id,
              wo.cod_auth_id,
              wo.dat_writeoff,
              wo.dat_last_mnt
       FROM ac_acct_writeoff@rptdb01_link wo;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ac_crr_codes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_crr_codes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_crr_codes;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ac_crr_codes
       SELECT ac.cod_crr,
              TRIM(ac.txt_crr_desc) txt_crr_desc,
              ac.flg_accr_status,
              ac.dat_last_mnt
       FROM ac_crr_codes@rptdb01_link ac;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_acct_status
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_acct_status';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_acct_status;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_acct_status
       SELECT ast.cod_module,
              ast.cod_acct_status,
              TRIM(ast.txt_acct_status) txt_acct_status,
              ast.dat_last_mnt
       FROM ba_acct_status@rptdb01_link ast;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_bank_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_bank_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_bank_mast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_bank_mast
       SELECT b.cod_bank,
              b.nam_bank,
              b.nam_bank_shrt,
              b.txt_bank_add1,
              b.nam_bank_city,
              b.nam_bank_state,
              b.nam_bank_country,
              b.txt_bank_zip,
              b.cod_bank_central,
              b.cod_bank_lcy,
              b.dat_process,
              b.dat_last_process,
              b.dat_next_process
       FROM ba_bank_mast@rptdb01_link b;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_brn_type_master
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_brn_type_master';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_brn_type_master;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_brn_type_master
       SELECT bt.cod_brn_type,
              bt.cod_brn_name,
              bt.dat_last_mnt
       FROM ba_brn_type_master@rptdb01_link bt;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_ccy_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_ccy_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_ccy_code;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_ccy_code
       SELECT c.cod_ccy,
              TRIM(c.nam_ccy_short) nam_ccy_short,
              TRIM(c.nam_currency) nam_currency,
              c.dat_last_mnt
       FROM ba_ccy_code@rptdb01_link c;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_ccy_rate
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_ccy_rate';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_ccy_rate;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_ccy_rate
       SELECT cr.cod_ccy,
              cr.rat_ccy_cb,
              cr.dat_tim_rate_eff,
              cr.dat_last_mnt
       FROM ba_ccy_rate@rptdb01_link cr;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_cc_brn_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_cc_brn_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_cc_brn_mast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_cc_brn_mast
       SELECT b.cod_cc_brn,
              b.cod_main_branch,
              b.cod_brn_type,
              TRIM(b.nam_branch) nam_branch,
              TRIM(b.nam_branch_shrt) nam_branch_shrt,
              TRIM(b.nam_cc_city) nam_cc_city,
              b.dat_last_mnt
       FROM ba_cc_brn_mast@rptdb01_link b;
       --WHERE b.flg_mnt_status = 'A';
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_coll_auto
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_auto';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_automobile;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_automobile
       SELECT bca.cod_collat_id,
              bca.cod_regn_no
       FROM ba_coll_automobile@rptdb01_link bca;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_coll_codes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_codes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_codes;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_codes
       SELECT cc.cod_coll,
              cc.cod_coll_type,
              TRIM(cc.nam_coll) nam_coll,
              cc.dat_last_mnt
       FROM ba_coll_codes@rptdb01_link cc;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_coll_hdr
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_hdr';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_hdr;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_hdr
       SELECT ch.cod_collat_id,
              ch.cod_coll,
              TRIM(ch.txt_deed_detl_1) txt_deed_detl_1,
              ch.cod_coll_homebrn,
              ch.cod_ccy,
              ch.amt_last_val,
              ch.amt_orig_value,
              ch.amt_market_val,
              ch.nam_lender,
              ch.cod_coll_class,
              ch.dat_last_mnt
       FROM ba_coll_hdr@rptdb01_link ch;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_coll_prop
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_prop';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_prop;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_prop
       SELECT TRIM(cp.cod_collat_id) cod_collat_id,
              cp.cod_coll,
              cp.amt_cost_price,
              cp.amt_forced_sale,
              cp.amt_quit_rent,
              cp.cod_area_unit,
              cp.dat_lease_expiry,
              cp.flt_area,
              TRIM(cp.txt_collat_id) txt_collat_id,
              TRIM(cp.txt_desc1) txt_desc1,
              TRIM(cp.txt_desc2) txt_desc2,
              cp.dat_last_mnt
       FROM ba_coll_prop@rptdb01_link cp;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_group_details
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_group_details';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_group_details;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_group_details
       SELECT gd.cod_group,
              gd.cod_group_cycle,
              TRIM(gd.nam_group) nam_group,
              gd.cod_branch_group,
              gd.cod_crofficer_id,
              TRIM(gd.cod_acct_no) cod_acct_no,
              gd.cod_cust,
              gd.cod_prod,
              gd.cod_application_id,
              TRIM(gd.nam_product) nam_product,
              gd.cod_sched_type,
              gd.nam_sched_type,
              gd.dat_group_created,
              gd.dat_acct_added,
              gd.flg_group_acc_attr,
              gd.ctr_stage_no,
              gd.ctr_term,
              gd.rat_interest_variance,
              gd.dat_last_mnt
       FROM ba_group_details@rptdb01_link gd;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_lo_coll_acct_xr
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_lo_coll_acct_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_lo_coll_acct_xref;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_lo_coll_acct_xref
       SELECT ca.cod_collat_id,
              TRIM(ca.cod_acct_no) cod_acct_no,
              ca.cod_coll,
              ca.cod_prod,
              ca.cod_cc_brn,
              ca.amt_coll_value,
              ca.dat_last_mnt
       FROM ba_lo_coll_acct_xref@rptdb01_link ca;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_sc_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_sc_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_sc_code;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_sc_code
       SELECT sc.cod_sc,
              TRIM(sc.nam_sc) nam_sc,
              sc.cod_ccy,
              sc.rat_sc_txn_pct,
              sc.amt_sc_fixed,
              sc.amt_sc_min,
              sc.amt_sc_max,
              sc.dat_last_mnt
       FROM ba_sc_code@rptdb01_link sc;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_txn_mis_xref
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_txn_mis_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_cust_prod_txn_mis_xref;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_cust_prod_txn_mis_xref
       SELECT m.flg_mis_type,
              TRIM(m.cod_prod_mnem_cust) cod_prod_mnem_cust,
              m.cod_mis_cust_2,
              m.cod_mis_txn_3,
              m.dat_last_mnt
       FROM ba_cust_prod_txn_mis_xref@rptdb01_link  m;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ba_txn_mnemonic
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_txn_mnemonic';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_txn_mnemonic;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_txn_mnemonic
       SELECT tm.cod_txn_mnemonic,
              tm.cod_drcr,
              tm.cod_txn_type,
              tm.cod_txn_mode,
              TRIM(tm.txt_txn_desc) txt_txn_desc,
              tm.dat_last_mnt
       FROM ba_txn_mnemonic@rptdb01_link tm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ch_acct_cust_xref
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ch_acct_cust_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ch_acct_cust_xref;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ch_acct_cust_xref
       SELECT TRIM(cx.cod_acct_no) cod_acct_no,
              cx.cod_cust,
              cx.cod_acct_cust_rel,
              cx.dat_last_mnt
       FROM ch_acct_cust_xref@rptdb01_link cx;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_business_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_business_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_business_types;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_business_types
       SELECT bt.cod_business_cat,
              TRIM(bt.txt_business) txt_business,
              bt.dat_last_mnt
       FROM ci_business_types@rptdb01_link bt;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_custdetl
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custdetl';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custdetl;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_custdetl
       SELECT cd.cod_cust_id,
              TRIM(cd.nam_cust_emp) nam_cust_emp,
              TRIM(cd.nam_spouse_cust) nam_spouse_cust,
              TRIM(cd.nam_empadr_cntry) nam_empadr_cntry,
              TRIM(cd.nam_empadr_city) nam_empadr_city,
              TRIM(cd.txt_empadr_add1) txt_empadr_add1,
              TRIM(cd.txt_empadr_add2) txt_empadr_add2,
              TRIM(cd.txt_empadr_add3) txt_empadr_add3,
              cd.ref_cust_emp_phone_country,
              cd.ref_cust_emp_phone_area,
              cd.ref_cust_emp_phone,
              cd.ref_cust_pspt,
              cd.ctr_yrs_job,
              cd.dat_last_mnt
       FROM ci_custdetl@rptdb01_link cd;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_custmast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custmast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custmast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_custmast
       SELECT c.cod_cust_id,
              TRIM(c.cod_officr_id) cod_officr_id,
              c.cod_cc_homebrn,
              c.cod_cust_natl_id,
              c.cod_business_cat,
              c.cod_cust_stat,
              c.cod_cust_marstat,
              TRIM(c.cod_employee_id) cod_employee_id,
              TRIM(c.nam_cust_full) nam_cust_full,
              TRIM(c.nam_cust_first) nam_cust_first,
              TRIM(c.nam_cust_mid) nam_cust_mid,
              TRIM(c.nam_cust_last) nam_cust_last,
              TRIM(c.nam_cust_shrt) nam_cust_shrt,
              TRIM(c.nam_custadr_cntry) nam_custadr_cntry,
              TRIM(c.nam_custadr_city) nam_custadr_city,
              TRIM(c.nam_custadr_state) nam_custadr_state,
              TRIM(c.txt_custadr_add1) txt_custadr_add1,
              TRIM(c.txt_custadr_add2) txt_custadr_add2,
              TRIM(c.txt_custadr_add3) txt_custadr_add3,
              TRIM(c.nam_permadr_cntry) nam_permadr_cntry,
              TRIM(c.nam_permadr_city) nam_permadr_city,
              TRIM(c.nam_permadr_state) nam_permadr_state,
              TRIM(c.txt_permadr_add1) txt_permadr_add1,
              TRIM(c.txt_permadr_add2) txt_permadr_add2,
              TRIM(c.txt_permadr_add3) txt_permadr_add3,
              TRIM(c.txt_permadr_zip) txt_permadr_zip,
              c.ref_res_phone_country,
              c.ref_res_phone_area,
              c.ref_cust_phone,
              c.ref_cust_phone_off,
              c.ref_cust_telex,
              c.ref_phone_mobile,
              c.ref_office_phone_area,
              c.flg_cust_typ,
              c.flg_ic_typ,
              c.txt_cust_sex,
              TRIM(c.txt_cust_desgn) txt_cust_desgn,
              TRIM(c.txt_cust_educn) txt_cust_educn,
              c.dat_birth_cust,
              c.dat_cust_open,
              TRIM(c.cod_last_mnt_makerid) cod_last_mnt_makerid,
              TRIM(c.cod_mis_cust_code_3) cod_mis_cust_code_3,
              c.ctr_cust_spouses,
              c.dat_last_mnt
       FROM ci_custmast@rptdb01_link c;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_custrel
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custrel';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custrel;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_custrel
       SELECT cr.cod_cust_p,
              cr.cod_cust_s,
              cr.cod_rel,
              cr.cod_inv_rel,
              cr.dat_last_mnt
       FROM ci_custrel@rptdb01_link cr;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_cust_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_cust_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_cust_types;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_cust_types
       SELECT ct.flg_cust_typ,
              TRIM(ct.txt_cust_typ) txt_cust_typ,
              ct.dat_last_mnt
       FROM ci_cust_types@rptdb01_link ct;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_ic_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_ic_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_ic_types;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_ic_types
       SELECT cit.flg_ic_typ,
              TRIM(cit.txt_ic_typ) txt_ic_typ,
              cit.dat_last_mnt
       FROM ci_ic_types@rptdb01_link cit;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ci_relmast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_relmast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_relmast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_relmast
       SELECT rm.cod_rel,
              TRIM(rm.txt_relation) txt_relation,
              rm.dat_last_mnt
       FROM ci_relmast@rptdb01_link rm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_conv_ci_custmast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_conv_ci_custmast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM conv_ci_custmast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO conv_ci_custmast
       SELECT cc.cod_cust_id,
              cc.dat_cust_open,
              cc.cod_bank_card,
              cc.flg_cust_typ,
              cc.flg_staff,
              cc.flg_loc_glob,
              cc.cod_cc_homebrn,
              cc.cod_officr_id,
              cc.nam_cust_shrt,
              cc.txt_cust_prefix,
              cc.nam_cust_full,
              cc.nam_cust_last,
              cc.nam_cust_mid,
              cc.nam_cust_first,
              cc.cod_cust_phonetic,
              cc.cod_mid_phonetic,
              cc.cod_first_phonetic,
              cc.flg_member,
              cc.cod_member_no,
              cc.cod_cust_stat,
              cc.txt_cust_desgn,
              cc.flg_ic_typ,
              cc.cod_cust_natl_id,
              cc.cod_natl_id_old,
              cc.cod_cust_lang,
              cc.cod_income_cat,
              cc.cod_sign_typ,
              cc.txt_profess_cat,
              cc.cod_tds,
              cc.ref_cust_it_num,
              cc.txt_custadr_add1,
              cc.txt_custadr_add2,
              cc.txt_custadr_add3,
              cc.nam_custadr_city,
              cc.nam_custadr_state,
              cc.nam_custadr_cntry,
              cc.txt_custadr_zip,
              cc.ref_cust_phone,
              cc.ref_cust_phone_off,
              cc.ref_cust_fax,
              cc.ref_cust_telex,
              cc.ref_cust_email,
              cc.txt_permadr_add1,
              cc.txt_permadr_add2,
              cc.txt_permadr_add3,
              cc.nam_permadr_city,
              cc.nam_permadr_cntry,
              cc.nam_permadr_state,
              cc.txt_permadr_zip,
              cc.dat_birth_cust,
              cc.txt_cust_sex,
              cc.cod_cust_marstat,
              cc.ctr_cust_spouses,
              cc.nam_cust_spouse,
              cc.txt_spouse_occpn,
              cc.ctr_dpndt_chldrn,
              cc.ctr_dpndt_othrs,
              cc.cod_typ_accom,
              cc.txt_lgl_rsdnc_stat,
              cc.txt_ethnic_origin,
              cc.txt_cust_natnlty,
              cc.cod_cust_bldgrp,
              cc.txt_cust_educn,
              cc.flg_cust_memo,
              cc.dat_incorporated,
              cc.nam_cntry_incorp,
              cc.cod_business_regno,
              cc.cod_business_cat,
              cc.txt_business_typ,
              cc.nam_signatory1,
              cc.nam_signatory2,
              cc.nam_signatory3,
              cc.nam_signatory4,
              cc.nam_signatory5,
              cc.txt_desgn_signtry1,
              cc.txt_desgn_signtry2,
              cc.txt_desgn_signtry3,
              cc.txt_desgn_signtry4,
              cc.txt_desgn_signtry5,
              cc.nam_director1,
              cc.nam_director2,
              cc.nam_director3,
              cc.nam_director4,
              cc.nam_director5,
              cc.cod_report1,
              cc.cod_report2,
              cc.cod_report3,
              cc.flg_mnt_status,
              cc.cod_mnt_action,
              cc.cod_last_mnt_makerid,
              cc.cod_last_mnt_chkrid,
              cc.dat_last_mnt,
              cc.ctr_updat_srlno,
              cc.flg_key_chg,
              cc.flg_cust_status,
              cc.flg_replicate,
              cc.cod_liab_no,
              cc.credit_rating,
              cc.dat_revision,
              cc.fx_cust_clean_risk_limit,
              cc.fx_clean_risk_limit,
              cc.amt_limit_tot,
              cc.cod_ccy_limit,
              cc.sec_cust_clean_risk_limit,
              cc.sec_clean_risk_limit,
              cc.sec_cust_pstl_risk_limit,
              cc.sec_pstl_risk_limit,
              cc.cod_cust_introducer_id,
              cc.cod_employee_id,
              cc.ctr_mis_codes,
              cc.cod_mis_cust_code_1,
              cc.cod_mis_cust_code_2,
              cc.cod_mis_cust_code_3,
              cc.cod_mis_cust_code_4,
              cc.cod_mis_cust_code_5,
              cc.cod_mis_cust_code_6,
              cc.cod_mis_cust_code_7,
              cc.cod_mis_cust_code_8,
              cc.cod_mis_cust_code_9,
              cc.cod_mis_cust_code_10,
              cc.cod_mis_comp_code_1,
              cc.cod_mis_comp_code_2,
              cc.cod_mis_comp_code_3,
              cc.cod_mis_comp_code_4,
              cc.cod_mis_comp_code_5,
              cc.cod_mis_comp_code_6,
              cc.cod_mis_comp_code_7,
              cc.cod_mis_comp_code_8,
              cc.cod_mis_comp_code_9,
              cc.cod_mis_comp_code_10,
              cc.flg_minor,
              cc.cod_guardian_type,
              cc.cod_acct_no_default,
              cc.txt_cust_residence,
              cc.afl_issue_date,
              cc.afl_expiry_date,
              cc.afl_effective_date,
              cc.last_renew_date,
              cc.flg_unadv,
              cc.flg_doc_complete,
              cc.flg_kio,
              cc.txt_holdadr_add1,
              cc.txt_holdadr_add2,
              cc.txt_holdadr_add3,
              cc.nam_holdadr_city,
              cc.nam_holdadr_state,
              cc.nam_holdadr_cntry,
              cc.txt_holdadr_zip,
              cc.ref_hold_phone,
              cc.ref_hold_phone_off,
              cc.ref_hold_fax,
              cc.ref_hold_telex,
              cc.ref_hold_email,
              cc.cod_cust_status,
              cc.cod_tds_2,
              cc.ref_res_phone_country,
              cc.ref_res_phone_area,
              cc.ref_res_phone_extn,
              cc.ref_phone_mobile,
              cc.ref_office_phone_country,
              cc.ref_office_phone_area,
              cc.ref_office_phone_extn
       FROM conv_ci_custmast@rptdb01_link cc;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_cs_ho_blacklist
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_cs_ho_blacklist';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM cs_ho_blacklist_info;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO cs_ho_blacklist_info
       SELECT bc.cod_cust_id,
              TRIM(bc.nam_cust_full) nam_cust_full,
              bc.cod_cust_natl_id,
              bc.cod_agency,
              bc.blacklst_dat_from,
              bc.blacklst_dat_to,
              TRIM(bc.txt_blacklst_desc1) txt_blacklst_desc1,
              bc.cod_last_mnt_makerid,
              bc.cod_last_mnt_chkrid,
              bc.dat_last_mnt
       FROM cs_ho_blacklist_info@rptdb01_link bc;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_cs_prop_log
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_cs_prop_log';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM cs_propagate_log;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO cs_propagate_log
       SELECT pt.cod_change,
              pt.cod_cust_id,
              TRIM(pt.prop_val1_old) prop_val1_old,
              TRIM(pt.prop_val1_new) prop_val1_new,
              pt.prop_val2_old,
              pt.prop_val2_new,
              TRIM(pt.prop_val3_old) prop_val3_old,
              TRIM(pt.prop_val3_new) prop_val3_new,
              pt.cod_cc_brn,
              pt.cod_last_mnt_makerid,
              pt.cod_last_mnt_chkrid,
              pt.dat_last_mnt,
              pt.timestamp
       FROM cs_propagate_log@rptdb01_link pt;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_gltm_glmaster
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gltm_glmaster';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gltm_glmaster;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO gltm_glmaster
       SELECT TRIM(gg.gl_code) gl_code,
              TRIM(gg.gl_desc) gl_desc,
              TRIM(gg.parent_gl) parent_gl,
              gg.leaf,
              gg.type,
              gg.category,
              gg.ulti_parent,
              gg.dat_last_mnt
       FROM gltm_glmaster@rptdb01_link gg;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_gl_detemp
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gl_detemp';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gl_detemp;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO gl_detemp
       SELECT TRIM(gd.cod_acct_no) cod_acct_no,
              gd.amt_txn_fcy,
              gd.amt_txn_lcy,
              gd.dat_value,
              gd.dat_posting,
              gd.dat_txn,
              TRIM(gd.txt_txn_desc) txt_txn_desc,
              gd.cod_drcr,
              gd.ctr_batch_no,
              gd.ref_chq_no,
              gd.cod_userno,
              gd.cod_checker,
              gd.cod_cc_brn,
              gd.cod_txn_ccy,
              gd.cod_txn_mnemonic
       FROM gl_detemp@rptdb01_link gd;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_gl_master
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gl_master';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gl_master;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO gl_master
       SELECT gm.cod_cc_brn,
              gm.cod_gl_acct,
              gm.cod_gl_acct_ccy,
              gm.cod_gl_type,
              TRIM(gm.nam_gl_code) nam_gl_code,
              gm.dat_acct_opened,
              gm.dat_txn_posting,
              gm.dat_last_mnt
       FROM gl_master@rptdb01_link gm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_attributes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_attributes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_attributes;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_attributes
       SELECT TRIM(aa.cod_acct_no) cod_acct_no,
              aa.cod_acct_off,
              aa.flg_include_to_date,
              aa.flg_negotiated,
              aa.flg_legal
       FROM ln_acct_attributes@rptdb01_link aa;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_balances
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_balances';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_balances;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_balances
       SELECT TRIM(ab.cod_acct_no) cod_acct_no,
              ab.cod_cc_brn,
              ab.cod_prod,
              ab.cod_ccy,
              ab.amt_princ_balance,
              ab.amt_rpa_balance,
              ab.amt_arrears_princ,
              ab.amt_arrears_fees,
              ab.amt_arrears_legal,
              ab.amt_arrears_outgoing,
              ab.amt_arrears_prem,
              ab.amt_arrears_susp_fees,
              ab.amt_arrears_susp_legal,
              ab.amt_arrears_susp_outgoing,
              ab.amt_arrears_susp_prem,
              ab.amt_disbursed,
              ab.amt_net_disbursed
       FROM ln_acct_balances@rptdb01_link ab;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_cbr_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_cbr_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_cbr_dtls;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_cbr_dtls
       SELECT TRIM(lac.cod_acct_no) cod_acct_no,
              lac.num_1,
              lac.cod_last_mnt_chkrid,
              lac.cod_last_mnt_makerid,
              lac.dat_last_mnt
       FROM ln_acct_cbr_dtls@rptdb01_link lac;
       v_row_count_processed := v_row_count_processed || '; ' || TRIM(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_dtls;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_dtls
       SELECT TRIM(a.cod_acct_no) cod_acct_no,
              a.dat_acct_open,
              a.dat_first_disb,
              a.dat_last_disb,
              a.dat_acct_close,
              a.dat_of_maturity,
              a.dat_last_payment,
              a.cod_cc_brn,
              a.cod_prod,
              a.cod_ccy,
              a.cod_cust_id,
              a.cod_acct_stat,
              a.cod_last_mnt_makerid,
              a.cod_last_mnt_chkrid,
              a.amt_face_value,
              a.amt_paid_today,
              a.rat_var_current,
              a.ctr_term_months,
              a.dat_last_mnt
       FROM ln_acct_dtls@rptdb01_link a;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_int_bal_dt
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_int_bal_dt';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_int_balance_dtls;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_int_balance_dtls
       SELECT TRIM(bd.cod_acct_no) cod_acct_no,
              bd.cod_cc_brn,
              bd.cod_prod,
              bd.cod_ccy,
              bd.bal_int1_arrears,
              bd.bal_int2_arrears,
              bd.bal_int3_arrears,
              bd.bal_int4_arrears,
              bd.bal_int5_arrears,
              bd.bal_int6_arrears,
              bd.bal_int7_arrears,
              bd.bal_int8_arrears,
              bd.bal_int9_arrears,
              bd.bal_int10_arrears,
              bd.bal_int11_arrears,
              bd.bal_int12_arrears,
              bd.bal_int13_arrears,
              bd.bal_int14_arrears,
              bd.bal_int15_arrears,
              bd.bal_int16_arrears,
              bd.bal_int1_arrears_susp,
              bd.bal_int2_arrears_susp,
              bd.bal_int3_arrears_susp,
              bd.bal_int4_arrears_susp,
              bd.bal_int5_arrears_susp,
              bd.bal_int6_arrears_susp,
              bd.bal_int7_arrears_susp,
              bd.bal_int8_arrears_susp,
              bd.bal_int9_arrears_susp,
              bd.bal_int10_arrears_susp,
              bd.bal_int11_arrears_susp,
              bd.bal_int12_arrears_susp,
              bd.bal_int13_arrears_susp,
              bd.bal_int14_arrears_susp,
              bd.bal_int15_arrears_susp,
              bd.bal_int16_arrears_susp
       FROM ln_acct_int_balance_dtls@rptdb01_link bd;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_sched_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_sched_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_schedule_detls;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls
       SELECT TRIM(s.cod_acct_no) cod_acct_no,
              s.dat_stage_start,
              s.ctr_instal,
              s.date_instal,
							s.rat_int,
              s.amt_principal,
              s.amt_interest,
              s.amt_princ_bal
       FROM ln_acct_schedule_detls@rptdb01_link s;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_acct_sdh
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_sdh';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_schedule_detls_hist;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls_hist
       SELECT TRIM(dh.cod_acct_no) cod_acct_no,
              dh.dat_stage_start,
              dh.dat_amd,
              dh.ctr_instal,
              dh.date_instal,
              dh.amt_principal,
              dh.amt_interest,
              dh.amt_princ_bal
       FROM ln_acct_schedule_detls_hist@rptdb01_link dh;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_arrear_txn_hist
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_arrear_txn_hist';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrear_txn_hist;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_arrear_txn_hist
       SELECT TRIM(th.cod_acct_no) cod_acct_no,
              th.ref_billno_srl,
              th.cod_arrear_type,
              th.dat_post,
              th.dat_value,
              th.dat_txn,
              th.dat_process,
              th.cod_acc_ccy,
              th.amt_txn_acy,
              th.cod_drcr,
              th.cod_txn_mnemonic
       FROM ln_arrear_txn_hist@rptdb01_link th;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_arrears_table
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_arrears_table';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrears_table;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_arrears_table
       SELECT TRIM(la.cod_acct_no) cod_acct_no,
              la.ref_billno_srl,
              la.cod_arrear_type,
              la.cod_charge_ccy,
              la.amt_arrears_assessed,
              la.amt_arrears_due,
              la.dat_arrears_due,
              la.dat_last_payment,
              la.dat_arrear_cancelled
       FROM ln_arrears_table@rptdb01_link la;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_dedn_plan
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_dedn_plan';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_dedn_plan;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_dedn_plan
       SELECT dp.cod_plan,
              dp.cod_deduction,
              dp.cod_dedn_ccy,
              TRIM(dp.nam_plan) nam_plan,
              dp.dat_last_mnt
       FROM ln_dedn_plan@rptdb01_link dp;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_prod_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_prod_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_prod_mast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_prod_mast
       SELECT pm.cod_prod,
              pm.cod_ccy,
              pm.cod_prod_term_typ,
              pm.cod_dedn_plan,
              pm.cod_class_prod,
              TRIM(pm.nam_product) nam_product,
              TRIM(pm.nam_ccy_short) nam_ccy_short,
              pm.cod_gl_ast_bal,
              pm.cod_gl_susp_ast_bal,
              pm.dat_last_mnt
       FROM ln_prod_mast@rptdb01_link pm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_ln_sched_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_sched_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_sched_types;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_sched_types
       SELECT s.cod_sched_type,
              s.cod_prod,
              s.ctr_stage_no,
              TRIM(s.nam_sched_type) nam_sched_type,
              s.dat_last_mnt
       FROM ln_sched_types@rptdb01_link s;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_applicants
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applicants';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applicants;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applicants
       SELECT a.apl_aad_id,
              a.apl_cus_id,
              a.apl_relation_type_cd,
              a.apl_gender_cd,
              TRIM(a.apl_first_name) apl_first_name,
              TRIM(a.apl_middle_name) apl_middle_name,
              TRIM(a.apl_last_name) apl_last_name,
              a.dat_last_mnt
       FROM or_applicants@rptdb01_link a;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_applicants_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applicants_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applicants_mmdd;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applicants_mmdd
       SELECT a.apl_aad_id,
              a.apl_cus_id,
              a.apl_relation_type_cd,
              a.apl_gender_cd,
              TRIM(a.apl_first_name) apl_first_name,
              TRIM(a.apl_middle_name) apl_middle_name,
              TRIM(a.apl_last_name) apl_last_name,
              a.dat_last_mnt
       FROM or_applicants_mmdd@rptdb01_link a;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_applications
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applications';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applications;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applications
       SELECT ap.app_aad_id,
              ap.app_dt,
              ap.app_pcb_branch,
              ap.app_nbr,
              TRIM(ap.app_acc_nbr) app_acc_nbr,
              ap.app_prd_product,
              ap.app_prd_ccy,
              ap.app_req_adv_amt,
              ap.app_req_term,
              ap.app_req_rate,
              ap.app_status_cd,
              ap.app_sub_status_cd,
              ap.app_sch_type_cd,
              ap.app_purpose_cd,
              ap.app_underwriter_usr_code,
              ap.app_entered_by,
              ap.app_decision_dt
       FROM or_applications@rptdb01_link ap;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_applications_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applications_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applications_mmdd;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applications_mmdd
       SELECT ap.app_aad_id,
              ap.app_dt,
              ap.app_pcb_branch,
              ap.app_nbr,
              TRIM(ap.app_acc_nbr) app_acc_nbr,
              ap.app_prd_product,
              ap.app_prd_ccy,
              ap.app_req_adv_amt,
              ap.app_req_term,
              ap.app_req_rate,
              ap.app_status_cd,
              ap.app_sub_status_cd,
              ap.app_sch_type_cd,
              ap.app_purpose_cd,
              ap.app_underwriter_usr_code,
              ap.app_entered_by,
              ap.app_decision_dt
       FROM or_applications_mmdd@rptdb01_link ap;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_app_assets
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_app_assets';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_application_assets;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_application_assets
       SELECT aa.ase_aad_id,
              TRIM(aa.ase_identification_nbr) ase_identification_nbr,
              aa.ase_asb_type,
              aa.dat_last_mnt
       FROM or_application_assets@rptdb01_link aa;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_app_assets_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_app_assets_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_application_assets_mmdd;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_application_assets_mmdd
       SELECT aa.ase_aad_id,
              TRIM(aa.ase_identification_nbr) ase_identification_nbr,
              aa.ase_asb_type,
              aa.dat_last_mnt
       FROM or_application_assets_mmdd@rptdb01_link aa;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_or_lookups
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_lookups';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_lookups;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_lookups
       SELECT l.lkc_lkt_type,
              l.lkc_code,
              l.lkc_sub_code,
              l.lkc_sort,
              l.lkc_desc,
              l.dat_last_mnt
       FROM or_lookups@rptdb01_link l;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_sm_temp_profile
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_sm_temp_profile';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM sm_temp_profile;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO sm_temp_profile
       SELECT tm.cod_user_templ,
              tm.cod_branch,
              tm.cod_category_user,
              tm.cod_user_level,
              tm.cod_user_access,
              TRIM(tm.nam_template) nam_template,
              tm.dat_last_mnt
       FROM sm_temp_profile@rptdb01_link tm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_sm_user_profile
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_sm_user_profile';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM sm_user_profile;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO sm_user_profile
       SELECT up.cod_userno,
              up.cod_user_id,
              up.cod_cc_brn,
              up.cod_lang,
              TRIM(up.nam_user) nam_user,
              up.dat_last_sign_on,
              up.cod_employee_id,
              up.cod_branch_templ,
              up.dat_prof_st,
              up.dat_prof_end,
              up.dat_vac_st,
              up.dat_vac_end,
              up.flg_status,
              up.flg_login_state,
              TRIM(up.cod_term_id) cod_term_id,
              up.dat_last_mnt
       FROM sm_user_profile@rptdb01_link up;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_udf_acct_log_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_acct_log_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_acct_log_details;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_acct_log_details
       SELECT TRIM(ual.cod_acct_no) cod_acct_no,
              ual.cod_field_tag,
              TRIM(ual.field_value) field_value,
              ual.ctr_udf_order
       FROM udf_acct_log_details@rptdb01_link ual;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_udf_cust_log_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_cust_log_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_cust_log_details;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_cust_log_details
       SELECT cld.cod_cust_id,
              cld.cod_field_tag,
              TRIM(cld.field_value) field_value,
              cld.ctr_udf_order
       FROM udf_cust_log_details@rptdb01_link cld;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_udf_lov_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_lov_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_lov_mast;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_lov_mast
       SELECT ulm.cod_field_tag,
              ulm.ctr_lov_order,
              TRIM(ulm.txt_lov_value) txt_lov_value,
              TRIM(ulm.txt_lov_name) txt_lov_name
       FROM udf_lov_mast@rptdb01_link ulm;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG',
                           v_proc_name,
                           NULL,
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
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  --rpt tables
  --------------------------------------------------------------------------------------------

  PROCEDURE refresh_rpt_add_txn
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_add_txn';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_additional_txn;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_additional_txn
       SELECT ra.txn_id,
              ra.txn_date,
              ra.table_name,
              ra.for_date,
              ra.txn_type,
              ra.txn_script
       FROM rpt_additional_txn@rptdb01_link ra;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_other_banks
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_other_banks';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_other_banks;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_other_banks
       SELECT bb.bank_id,
              bb.bank_name,
              bb.bank_branch_id,
              bb.bank_branch_name,
              bb.bank_voen,
              bb.bank_code,
              bb.bank_corresp_acc,
              bb.bank_swift,
              bb.bank_acy,
              bb.bank_account,
              bb.voin_pointer,
              bb.bm_name,
              bb.emittent,
              bb.bm_gender
       FROM rpt_other_banks@rptdb01_link bb;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_bme
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_bme';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_branch_manager_emails;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_branch_manager_emails
       SELECT bme.branch_id,
              bme.bm_email,
              bme.bm_cc_email,
              bme.rr_email,
              bme.sv_email
       FROM rpt_branch_manager_emails@rptdb01_link bme;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_co_bnk_rel
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_co_bnk_rel';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_co_bank_relations;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_co_bank_relations
       SELECT cbr.user_no,
              cbr.employee_id,
              cbr.bank_id,
              cbr.bank_branch_id
       FROM rpt_co_bank_relations@rptdb01_link cbr;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_co_cs_rel
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_co_cs_rel';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_co_cs_relations ccr
       WHERE (ccr.co_user_no, ccr.cs_user_no) NOT IN (SELECT DISTINCT cr.co_user_no,
                                                             cr.cs_user_no
                                                      FROM rpt_co_cs_relations@rptdb01_link cr);
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_co_cs_relations
       SELECT ccr.co_user_no,
              ccr.cs_user_no,
              ccr.inserted,
              ccr.updated
       FROM rpt_co_cs_relations@rptdb01_link ccr
       WHERE (ccr.co_user_no, ccr.cs_user_no) NOT IN (SELECT DISTINCT cr.co_user_no,
                                                             cr.cs_user_no
                                                      FROM rpt_co_cs_relations cr);
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_co_levels
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_co_levels';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_co_levels;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_co_levels
       SELECT cl.level_date,
              cl.employee_id,
              cl.base_salary,
              cl.co_level
       FROM rpt_co_levels@rptdb01_link cl;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_gl_rpt_ln
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_gl_rpt_ln';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_gl_report_lines;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_gl_report_lines
       SELECT rl.gl_account_no,
              rl.ccy,
              rl.line_desc,
              rl.pl_code,
              rl.tree_code
       FROM rpt_gl_report_lines@rptdb01_link rl;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_report_col
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_report_col';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_report_columns;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_report_columns
       SELECT rc.col_id,
              rc.name_en,
              rc.name_local,
							rc.name_ru
       FROM rpt_report_columns@rptdb01_link rc;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_report_col_rel
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_report_col_rel';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_report_column_relations;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_report_column_relations
       SELECT rcr.rep_id,
              rcr.col_id
       FROM rpt_report_column_relations@rptdb01_link rcr;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_reports
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_reports';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_reports;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_reports
       SELECT r.rep_id,
              r.rep_code,
              r.name_en,
              r.name_local,
							       r.name_ru,
              r.ssrs_2008_r2,
              r.export_formats,
              r.rep_admins
       FROM rpt_reports@rptdb01_link r;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_settings
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_settings';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_report_settings;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_report_settings
       SELECT rs.setting_key,
              rs.setting_value,
              rs.setting_desc,
              rs.setting_date
       FROM rpt_report_settings@rptdb01_link rs;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_siem_prst_shrt
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_siem_prst_shrt';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_siem_prestamo_short;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_siem_prestamo_short
       SELECT sps.suc_codigo,
              sps.pre_codigo,
              sps.usu_codigo,
              sps.gru_codigo,
              sps.pre_capsal,
              sps.pre_intsal,
              sps.pre_entfec,
              sps.pre_close,
              sps.cod_new_custid
       FROM rpt_siem_prestamo_short@rptdb01_link sps;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_sys_problem_lns
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_sys_problem_lns';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_system_problem_loans;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_system_problem_loans
       SELECT spl.account_no
       FROM rpt_system_problem_loans@rptdb01_link spl;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_tree_items
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_tree_items';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_tree_items;
       v_row_count_processed := trim(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_tree_items
       SELECT ti.report_name,
              ti.tree_id,
              ti.tree_parent_id,
              ti.tree_order,
              ti.tree_code,
              ti.tree_parent_code,
              ti.tree_desc,
              ti.tree_desc_az
       FROM rpt_tree_items@rptdb01_link ti;
       v_row_count_processed := v_row_count_processed || '; ' || trim(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RPT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE refresh_rpt_logs
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_rpt_logs';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_report_logs;

       INSERT INTO rpt_report_logs
       SELECT rl.log_id,
              rl.log_key,
              rl.log_date,
              rl.proc_name,
              rl.proc_params,
              rl.proc_start_date,
              rl.proc_end_date,
              rl.exec_duration,
              rl.log_desc
       FROM rpt_report_logs@rptdb01_link rl;
       COMMIT;

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RPT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

	-------------------- MASK DATA FOR OTHER FINCA HUB ---------------

	PROCEDURE mask_sensitive_tables
	IS
	BEGIN
	     /*
			 select cm.cod_cust_natl_id, --cm.cod_natl_id_old,
			 cm.nam_custadr_city, cm.nam_permadr_city,
			 cm.txt_custadr_add1, cm.txt_custadr_add2, cm.txt_custadr_add3,
			 cm.txt_permadr_add1, cm.txt_permadr_add2, cm.txt_permadr_add3,
			 cm.nam_custadr_city,
			 cm.ref_cust_phone, cm.ref_res_phone_area, cm.ref_phone_mobile,
			 cm.dat_birth_cust,
			 --cm.txt_holdadr_add1, cm.txt_holdadr_add2, cm.txt_holdadr_add3,
			 cm.* from ci_custmast cm;
			 */
			 UPDATE ci_custmast cm
			 SET cm.cod_cust_natl_id = cod_cust_id,
					 --cm.cod_natl_id_old = '',
					 cm.nam_custadr_city = NVL2(cm.nam_custadr_city, 'UNDEFINED', ''),
					 cm.nam_permadr_city = NVL2(cm.nam_permadr_city, 'UNDEFINED', ''),
					 cm.txt_custadr_add1 = NVL2(cm.txt_custadr_add1, 'UNDEFINED', ''),
					 cm.txt_custadr_add2 = NVL2(cm.txt_custadr_add2, 'UNDEFINED', ''),
					 cm.txt_custadr_add3 = NVL2(cm.txt_custadr_add3, 'UNDEFINED', ''),
					 cm.txt_permadr_add1 = NVL2(cm.txt_permadr_add1, 'UNDEFINED', ''),
					 cm.txt_permadr_add2 = NVL2(cm.txt_permadr_add2, 'UNDEFINED', ''),
					 cm.txt_permadr_add3 = NVL2(cm.txt_permadr_add3, 'UNDEFINED', ''),
					 cm.ref_cust_phone = NVL2(cm.ref_cust_phone, '9876543', ''),
					 cm.ref_res_phone_area = NVL2(cm.ref_res_phone_area, '9876543', ''),
					 cm.ref_phone_mobile = NVL2(cm.ref_phone_mobile, '9876543', ''),
					 cm.dat_birth_cust = TRUNC(SYSDATE - 30*365)/*,
					 cm.txt_holdadr_add1 = nvl2(cm.txt_holdadr_add1, 'UNDEFINED', ''),
					 cm.txt_holdadr_add2 = nvl2(cm.txt_holdadr_add1, 'UNDEFINED', ''),
					 cm.txt_holdadr_add3 = nvl2(cm.txt_holdadr_add1, 'UNDEFINED', '')*/;

			 COMMIT;

			 ---------------------------------------------

			 /*
			 select cm.cod_cust_natl_id, cm.cod_natl_id_old,
			 cm.nam_custadr_city, cm.nam_permadr_city,
			 cm.txt_custadr_add1, cm.txt_custadr_add2, cm.txt_custadr_add3,
			 cm.txt_permadr_add1, cm.txt_permadr_add2, cm.txt_permadr_add3,
			 cm.ref_cust_phone, cm.ref_res_phone_area, cm.ref_phone_mobile,
			 cm.dat_birth_cust,
			 cm.txt_holdadr_add1, cm.txt_holdadr_add2, cm.txt_holdadr_add3,
			 cm.* from conv_ci_custmast cm
			 */
			 UPDATE conv_ci_custmast cm
			 SET cm.cod_cust_natl_id = cod_cust_id,
					 cm.cod_natl_id_old = '',
					 cm.nam_custadr_city = NVL2(cm.nam_custadr_city, 'UNDEFINED', ''),
					 cm.nam_permadr_city = NVL2(cm.nam_permadr_city, 'UNDEFINED', ''),
					 cm.txt_custadr_add1 = NVL2(cm.txt_custadr_add1, 'UNDEFINED', ''),
					 cm.txt_custadr_add2 = NVL2(cm.txt_custadr_add2, 'UNDEFINED', ''),
					 cm.txt_custadr_add3 = NVL2(cm.txt_custadr_add3, 'UNDEFINED', ''),
					 cm.txt_permadr_add1 = NVL2(cm.txt_permadr_add1, 'UNDEFINED', ''),
					 cm.txt_permadr_add2 = NVL2(cm.txt_permadr_add2, 'UNDEFINED', ''),
					 cm.txt_permadr_add3 = NVL2(cm.txt_permadr_add3, 'UNDEFINED', ''),
					 cm.ref_phone_mobile = NVL2(cm.ref_phone_mobile, '9876543', ''),
					 cm.ref_res_phone_area = NVL2(cm.ref_res_phone_area, '9876543', ''),
					 cm.ref_cust_phone = NVL2(cm.ref_cust_phone, '9876543', ''),
					 cm.dat_birth_cust = TRUNC(SYSDATE - 30*365),
					 cm.txt_holdadr_add1 = NVL2(cm.txt_holdadr_add1, 'UNDEFINED', ''),
					 cm.txt_holdadr_add2 = NVL2(cm.txt_holdadr_add1, 'UNDEFINED', ''),
					 cm.txt_holdadr_add3 = NVL2(cm.txt_holdadr_add1, 'UNDEFINED', '');

			 COMMIT;

			 ---------------------------------------------

			 /*
			 select nam_empadr_city,
			 cd.txt_empadr_add1, --cd.txt_empadr_add2, cd.txt_empadr_add3,
			 --cd.txt_addr1_spo_emp, cd.txt_addr2_spo_emp, cd.txt_addr3_spo_emp,
			 cd.ref_cust_emp_phone_area, cd.ref_cust_emp_phone,
			 cd.ref_cust_pspt,
			 --cd.dat_dob_spouse,
			 cd.* from ci_custdetl cd
			 */
			 UPDATE ci_custdetl cd
			 SET cd.nam_empadr_city = NVL2(cd.nam_empadr_city, 'UNDEFINED', ''),
					 cd.txt_empadr_add1 = NVL2(cd.txt_empadr_add1, 'UNDEFINED', ''),
					 /*cd.txt_empadr_add2 = nvl2(cd.txt_empadr_add2, 'UNDEFINED', ''),
					 cd.txt_empadr_add3 = nvl2(cd.txt_empadr_add3, 'UNDEFINED', ''),
					 cd.txt_addr1_spo_emp = nvl2(cd.txt_addr1_spo_emp, 'UNDEFINED', ''),
					 cd.txt_addr2_spo_emp = nvl2(cd.txt_addr2_spo_emp, 'UNDEFINED', ''),
					 cd.txt_addr3_spo_emp = nvl2(cd.txt_addr3_spo_emp, 'UNDEFINED', ''),*/
					 cd.ref_cust_emp_phone_area = NVL2(cd.ref_cust_emp_phone_area, '9876543', ''),
					 cd.ref_cust_emp_phone = NVL2(cd.ref_cust_emp_phone, '9876543', ''),
					 cd.ref_cust_pspt = NVL2(cd.ref_cust_pspt, cd.cod_cust_id, '');
					 --cd.dat_dob_spouse = nvl2(cd.dat_dob_spouse, trunc(sysdate - 30*365), '');

			 COMMIT;

			 ---------------------------------------------

			 /*
			 select ucl.field_value, ucl.* from udf_cust_log_details ucl where ucl.cod_field_tag = 'TXT_5'
			 */
			 UPDATE udf_cust_log_details ucl
			 SET ucl.field_value = cod_cust_id
			 WHERE ucl.cod_field_tag = 'TXT_5';

			 COMMIT;

			 ---------------------------------------------

			 /*
			 select * from cs_propagate_log;
			 */
			 DELETE FROM cs_propagate_log;
			 COMMIT;

			 ---------------------------------------------

			 /*
			 select bi.cod_cust_natl_id, bi.* from CS_HO_BLACKLIST_INFO bi
			 */
			 UPDATE cs_ho_blacklist_info bi
			 SET bi.cod_cust_natl_id = cod_cust_id;

			 COMMIT;
	END;

	PROCEDURE restore_masked_tables
	IS
	BEGIN
			 data_rptdb01.refresh_ci_custmast;
			 data_rptdb01.refresh_conv_ci_custmast;
			 data_rptdb01.refresh_ci_custdetl;
			 data_rptdb01.refresh_udf_cust_log_dtls;
			 data_rptdb01.refresh_cs_prop_log;
			 data_rptdb01.refresh_cs_ho_blacklist;
	END;

END;
/
