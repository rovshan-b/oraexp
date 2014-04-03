CREATE OR REPLACE PACKAGE reports_gl
IS

  -- Author  : Vasiliy Smirnov
  -- Created : 21.07.2009
  -- Purpose :

  TYPE result_cursor IS REF CURSOR;

  FUNCTION get_trial_balance_gl1011 (p_date DATE,
                                     p_branch_id NUMBER,
                                     p_account VARCHAR2)
  RETURN result_cursor;

  FUNCTION get_trial_balance_gl1012 (p_start_date DATE,
                                     p_end_date DATE,
                                     p_branch_id NUMBER,
                                     p_account VARCHAR2)
  RETURN result_cursor;
  
  FUNCTION get_non_posted_txn_gl1022 (p_start_date DATE,
                                       p_end_date DATE,
                                       p_branch_id NUMBER,
                                       p_date_type VARCHAR2,
                                       p_account VARCHAR2)
  RETURN result_cursor;

  -- used in
  -- GL1020 - GL account statement
  FUNCTION get_acc_balance_and_turnover(p_start_date DATE,
                                        p_end_date DATE,
                                        p_date_type VARCHAR2,
                                        p_account VARCHAR2,
                                        p_branch_id NUMBER,
                                        p_acy_short VARCHAR2)
  RETURN result_cursor;


  FUNCTION get_gl_account_desc(p_account VARCHAR2) RETURN result_cursor;

  -- GL1010 - Trial balance
  FUNCTION gl1010(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_branch_id NUMBER,
                  p_account VARCHAR2)
  RETURN result_cursor;

  -- GL1020 - GL account statement
  FUNCTION gl1020 (p_start_date DATE,
                   p_end_date DATE,
                   p_branch_id NUMBER,
                   p_date_type VARCHAR2,
                   p_account VARCHAR2,
                   p_acy_short VARCHAR2)
  RETURN result_cursor;

  -- GL1030 - Loan transactions
  FUNCTION gl1030(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_account VARCHAR2)
  RETURN result_cursor;

  -- GL1040 - Chart of accounts
  FUNCTION gl1040(p_account VARCHAR2,
                  p_account_name VARCHAR2)
  RETURN result_cursor;
  
  -- GL1050 - Balance sheet (Bank wise)
  FUNCTION gl1050(p_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1051 - Balance sheet (Branch wise)
  FUNCTION gl1051(p_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1052 - Balance sheet (Bank wise - new source)
  FUNCTION gl1052(p_year_month VARCHAR2) RETURN result_cursor;

  -- GL1060 - Profit and loss (Bank wise)
  FUNCTION gl1060(p_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1061 - Profit and loss (Branch wise)
  FUNCTION gl1061(p_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1062 - Profit and loss (Bank wise - new source)
  FUNCTION gl1062(p_year_month VARCHAR2) RETURN result_cursor;

  -- GL1070 - Transit accounts
  FUNCTION gl1070(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1080 - Batch report
  FUNCTION gl1080(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_batch_no NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor;
  
  -- GL1090 - Profit and loss report
  FUNCTION gl1090(p_from_month VARCHAR2,
                  p_to_month VARCHAR2,
                  p_lang_id NUMBER := 0) -- 0 - english, 1 - azeri
  RETURN result_cursor;
  
  -- GL1091 - Branch profitability report
  FUNCTION gl1091(p_from_month VARCHAR2,
                  p_to_month VARCHAR2,
                  p_lang_id NUMBER := 0) -- 0 - english, 1 - azeri
  RETURN result_cursor;

  -- GL1100 - Cash reconciliation
  FUNCTION gl1100(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor;

  -- GL1120 - GLM01 transactions
  FUNCTION gl1120(p_start_date DATE,
                  p_end_date DATE)
  RETURN data_types.result_cursor;

END;
/
CREATE OR REPLACE PACKAGE BODY reports_gl
IS

  FUNCTION get_trial_balance_gl1011(p_date DATE,
                                    p_branch_id NUMBER,
                                    p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR

       select
        gl.cod_gl_acct,
        gl.nam_gl_code,
        gl.cod_gl_acct_ccy,
          decode(
            g2.category,
            1, 'ASSETS',
            2, 'LIABILITIES',
            3, 'INCOME',
            4, 'EXPENSE',
            5, 'CONTINGENT ASSETS',
            6, 'CONTINGENT LIABILITIES',
            7, 'MEMO',
            8, 'POSITION',
            9, 'POSITION EQUIVALENT'
          ) as ACCOUNT_TYPE,
        nvl(sum(bl.acy_closing_bal),0) as balance_acy,
        (CASE WHEN g2.category IN(3, 4)
              THEN (CASE WHEN nvl(sum(bl.acy_closing_bal),0) != 0
                          THEN nvl(sum(bl.lcy_closing_bal),0)
                          ELSE 0
                     END)
              ELSE nvl(sum(bl.lcy_closing_bal),0)
         END) as balance_lcy
        --nvl(sum(bl.lcy_closing_bal),0) as balance_lcy
      from
        (
            select
               b1.branch_code,
               b1.account,
               b1.acc_ccy,
               b2.acy_closing_bal,
               b2.lcy_closing_bal
            from
              (
                  select
                     branch_code,
                     account,
                     acc_ccy,
                     max(bkg_date) as max_bkg_date
                  from
                     actb_accbal_history/*@host_link*/
                  where
                     bkg_date <= p_date
                     and branch_code = nvl(p_branch_id, branch_code)
                  group by
                     branch_code,
                     account,
                     acc_ccy
               ) b1,
               actb_accbal_history/*@host_link*/ b2
            where
               b1.branch_code = b2.branch_code
               and b1.account = b2.account
               and b1.acc_ccy = b2.acc_ccy
               and b1.max_bkg_date = b2.bkg_date
        ) bl,
        (
          select
            gl.cod_gl_acct,
            gl.nam_gl_code,
            gl.cod_cc_brn,
            cc.nam_ccy_short as cod_gl_acct_ccy
          from
            gl_master/*@host_link*/ gl,
            ba_ccy_code/*@host_link*/ cc
          where
            /*gl.flg_mnt_status = 'A'
            and cc.flg_mnt_status = 'A'
            and */gl.cod_gl_acct_ccy = cc.cod_ccy
        ) gl,
        gltm_glmaster/*@host_link*/ g2
      where
        bl.account (+) = gl.cod_gl_acct
        and bl.branch_code (+) = gl.cod_cc_brn
        and bl.acc_ccy (+) = gl.cod_gl_acct_ccy
        --and g2.flg_mnt_status = 'A'
        and g2.gl_code = gl.cod_gl_acct
        and gl.cod_gl_acct like nvl(p_account, '') || '%'
      group by
        gl.cod_gl_acct,
        gl.nam_gl_code,
        gl.cod_gl_acct_ccy,
        g2.category
      order by
        gl.cod_gl_acct,
        gl.cod_gl_acct_ccy;

       RETURN c_result;
  END;


  FUNCTION get_trial_balance_gl1012(p_start_date DATE,
                                    p_end_date DATE,
                                    p_branch_id NUMBER,
                                    p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR

       select distinct
          nvl(t1.gl_code, t2.gl_code) as gl_code,
          nvl(t1.gl_desc, t2.gl_desc) as gl_desc,
          decode(
            nvl(t1.category, t2.category),
            1, 'ASSETS',
            2, 'LIABILITIES',
            3, 'INCOME',
            4, 'EXPENSE',
            5, 'CONTINGENT ASSETS',
            6, 'CONTINGENT LIABILITIES',
            7, 'MEMO',
            8, 'POSITION',
            9, 'POSITION EQUIVALENT'
          ) as ACCOUNT_TYPE,
          nvl(t1.balance,0) as opening_balance_lcy,
          nvl(t2.balance,0) as closing_balance_lcy,
          nvl(t2.balance,0) - nvl(t1.balance,0) as turnover_lcy
       from
       (
         select
            gl.gl_code,
            gl.gl_desc,
            gl.category,
            nvl(sum(bl.lcy_closing_bal),0) as balance
          from
            (
                select
                   b1.branch_code,
                   b1.account,
                   b1.acc_ccy,
                   b2.lcy_closing_bal
                from
                  (
                      select
                         branch_code,
                         account,
                         acc_ccy,
                         max(bkg_date) as max_bkg_date
                      from
                         actb_accbal_history/*@host_link*/
                      where
                         bkg_date <= p_start_date
                         and branch_code = nvl(p_branch_id, branch_code)
                      group by
                         branch_code,
                         account,
                         acc_ccy
                   ) b1,
                   actb_accbal_history/*@host_link*/ b2
                where
                   b1.branch_code = b2.branch_code
                   and b1.account = b2.account
                   and b1.acc_ccy = b2.acc_ccy
                   and b1.max_bkg_date = b2.bkg_date
            ) bl,
            gltm_glmaster/*@host_link*/ gl
          where
            bl.account (+) = gl.gl_code
            --and gl.flg_mnt_status = 'A'
            and gl.leaf = 'Y'
            and gl.gl_code like nvl(p_account, '') || '%'
          group by
            gl.gl_code,
            gl.gl_desc,
            gl.category
          order by
            gl.gl_code
       ) t1
       full outer join
       (
         select
            gl.gl_code,
            gl.gl_desc,
            gl.category,
            nvl(sum(bl.lcy_closing_bal),0) as balance
          from
            (
                select
                   b1.branch_code,
                   b1.account,
                   b1.acc_ccy,
                   b2.lcy_closing_bal
                from
                  (
                      select
                         branch_code,
                         account,
                         acc_ccy,
                         max(bkg_date) as max_bkg_date
                      from
                         actb_accbal_history/*@host_link*/
                      where
                         bkg_date <= p_end_date
                         and branch_code = nvl(p_branch_id, branch_code)
                      group by
                         branch_code,
                         account,
                         acc_ccy
                   ) b1,
                   actb_accbal_history/*@host_link*/ b2
                where
                   b1.branch_code = b2.branch_code
                   and b1.account = b2.account
                   and b1.acc_ccy = b2.acc_ccy
                   and b1.max_bkg_date = b2.bkg_date
            ) bl,
            gltm_glmaster/*@host_link*/ gl
          where
            bl.account (+) = gl.gl_code
            --and gl.flg_mnt_status = 'A'
            and gl.leaf = 'Y'
            and gl.gl_code like nvl(p_account, '') || '%'
          group by
            gl.gl_code,
            gl.gl_desc,
            gl.category
          order by
            gl.gl_code
       ) t2 on
         t1.gl_code = t2.gl_code;

       RETURN c_result;
  END;
  
  -- GL1022 - Non-posted account transactions - Real time
  FUNCTION get_non_posted_txn_gl1022 (p_start_date DATE,
                                         p_end_date DATE,
                                         p_branch_id NUMBER,
                                         p_date_type VARCHAR2,
                                         p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
 
      select
      tr.cod_cc_brn || ' - ' || br.nam_branch as branch,
      to_char(tr.cod_txn_mnemonic) as trn_code,
      mn.txt_txn_desc,
      tr.txt_txn_desc as txt_txn_desc2,
      tr.dat_txn as txn_init_date,
      tr.dat_posting as trn_dt,
      tr.dat_value as value_dt,
      tr.cod_txn_ccy ac_ccy,
      case tr.cod_drcr
        when 'D' then
          tr.amt_txn_fcy
        else
          0
      end as fcy_amount_dr,
      case tr.cod_drcr
        when 'C' then
          tr.amt_txn_fcy
        else
          0
      end as fcy_amount_cr,
      case tr.cod_drcr
        when 'D' then
          tr.amt_txn_lcy
        else
          0
      end as lcy_amount_dr,
      case tr.cod_drcr
        when 'C' then
          tr.amt_txn_lcy
        else
          0
      end as lcy_amount_cr,
      to_char(tr.cod_userno) as user_id,
      to_char(tr.cod_checker) as auth_id
    from
      gl_detemp tr,
      ba_cc_brn_mast br,
      ba_txn_mnemonic mn
    where
      tr.cod_cc_brn = br.cod_cc_brn
      and tr.cod_txn_mnemonic = mn.cod_txn_mnemonic(+)
      and tr.cod_cc_brn = nvl(p_branch_id, tr.cod_cc_brn) 
      and trim(tr.cod_acct_no) = trim(p_account)
      and decode(p_date_type,
                 'value_date', trunc(tr.dat_value),
                 'tran_date', trunc(tr.dat_txn),
                 'post_date', trunc(tr.dat_posting)) between p_start_date and p_end_date     
    union all
    select
      tr.cod_brn || ' - ' || br.nam_branch,
      tr.cod_tran,
      mn.txt_txn_desc,
      tr.buf_stat_scr,
      tr.tim_sys,
      tr.dat_post,
      tr.dat_val,
      tr.cod_tran_cur,
      case tr.cod_fnd_typ
        when 'D' then
          tr.amt_tran_tcye
        else
          0
      end,
      case tr.cod_fnd_typ
        when 'C' then
          tr.amt_tran_tcye
        else
          0
      end,
      case tr.cod_fnd_typ
        when 'D' then
          tr.amt_tran_lcye
        else
          0
      end,
      case tr.cod_fnd_typ
        when 'C' then
          tr.amt_tran_lcye
        else
          0
      end,
      tr.user_id,
      tr.id_auth
    from
      rec_txnlog@brn_link tr,
      ba_cc_brn_mast br,
      ba_txn_mnemonic mn
    where
      tr.cod_brn = br.cod_cc_brn
      and tr.cod_tran = mn.cod_txn_mnemonic(+)
      and tr.cod_brn = nvl(p_branch_id, tr.cod_brn) 
      and trim(tr.id_acct) = trim(p_account)
      and decode(p_date_type,
                 'value_date', trunc(tr.dat_val),
                 'tran_date', trunc(tr.tim_sys),
                 'post_date', trunc(tr.dat_post)) between p_start_date and p_end_date; 
    
    RETURN c_result;
  END;
  -- GL1022 - Non-posted account transactions - Real time

  -- used in
  -- GL1020 - GL account statement
  FUNCTION get_acc_balance_and_turnover(p_start_date DATE,
                                        p_end_date DATE,
                                        p_date_type VARCHAR2,
                                        p_account VARCHAR2,
                                        p_branch_id NUMBER,
                                        p_acy_short VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_gl_start_date DATE := NULL;
  BEGIN
       SELECT (to_date(rs.setting_value, 'yyyy-mm-dd')-1) INTO v_gl_start_date
       FROM rpt_report_settings rs
       WHERE rs.setting_key = 'MIGRATION_DATE';

       OPEN c_result FOR
            SELECT ah.ac_ccy ccy, --ah.*
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) >= p_start_date
                             THEN DECODE(ah.drcr_ind, 'D', ah.fcy_amount, 0)
                             ELSE 0
                        END) fcy_turnover_dr,
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) >= p_start_date
                             THEN DECODE(ah.drcr_ind, 'C', ah.fcy_amount, 0)
                             ELSE 0
                        END) fcy_turnover_cr,
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) >= p_start_date
                             THEN DECODE(ah.drcr_ind, 'D', ah.lcy_amount, 0)
                             ELSE 0
                        END) lcy_turnover_dr,
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) >= p_start_date
                             THEN DECODE(ah.drcr_ind, 'C', ah.lcy_amount, 0)
                             ELSE 0
                        END) lcy_turnover_cr,
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) < p_start_date
                             THEN DECODE(ah.drcr_ind, 'C', ah.fcy_amount, -ah.fcy_amount)
                             ELSE 0
                        END) fcy_opening_balance,
                    SUM(DECODE(ah.drcr_ind, 'C', ah.fcy_amount, -ah.fcy_amount)) fcy_closing_balance,
                    SUM(CASE WHEN DECODE(p_date_type,
                                         'value_date', trunc(ah.value_dt),
                                         'tran_date', trunc(ah.txn_init_date),
                                         'post_date', trunc(ah.trn_dt)) < p_start_date
                             THEN DECODE(ah.drcr_ind, 'C', ah.lcy_amount, -ah.lcy_amount)
                             ELSE 0
                        END) lcy_opening_balance,
                    SUM(DECODE(ah.drcr_ind, 'C', ah.lcy_amount, -ah.lcy_amount)) lcy_closing_balance
            FROM actb_history ah,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT LPAD(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND ah.ac_no LIKE (TRIM(p_account) || '%')
                  AND ah.ac_ccy = nvl(p_acy_short, ah.ac_ccy)
                  AND DECODE(p_date_type,
                             'value_date', trunc(ah.value_dt),
                             'tran_date', trunc(ah.txn_init_date),
                             'post_date', trunc(ah.trn_dt)) BETWEEN trunc(v_gl_start_date) AND trunc(p_end_date)
            GROUP BY ah.ac_ccy
            ORDER BY 1;

       RETURN c_result;
  END;
  -- GL1020 - GL account statement

  FUNCTION get_gl_account_desc(p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (gg.gl_code || ' - ' || gg.gl_desc) gl_desc
            FROM gltm_glmaster gg
            WHERE gg.gl_code = TRIM(p_account);

       RETURN c_result;
  END;

  -- GL1010 - Trial balance
  FUNCTION gl1010(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_branch_id NUMBER,
                  p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ah.ac_no gl_account_no,
                   gg.gl_desc,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch,
                   ah.ac_ccy ccy,
                   SUM(CASE WHEN trunc(ah.value_dt) < trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)
                            ELSE 0
                       END) opening_balance_fcy,
                   SUM(CASE WHEN trunc(ah.value_dt) < trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)
                            ELSE 0
                       END) opening_balance_lcy,
                   SUM(CASE WHEN trunc(ah.value_dt) >= trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'D', ah.fcy_amount, 0)
                            ELSE 0
                       END) debit_turnover_fcy,
                   SUM(CASE WHEN trunc(ah.value_dt) >= trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'D', ah.lcy_amount, 0)
                            ELSE 0
                       END) debit_turnover_lcy,
                   SUM(CASE WHEN trunc(ah.value_dt) >= trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'C', ah.fcy_amount, 0)
                            ELSE 0
                       END) credit_turnover_fcy,
                   SUM(CASE WHEN trunc(ah.value_dt) >= trunc(p_start_date)
                            THEN decode(ah.drcr_ind, 'C', ah.lcy_amount, 0)
                            ELSE 0
                       END) credit_turnover_lcy,
                   SUM(decode(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)) closing_balance_fcy,
                   SUM(decode(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) closing_balance_lcy
            FROM actb_history ah,
                 ba_cc_brn_mast bm,
                 gltm_glmaster gg,
                 (SELECT nvl((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT lpad(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE bm.cod_cc_brn = ah.ac_branch
                  AND gg.gl_code = ah.ac_no
                  --
                  AND INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND ah.ac_no LIKE (NVL(TRIM(p_account), ah.ac_no) || '%')
                  AND DECODE(p_date_type,
                             'value_date', ah.value_dt,
                             'tran_date', ah.txn_init_date,
                             'post_date', ah.trn_dt) <= p_end_date
            GROUP BY ah.ac_no,
                     gg.gl_desc,
                     (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch),
                     ah.ac_ccy
            ORDER BY 1, 3, 4;

       RETURN c_result;
  END;
  -- GL1010 - Trial balance

  -- GL1020 - GL account statement
  FUNCTION gl1020 (p_start_date DATE,
                   p_end_date DATE,
                   p_branch_id NUMBER,
                   p_date_type VARCHAR2,
                   p_account VARCHAR2,
                   p_acy_short VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch,
                    ah.trn_code,
                    tm.txt_txn_desc,
                    ah.txt_txn_desc txt_txn_desc2,
                    --ah.ac_no gl_account,
                    --ah.related_customer,
                    DECODE(c.cod_cust_id, NULL, NULL, (TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full)) related_customer,
                    DECODE(g.cod_group, NULL, NULL, (g.cod_group || ' - ' || g.nam_group)) group_name,
                    ah.related_account related_account,
                    ah.batch_no,
                    ah.instrument_code,
                    ah.txn_init_date,
                    ah.trn_dt,
                    ah.value_dt,
                    ah.ac_ccy,
                    DECODE(ah.drcr_ind, 'D', ah.fcy_amount, 0) fcy_amount_dr,
                    DECODE(ah.drcr_ind, 'C', ah.fcy_amount, 0) fcy_amount_cr,
                    DECODE(ah.drcr_ind, 'D', ah.lcy_amount, 0) lcy_amount_dr,
                    DECODE(ah.drcr_ind, 'C', ah.lcy_amount, 0) lcy_amount_cr,
                    ah.user_id,
                    DECODE(up.cod_user_id, NULL, NULL, (up.nam_user || ' ( ' || up.cod_user_id || ' )')) user_name,
                    ah.auth_id,
                    DECODE(ap.cod_user_id, NULL, NULL, (ap.nam_user || ' ( ' || ap.cod_user_id || ' )')) author_name
            FROM actb_history ah,
                 ba_cc_brn_mast bm,
                 ba_txn_mnemonic tm,
                 ci_custmast c,
                 sm_user_profile up,
                 sm_user_profile ap,
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
            WHERE ah.ac_branch = bm.cod_cc_brn
                  AND ah.trn_code = tm.cod_txn_mnemonic(+)
                  AND c.cod_cust_id(+) = ah.related_customer
                  AND up.cod_user_id(+) = ah.user_id
                  AND ap.cod_user_id(+) = ah.auth_id
                  AND g.cod_acct_no(+) = ah.related_account
                  --
                  /*AND bm.flg_mnt_status = 'A'
                  AND tm.flg_mnt_status(+) = 'A'*/
                  --
                  --AND ah.ac_branch = nvl(p_branch_id, ah.ac_branch)
                  AND INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND ah.ac_no LIKE (TRIM(p_account) || '%')
                  AND ah.ac_ccy = NVL(p_acy_short, ah.ac_ccy)
                  AND DECODE(p_date_type,
                             'value_date', trunc(ah.value_dt),
                             'tran_date', trunc(ah.txn_init_date),
                             'post_date', trunc(ah.trn_dt)) BETWEEN p_start_date AND p_end_date
            ORDER BY DECODE(p_date_type,
                            'value_date', trunc(ah.value_dt),
                            'tran_date', trunc(ah.txn_init_date),
                            'post_date', trunc(ah.trn_dt));

       RETURN c_result;
  END;
  -- GL1020 - GL account statement

  -- GL1030 - Loan transactions
  FUNCTION gl1030(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_account VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    --
    v_account_no VARCHAR2(20) := report_methods.get_account_no(p_account);
  BEGIN
       OPEN c_result FOR
            SELECT (CASE WHEN ah.trn_code IN (1003,1011,1017,1019,1085,1087,1413)
                         THEN 'Disbursement transactions'
                         WHEN ah.trn_code IN (3260,3261,3262,3263,3264,3265,4260,4262,4263)
                         THEN 'Interest accrual and income transactions'
                         WHEN ah.trn_code IN (1403,1404,1405,1407,1410,1081,1082,1083,1084,1086,3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097)
                         THEN 'Payment and appropriation transactions'
                         WHEN ah.trn_code IN (3400,4400)
                         THEN 'Loan provision transactions'
                         ELSE 'Other transactions'
                    END) txn_category,
                   (CASE WHEN ah.trn_code IN (1003,1011,1017,1019,1085,1087,1413)
                         THEN 1
                         WHEN ah.trn_code IN (3260,3261,3262,3263,3264,3265,4260,4262,4263)
                         THEN 3
                         WHEN ah.trn_code IN (1403,1404,1405,1407,1410,1081,1082,1083,1084,1086,3082,3083,3084,3085,3086,3087,3088,3089,3091,3092,3093,3094,3095,3096,3097)
                         THEN 2
                         WHEN ah.trn_code IN (3400,4400)
                         THEN 4
                         ELSE 5
                    END) txn_category_order,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch,
                   ah.ac_no,
                   ah.trn_code,
                   tm.txt_txn_desc,
                   ah.txt_txn_desc txt_txn_desc2,
                   ah.related_customer,
                   ah.txn_init_date,
                   ah.trn_dt,
                   ah.value_dt,
                   ah.ac_ccy,
                   DECODE(ah.drcr_ind, 'D', ah.fcy_amount, 0) fcy_amount_dr,
                   DECODE(ah.drcr_ind, 'C', ah.fcy_amount, 0) fcy_amount_cr,
                   DECODE(ah.drcr_ind, 'D', ah.lcy_amount, 0) lcy_amount_dr,
                   DECODE(ah.drcr_ind, 'C', ah.lcy_amount, 0) lcy_amount_cr,
                   ah.user_id,
                   ah.auth_id
            FROM actb_history ah,
                 ba_cc_brn_mast bm,
                 ba_txn_mnemonic tm
            WHERE ah.ac_branch = bm.cod_cc_brn
                  AND ah.trn_code = tm.cod_txn_mnemonic(+)
                  --AND bm.flg_mnt_status = 'A'
                  --AND tm.flg_mnt_status(+) = 'A'
                  AND ah.related_account = v_account_no
                  AND DECODE(p_date_type,
                             'value_date', TRUNC(ah.value_dt),
                             'tran_date', TRUNC(ah.txn_init_date),
                             'post_date', TRUNC(ah.trn_dt)) BETWEEN p_start_date AND p_end_date
            ORDER BY txn_category_order,
                     DECODE(p_date_type,
                            'value_date', TRUNC(ah.value_dt),
                            'tran_date', TRUNC(ah.txn_init_date),
                            'post_date', TRUNC(ah.trn_dt));

       RETURN c_result;
  END;
  -- GL1030 - Loan transactions

  -- GL1040 - Chart of accounts
  FUNCTION gl1040(p_account VARCHAR2,
                  p_account_name VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
        SELECT gl.gl_code,
               gl.gl_desc,
               DECODE(gl.category,
                      1, 'ASSETS',
                      2, 'LIABILITIES',
                      3, 'INCOME',
                      4, 'EXPENSE',
                      5, 'CONTINGENT ASSETS',
                      6, 'CONTINGENT LIABILITIES',
                      7, 'MEMO',
                      8, 'POSITION',
                      9, 'POSITION EQUIVALENT') account_type
        FROM gltm_glmaster /*@host_link*/ gl
        WHERE gl.leaf = 'Y'
              --
              AND gl.gl_code LIKE NVL(p_account, '') || '%'
              AND LOWER(gl.gl_desc) LIKE '%' || NVL(LOWER(p_account_name), '') || '%'
        GROUP BY gl.gl_code,
                 gl.gl_desc,
                 gl.category
        ORDER BY gl.gl_code;

       RETURN c_result;
  END;
  -- GL1040 - Chart of accounts
  
  -- GL1050 - Balance sheet (Bank wise)
  FUNCTION gl1050(p_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_gl_parent_codes VARCHAR2(200) := '';
  BEGIN
       SELECT --MAX(DECODE(rs.setting_key, 'SUSP_GL_CODE_LIST', rs.setting_value)) gl_codes,
              MAX(DECODE(rs.setting_key, 'SUSP_GL_PARENT_CODE_LIST', rs.setting_value))
       INTO v_gl_parent_codes
       FROM rpt_report_settings rs
       WHERE rs.setting_key IN(/*'SUSP_GL_CODE_LIST', */'SUSP_GL_PARENT_CODE_LIST');

       OPEN c_result FOR
            WITH w_ah AS (SELECT /*+ full(ah) */ --/*+ index(ah actb_history_idx1) */
                                 ah.ac_no,
                                 ah.ac_ccy,
                                 SUM(DECODE(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)) fcy_balance,
                                 SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) lcy_balance
                          FROM actb_history ah
                          WHERE DECODE(p_date_type,
                                       'value_date', ah.value_dt,
                                       'tran_date', ah.txn_init_date,
                                       'post_date', ah.trn_dt) <= p_date
                          GROUP BY ah.ac_no,
                                   ah.ac_ccy)
            SELECT w.ac_no gl_account_no,
                   rl.line_desc,
                   gg.gl_desc,
                   gg.category,
                   w.ac_ccy fcy,
                   DECODE(gg.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   w.fcy_balance,
                   w.lcy_balance,
                   (w.fcy_balance * cru.currency_rate) usd_balance
            FROM w_ah w,
                 gltm_glmaster gg,
                 rpt_gl_report_lines rl,
                 mv_currency_rates_for_usd cru
            WHERE w.ac_no = gg.gl_code
                  AND rl.gl_account_no(+) = w.ac_no
                  AND rl.ccy(+) = w.ac_ccy
                  AND cru.currency_name = w.ac_ccy
                  AND gg.category IN (1, 2)
                  AND gg.leaf = 'Y'
                  AND INSTR(v_gl_parent_codes, gg.ulti_parent) = 0
            ORDER BY 3, 1, 4;

       RETURN c_result;
  END;
  -- GL1050 - Balance sheet (Bank wise)

  -- GL1051 - Balance sheet (Branch wise)
  FUNCTION gl1051(p_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_gl_parent_codes VARCHAR2(200) := '';
  BEGIN
       SELECT --MAX(DECODE(rs.setting_key, 'SUSP_GL_CODE_LIST', rs.setting_value)) gl_codes,
              MAX(DECODE(rs.setting_key, 'SUSP_GL_PARENT_CODE_LIST', rs.setting_value))
       INTO v_gl_parent_codes
       FROM rpt_report_settings rs
       WHERE rs.setting_key IN(/*'SUSP_GL_CODE_LIST', */'SUSP_GL_PARENT_CODE_LIST');

       OPEN c_result FOR
            SELECT /*+ full(ah) */ --/*+ index(ah actb_history_ac_idx) */
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch,
                   TRIM(ah.ac_no) gl_account_no,
                   gg.gl_desc,
                   gg.category,
                   ah.ac_ccy fcy,
                   DECODE(gg.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   SUM(DECODE(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)) fcy_balance,
                   SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) lcy_balance
            FROM actb_history ah,
                 gltm_glmaster gg,
                 ba_cc_brn_mast bm,
                 (SELECT NVL((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT lpad(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE gg.gl_code = ah.ac_no
                  AND bm.cod_cc_brn = ah.ac_branch
                  AND gg.category IN (1, 2)
                  AND gg.leaf = 'Y'
                  --AND INSTR(t.gl_codes, gg.gl_code) = 0
                  AND INSTR(v_gl_parent_codes, gg.ulti_parent) = 0
                  --
                  AND INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND DECODE(p_date_type,
                             'value_date', ah.value_dt,
                             'tran_date', ah.txn_init_date,
                             'post_date', ah.trn_dt) <= p_date
            GROUP BY (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch),
                     TRIM(ah.ac_no),
                     gg.gl_desc,
                     gg.category,
                     ah.ac_ccy
            ORDER BY 4, 2, 1, 5;

       RETURN c_result;
  END;
  -- GL1051 - Balance sheet (Branch wise)

  -- GL1052 - Balance sheet (Bank wise - new source)
  FUNCTION gl1052(p_year_month VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
    v_parent_codes VARCHAR2(1000);
  BEGIN
       SELECT MAX(decode(rs.setting_key, 'SUSP_GL_PARENT_CODE_LIST', rs.setting_value)) INTO v_parent_codes
       FROM rpt_report_settings rs
       WHERE rs.setting_key = 'SUSP_GL_PARENT_CODE_LIST';
       
       OPEN c_result FOR
            SELECT TRIM(gb.gl_code) gl_account_no,
                   gg.gl_desc,
                   gb.category,
                   gb.ccy_code fcy,
                   DECODE(gb.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   --SUM(gb.dr_bal) dr_bal_fcy,
                   --SUM(gb.cr_bal) cr_bal_fcy,
                   --SUM(gb.dr_bal_lcy) dr_bal_lcy,
                   --SUM(gb.cr_bal_lcy) cr_bal_lcy,
                   SUM(gb.cr_bal - gb.dr_bal) fcy_balance,
                   SUM(gb.cr_bal_lcy - gb.dr_bal_lcy) lcy_balance
            FROM gltb_gl_bal@host_link gb,
                 gltm_glmaster@host_link gg
            WHERE gg.gl_code = gb.gl_code
                  AND gb.category IN (1,2)
                  AND gb.leaf = 'Y'
                  --AND gb.fin_year = 'FY2010' 
                  --AND gb.period_code = 4--'FIN'
                  --
                  AND INSTR(v_parent_codes, trim(gg.ulti_parent)) = 0
                  AND (REPLACE(gb.fin_year, 'FY', '') || LPAD(gb.period_code, 2, '0')) = p_year_month
            GROUP BY TRIM(gb.gl_code),
                     gg.gl_desc,
                     gb.category,
                     gb.ccy_code
            HAVING SUM(gb.cr_bal - gb.dr_bal) != 0
            ORDER BY 3, 1, 4;

       RETURN c_result;
  END;
  -- GL1052 - Balance sheet (Bank wise - new source)

  -- GL1060 - Profit and loss (Bank wise)
  FUNCTION gl1060(p_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ah AS (SELECT /*+ full(ah) */ --/*+ index(ah actb_history_idx1) */
                                 ah.ac_no,
                                 ah.ac_ccy,
                                 SUM(DECODE(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)) fcy_balance,
                                 SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) lcy_balance
                          FROM actb_history ah
                          WHERE DECODE(p_date_type,
                                       'value_date', ah.value_dt,
                                       'tran_date', ah.txn_init_date,
                                       'post_date', ah.trn_dt) <= p_date
                                --AND (CASE WHEN ah.value_dt >= to_date('201012', 'yyyymm') THEN ah.txt_txn_desc ELSE 'X' END) != 'YEAR END'
                          GROUP BY ah.ac_no,
                                   ah.ac_ccy)
            SELECT w.ac_no gl_account_no,
                   rl.line_desc,
                   gg.gl_desc,
                   gg.category,
                   w.ac_ccy fcy,
                   DECODE(gg.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   w.fcy_balance,
                   w.lcy_balance,
                   (w.fcy_balance * cru.currency_rate) usd_balance
            FROM w_ah w,
                 gltm_glmaster gg,
                 rpt_gl_report_lines rl,
                 mv_currency_rates_for_usd cru
            WHERE gg.gl_code = w.ac_no
                  AND rl.gl_account_no(+) = w.ac_no
                  AND rl.ccy(+) = w.ac_ccy
                  AND cru.currency_name = w.ac_ccy
                  AND gg.category IN (3, 4)
                  AND gg.leaf = 'Y'
            ORDER BY 3, 1, 4;

       RETURN c_result;
  END;
  -- GL1060 - Profit and loss (Bank wise)

  -- GL1061 - Profit and loss (Branch wise)
  FUNCTION gl1061(p_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT /*+ full(ah) */ --/*+ index(ah actb_history_ac_idx) */
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch,
                   ah.ac_no gl_account_no,
                   gg.gl_desc,
                   gg.category,
                   ah.ac_ccy fcy,
                   DECODE(gg.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   SUM(DECODE(ah.drcr_ind, 'D', -ah.fcy_amount, ah.fcy_amount)) fcy_balance,
                   SUM(DECODE(ah.drcr_ind, 'D', -ah.lcy_amount, ah.lcy_amount)) lcy_balance
            FROM actb_history ah,
                 gltm_glmaster gg,
                 ba_cc_brn_mast bm,
                 (SELECT nvl((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT lpad(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE gg.gl_code = ah.ac_no
                  AND bm.cod_cc_brn = ah.ac_branch
                  AND gg.category IN (3, 4)
                  AND gg.leaf = 'Y'
                  --
                  AND INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND DECODE(p_date_type,
                             'value_date', ah.value_dt,
                             'tran_date', ah.txn_init_date,
                             'post_date', ah.trn_dt) <= p_date
            GROUP BY (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch),
                      ah.ac_no,
                      gg.gl_desc,
                      gg.category,
                      ah.ac_ccy
            ORDER BY 4, 2, 1, 5;

       RETURN c_result;
  END;
  -- GL1061 - Profit and loss (Branch wise)

  -- GL1062 - Profit and loss (Bank wise - new source)
  FUNCTION gl1062(p_year_month VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT TRIM(gb.gl_code) gl_account_no,
                   gg.gl_desc,
                   gb.category,
                   gb.ccy_code fcy,
                   DECODE(gb.category,
                          1, 'Assets',
                          2, 'Liabilities',
                          3, 'Income',
                          4, 'Expense',
                          5, 'Contingent Assets',
                          6, 'Contingent Liabilities',
                          7, 'Memo',
                          8, 'Position',
                          9, 'Position Equivalent') gl_category,
                   --SUM(gb.dr_bal) dr_bal_fcy,
                   --SUM(gb.cr_bal) cr_bal_fcy,
                   --SUM(gb.dr_bal_lcy) dr_bal_lcy,
                   --SUM(gb.cr_bal_lcy) cr_bal_lcy,
                   SUM(gb.cr_bal - gb.dr_bal) fcy_balance,
                   SUM(gb.cr_bal_lcy - gb.dr_bal_lcy) lcy_balance
            FROM gltb_gl_bal@host_link gb,
                 gltm_glmaster@host_link gg
            WHERE gg.gl_code = gb.gl_code
                  AND gb.category IN (3,4)
                  AND gb.leaf = 'Y'
                  --AND gb.fin_year = 'FY2010' 
                  --AND gb.period_code = 4--'FIN'
                  --
                  AND (REPLACE(gb.fin_year, 'FY', '') || LPAD(gb.period_code, 2, '0')) = p_year_month
            GROUP BY TRIM(gb.gl_code),
                     gg.gl_desc,
                     gb.category,
                     gb.ccy_code
            HAVING SUM(gb.cr_bal - gb.dr_bal) != 0
            ORDER BY 3, 1, 4;

       RETURN c_result;
  END;
  -- GL1062 - Profit and loss (Bank wise - new source)

  -- GL1070 - Transit accounts
  FUNCTION gl1070(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   t.txn_category,
                   t.gl_account_no,
                   t.acy,
                   t.sum_fcy_amount_dr,
                   t.sum_fcy_amount_cr
            FROM ba_cc_brn_mast bm,
                 (SELECT DECODE(ah.ac_no,
                                '250963002', ah.ac_branch, --decode(nvl(ah.related_customer, 0), 0, ah.ac_branch, to_number(substr(TRIM(ah.related_account), 1, 4))),
                                '250999005', ah.ac_branch, --decode(nvl(ah.related_customer, 0), 0, ah.ac_branch, to_number(substr(TRIM(ah.related_account), 1, 4))),
                                '250963001', ah.ac_branch,
                                '250999003', ah.ac_branch,
                                '250963003', ah.ac_branch,
                                '250999004', ah.ac_branch,
                                '250261002', ah.ac_branch,
                                '250963005', ah.ac_branch,
                                '250962002', ah.ac_branch,
                                '250963006', ah.ac_branch) txn_branch,
                         DECODE(ah.ac_no,
                                '250963002', 'DISBURSEMENT',
                                '250999005', 'UPLOAD DISBURSEMENT',
                                '250963001', 'REPAYMENT',
                                '250999003', 'UPLOAD REPAYMENT',
                                '250963003', 'FEE',
                                '250999004', 'UPLOAD FEE',
                                '250261002', 'LOAN TRANSITORY',
                                '250963005', 'EFS',
                                '250962002', 'BANK-TO-BANK',
                                '250963006', 'CASH') txn_category,
                         ah.ac_no gl_account_no,
                         ah.ac_ccy acy,
                         SUM(DECODE(ah.drcr_ind, 'D', ah.fcy_amount, 0)) sum_fcy_amount_dr,
                         SUM(DECODE(ah.drcr_ind, 'C', ah.fcy_amount, 0)) sum_fcy_amount_cr
                  FROM actb_history/*@host_link*/ ah
                  WHERE ah.ac_no IN('250963002',
                                    '250999005',
                                    '250963001',
                                    '250999003',
                                    '250963003',
                                    '250999004',
                                    '250261002',
                                    '250963005',
                                    '250962002',
                                    '250963006')
                        AND DECODE(p_date_type,
                                   'value_date', ah.value_dt,
                                   'tran_date', ah.txn_init_date,
                                   'post_date', ah.trn_dt) BETWEEN p_start_date AND p_end_date
                  GROUP BY DECODE(ah.ac_no,
                                  '250963002', ah.ac_branch, --decode(nvl(ah.related_customer, 0), 0, ah.ac_branch, to_number(substr(TRIM(ah.related_account), 1, 4))),
                                  '250999005', ah.ac_branch, --decode(nvl(ah.related_customer, 0), 0, ah.ac_branch, to_number(substr(TRIM(ah.related_account), 1, 4))),
                                  '250963001', ah.ac_branch,
                                  '250999003', ah.ac_branch,
                                  '250963003', ah.ac_branch,
                                  '250999004', ah.ac_branch,
                                  '250261002', ah.ac_branch,
                                  '250963005', ah.ac_branch,
                                  '250962002', ah.ac_branch,
                                  '250963006', ah.ac_branch),
                           DECODE(ah.ac_no,
                                  '250963002', 'DISBURSEMENT',
                                  '250999005', 'UPLOAD DISBURSEMENT',
                                  '250963001', 'REPAYMENT',
                                  '250999003', 'UPLOAD REPAYMENT',
                                  '250963003', 'FEE',
                                  '250999004', 'UPLOAD FEE',
                                  '250261002', 'LOAN TRANSITORY',
                                  '250963005', 'EFS',
                                  '250962002', 'BANK-TO-BANK',
                                  '250963006', 'CASH'),
                           ah.ac_no,
                           ah.ac_ccy) t
            WHERE bm.cod_cc_brn = t.txn_branch
            ORDER BY bm.nam_branch,
                     t.txn_category,
                     t.gl_account_no,
                     t.acy;

       RETURN c_result;
  END;
  -- GL1070 - Transit accounts

  -- GL1080 - Batch report
  FUNCTION gl1080(p_start_date DATE,
                  p_end_date DATE,
                  p_date_type VARCHAR2,
                  p_batch_no NUMBER,
                  p_user_no NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT gd.cod_acct_no gl_account_no,
                   gm.gl_desc gl_desc,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   ROUND(gd.amt_txn_fcy, 2) fcy_amount,
                   cc.nam_ccy_short tcy,
                   ROUND(gd.amt_txn_lcy, 2) lcy_amount,
                   gd.dat_value value_date,
                   gd.dat_posting posting_date,
                   TRUNC(gd.dat_txn) txn_date,
                   gd.txt_txn_desc txn_desc,
                   gd.cod_drcr drcr,
                   ref_chq_no instrument_code,
                   DECODE(up.cod_user_id, NULL, NULL, (up.nam_user || ' (' || up.cod_user_id || ')')) user_name
            FROM gl_detemp gd,
                 gltm_glmaster gm,
                 sm_user_profile up,
                 ba_cc_brn_mast bm,
                 ba_ccy_code cc
            WHERE gm.gl_code = gd.cod_acct_no
                  AND up.cod_userno = gd.cod_userno
                  AND bm.cod_cc_brn = gd.cod_cc_brn
                  AND cc.cod_ccy = gd.cod_txn_ccy
                  --
                  AND gm.gl_code NOT IN('280111001', '480111001')
                  --
                  AND gd.ctr_batch_no = p_batch_no
                  AND gd.cod_userno = NVL(p_user_no, gd.cod_userno)
                  AND DECODE(p_date_type,
                             'value_date', gd.dat_value,
                             'tran_date', gd.dat_txn,
                             'post_date', gd.dat_posting) BETWEEN p_start_date AND p_end_date
            ---
            UNION-- ALL
            ---
            SELECT ah.ac_no gl_account_no,
                   gm.gl_desc gl_desc,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   ROUND(ah.fcy_amount, 2) fcy_amount,
                   ah.ac_ccy acy,
                   ROUND(ah.lcy_amount, 2) lcy_amount,
                   ah.value_dt value_date,
                   TRUNC(ah.trn_dt) posting_date,
                   TRUNC(ah.txn_init_date) txn_date,
                   ah.txt_txn_desc txn_desc,
                   ah.drcr_ind drcr,
                   ah.instrument_code,
                   DECODE(up.cod_user_id, NULL, NULL, (up.nam_user || ' (' || up.cod_user_id || ')')) user_name
            FROM actb_history ah,
                 gltm_glmaster gm,
                 sm_user_profile up,
                 ba_cc_brn_mast bm
            WHERE gm.gl_code = ah.ac_no
                  AND up.cod_user_id = ah.user_id
                  AND bm.cod_cc_brn = ah.ac_branch
                  --
                  AND gm.gl_code NOT IN('280111001', '480111001')
                  --
                  AND ah.batch_no = p_batch_no
                  AND up.cod_userno = NVL(p_user_no, up.cod_userno)
                  AND DECODE(p_date_type,
                             'value_date', ah.value_dt,
                             'tran_date', ah.txn_init_date,
                             'post_date', ah.trn_dt) BETWEEN p_start_date AND p_end_date
            ORDER BY 9 DESC,
                     8,
                     1,
                     3;

       RETURN c_result;
  END;
  -- GL1080 - Batch report
  
  -- GL1090 - Profit and loss report
  FUNCTION gl1090(p_from_month VARCHAR2,
                  p_to_month VARCHAR2,
                  p_lang_id NUMBER := 0) -- 0 - english, 1 - azeri
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       -- vbr.branch_id/10000000 to make column data unique in distinct
       OPEN c_result FOR
            SELECT DECODE(p_lang_id,
                          0, DECODE(vbr.branch_id, 999, 'Head Office', 'Branches'),
                          1, DECODE(vbr.branch_id, 999, 'Baş Ofis', ' Filiallar')) ho_and_branches,
                   (vbr.region_id || ' - ' || vbr.region_name) region_name,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   r.pl_desc_tree,
                   r.pl_order,
                   NVL(r.font_weight, 'Bold') font_weight,
                   NVL(r.font_style, 'Default') font_style,
                   NVL(r.year_month, MIN(r.year_month) OVER (ORDER BY NULL)) ym,
                   TRIM(to_char(to_date(nvl(r.year_month, MIN(r.year_month) OVER (ORDER BY NULL)), 'yyyymm'), 'yyyy Month')) year_month,
                   ---
                   MAX(NVL(r.po_usd, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month) po_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.po_usd, 0) + (vbr.branch_id/10000000000))) OVER (PARTITION BY vbr.region_id, r.year_month), 6) po_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.po_usd, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(r.po_usd_total) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.po_usd, 0), 0)) OVER (PARTITION BY r.year_month) po_total,
                   --
                   NVL(r.co_count, 0) co_count_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.co_count, 0) + (vbr.branch_id/10000000))) OVER (PARTITION BY vbr.region_id, r.year_month)) co_count_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.co_count, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(NVL(r.co_count_total, 0)) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.co_count, 0), 0)) OVER (PARTITION BY r.year_month) co_count_total,
                   --
                   NVL(r.active_clients, 0) active_clients_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.active_clients, 0) + (vbr.branch_id/10000000))) OVER (PARTITION BY vbr.region_id, r.year_month)) active_clients_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.active_clients, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(NVL(r.active_clients_total, 0)) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.active_clients, 0), 0)) OVER (PARTITION BY r.year_month) active_clients_total,
                   --
                   NVL(r.balance_usd, 0) balance,
                   ROUND(DECODE(r.pl_order,
                                6100, DECODE(r.branch_id, 999, -1, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month))) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                7000, NVL(r.balance_usd_total, 0) + DECODE(r.branch_id, 999, -1, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month))) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_by_branch,
                   ROUND(DECODE(r.pl_order,
                                6100, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                7000, NVL(r.balance_usd_total, 0) + (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_by_region,
                   ROUND(DECODE(r.pl_order,
                                6100, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_all
            FROM v_branches_and_regions vbr,
                 (SELECT NVL(bp.tree_order, 10000) pl_order,
                         NVL(DECODE(p_lang_id, 0, bp.tree_desc, bp.tree_desc_az), t.pl_code) pl_desc_tree,
                         bp.font_weight,
                         bp.font_style,
                         nvl(bp.branch_id, t.branch_id) branch_id,
                         NVL(bp.year_month, t.year_month) year_month,
                         t.po_usd,
                         t.po_usd_total,
                         t.co_count,
                         t.co_count_total,
                         t.active_clients,
                         t.active_clients_total,
                         t.allocation_percentage,
                         SUM(DECODE(bp.tree_code, t.pl_code, t.balance_usd, 0)) balance_usd,
                         AVG(DECODE(bp.tree_code, t.pl_code, t.balance_usd_ho, 0)) balance_usd_ho,
                         SUM(t.balance_usd) balance_usd_total,
                         AVG(t.balance_usd_ho) balance_usd_ho_total
                  FROM v_tree_desc_pl bp,
                       (SELECT NVL(t1.branch_id, t2.branch_id) branch_id,
                               NVL(t1.tree_code, 'OR') pl_code,
                               (',' || NVL(t1.tree_code, 'OR') || ',') tree_code_for_list,
                               NVL(t1.year_month, t2.year_month) year_month,
                               NVL(t1.amount_usd, 0) balance_usd,
                               NVL(t1.amount_usd_ho, 0) balance_usd_ho,
                               NVL(t2.po_usd, 0) po_usd,
                               NVL(t2.po_usd_total, 0) po_usd_total,
                               NVL(t2.co_count, 0) co_count,
                               NVL(t2.co_count_total, 0) co_count_total,
                               NVL(t2.active_clients, 0) active_clients,
                               NVL(t2.active_clients_total, 0) active_clients_total,
                               NVL(ROUND(t2.allocation_percentage, 8), 0) allocation_percentage
                        FROM (SELECT gt.year_month,
                                     gt.branch_id,
                                     NVL(rl.pl_code, gt.gl_account_no) tree_code,
                                     SUM(gt.amount_lcy / cr.ccy_rate) amount_usd,
                                     SUM(DECODE(gt.branch_id, 999, SUM(gt.amount_lcy / cr.ccy_rate), 0))
                                     OVER (PARTITION BY gt.year_month, NVL(rl.pl_code, gt.gl_account_no)) amount_usd_ho
                              FROM rpt_gl_totals gt,
                                   rpt_gl_report_lines rl,
                                   v_ccy_rate_by_month cr
                              WHERE gt.gl_account_no = rl.gl_account_no(+)
                                    AND gt.ccy = rl.ccy(+)
                                    AND gt.year_month = cr.year_month
                                    AND cr.currency_no = 840
                              GROUP BY gt.year_month,
                                       gt.branch_id,
                                       NVL(rl.pl_code, gt.gl_account_no)) t1 FULL OUTER JOIN
                             mv_po_by_branch t2 ON t1.branch_id = t2.branch_id
                                                AND t1.year_month = t2.year_month) t
                   WHERE INSTR(bp.tree_code_list, t.tree_code_for_list(+), 1) > 0
                         AND bp.branch_id = t.branch_id(+)
                         AND bp.year_month = t.year_month(+)
                         --
                         AND bp.year_month BETWEEN p_from_month AND p_to_month
                   GROUP BY NVL(bp.tree_order, 10000),
                            NVL(DECODE(p_lang_id, 0, bp.tree_desc, bp.tree_desc_az), t.pl_code),
                            bp.font_weight,
                            bp.font_style,
                            nvl(bp.branch_id, t.branch_id),
                            NVL(bp.year_month, t.year_month),
                            t.po_usd,
                            t.po_usd_total,
                            t.co_count,
                            t.co_count_total,
                            t.active_clients,
                            t.active_clients_total,
                            t.allocation_percentage) r
            WHERE vbr.branch_id = r.branch_id(+);

       RETURN c_result;
  END;  
  -- GL1090 - Profit and loss report
  
  -- GL1091 - Branch profitability report
  FUNCTION gl1091(p_from_month VARCHAR2,
                  p_to_month VARCHAR2,
                  p_lang_id NUMBER := 0) -- 0 - english, 1 - azeri
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       -- vbr.branch_id/10000000 to make column data unique in distinct
       OPEN c_result FOR
            SELECT DECODE(p_lang_id,
                          0, DECODE(vbr.branch_id, 999, 'Head Office', 'Branches'),
                          1, DECODE(vbr.branch_id, 999, 'Baş Ofis', ' Filiallar')) ho_and_branches,
                   (vbr.region_id || ' - ' || vbr.region_name) region_name,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   r.pl_desc_tree,
                   r.tree_order,
                   NVL(r.font_weight, 'Bold') font_weight,
                   NVL(r.font_style, 'Default') font_style,
                   NVL(r.year_month, MIN(r.year_month) OVER (ORDER BY NULL)) ym,
                   TRIM(to_char(to_date(nvl(r.year_month, MIN(r.year_month) OVER (ORDER BY NULL)), 'yyyymm'), 'yyyy Month')) year_month,
                   ---
                   MAX(r.ccy_rate) OVER (PARTITION BY r.year_month) ccy_rate,
                   ---
                   MAX(NVL(r.po_usd, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month) po_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.po_usd, 0) + (vbr.branch_id/10000000000))) OVER (PARTITION BY vbr.region_id, r.year_month), 6) po_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.po_usd, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(r.po_usd_total) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.po_usd, 0), 0)) OVER (PARTITION BY r.year_month) po_total,
                   --
                   NVL(r.co_count, 0) co_count_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.co_count, 0) + (vbr.branch_id/10000000000))) OVER (PARTITION BY vbr.region_id, r.year_month)) co_count_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.co_count, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(NVL(r.co_count_total, 0)) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.co_count, 0), 0)) OVER (PARTITION BY r.year_month) co_count_total,
                   --
                   NVL(r.active_clients, 0) active_clients_by_branch,
                   TRUNC(SUM(DISTINCT (NVL(r.active_clients, 0) + (vbr.branch_id/10000000000))) OVER (PARTITION BY vbr.region_id, r.year_month)) active_clients_by_region,
                   DECODE(vbr.branch_id,
                          999, 2 * MAX(NVL(r.active_clients, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month),
                          MAX(NVL(r.active_clients_total, 0)) OVER (PARTITION BY r.year_month)) -
                   MAX(DECODE(vbr.branch_id, 999, NVL(r.active_clients, 0), 0)) OVER (PARTITION BY r.year_month) active_clients_total,
                   --
                   NVL(r.balance_usd, 0) balance,
                   ROUND(DECODE(r.tree_order,
                                14000, DECODE(r.branch_id, 999, -1, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month))) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                15000, NVL(r.balance_usd_total, 0) + DECODE(r.branch_id, 999, -1, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month))) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_by_branch,
                   ROUND(DECODE(r.tree_order,
                                14000, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                15000, NVL(r.balance_usd_total, 0) + (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_by_region,
                   ROUND(DECODE(r.tree_order,
                                14000, (SUM(DISTINCT NVL(r.allocation_percentage, 0)) OVER (PARTITION BY vbr.branch_id, r.year_month)) * (SUM(DECODE(r.branch_id, 999, r.balance_usd, 0)) OVER (PARTITION BY r.year_month)),
                                NVL(r.balance_usd_total, 0)),
                         6) balance_total_all
            FROM v_branches_and_regions vbr,
                 (SELECT NVL(bp.tree_order, 20000) tree_order,
                         NVL(DECODE(p_lang_id, 0, bp.tree_desc, bp.tree_desc_az), t.tree_code) pl_desc_tree,
                         bp.font_weight,
                         bp.font_style,
                         nvl(bp.branch_id, t.branch_id) branch_id,
                         NVL(bp.year_month, t.year_month) year_month,
                         t.ccy_rate,
                         t.po_usd,
                         t.po_usd_total,
                         t.co_count,
                         t.co_count_total,
                         t.active_clients,
                         t.active_clients_total,
                         t.allocation_percentage,
                         SUM(DECODE(bp.tree_code, t.tree_code, t.balance_usd, 0)) balance_usd,
                         AVG(DECODE(bp.tree_code, t.tree_code, t.balance_usd_ho, 0)) balance_usd_ho,
                         SUM(t.balance_usd) balance_usd_total,
                         AVG(t.balance_usd_ho) balance_usd_ho_total
                  FROM v_tree_desc_bp bp,
                       (SELECT NVL(t1.branch_id, t2.branch_id) branch_id,
                               NVL(t1.tree_code, 'OR') tree_code,
                               (',' || NVL(t1.tree_code, 'OR') || ',') tree_code_for_list,
                               NVL(t1.year_month, t2.year_month) year_month,
                               NVL(t1.ccy_rate, 0) ccy_rate,
                               NVL(t1.amount_usd, 0) balance_usd,
                               NVL(t1.amount_usd_ho, 0) balance_usd_ho,
                               NVL(t2.po_usd, 0) po_usd,
                               NVL(t2.po_usd_total, 0) po_usd_total,
                               NVL(t2.co_count, 0) co_count,
                               NVL(t2.co_count_total, 0) co_count_total,
                               NVL(t2.active_clients, 0) active_clients,
                               NVL(t2.active_clients_total, 0) active_clients_total,
                               NVL(ROUND(t2.allocation_percentage, 8), 0) allocation_percentage
                        FROM (SELECT gt.year_month,
                                     gt.branch_id,
                                     MAX(cr.ccy_rate) ccy_rate,
                                     NVL((CASE WHEN rl.tree_code LIKE 'SL%' THEN DECODE(gt.branch_id, 999, 'SL1', 'SL2')
                                               WHEN rl.tree_code LIKE 'TE%' THEN DECODE(gt.branch_id, 999, 'TE2', 'TE1')
                                               ELSE rl.tree_code
                                          END), gt.gl_account_no) tree_code,
                                     SUM(gt.amount_lcy / cr.ccy_rate) amount_usd,
                                     SUM(DECODE(gt.branch_id, 999, SUM(gt.amount_lcy / cr.ccy_rate), 0))
                                     OVER (PARTITION BY gt.year_month, 
                                       NVL((CASE WHEN rl.tree_code LIKE 'SL%' THEN DECODE(gt.branch_id, 999, 'SL1', 'SL2')
                                                 WHEN rl.tree_code LIKE 'TE%' THEN DECODE(gt.branch_id, 999, 'TE2', 'TE1')
                                                 ELSE rl.tree_code
                                            END), gt.gl_account_no)) amount_usd_ho
                              FROM rpt_gl_totals gt,
                                   rpt_gl_report_lines rl,
                                   v_ccy_rate_by_month cr
                              WHERE gt.gl_account_no = rl.gl_account_no(+)
                                    AND gt.ccy = rl.ccy(+)
                                    AND gt.year_month = cr.year_month
                                    AND cr.currency_no = 840
                              GROUP BY gt.year_month,
                                       gt.branch_id,
                                       NVL((CASE WHEN rl.tree_code LIKE 'SL%' THEN DECODE(gt.branch_id, 999, 'SL1', 'SL2')
                                                 WHEN rl.tree_code LIKE 'TE%' THEN DECODE(gt.branch_id, 999, 'TE2', 'TE1')
                                                 ELSE rl.tree_code
                                            END), gt.gl_account_no)) t1 FULL OUTER JOIN
                             mv_po_by_branch t2 ON t1.branch_id = t2.branch_id
                                                AND t1.year_month = t2.year_month) t
                   WHERE INSTR(bp.tree_code_list, t.tree_code_for_list(+), 1) > 0
                         AND bp.branch_id = t.branch_id(+)
                         AND bp.year_month = t.year_month(+)
                         --
                         AND bp.year_month BETWEEN p_from_month AND p_to_month
                   GROUP BY NVL(bp.tree_order, 20000),
                            NVL(DECODE(p_lang_id, 0, bp.tree_desc, bp.tree_desc_az), t.tree_code),
                            bp.font_weight,
                            bp.font_style,
                            nvl(bp.branch_id, t.branch_id),
                            NVL(bp.year_month, t.year_month),
                            t.ccy_rate,
                            t.po_usd,
                            t.po_usd_total,
                            t.co_count,
                            t.co_count_total,
                            t.active_clients,
                            t.active_clients_total,
                            t.allocation_percentage) r
            WHERE vbr.branch_id = r.branch_id(+);

       RETURN c_result;
  END;  
  -- GL1091 - Branch profitability report

  -- GL1100 - Cash reconciliation
  FUNCTION gl1100(p_start_date DATE,
                  p_end_date DATE,
                  p_branch_id NUMBER,
                  p_date_type VARCHAR2)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT ah.ac_no gl_account_no,
                   (TRIM(vbr.region_id) || ' - ' || vbr.region_name) region_name,
                   (vbr.branch_id || ' - ' || vbr.branch_name) branch_name,
                   DECODE(p_date_type,
                          'value_date', ah.value_dt,
                          'tran_date', ah.txn_init_date,
                          'post_date', ah.trn_dt) r_date,
                   ah.ac_ccy ccy,
                   SUM(DECODE(ah.drcr_ind, 'D', ah.fcy_amount, 0)) sum_fcy_amount_dr,
                   SUM(DECODE(ah.drcr_ind, 'C', ah.fcy_amount, 0)) sum_fcy_amount_cr
            FROM actb_history ah,
                 v_branches_and_regions vbr,
                 (SELECT nvl((SELECT TRIM(rs.setting_value)
                              FROM rpt_report_settings rs
                              WHERE rs.setting_key = ('REGION_' || b.branch_id)),
                            b.branch_id) branch_id_list
                  FROM (SELECT lpad(TRIM(p_branch_id), 3, '0') branch_id FROM dual) b) br
            WHERE vbr.branch_id = ah.ac_branch
                  --
                  AND ah.ac_no IN('100211001', '100261001')
                  --
                  AND INSTR(NVL(br.branch_id_list, ah.ac_branch), ah.ac_branch, 1) > 0
                  AND DECODE(p_date_type,
                             'value_date', ah.value_dt,
                             'tran_date', ah.txn_init_date,
                             'post_date', ah.trn_dt) BETWEEN p_start_date AND p_end_date
            GROUP BY ah.ac_no,
                   (TRIM(vbr.region_id) || ' - ' || vbr.region_name),
                   (vbr.branch_id || ' - ' || vbr.branch_name),
                     DECODE(p_date_type,
                            'value_date', ah.value_dt,
                            'tran_date', ah.txn_init_date,
                            'post_date', ah.trn_dt),
                     ah.ac_ccy
            ORDER BY 1, 4, 2, 3;

       RETURN c_result;
  END;
  -- GL1100 - Cash reconciliation

  -- GL1120 - GLM01 transactions
  FUNCTION gl1120(p_start_date DATE,
                  p_end_date DATE)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ah AS (SELECT ah.ac_branch,
                                 ah.ac_no,
                                 ah.ac_ccy,
                                 ah.drcr_ind,
                                 ah.fcy_amount,
                                 ah.lcy_amount,
                                 ah.trn_dt,
                                 ah.value_dt,
                                 ah.txn_init_date,
                                 --financial_cycle,
                                 --period_code,
                                 ah.batch_no,
                                 ah.user_id,
                                 ah.auth_id,
                                 ah.txt_txn_desc
                          FROM actb_history ah
                          WHERE ah.trn_code IN(7601,7602)
                                AND ah.cust_gl = 'G'
                                AND ah.user_id NOT IN('EOD','SYSTELLER','SYSTEM')
                                --
                                AND ah.value_dt BETWEEN p_start_date AND p_end_date)
            SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   w.ac_no gl_account_no,
                   w.trn_dt post_date,
                   w.value_dt value_date,
                   w.txn_init_date txn_date,
                   --w.drcr_ind drcr,
                   --w.fcy_amount,
                   --w.lcy_amount,
                   DECODE(w.drcr_ind, 'D', w.fcy_amount, 0) fcy_amount_dr,
                   DECODE(w.drcr_ind, 'C', w.fcy_amount, 0) fcy_amount_cr,
                   w.ac_ccy ccy,
                   DECODE(w.drcr_ind, 'D', w.lcy_amount, 0) lcy_amount_dr,
                   DECODE(w.drcr_ind, 'C', w.lcy_amount, 0) lcy_amount_cr,
                   w.batch_no,
                   w.user_id,
                   w.auth_id,
                   w.txt_txn_desc txn_desc
            FROM w_ah w,
                 ba_cc_brn_mast bm
            WHERE w.ac_branch = bm.cod_cc_brn
            ORDER BY value_date,
                     batch_no,
                     txn_date,
                     1;
       
       RETURN c_result;
  END;
  -- GL1120 - GLM01 transactions

END;
/
