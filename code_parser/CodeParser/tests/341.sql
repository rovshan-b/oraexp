CREATE OR REPLACE PACKAGE data_transfer
IS

  -- Author  : ELDANIZ
  -- Created : 19.01.2009 16:26:28
  -- Purpose :

  --PROCEDURE empty_all_tables;

  PROCEDURE refresh_all_tables;
  PROCEDURE refresh_simple_tables;
  PROCEDURE refresh_main_tables;
  PROCEDURE refresh_history_tables;
  PROCEDURE refresh_mv_tables(p_mv_type NUMBER);
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
  PROCEDURE refresh_co_old_new_cust;
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
  PROCEDURE update_rec_txnlog;

END;
/
CREATE OR REPLACE PACKAGE BODY data_transfer
IS

  /*PROCEDURE empty_all_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'empty_all_tables';
  BEGIN
       v_started := SYSDATE;
       --
       DELETE FROM actb_accbal_history;
       --DELETE FROM actb_history;
       DELETE FROM ac_acct_crr_code;
       DELETE FROM ac_acct_writeoff;
       DELETE FROM ac_crr_codes;
       DELETE FROM ba_acct_status;
       DELETE FROM ba_bank_mast;
       DELETE FROM ba_brn_type_master;
       DELETE FROM ba_ccy_code;
       DELETE FROM ba_ccy_rate;
       DELETE FROM ba_cc_brn_mast;
       DELETE FROM ba_coll_codes;
       DELETE FROM ba_coll_hdr;
       DELETE FROM ba_group_details;
       DELETE FROM ba_sc_code;
       DELETE FROM ba_txn_mis_xref;
       DELETE FROM ba_txn_mnemonic;
       DELETE FROM ch_acct_cust_xref;
       DELETE FROM ci_business_types;
       DELETE FROM ci_custdetl;
       DELETE FROM ci_custmast;
       DELETE FROM ci_custrel;
       DELETE FROM ci_cust_types;
       DELETE FROM ci_ic_types;
       DELETE FROM ci_relmast;
       DELETE FROM cs_ho_blacklist_info;
       DELETE FROM gltm_glmaster;
       DELETE FROM gl_master;
       DELETE FROM ln_acct_attributes;
       DELETE FROM ln_acct_balances;
       DELETE FROM ln_acct_dtls;
       DELETE FROM ln_acct_int_balance_dtls;
       DELETE FROM ln_acct_schedule_detls;
       DELETE FROM ln_arrears_table;
       --DELETE FROM ln_daily_txn_log;
       DELETE FROM ln_dedn_plan;
       DELETE FROM ln_prod_mast;
       DELETE FROM ln_sched_types;
       DELETE FROM or_applicants;
       DELETE FROM or_applications;
       DELETE FROM or_application_assets;
       DELETE FROM ln_acct_sched_detls_hist;
       DELETE FROM or_lookups;
       DELETE FROM sm_temp_profile;
       DELETE FROM sm_user_profile;
       DELETE FROM ln_arrear_txn_hist;
       DELETE FROM rpt_ci_docmast;

       log_manager.add_log('DEL_ALL',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'DELETE FROM ALL TABLES SUCCESSFUL',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('DEL_ALL_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;*/

  PROCEDURE refresh_all_tables
  IS -- TRUNC(SYSDATE)+(CASE WHEN to_char(SYSDATE,'hh24') BETWEEN 0 AND 7 THEN ((to_char(SYSDATE,'hh24')+1)/24) ELSE 1 END)
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_all_tables';
  BEGIN
       v_started := SYSDATE;

       IF (report_methods.job_can_run > 0)
       THEN
           refresh_simple_tables;
           refresh_main_tables;
           refresh_history_tables;

           log_manager.add_log('REF_ALL',
                               v_proc_name,
                               NULL,
                               v_started,
                               SYSDATE,
                               'REFRESHING DATA FINISHED FOR ALL TABLES',
                               1);
       END IF;

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
                             SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1, 3)+1, LENGTH(rs.setting_value)-INSTR(rs.setting_value, ';', 1, 3)) db_links
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
                                               '') proc_name,
                                REGEXP_REPLACE(REGEXP_SUBSTR(t.mnt_status_list || ',',
                                                             '(.*?),',
                                                             1,
                                                             LEVEL),
                                               ',$',
                                               '') mnt_status
                         FROM (SELECT SUBSTR(rs.setting_value, 0, INSTR(rs.setting_value, ';', 1)-1) table_list,
                                      SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1)+1, INSTR(rs.setting_value, ';', 1, 2)-INSTR(rs.setting_value, ';', 1)-1) proc_list,
                                      SUBSTR(rs.setting_value, INSTR(rs.setting_value, ';', 1, 2)+1) mnt_status_list
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
                               || ' t'
                               || (CASE WHEN i.rnum = 2 AND j.mnt_status = '1' THEN ' WHERE t.flg_mnt_status = ''A''' END);
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

  PROCEDURE refresh_main_tables
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_main_tables';
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
       refresh_mv_tables(0);

       log_manager.add_log('REF_MAIN',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'REFRESHING DATA FINISHED FOR MAIN TABLES',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_MAIN_ERR',
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
       data_collector.refresh_dtl_po;
       data_collector.refresh_rpt_gl_totals;
       --refresh_ci;
       --
       refresh_mv_tables(1);

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

  PROCEDURE refresh_mv_tables(p_mv_type NUMBER)
  IS
    v_started DATE;
    v_proc_name VARCHAR2(20) := 'refresh_mv_tables';
    v_params VARCHAR2(20) := 'p_mv_type = ' || TRIM(p_mv_type);
  BEGIN
       v_started := SYSDATE;

       IF p_mv_type = 0 -- main tables
       THEN
           dbms_refresh.refresh('"FAZRPT"."MV_CUSTOMER_CYCLE_LIST"');
           dbms_refresh.refresh('"FAZRPT"."MV_LOAN_GUARANTORS"');
           dbms_refresh.refresh('"FAZRPT"."MV_CURRENCY_RATES_FOR_USD"');
           --
           dbms_refresh.refresh('"FAZRPT"."MV_PRODUCT_INT_RULES"');
           dbms_refresh.refresh('"FAZRPT"."MV_PENALTY_STOPPED_LOANS"');
       ELSIF p_mv_type = 1 -- history tables
       THEN
           dbms_refresh.refresh('"FAZRPT"."MV_HIST_ROW_COUNT_HOST"');
           dbms_refresh.refresh('"FAZRPT"."MV_PO_BY_BRANCH"'); -- TRUNC(SYSDATE) + 199/144
           dbms_refresh.refresh('"FAZRPT"."MV_TXN_CODES_AH"'); -- TRUNC(SYSDATE, 'd') + 7 + 1/144
           dbms_refresh.refresh('"FAZRPT"."MV_HIST_ROW_COUNT_RPT"');
           --
           dbms_refresh.refresh('"FAZRPT"."MV_CO_LOAN_CHANGES"');
           dbms_refresh.refresh('"FAZRPT"."MV_LEGAL_DEP_LOANS"');
           dbms_refresh.refresh('"FAZRPT"."MV_WRITE_OFF_AMOUNTS"');
           dbms_refresh.refresh('"FAZRPT"."MV_USER_INFO_CHANGES"');
       ELSIF p_mv_type = 2 -- rec_txnlog table
       THEN
           dbms_refresh.refresh('"FAZRPT"."MV_TXN_CODES_RT"');
           dbms_refresh.refresh('"FAZRPT"."MV_BRANCH_TELLERS"');
           dbms_refresh.refresh('"FAZRPT"."MV_REC_TXNLOG_MAX_DT"');
       END IF;

       log_manager.add_log('REF_MV',
                           v_proc_name,
                           v_params,
                           v_started,
                           SYSDATE,
                           'refreshing materialized views finished for ' || (CASE WHEN p_mv_type = 0 THEN 'main'
                                                                                  WHEN p_mv_type = 1 THEN 'history'
                                                                                  ELSE 'rec_txnlog' END)
                                                                         || ' tables',
                           1);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('REF_MV_ERR',
                                       v_proc_name,
                                       v_params,
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
                        SUM(DECODE(t.table_name, 'actb_accbal_history@host_link', t.cnt, 0)) "actb_accbal_history@host_link",
                        SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0)) "actb_accbal_history",
                        (SUM(DECODE(t.table_name, 'actb_accbal_history@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0))) diff*/
                 FROM (SELECT 'actb_accbal_history@host_link' table_name,
                              TRUNC(ah.bkg_date) bkg_date,
                              COUNT(*) cnt
                       FROM actb_accbal_history@host_link ah
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
                 HAVING (SUM(DECODE(t.table_name, 'actb_accbal_history@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'actb_accbal_history', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_aah_for_date(i.bkg_date);
       END LOOP;
  END;

  PROCEDURE refresh_ah
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'actb_history@host_link', t.cnt, 0)) "actb_history@host_link",
                        SUM(decode(t.table_name, 'actb_history', t.cnt, 0)) "actb_history",
                        (SUM(decode(t.table_name, 'actb_history@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'actb_history', t.cnt, 0))) diff*/
                 FROM (SELECT 'actb_history' table_name,
                              trunc(ah.trn_dt) txn_date,
                              COUNT(*) cnt
                       FROM actb_history ah
                       WHERE ah.trn_dt >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY trunc(ah.trn_dt)
                       UNION ALL
                       SELECT 'actb_history@host_link' table_name,
                              trunc(ah.trn_dt) txn_date,
                              COUNT(*) cnt
                       FROM actb_history@host_link ah
                       WHERE ah.trn_dt >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -1)
                       GROUP BY trunc(ah.trn_dt)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'actb_history@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'actb_history', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_ah_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_dtl
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*, -- new
                        SUM(decode(t.table_name, 'ln_daily_txn_log_hist@host_link', t.cnt, 0)) "ln_daily_txn_log@host_link",
                        SUM(decode(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0)) "ln_daily_txn_log_hist",
                        (SUM(decode(t.table_name, 'ln_daily_txn_log_hist@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_daily_txn_log_hist@host_link' table_name,
                              trunc(tl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_daily_txn_log_hist@host_link tl
                       WHERE tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
                       GROUP BY trunc(tl.dat_txn)
                       UNION ALL
                       SELECT t1.table_name,
                              t1.txn_date,
                              (t1.cnt - NVL(t2.txn_diff_count, 0)) cnt
                       FROM (SELECT 'ln_daily_txn_log_hist' table_name,
                                    TRUNC(tl.dat_txn) txn_date,
                                    COUNT(*) cnt
                             FROM ln_daily_txn_log_hist tl
                             WHERE tl.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
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
                 HAVING (SUM(DECODE(t.table_name, 'ln_daily_txn_log_hist@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ln_daily_txn_log_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_dtl_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_ath
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'ln_arrear_txn_hist@host_link', t.cnt, 0)) "ln_arrear_txn_hist@host_link",
                        SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0)) "ln_arrear_txn_hist",
                        (SUM(decode(t.table_name, 'ln_arrear_txn_hist@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_arrear_txn_hist@host_link' table_name,
                              trunc(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_arrear_txn_hist@host_link th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
                       GROUP BY trunc(th.dat_txn)
                       UNION ALL
                       SELECT 'ln_arrear_txn_hist' table_name,
                              trunc(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_arrear_txn_hist th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
                       GROUP BY trunc(th.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'ln_arrear_txn_hist@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrear_txn_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_ath_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_asd
  IS
  BEGIN
       FOR i IN (SELECT t.stage_start_date/*,
                        SUM(decode(t.table_name, 'ln_acct_schedule_detls@host_link', t.cnt, 0)) "ln_acct_sched_dtls@host_link",
                        SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0)) "ln_acct_schedule_detls",
                        (SUM(decode(t.table_name, 'ln_acct_schedule_detls@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_acct_schedule_detls@host_link' table_name,
                              trunc(s.dat_stage_start) stage_start_date,
                              COUNT(*) cnt
                       FROM ln_acct_schedule_detls@host_link s
                       WHERE s.dat_stage_start >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -18)
                       GROUP BY trunc(s.dat_stage_start)
                       UNION ALL
                       SELECT 'ln_acct_schedule_detls' table_name,
                              trunc(s.dat_stage_start) stage_start_date,
                              COUNT(*) cnt
                       FROM ln_acct_schedule_detls s
                       WHERE s.dat_stage_start >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -18)
                       GROUP BY trunc(s.dat_stage_start)) t
                 GROUP BY t.stage_start_date
                 HAVING (SUM(decode(t.table_name, 'ln_acct_schedule_detls@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_schedule_detls', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_asd_for_date(i.stage_start_date);
       END LOOP;
  END;

  PROCEDURE refresh_art
  IS
  BEGIN
       FOR i IN (SELECT t.arr_due_date/*,
                        SUM(decode(t.table_name, 'ln_arrears_table@host_link', t.cnt, 0)) "ln_arrears_table@host_link",
                        SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0)) "ln_arrears_table",
                        (SUM(decode(t.table_name, 'ln_arrears_table@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_arrears_table@host_link' table_name,
                              trunc(lat.dat_arrears_due) arr_due_date,
                              COUNT(*) cnt
                       FROM ln_arrears_table@host_link lat
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
                 HAVING (SUM(decode(t.table_name, 'ln_arrears_table@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_arrears_table', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_art_for_date(i.arr_due_date);
       END LOOP;
  END;

  PROCEDURE refresh_al
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(decode(t.table_name, 'ln_acct_ledg@host_link', t.cnt, 0)) "ln_acct_ledg@host_link",
                        SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0)) "ln_acct_ledg",
                        (SUM(decode(t.table_name, 'ln_acct_ledg@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_acct_ledg@host_link' table_name,
                              trunc(al.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_acct_ledg@host_link al
                       WHERE al.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
                       GROUP BY trunc(al.dat_txn)
                       UNION ALL
                       SELECT 'ln_acct_ledg' table_name,
                              trunc(al.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_acct_ledg al
                       WHERE al.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -3)
                       GROUP BY trunc(al.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(decode(t.table_name, 'ln_acct_ledg@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ln_acct_ledg', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_al_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_ci
  IS
  BEGIN
       FOR i IN (SELECT t.mnt_date/*,
                        SUM(decode(t.table_name, 'ci_custmast@host_link', t.cnt, 0)) "ci_custmast@host_link",
                        SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0)) "ci_custmast",
                        (SUM(decode(t.table_name, 'ci_custmast@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0))) diff*/
                 FROM (SELECT 'ci_custmast@host_link' table_name,
                              trunc(c.dat_last_mnt) mnt_date,
                              COUNT(*) cnt
                       FROM ci_custmast@host_link c
                       WHERE c.flg_mnt_status = 'A'
                       GROUP BY trunc(c.dat_last_mnt)
                       UNION ALL
                       SELECT 'ci_custmast' table_name,
                              trunc(c.dat_last_mnt) mnt_date,
                              COUNT(*) cnt
                       FROM ci_custmast c
                       GROUP BY trunc(c.dat_last_mnt)) t
                 GROUP BY t.mnt_date
                 HAVING (SUM(decode(t.table_name, 'ci_custmast@host_link', t.cnt, 0)) -
                         SUM(decode(t.table_name, 'ci_custmast', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_ci_for_date(i.mnt_date);
       END LOOP;
  END;

  PROCEDURE refresh_rth
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@host_link', t.cnt, 0)) "ln_rpa_txnlog_hist@host_link",
                        SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0)) "ln_rpa_txnlog_hist",
                        (SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0))) diff*/
                 FROM (SELECT 'ln_rpa_txnlog_hist@host_link' table_name,
                              TRUNC(th.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ln_rpa_txnlog_hist@host_link th
                       WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -13)
                       GROUP BY TRUNC(th.dat_txn)
                       UNION ALL
                       SELECT t1.table_name,
                              t1.txn_date,
                              (t1.cnt - NVL(t2.txn_diff_count, 0)) cnt
                       FROM (SELECT 'ln_rpa_txnlog_hist' table_name,
                                    TRUNC(th.dat_txn) txn_date,
                                    COUNT(*) cnt
                             FROM ln_rpa_txnlog_hist th
                             WHERE th.dat_txn >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -13)
                             GROUP BY TRUNC(th.dat_txn)) t1,
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
                 HAVING (SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ln_rpa_txnlog_hist', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_rth_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_crl
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'ba_change_record_log@host_link', t.cnt, 0)) "ba_change_record_log@host_link",
                        SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0)) "ba_change_record_log",
                        (SUM(DECODE(t.table_name, 'ba_change_record_log@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0))) diff*/
                 FROM (SELECT 'ba_change_record_log@host_link' table_name,
                              TRUNC(crl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ba_change_record_log@host_link crl
                       WHERE crl.dat_txn >= TRUNC(SYSDATE) - 15
                       GROUP BY TRUNC(crl.dat_txn)
                       UNION ALL
                       SELECT 'ba_change_record_log' table_name,
                              TRUNC(rl.dat_txn) txn_date,
                              COUNT(*) cnt
                       FROM ba_change_record_log rl
                       WHERE rl.dat_txn >= TRUNC(SYSDATE) - 15
                       GROUP BY TRUNC(rl.dat_txn)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'ba_change_record_log@host_link', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'ba_change_record_log', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_crl_for_date(i.txn_date);
       END LOOP;
  END;

  PROCEDURE refresh_cfl
  IS
    v_max_change_no NUMBER := 0;
  BEGIN
       SELECT MAX(crl.ctr_change_no) INTO v_max_change_no
       FROM ba_change_record_log crl
       WHERE crl.dat_txn < TRUNC(SYSDATE) - 15;

       FOR i IN (SELECT cf.table_name,
                        WM_CONCAT(cf.field_name) field_list/*,
                        SUM(cf."@host_link") "@host_link",
                        SUM(cf."reporting") "reporting",
                        SUM(cf.diff) diff*/
                 FROM (SELECT t.table_name,
                              t.field_name,
                              SUM(DECODE(t.host, '@host_link', t.cnt, 0)) "@host_link",
                              SUM(DECODE(t.host, 'reporting', t.cnt, 0)) "reporting",
                              (SUM(DECODE(t.host, '@host_link', t.cnt, 0)) -
                               SUM(DECODE(t.host, 'reporting', t.cnt, 0))) diff
                       FROM (SELECT '@host_link' host,
                                    cfl.name_table table_name,
                                    cfl.cod_field_name field_name,
                                    COUNT(*) cnt
                             FROM ba_change_field_log@host_link cfl
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
                       HAVING (SUM(DECODE(t.host, '@host_link', t.cnt, 0)) -
                               SUM(DECODE(t.host, 'reporting', t.cnt, 0))) != 0) cf
                 GROUP BY cf.table_name
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_cfl_for_date(v_max_change_no, i.table_name, i.field_list);
       END LOOP;
  END;

  PROCEDURE refresh_rt -- rec_txnlog
  IS
  BEGIN
       FOR i IN (SELECT t.txn_date/*,
                        SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) "v_rec_txnlog",
                        SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0)) "rpt_rec_txnlog",
                        (SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0))) diff*/
                 FROM (SELECT 'v_rec_txnlog' table_name,
                              TRUNC(vrt.tim_sys) txn_date,
                              COUNT(*) cnt
                       FROM v_rec_txnlog vrt
                       WHERE TRUNC(vrt.tim_sys) < TRUNC(SYSDATE)
                             AND vrt.tim_sys >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -2)
                       GROUP BY TRUNC(vrt.tim_sys)
                       UNION ALL
                       SELECT 'rpt_rec_txnlog' table_name,
                              TRUNC(rt.tim_sys) txn_date,
                              COUNT(*) cnt
                       FROM rpt_rec_txnlog rt
                       WHERE TRUNC(rt.tim_sys) < TRUNC(SYSDATE)
                             AND rt.tim_sys >= ADD_MONTHS(TRUNC(SYSDATE, 'MONTH'), -2)
                       GROUP BY TRUNC(rt.tim_sys)) t
                 GROUP BY t.txn_date
                 HAVING (SUM(DECODE(t.table_name, 'v_rec_txnlog', t.cnt, 0)) -
                         SUM(DECODE(t.table_name, 'rpt_rec_txnlog', t.cnt, 0))) != 0
                 ORDER BY 1)
       LOOP
           data_transfer.refresh_rt_for_date(i.txn_date);
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
                       FROM xf_ol_st_txnlog_mmdd@host_link xt
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM actb_accbal_history@host_link aah
       WHERE trunc(aah.bkg_date) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM actb_history@host_link ah
       WHERE trunc(ah.trn_dt) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_daily_txn_log_hist@host_link tl
       WHERE trunc(tl.dat_txn) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_arrear_txn_hist@host_link th
       WHERE trunc(th.dat_txn) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls
       SELECT TRIM(s.cod_acct_no) cod_acct_no,
              s.dat_stage_start,
              s.ctr_instal,
              s.date_instal,
							s.rat_int,
              s.amt_principal,
              s.amt_interest,
              s.amt_princ_bal
       FROM ln_acct_schedule_detls@host_link s
       WHERE trunc(s.dat_stage_start) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_arrears_table@host_link la
       WHERE trunc(la.dat_arrears_due) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_acct_ledg@host_link al
       WHERE trunc(al.dat_txn) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ci_custmast@host_link c
       WHERE c.flg_mnt_status = 'A'
             AND trunc(c.dat_last_mnt) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_rpa_txnlog_hist@host_link th
       WHERE trunc(th.dat_txn) = trunc(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ba_change_record_log@host_link crl
       WHERE TRUNC(crl.dat_txn) = TRUNC(p_date);
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_change_field_log
       SELECT cfl.ctr_change_no,
              cfl.ctr_table_no,
              cfl.name_table,
              cfl.row_no,
              cfl.cod_field_name,
              TRIM(cfl.old_field_value) old_field_value,
              TRIM(cfl.new_field_value) new_field_value,
              cfl.key_flg
       FROM ba_change_field_log@host_link cfl
       WHERE cfl.ctr_change_no > p_max_change_no
             AND cfl.name_table = p_table_name
             AND INSTR(p_field_list, cfl.cod_field_name) > 0;
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM rec_txnlog@brn_link rt
       WHERE trunc(rt.tim_sys) = trunc(p_date);
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
       FROM xf_ol_st_txnlog_mmdd@host_link xt
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM actb_accbal_history@host_link aah;
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

  PROCEDURE refresh_ac_acct_crr_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_acct_crr_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_acct_crr_code;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ac_acct_crr_code@host_link aac
       WHERE aac.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ac_acct_writeoff
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_acct_writeoff';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_acct_writeoff;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ac_acct_writeoff@host_link wo;
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

  PROCEDURE refresh_ac_crr_codes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ac_crr_codes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ac_crr_codes;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ac_crr_codes
       SELECT ac.cod_crr,
              TRIM(ac.txt_crr_desc) txt_crr_desc,
              ac.flg_accr_status,
              ac.dat_last_mnt
       FROM ac_crr_codes@host_link ac
       WHERE ac.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_acct_status
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_acct_status';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_acct_status;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_acct_status
       SELECT ast.cod_module,
              ast.cod_acct_status,
              TRIM(ast.txt_acct_status) txt_acct_status,
              ast.dat_last_mnt
       FROM ba_acct_status@host_link ast
       WHERE ast.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_bank_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_bank_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_bank_mast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ba_bank_mast@host_link b
       WHERE b.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_brn_type_master
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_brn_type_master';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_brn_type_master;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_brn_type_master
       SELECT bt.cod_brn_type,
              bt.cod_brn_name,
              bt.dat_last_mnt
       FROM ba_brn_type_master@host_link bt
       WHERE bt.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_ccy_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_ccy_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_ccy_code;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_ccy_code
       SELECT c.cod_ccy,
              TRIM(c.nam_ccy_short) nam_ccy_short,
              TRIM(c.nam_currency) nam_currency,
              c.dat_last_mnt
       FROM ba_ccy_code@host_link c;
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

  PROCEDURE refresh_ba_ccy_rate
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_ccy_rate';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_ccy_rate;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_ccy_rate
       SELECT cr.cod_ccy,
              cr.rat_ccy_cb,
              cr.dat_tim_rate_eff,
              cr.dat_last_mnt
       FROM ba_ccy_rate@host_link cr
       WHERE cr.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_cc_brn_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_cc_brn_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_cc_brn_mast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_cc_brn_mast
       SELECT b.cod_cc_brn,
              b.cod_main_branch,
              b.cod_brn_type,
              TRIM(b.nam_branch) nam_branch,
              TRIM(b.nam_branch_shrt) nam_branch_shrt,
              TRIM(b.nam_cc_city) nam_cc_city,
              b.dat_last_mnt
       FROM rpt_ba_cc_brn_mast@host_link b;
       --WHERE b.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_coll_auto
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_auto';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_automobile;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_automobile
       SELECT bca.cod_collat_id,
              bca.cod_regn_no
       FROM ba_coll_automobile@host_link bca
       WHERE bca.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_coll_codes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_codes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_codes;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_codes
       SELECT cc.cod_coll,
              cc.cod_coll_type,
              TRIM(cc.nam_coll) nam_coll,
              cc.dat_last_mnt
       FROM ba_coll_codes@host_link cc
       WHERE cc.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_coll_hdr
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_hdr';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_hdr;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_coll_hdr
       SELECT ch.cod_collat_id,
              ch.cod_coll,
              TRIM(ch.txt_deed_detl_1) txt_deed_detl_1,
              ch.cod_coll_homebrn,
              ch.cod_ccy,
              ch.amt_last_val,
              ch.amt_orig_value,
              ch.amt_market_val,
              TRIM(ch.nam_lender) nam_lender,
              ch.cod_coll_class,
              ch.dat_last_mnt
       FROM ba_coll_hdr@host_link ch
       WHERE ch.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_coll_prop
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_coll_prop';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_coll_prop;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ba_coll_prop@host_link cp
       WHERE cp.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_group_details
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_group_details';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_group_details;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ba_group_details@host_link gd
       WHERE gd.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_lo_coll_acct_xr
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_lo_coll_acct_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_lo_coll_acct_xref;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_lo_coll_acct_xref
       SELECT ca.cod_collat_id,
              TRIM(ca.cod_acct_no) cod_acct_no,
              ca.cod_coll,
              ca.cod_prod,
              ca.cod_cc_brn,
              ca.amt_coll_value,
              ca.dat_last_mnt
       FROM ba_lo_coll_acct_xref@host_link ca;
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

  PROCEDURE refresh_ba_sc_code
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_sc_code';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_sc_code;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_sc_code
       SELECT sc.cod_sc,
              TRIM(sc.nam_sc) nam_sc,
              sc.cod_ccy,
              sc.rat_sc_txn_pct,
              sc.amt_sc_fixed,
              sc.amt_sc_min,
              sc.amt_sc_max,
              sc.dat_last_mnt
       FROM ba_sc_code@host_link sc
       WHERE sc.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_txn_mis_xref
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_txn_mis_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_cust_prod_txn_mis_xref;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_cust_prod_txn_mis_xref
       SELECT m.flg_mis_type,
              TRIM(m.cod_prod_mnem_cust) cod_prod_mnem_cust,
              m.cod_mis_cust_2,
              m.cod_mis_txn_3,
              m.dat_last_mnt
       FROM ba_cust_prod_txn_mis_xref@host_link  m
       WHERE m.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ba_txn_mnemonic
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ba_txn_mnemonic';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ba_txn_mnemonic;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ba_txn_mnemonic
       SELECT tm.cod_txn_mnemonic,
              tm.cod_drcr,
              tm.cod_txn_type,
              tm.cod_txn_mode,
              TRIM(tm.txt_txn_desc) txt_txn_desc,
              tm.dat_last_mnt
       FROM ba_txn_mnemonic@host_link tm;
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

  PROCEDURE refresh_ch_acct_cust_xref
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ch_acct_cust_xref';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ch_acct_cust_xref;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ch_acct_cust_xref
       SELECT TRIM(cx.cod_acct_no) cod_acct_no,
              cx.cod_cust,
              cx.cod_acct_cust_rel,
              cx.dat_last_mnt
       FROM ch_acct_cust_xref@host_link cx;
       --WHERE cx.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_business_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_business_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_business_types;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_business_types
       SELECT bt.cod_business_cat,
              TRIM(bt.txt_business) txt_business,
              bt.dat_last_mnt
       FROM ci_business_types@host_link bt
       WHERE bt.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_custdetl
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custdetl';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custdetl;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ci_custdetl@host_link cd
       WHERE cd.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_custmast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custmast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custmast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ci_custmast@host_link c
       WHERE c.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_custrel
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_custrel';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_custrel;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_custrel
       SELECT cr.cod_cust_p,
              cr.cod_cust_s,
              cr.cod_rel,
              cr.cod_inv_rel,
              cr.dat_last_mnt
       FROM ci_custrel@host_link cr
       WHERE cr.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_cust_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_cust_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_cust_types;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_cust_types
       SELECT ct.flg_cust_typ,
              TRIM(ct.txt_cust_typ) txt_cust_typ,
              ct.dat_last_mnt
       FROM ci_cust_types@host_link ct
       WHERE ct.flg_mnt_status ='A';
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

  PROCEDURE refresh_ci_ic_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_ic_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_ic_types;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_ic_types
       SELECT cit.flg_ic_typ,
              TRIM(cit.txt_ic_typ) txt_ic_typ,
              cit.dat_last_mnt
       FROM ci_ic_types@host_link cit
       WHERE cit.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ci_relmast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ci_relmast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ci_relmast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ci_relmast
       SELECT rm.cod_rel,
              TRIM(rm.txt_relation) txt_relation,
              rm.dat_last_mnt
       FROM ci_relmast@host_link rm
       WHERE rm.flg_mnt_status = 'A';
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

  PROCEDURE refresh_co_old_new_cust
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_co_old_new_cust';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM co_old_x_new_custid;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO co_old_x_new_custid
       SELECT nc.cod_cc_homebrn,
              TRIM(nc.cod_old_custid) cod_old_custid,
              nc.cod_new_custid,
              TRIM(nc.flg_cust_typ) flg_cust_typ,
              TRIM(nc.flg_ic_typ) flg_ic_typ,
              TRIM(nc.cod_old_introducerid)cod_old_introducerid,
              TRIM(nc.cod_last_mnt_makerid) cod_last_mnt_makerid,
              TRIM(nc.cod_last_mnt_chkrid) cod_last_mnt_chkrid
       FROM co_old_x_new_custid@host_link nc;
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

  PROCEDURE refresh_cs_ho_blacklist
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_cs_ho_blacklist';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM cs_ho_blacklist_info;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO cs_ho_blacklist_info
       SELECT bc.cod_cust_id,
              TRIM(bc.nam_cust_full) nam_cust_full,
              TRIM(bc.cod_cust_natl_id) cod_cust_natl_id,
              TRIM(bc.cod_agency) cod_agency,
              bc.blacklst_dat_from,
              bc.blacklst_dat_to,
              TRIM(bc.txt_blacklst_desc1) txt_blacklst_desc1,
              bc.cod_last_mnt_makerid,
              bc.cod_last_mnt_chkrid,
              bc.dat_last_mnt
       FROM cs_ho_blacklist_info@host_link bc
       WHERE bc.flg_mnt_status = 'A';
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

  PROCEDURE refresh_cs_prop_log
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_cs_prop_log';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM cs_propagate_log;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM cs_propagate_log@host_link pt
       WHERE pt.flg_process = 'Y';
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

  PROCEDURE refresh_gltm_glmaster
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gltm_glmaster';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gltm_glmaster;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO gltm_glmaster
       SELECT TRIM(gg.gl_code) gl_code,
              TRIM(gg.gl_desc) gl_desc,
              TRIM(gg.parent_gl) parent_gl,
              gg.leaf,
              gg.type,
              gg.category,
              TRIM(gg.ulti_parent) ulti_parent,
              gg.dat_last_mnt
       FROM gltm_glmaster@host_link gg
       WHERE gg.flg_mnt_status = 'A';
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

  PROCEDURE refresh_gl_detemp
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gl_detemp';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gl_detemp;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM gl_detemp@host_link gd;
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

  PROCEDURE refresh_gl_master
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_gl_master';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM gl_master;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO gl_master
       SELECT gm.cod_cc_brn,
              gm.cod_gl_acct,
              gm.cod_gl_acct_ccy,
              gm.cod_gl_type,
              TRIM(gm.nam_gl_code) nam_gl_code,
              gm.dat_acct_opened,
              gm.dat_txn_posting,
              gm.dat_last_mnt
       FROM gl_master@host_link gm
       WHERE gm.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ln_acct_attributes
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_attributes';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_attributes;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_attributes
       SELECT TRIM(aa.cod_acct_no) cod_acct_no,
              aa.cod_acct_off,
              aa.flg_include_to_date,
              aa.flg_negotiated,
              aa.flg_legal
       FROM ln_acct_attributes@host_link aa
       WHERE aa.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ln_acct_balances
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_balances';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_balances;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_acct_balances@host_link ab;
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
       FROM ln_acct_cbr_dtls@host_link lac
       WHERE lac.flg_mnt_status = 'A';
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
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_acct_dtls@host_link a
       WHERE a.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ln_acct_int_bal_dt
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_int_bal_dt';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_int_balance_dtls;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_acct_int_balance_dtls@host_link bd;
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

  PROCEDURE refresh_ln_acct_sched_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_sched_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_schedule_detls;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls
       SELECT TRIM(s.cod_acct_no) cod_acct_no,
              s.dat_stage_start,
              s.ctr_instal,
              s.date_instal,
							s.rat_int,
              s.amt_principal,
              s.amt_interest,
              s.amt_princ_bal
       FROM ln_acct_schedule_detls@host_link s;
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

  PROCEDURE refresh_ln_acct_sdh
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_acct_sdh';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_acct_schedule_detls_hist;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_acct_schedule_detls_hist
       SELECT TRIM(dh.cod_acct_no) cod_acct_no,
              dh.dat_stage_start,
              dh.dat_amd,
              dh.ctr_instal,
              dh.date_instal,
              dh.amt_principal,
              dh.amt_interest,
              dh.amt_princ_bal
       FROM ln_acct_schedule_detls_hist@host_link dh;
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

  PROCEDURE refresh_ln_arrear_txn_hist
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_arrear_txn_hist';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrear_txn_hist;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_arrear_txn_hist@host_link th;
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

  PROCEDURE refresh_ln_arrears_table
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_arrears_table';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_arrears_table;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM ln_arrears_table@host_link la;
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

  PROCEDURE refresh_ln_dedn_plan
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_dedn_plan';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_dedn_plan;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_dedn_plan
       SELECT dp.cod_plan,
              dp.cod_deduction,
              dp.cod_dedn_ccy,
              TRIM(dp.nam_plan) nam_plan,
              dp.dat_last_mnt
       FROM ln_dedn_plan@host_link dp
       WHERE dp.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ln_prod_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_prod_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_prod_mast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_prod_mast
       SELECT pm.cod_prod,
              pm.cod_ccy,
              pm.cod_prod_term_typ,
              pm.cod_dedn_plan,
              TRIM(pm.cod_class_prod) cod_class_prod,
              TRIM(pm.nam_product) nam_product,
              TRIM(pm.nam_ccy_short) nam_ccy_short,
              pm.cod_gl_ast_bal,
              pm.cod_gl_susp_ast_bal,
              pm.dat_last_mnt
       FROM ln_prod_mast@host_link pm
       WHERE pm.flg_mnt_status = 'A';
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

  PROCEDURE refresh_ln_sched_types
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_ln_sched_types';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM ln_sched_types;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO ln_sched_types
       SELECT s.cod_sched_type,
              s.cod_prod,
              s.ctr_stage_no,
              TRIM(s.nam_sched_type) nam_sched_type,
              s.dat_last_mnt
       FROM ln_sched_types@host_link s
       WHERE s.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_applicants
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applicants';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applicants;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applicants
       SELECT a.apl_aad_id,
              a.apl_cus_id,
              a.apl_relation_type_cd,
              a.apl_gender_cd,
              TRIM(a.apl_first_name) apl_first_name,
              TRIM(a.apl_middle_name) apl_middle_name,
              TRIM(a.apl_last_name) apl_last_name,
              a.dat_last_mnt
       FROM or_applicants@host_link a
       WHERE a.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_applicants_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applicants_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applicants_mmdd;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_applicants_mmdd
       SELECT a.apl_aad_id,
              a.apl_cus_id,
              a.apl_relation_type_cd,
              a.apl_gender_cd,
              TRIM(a.apl_first_name) apl_first_name,
              TRIM(a.apl_middle_name) apl_middle_name,
              TRIM(a.apl_last_name) apl_last_name,
              a.dat_last_mnt
       FROM or_applicants_mmdd@host_link a
       WHERE a.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_applications
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applications';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applications;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM or_applications@host_link ap
       WHERE ap.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_applications_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_applications_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_applications_mmdd;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM or_applications_mmdd@host_link ap
       WHERE ap.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_app_assets
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_app_assets';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_application_assets;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_application_assets
       SELECT aa.ase_aad_id,
              TRIM(aa.ase_identification_nbr) ase_identification_nbr,
              aa.ase_asb_type,
              aa.dat_last_mnt
       FROM or_application_assets@host_link aa
       WHERE aa.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_app_assets_md
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_app_assets_md';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_application_assets_mmdd;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_application_assets_mmdd
       SELECT aa.ase_aad_id,
              TRIM(aa.ase_identification_nbr) ase_identification_nbr,
              aa.ase_asb_type,
              aa.dat_last_mnt
       FROM or_application_assets_mmdd@host_link aa
       WHERE aa.flg_mnt_status = 'A';
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

  PROCEDURE refresh_or_lookups
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_or_lookups';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM or_lookups;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO or_lookups
       SELECT l.lkc_lkt_type,
              l.lkc_code,
              l.lkc_sub_code,
              l.lkc_sort,
              l.lkc_desc,
              l.dat_last_mnt
       FROM or_lookups@host_link l
       WHERE l.flg_mnt_status = 'A';
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

  PROCEDURE refresh_sm_temp_profile
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_sm_temp_profile';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM sm_temp_profile;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO sm_temp_profile
       SELECT tm.cod_user_templ,
              tm.cod_branch,
              tm.cod_category_user,
              tm.cod_user_level,
              tm.cod_user_access,
              TRIM(tm.nam_template) nam_template,
              tm.dat_last_mnt
       FROM sm_temp_profile@host_link tm
       WHERE tm.flg_mnt_status = 'A';
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

  PROCEDURE refresh_sm_user_profile
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_sm_user_profile';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM sm_user_profile;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

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
       FROM sm_user_profile@host_link up
       WHERE up.flg_mnt_status = 'A';
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

  PROCEDURE refresh_udf_acct_log_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_acct_log_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_acct_log_details;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_acct_log_details
       SELECT TRIM(ual.cod_acct_no) cod_acct_no,
              ual.cod_field_tag,
              TRIM(ual.field_value) field_value,
              ual.ctr_udf_order
       FROM udf_acct_log_details@host_link ual
       WHERE ual.flg_mnt_status = 'A';
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

  PROCEDURE refresh_udf_cust_log_dtls
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_cust_log_dtls';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_cust_log_details;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_cust_log_details
       SELECT cld.cod_cust_id,
              cld.cod_field_tag,
              TRIM(cld.field_value) field_value,
              cld.ctr_udf_order
       FROM udf_cust_log_details@host_link cld
       WHERE cld.flg_mnt_status = 'A';
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

  PROCEDURE refresh_udf_lov_mast
  IS
    v_started DATE;
    v_proc_name VARCHAR2(50) := 'refresh_udf_lov_mast';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM udf_lov_mast;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO udf_lov_mast
       SELECT ulm.cod_field_tag,
              ulm.ctr_lov_order,
              TRIM(ulm.txt_lov_value) txt_lov_value,
              TRIM(ulm.txt_lov_name) txt_lov_name
       FROM udf_lov_mast@host_link ulm
       WHERE ulm.flg_mnt_status = 'A';
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

  PROCEDURE update_rec_denm
  IS
    v_started DATE;
    v_proc_name VARCHAR2(20) := 'update_rec_denm';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;
       DELETE FROM rpt_rec_denm_mmdd;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows deleted';

       INSERT INTO rpt_rec_denm_mmdd
       SELECT rdm.cod_brn,
              rdm.dat_post,
              rdm.user_no,
              rdm.cod_denm_cur,
              rdm.tot_amount
       FROM rec_denm_mmdd@brn_link rdm;
       v_row_count_processed := v_row_count_processed || '; ' || TRIM(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RD',
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
                   log_manager.add_log('MIG_RD_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

  PROCEDURE update_rec_txnlog
  IS -- job runs at -> TRUNC(SYSDATE) + 13/12
    v_started DATE;
    v_proc_name VARCHAR2(20) := 'update_rec_txnlog';
    v_row_count_processed VARCHAR2(50) := '';
  BEGIN
       v_started := SYSDATE;

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
       FROM rec_txnlog@brn_link rt,
            (SELECT MAX(rrt.tim_sys) max_date
             FROM rpt_rec_txnlog rrt) md
       WHERE rt.tim_sys > md.max_date
             AND rt.tim_sys < trunc(SYSDATE)
       ORDER BY rt.tim_sys;
       v_row_count_processed := TRIM(SQL%ROWCOUNT) || ' rows inserted';

       log_manager.add_log('MIG_RT',
                           v_proc_name,
                           NULL,
                           v_started,
                           SYSDATE,
                           'process succeed: ' || v_row_count_processed,
                           1);

       IF TO_CHAR(SYSDATE, 'mmdd') = '0102'
       THEN
           update_rec_denm;
       END IF;

       refresh_mv_tables(2);

       EXCEPTION
         WHEN OTHERS THEN
              BEGIN
                   ROLLBACK;
                   log_manager.add_log('MIG_RT_ERR',
                                       v_proc_name,
                                       NULL,
                                       v_started,
                                       SYSDATE,
                                       'Error message : ' || SQLERRM,
                                       1);
              END;
  END;

END;
/
