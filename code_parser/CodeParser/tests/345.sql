CREATE OR REPLACE PACKAGE reports_pr
IS

  -- Author  : ELDANIZ
  -- Created : 19.01.2009 16:26:28
  -- Purpose :

  TYPE result_cursor IS REF CURSOR;

  -- PR1010 - Calculation of bonuses for credit officers
  FUNCTION pr1010(p_date DATE,
                  p_branch_id NUMBER,
                  p_usd_currency_rate NUMBER)
  RETURN result_cursor;

  -- PR1020 - Ratings of branches and regions
  FUNCTION pr1020(p_date1 DATE,
                  p_date2 DATE,
                  p_branch_or_region NUMBER)
  RETURN result_cursor;

  -- PR1030 - Control of work with schedule
  FUNCTION pr1030(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE,
                  p_txn_code NUMBER,
                  p_time_from NUMBER,
                  p_time_to NUMBER)
  RETURN result_cursor;

  -- PR1040 - Login history of branch managers and supervisors
  FUNCTION pr1040(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE,
                  p_position_id NUMBER)
  RETURN result_cursor;

  -- PR1050 - Calculation of bonuses for lawyers
  FUNCTION pr1050(p_start_date DATE,
                  p_end_date DATE,
                  p_usd_currency_rate NUMBER)
  RETURN data_types.result_cursor;

END;
/
CREATE OR REPLACE PACKAGE BODY reports_pr
IS

  -- PR1010 - Calculation of bonuses for credit officers
  FUNCTION pr1010(p_date DATE,
                  p_branch_id NUMBER,
                  p_usd_currency_rate NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT DECODE(r.co_type_key, 'U', 'Universal', 'G', 'Group', 'I', 'Individual') co_type,
                   LEAST(report_methods.calculate_bonus(r.co_type_key,
                                                        r.group_clients_active,
                                                        r.group_clients_new,
                                                        r.group_clients_repeat,
                                                        r.group_outstanding_lcy,
                                                        r.individual_clients_active,
                                                        r.individual_clients_new,
                                                        r.individual_clients_repeat,
                                                        r.individual_outstanding_lcy,
                                                        p_usd_currency_rate,
                                                        --r.total_par_lcy,
                                                        r.par_out,
                                                        r.years) + --bonus_due,
                         (report_methods.calculate_bonus(r.co_type_key, -- bonus due temp
                                                         r.group_clients_tmp,
                                                         0,
                                                         0,
                                                         r.group_outstanding_lcy_tmp,
                                                         r.individual_clients_tmp,
                                                         0,
                                                         0,
                                                         r.individual_outstanding_lcy_tmp,
                                                         p_usd_currency_rate,
                                                         (CASE WHEN r.par_out > 0.03 THEN r.par_out ELSE 0 END),
                                                         r.years) / 2), 1000) bonus_due,
                   report_methods.get_co_level(r.co_type_key,
                                               r.old_co_level,
                                               r.group_clients_active,
                                               r.individual_clients_active,
                                               (r.individual_outstanding_lcy / DECODE(p_usd_currency_rate, 0, 0.8, p_usd_currency_rate))) co_level_current,
                   r.old_co_level co_level_prev_month,
                   r.*
            FROM (SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                         (up.nam_user || ' (' || up.cod_user_id || ')') co_name,
                         up.cod_user_id co_user_id,
                         up.cod_employee_id co_employee_id,
                         cl.co_level old_co_level,
                         (CASE WHEN bn.group_clients_active > 0 AND bn.individual_clients_active > 0
                               THEN 'U'
                               WHEN bn.group_clients_active > 0 THEN 'G'
                               WHEN bn.individual_clients_active > 0 THEN 'I'
                          END) co_type_key,
                         --ROUND(DECODE(bn.total_outstanding_lcy, 0, 0, (bn.total_par_lcy / bn.total_outstanding_lcy)), 4) par_out,
                         DECODE(up.cod_cc_brn, 307, 0, ROUND(DECODE(bn.total_outstanding_lcy, 0, 0, (bn.total_par_lcy / bn.total_outstanding_lcy)), 4)) par_out,
                         --DECODE(up.cod_cc_brn, 307, 0, ROUND(DECODE(bn.total_outstanding_lcy_tmp, 0, 0, (bn.total_par_lcy_tmp / bn.total_outstanding_lcy_tmp)), 4)) par_out_tmp,
                         --TRUNC(((TRUNC(p_date) - TRUNC(bn.start_date)) / 365.25), 2) years,
                         TRUNC(((TRUNC(p_date) - NVL(hu.date_start, bn.first_disb_date)) / 365.25), 2) years,
                         NVL(hu.date_start, bn.first_disb_date) start_date,
                         bn.*
                  FROM sm_user_profile up,
                       ba_cc_brn_mast bm,
                       mv_hr_users hu,
                       (SELECT DISTINCT cls.level_date,
                               cls.employee_id,
                               cls.co_level
                        FROM rpt_co_levels cls,
                             (SELECT TRUNC(MAX(l.level_date)) max_level_date
                              FROM rpt_co_levels l
                              WHERE l.level_date < p_date) ld
                        WHERE cls.level_date = ld.max_level_date) cl,
                       (SELECT DISTINCT NVL(mlc.co_user_no, aa.cod_acct_off) co_user_no,
                               --t.customer_id,
                               --
                               MIN(MIN(t.first_disb_date)) OVER (PARTITION BY nvl(mlc.co_user_no, aa.cod_acct_off)) first_disb_date,
                               ---
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.account_type, 'GN', DECODE(t.is_temp, 0, 1, 0), 0)), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_clients_new,
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.account_type, 'IN', DECODE(t.is_temp, 0, 1, 0), 0)), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_clients_new,
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE '%N' THEN DECODE(t.is_temp, 0, 1, 0) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_clients_new,
                               ---
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.account_type, 'GR', DECODE(t.is_temp, 0, 1, 0), 0)), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_clients_repeat,
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.account_type, 'IR', DECODE(t.is_temp, 0, 1, 0), 0)), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_clients_repeat,
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE '%R' THEN DECODE(t.is_temp, 0, 1, 0) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_clients_repeat,
                               ---
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE 'G%' THEN DECODE(t.is_temp, 0, 1, 0) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_clients_active,
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE 'I%' THEN DECODE(t.is_temp, 0, 1, 0) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_clients_active,
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.is_temp, 0, 1, 0)), 0, NULL, t.customer_id)) OVER (PARTITION BY nvl(mlc.co_user_no, aa.cod_acct_off)) total_clients_active,
                               ---
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE 'G%' THEN DECODE(t.is_temp, 0, 0, 1) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_clients_tmp,
                               COUNT(DISTINCT DECODE(SUM(CASE WHEN t.account_type LIKE 'I%' THEN DECODE(t.is_temp, 0, 0, 1) ELSE 0 END), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_clients_tmp,
                               COUNT(DISTINCT DECODE(SUM(DECODE(t.is_temp, 0, 0, 1)), 0, NULL, t.customer_id)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_clients_tmp,
                               ---
                               SUM(SUM(CASE WHEN t.account_type LIKE 'G%'
                                            THEN (t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_outstanding_lcy,
                               SUM(SUM(CASE WHEN t.account_type LIKE 'I%'
                                            THEN (t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_outstanding_lcy,
                               SUM(SUM(t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_outstanding_lcy,
                               ---
                               SUM(SUM(CASE WHEN t.account_type LIKE 'G%'
                                            THEN (t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_outstanding_lcy_tmp,
                               SUM(SUM(CASE WHEN t.account_type LIKE 'I%'
                                            THEN (t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_outstanding_lcy_tmp,
                               SUM(SUM(t.principal_outstanding * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_outstanding_lcy_tmp,
                               ---
                               SUM(SUM(CASE WHEN t.account_type LIKE 'G%'
                                            THEN (t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_par_lcy,
                               SUM(SUM(CASE WHEN t.account_type LIKE 'I%'
                                            THEN (t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_par_lcy,
                               SUM(SUM(t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 1, 0))) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_par_lcy,
                               ---
                               SUM(SUM(CASE WHEN t.account_type LIKE 'G%'
                                            THEN (t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) group_par_lcy_tmp,
                               SUM(SUM(CASE WHEN t.account_type LIKE 'I%'
                                            THEN (t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))
                                            ELSE 0
                                       END)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) individual_par_lcy_tmp,
                               SUM(SUM(t.par_amount * cr.rat_ccy_cb * DECODE(t.is_temp, 0, 0, 1))) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) total_par_lcy_tmp
                        FROM ln_acct_attributes aa,
                             ba_ccy_rate cr,
                             (SELECT lc.account_no,
                                     lc.change_date,
                                     lc.user_no_old co_user_no
                              FROM mv_co_loan_changes lc,
                                   (SELECT clc.account_no,
                                           MIN(clc.change_date) min_date
                                    FROM mv_co_loan_changes clc
                                    WHERE clc.change_date >= p_date
                                    GROUP BY clc.account_no) lmd
                              WHERE lc.account_no = lmd.account_no
                                    AND lc.change_date = lmd.min_date) mlc,
                             (SELECT cc.cod_ccy,
                                     TRUNC(MAX(cc.dat_tim_rate_eff)) max_date
                              FROM ba_ccy_rate cc
                              WHERE TRUNC(cc.dat_tim_rate_eff) <= p_date
                              GROUP BY cc.cod_ccy) md,
                             (SELECT a.cod_acct_no,
                                     --DECODE(mx.cod_mis_txn_3, 20, 1, 0) is_temp,
																		                   (CASE WHEN NVL(mx.dat_last_mnt, p_date) < p_date + 1 THEN DECODE(mx.cod_mis_txn_3, 20, 1, 0) ELSE 0 END) is_temp,
                                     DECODE(g.cod_acct_no, NULL, 'I', 'G') ||
                                     DECODE(to_char(a.dat_first_disb, 'yyyymm'),
                                            to_char(p_date, 'yyyymm'), DECODE(nvl(cl.customer_cycle, 1), 1, 'N', 'R'),
                                            '') account_type,
                                     a.cod_cust_id customer_id,
                                     NVL(cl.customer_cycle, 1) customer_cycle,
                                     TRUNC(a.dat_first_disb) first_disb_date,
                                     a.cod_ccy acy,
                                     (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_outstanding,
                                     (CASE WHEN NVL(ar.principal_arrears, 0) > 0 AND
                                                report_methods.get_arrears_in_days(p_date,
                                                                                   a.cod_acct_no,
                                                                                   ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))),
                                                                                   NVL(ps.principal_due_by_schedule, 0)) > 15
                                           THEN (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                                           ELSE 0
                                      END) par_amount
                              FROM ln_acct_dtls a,
                                   ln_acct_balances ab,
                                   conv_ln_acct_balances cab,
                                   conv_ln_acct_int_balance_dtls caib,
                                   ba_cust_prod_txn_mis_xref mx,
                                   ac_acct_writeoff wo,
                                   mv_customer_cycle_list cl,
                                   (SELECT DISTINCT gd.cod_group,
                                           gd.nam_group,
                                           gd.cod_group_cycle,
                                           gd.cod_acct_no
                                    FROM ba_group_details gd
                                    WHERE gd.flg_group_acc_attr = 'A') g,
                                   (SELECT th.cod_acct_no,
                                           SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                    THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, 'C', -th.amt_txn_acy, 0)
                                                    ELSE 0
                                               END) principal_arrears
                                    FROM ln_arrear_txn_hist th
                                    WHERE th.dat_value <= p_date
                                    GROUP BY th.cod_acct_no
                                    HAVING SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                    THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, 'C', -th.amt_txn_acy, 0)
                                                    ELSE 0
                                               END) > 0) ar,
                                   (SELECT dtl.cod_acct_no,
                                           SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                                    FROM rpt_ln_daily_txn_log_po dtl
                                    WHERE dtl.dat_value <= p_date
                                    GROUP BY dtl.cod_acct_no) tl,
                                   (SELECT sd.cod_acct_no,
                                           SUM(sd.amt_principal) principal_due_by_schedule
                                    FROM ln_acct_schedule_detls sd
                                    WHERE sd.date_instal < p_date
                                    GROUP BY sd.cod_acct_no) ps
                              WHERE ab.cod_acct_no = a.cod_acct_no
                                    AND tl.cod_acct_no = a.cod_acct_no
                                    AND ar.cod_acct_no(+) = a.cod_acct_no
                                    AND ps.cod_acct_no(+) = a.cod_acct_no
                                    AND cab.cod_acct_no(+) = a.cod_acct_no
                                    AND caib.cod_acct_no(+) = a.cod_acct_no
                                    AND wo.cod_acct_no(+) = a.cod_acct_no
                                    AND g.cod_acct_no(+) = a.cod_acct_no
                                    AND mx.cod_prod_mnem_cust(+) = a.cod_acct_no
                                    AND cl.cod_cust_id(+) = a.cod_cust_id
                                    AND ab.amt_disbursed > 0
																		                  AND a.cod_acct_no NOT IN(SELECT spl.account_no FROM rpt_system_problem_loans spl)
                                    --
                                    AND NVL(a.dat_acct_close, p_date + 1) > p_date
                                    AND NVL(wo.dat_writeoff, p_date + 1) > p_date
                                    AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0) t
                        WHERE aa.cod_acct_no = t.cod_acct_no
                              AND mlc.account_no(+) = t.cod_acct_no
                              AND TRUNC(cr.dat_tim_rate_eff) = md.max_date
                              AND t.acy = md.cod_ccy
                              AND cr.cod_ccy = md.cod_ccy
                        GROUP BY NVL(mlc.co_user_no, aa.cod_acct_off),
                                 t.customer_id) bn
                  WHERE up.cod_userno = bn.co_user_no
                        AND bm.cod_cc_brn = up.cod_cc_brn
                        AND cl.employee_id(+) = up.cod_employee_id
                        AND hu.employee_id(+) = up.cod_employee_id
                        --
                        AND up.cod_cc_brn != 998
                        --
                        AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)) r
            ORDER BY 5, 6, 1;

       RETURN c_result;
  END;
  -- PR1010 - Calculation of bonuses for credit officers

  -- PR1020 - Ratings of branches and regions
  FUNCTION pr1020(p_date1 DATE,
                  p_date2 DATE,
                  p_branch_or_region NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR

            SELECT p.*,
                   --
                   dense_rank() OVER (PARTITION BY 1 ORDER BY ROUND(p.active_clients2, 4) DESC) ac_order,
                   dense_rank() OVER (PARTITION BY 1 ORDER BY ROUND(p.active_clients_increase, 4) DESC) aci_order,
                   dense_rank() OVER (PARTITION BY 1 ORDER BY ROUND(p.portfolio_increase, 4) DESC) opi_order,
                   --row_number() OVER (ORDER BY p.par_30_days) par_order,
                   dense_rank() OVER (PARTITION BY 1 ORDER BY ROUND(p.par_over_30_days, 4)) par_order,
                   dense_rank() OVER (PARTITION BY 1 ORDER BY ROUND(p.clients_per_co, 4) DESC) cpo_order
            FROM (SELECT br.region_id,
                         DECODE(p_branch_or_region, 1, br.region_name, (br.branch_id || ' - ' || br.branch_name)) branch_name,
                         SUM(NVL(r.active_clients1, 0)) active_clients1,
                         SUM(NVL(r.active_clients2, 0)) active_clients2,
                         SUM(NVL(r.portfolio_outstanding_lcy1, 0)) portfolio_outstanding_lcy1,
                         SUM(NVL(r.portfolio_outstanding_lcy2, 0)) portfolio_outstanding_lcy2,
                         COUNT(up.cod_userno) co_count,
                         --COUNT(r.co_user_no) co_count,
                         SUM(NVL(r.par_lcy, 0)) par_lcy,
                         --
                         ROUND(DECODE(SUM(NVL(r.active_clients1, 0)),
                                      0,
                                      0,
                                      (SUM(NVL(r.active_clients2, 0))/SUM(NVL(r.active_clients1, 0)))), 6) active_clients_increase,
                         round(DECODE(SUM(NVL(r.portfolio_outstanding_lcy1, 0)),
                                      0,
                                      0,
                                      (SUM(NVL(r.portfolio_outstanding_lcy2, 0))/SUM(NVL(r.portfolio_outstanding_lcy1, 0)))), 6) portfolio_increase,
                         ROUND(DECODE(SUM(NVL(r.portfolio_outstanding_lcy2, 0)),
                                      0,
                                      0,
                                      (SUM(NVL(r.par_lcy, 0))/SUM(NVL(r.portfolio_outstanding_lcy2, 0)))), 6) par_over_30_days,
                         ROUND(DECODE(COUNT(up.cod_userno),
                                      0,
                                      0,
                                      (SUM(NVL(r.active_clients2, 0))/COUNT(up.cod_userno))), 6) clients_per_co
                  FROM v_branches_and_regions br,
                       sm_user_profile up,
                       (SELECT NVL(mlc.co_user_no, aa.cod_acct_off) co_user_no,
                               COUNT(DISTINCT DECODE(t.date_id, 1, t.cod_cust_id, NULL)) active_clients1,
                               COUNT(DISTINCT DECODE(t.date_id, 2, t.cod_cust_id, NULL)) active_clients2,
                               --
                               SUM(SUM(DECODE(t.date_id, 1, t.principal_outstanding, 0) * cr1.rat_ccy_cb)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) portfolio_outstanding_lcy1,
                               SUM(SUM(DECODE(t.date_id, 2, t.principal_outstanding, 0) * cr2.rat_ccy_cb)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) portfolio_outstanding_lcy2,
                               --
                               SUM(SUM(DECODE(t.date_id, 2, t.par_amount, 0) * cr2.rat_ccy_cb)) OVER (PARTITION BY NVL(mlc.co_user_no, aa.cod_acct_off)) par_lcy
                        FROM ln_acct_attributes aa,
                             ba_ccy_rate cr1,
                             ba_ccy_rate cr2,
                             (SELECT lc.account_no,
                                     lc.change_date,
                                     lc.user_no_old co_user_no,
                                     decode(lc.change_date, lmd.min_date1, 1, 2) date_id
                              FROM mv_co_loan_changes lc,
                                   (SELECT clc.account_no,
                                           MIN(clc.change_date) min_date1,
                                           MIN(CASE WHEN clc.change_date > p_date2 THEN clc.change_date END) min_date2
                                    FROM mv_co_loan_changes clc
                                    WHERE clc.change_date > p_date1
                                    GROUP BY clc.account_no) lmd
                              WHERE lc.account_no = lmd.account_no
                                    AND lc.change_date IN(lmd.min_date1, lmd.min_date2)) mlc,
                             (SELECT cc.cod_ccy,
                                     MAX(CASE WHEN cc.dat_tim_rate_eff <= p_date1 THEN cc.dat_tim_rate_eff END) max_date1,
                                     MAX(cc.dat_tim_rate_eff) max_date2
                              FROM ba_ccy_rate cc
                              WHERE TRUNC(cc.dat_tim_rate_eff) <= TRUNC(p_date2)
                              GROUP BY cc.cod_ccy) md,
                             (SELECT 1 date_id,
                                     a.cod_acct_no,
                                     a.cod_cust_id,
                                     a.cod_ccy,
                                     (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_outstanding,
                                     0 par_amount
                              FROM ln_acct_dtls a,
                                   ln_acct_balances ab,
                                   conv_ln_acct_balances cab,
                                   conv_ln_acct_int_balance_dtls caib,
                                   ac_acct_writeoff wo,
                                   (SELECT dtl.cod_acct_no,
                                           SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                                    FROM rpt_ln_daily_txn_log_po dtl
                                    WHERE dtl.dat_value <= p_date1
                                    GROUP BY dtl.cod_acct_no) tl
                              WHERE ab.cod_acct_no = a.cod_acct_no
                                    AND tl.cod_acct_no = a.cod_acct_no
                                    AND cab.cod_acct_no(+) = a.cod_acct_no
                                    AND caib.cod_acct_no(+) = a.cod_acct_no
                                    AND wo.cod_acct_no(+) = a.cod_acct_no
                                    --
                                    AND ab.amt_disbursed > 0
                                    AND NVL(wo.dat_writeoff, p_date1 + 1) > p_date1
                                    AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0
                              UNION ALL
                              SELECT 2 date_id,
                                     a.cod_acct_no,
                                     a.cod_cust_id,
                                     a.cod_ccy,
                                     (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) principal_outstanding,
                                     (CASE WHEN NVL(ar.principal_arrears, 0) > 0 AND
                                                report_methods.get_arrears_in_days(p_date2,
                                                                                   a.cod_acct_no,
                                                                                   (ab.amt_disbursed - (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))),
                                                                                   NVL(ps.principal_due_by_schedule, 0)) >= 30
                                           THEN (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0)))
                                           ELSE 0
                                      END) par_amount
                              FROM ln_acct_dtls a,
                                   ln_acct_balances ab,
                                   conv_ln_acct_balances cab,
                                   conv_ln_acct_int_balance_dtls caib,
                                   ac_acct_writeoff wo,
                                   (SELECT th.cod_acct_no cod_acct_no,
                                           SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                    THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, 'C', -th.amt_txn_acy, 0)
                                                    ELSE 0
                                               END) principal_arrears
                                    FROM ln_arrear_txn_hist th
                                    WHERE th.dat_value <= p_date2
                                    GROUP BY th.cod_acct_no
                                    HAVING SUM(CASE WHEN th.cod_arrear_type = 'C'
                                                    THEN DECODE(th.cod_drcr, 'D', th.amt_txn_acy, 'C', -th.amt_txn_acy, 0)
                                                    ELSE 0
                                               END) > 0) ar,
                                   (SELECT dtl.cod_acct_no,
                                           SUM(DECODE(dtl.flg_drcr, 'D', dtl.amt_txn_acy, -dtl.amt_txn_acy)) principal_due
                                    FROM rpt_ln_daily_txn_log_po dtl
                                    WHERE dtl.dat_value <= p_date2
                                    GROUP BY dtl.cod_acct_no) tl,
                                   (SELECT sd.cod_acct_no cod_acct_no,
                                           SUM(sd.amt_principal) principal_due_by_schedule
                                    FROM ln_acct_schedule_detls sd
                                    WHERE sd.date_instal < p_date2
                                    GROUP BY sd.cod_acct_no) ps
                              WHERE ab.cod_acct_no = a.cod_acct_no
                                    AND tl.cod_acct_no = a.cod_acct_no
                                    AND cab.cod_acct_no(+) = a.cod_acct_no
                                    AND caib.cod_acct_no(+) = a.cod_acct_no
                                    AND wo.cod_acct_no(+) = a.cod_acct_no
                                    AND ar.cod_acct_no(+) = a.cod_acct_no
                                    AND ps.cod_acct_no(+) = a.cod_acct_no
                                    --
                                    AND ab.amt_disbursed > 0
                                    AND NVL(wo.dat_writeoff, p_date2 + 1) > p_date2
                                    AND (tl.principal_due + NVL(cab.amt_rpa_balance, 0) - (NVL(caib.bal_int1_arrears, 0) + NVL(caib.bal_int2_arrears, 0))) > 0) t
                        WHERE aa.cod_acct_no = t.cod_acct_no
                              AND t.cod_acct_no = mlc.account_no(+)
                              AND t.date_id = mlc.date_id(+)
                              AND t.cod_ccy = md.cod_ccy
                              AND cr1.dat_tim_rate_eff = md.max_date1
                              AND cr2.dat_tim_rate_eff = md.max_date2
                              AND cr1.cod_ccy = md.cod_ccy
                              AND cr2.cod_ccy = md.cod_ccy
                        GROUP BY NVL(mlc.co_user_no, aa.cod_acct_off)) r
                  WHERE up.cod_cc_brn = br.branch_id
                        AND up.cod_userno = r.co_user_no
                        --AND up.cod_branch_templ = 51
                  GROUP BY br.region_id,
                           DECODE(p_branch_or_region, 1, br.region_name, (br.branch_id || ' - ' || br.branch_name))) p
            ORDER BY 3, 2;

       RETURN c_result;
  END;
  -- PR1020 - Ratings of branches and regions

  -- PR1030 - Control of work with schedule
  FUNCTION pr1030(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE,
                  p_txn_code NUMBER,
                  p_time_from NUMBER,
                  p_time_to NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            SELECT t.branch_name,
                   (up.nam_user || ' (' || up.cod_user_id || ')') user_name,
                   t.txn_desc,
                   t.txn_count
            FROM sm_user_profile up,
                 (SELECT (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(nam_branch)) branch_name,
                         rt.user_no,
                         (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)) txn_desc,
                         COUNT(*) txn_count
                  FROM v_rec_txnlog rt,
                       dl_ba_cc_brn_mast@brn_link bm,
                       dl_ba_txn_mnemonic@brn_link tm
                  WHERE bm.cod_cc_brn = rt.cod_brn
                        AND tm.cod_txn_mnemonic = rt.cod_tran
                        --
                        AND rt.dat_sys BETWEEN p_start_date AND p_end_date
                        AND rt.cod_brn = nvl(p_branch_id, rt.cod_brn)
                        AND rt.cod_tran = nvl(p_txn_code, rt.cod_tran)
                        AND to_number(to_char(rt.tim_sys, 'HH24')) BETWEEN p_time_from AND p_time_to
                  GROUP BY (TRIM(bm.cod_cc_brn) || ' - ' || TRIM(nam_branch)),
                           (TRIM(tm.cod_txn_mnemonic) || ' - ' || TRIM(tm.txt_txn_desc)),
                           rt.user_no) t
            WHERE up.cod_userno = t.user_no
            ORDER BY 1, 2, 3, 4 DESC;

       RETURN c_result;
  END;
  -- PR1030 - Control of work with schedule

  -- PR1040 - Login history of branch managers and supervisors
  FUNCTION pr1040(p_branch_id NUMBER,
                  p_start_date DATE,
                  p_end_date DATE,
                  p_position_id NUMBER)
  RETURN result_cursor
  IS
    c_result result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ul_log AS (SELECT el.cod_user_id,
                                     el.dat_event,
                                     TRIM(el.cod_term_id) cod_term_id
                                     --COUNT(*)
                              FROM sm_br_event_log@brn_link el
                              WHERE el.cod_action = 1
                                    AND el.dat_event BETWEEN p_start_date AND p_end_date)
            SELECT *
            FROM (SELECT (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                          up.nam_user user_name,
                          tp.nam_template position,
                          up.cod_user_id user_id,
                          up.cod_userno user_no,
                          l.dat_event login_date,
                          l.cod_term_id computer_name,
                          (CASE WHEN up.cod_branch_templ = 55 AND UPPER(l.cod_term_id) NOT LIKE '%BM%' THEN 1
                                WHEN up.cod_branch_templ = 54 AND NOT (UPPER(l.cod_term_id) LIKE '%SV%' OR UPPER(l.cod_term_id) LIKE '%SUPERV%' OR UPPER(l.cod_term_id) LIKE '%SPV%') THEN 2
                                ELSE 0
                           END) login_result
                   FROM w_ul_log l,
                        sm_user_profile up,
                        sm_temp_profile tp,
                        ba_cc_brn_mast bm
                   WHERE l.cod_user_id = up.cod_user_id
                         AND up.cod_branch_templ = tp.cod_user_templ
                         AND bm.cod_cc_brn = up.cod_cc_brn
                         AND up.cod_cc_brn = tp.cod_branch
                         AND up.cod_branch_templ IN(54, 55)
                         --
                         AND up.cod_cc_brn = NVL(p_branch_id, up.cod_cc_brn)) lh
            WHERE lh.login_result = DECODE(p_position_id, 0, lh.login_result, p_position_id)
            ORDER BY 6 DESC, 1, 2;

       RETURN c_result;
  END;
  -- PR1040 - Login history of branch managers and supervisors

  -- PR1050 - Calculation of bonuses for lawyers
  FUNCTION pr1050(p_start_date DATE,
                  p_end_date DATE,
                  p_usd_currency_rate NUMBER)
  RETURN data_types.result_cursor
  IS
    c_result data_types.result_cursor;
  BEGIN
       OPEN c_result FOR
            WITH w_ldr AS (SELECT TO_NUMBER(NULL) branch_id,
                                  ab.cod_acct_no,
                                  TRIM(aa.cod_acct_off) co_user_no,
                                  ar.last_repayment_date,
                                  '' ccy,
                                  ar.total_paid_last_month,
                                  TO_NUMBER(NULL) total_paid_last_month_lcy,
                                  ab.amt_rpa_balance,
                                  1 num
                           FROM ln_acct_attributes aa,
                                ln_acct_balances ab,
                                (SELECT th.cod_acct_no,
                                        MAX(CASE WHEN th.cod_drcr = 'C'
                                                 THEN th.dat_value
                                                 ELSE NULL
                                            END) last_repayment_date,
                                        SUM(CASE WHEN th.cod_arrear_type IN ('C','I','N','A','L','R') AND TO_CHAR(th.dat_value, 'yyyymm') = TO_CHAR(p_end_date, 'yyyymm')
                                                 THEN DECODE(th.cod_drcr, 'C', th.amt_txn_acy, DECODE(th.cod_txn_mnemonic, 3121, -th.amt_txn_acy, 0))
                                                 ELSE 0
                                            END) total_paid_last_month
                                 FROM ln_arrear_txn_hist th
                                 WHERE th.dat_value <= p_end_date
                                 GROUP BY th.cod_acct_no
                                 HAVING SUM(CASE WHEN th.cod_arrear_type IN ('C','I','N','A','L','R') AND TO_CHAR(th.dat_value, 'yyyymm') = TO_CHAR(p_end_date, 'yyyymm')
                                                 THEN DECODE(th.cod_drcr, 'C', th.amt_txn_acy, DECODE(th.cod_txn_mnemonic, 3121, -th.amt_txn_acy, 0))
                                                 ELSE 0
                                            END) > 0
                                        AND MAX(CASE WHEN th.cod_drcr = 'C'
                                                     THEN th.dat_value
                                                     ELSE NULL
                                                END) BETWEEN p_start_date AND p_end_date) ar
                           WHERE aa.cod_acct_no = ab.cod_acct_no
                                 AND aa.cod_acct_no = ar.cod_acct_no
                                 --
                                 AND ab.amt_disbursed > 0
                                 AND aa.flg_legal = 'Y'),
                 w_act AS (SELECT TO_NUMBER(ah.ac_branch) branch_id,
                                  ah.instrument_code account_no,
                                  ah.txt_txn_desc,
                                  ah.value_dt,
                                  ah.ac_ccy,
                                  ah.fcy_amount,
                                  ah.lcy_amount,
                                  TO_NUMBER(NULL) rpa_balance,
                                  2 num
                           FROM actb_history ah
                           WHERE (ah.ac_no LIKE '706962%' OR (ah.ac_no IN('250964003','630161001','630161002') AND ah.ac_branch = '999'))
                                 AND ah.drcr_ind = 'C'
                                 --
                                 AND ah.value_dt BETWEEN p_start_date AND p_end_date)
            SELECT lrl.collector_name,
                   (TRIM(bm.cod_cc_brn) || ' - ' || bm.nam_branch) branch_name,
                   NVL2(c.cod_cust_id, TRIM(c.cod_cust_id) || ' - ' || c.nam_cust_full, w.co_user_no) customer_name,
                   NVL2(g.cod_group, (g.cod_group || ' - ' || g.nam_group), NULL) group_name,
                   w.cod_acct_no account_no,
                   lrl.loan_status legal_info,
                   NVL(ldl.change_date, TO_DATE(NULL)) transfer_date,
                   w.last_repayment_date,
                   NVL(cc.nam_ccy_short, w.ccy) ccy,
                   w.total_paid_last_month,
                   (w.total_paid_last_month * DECODE(NVL(w.ccy, cc.nam_ccy_short), 'USD', NVL(p_usd_currency_rate, .8), 1)) total_paid_last_month_lcy,
                   w.amt_rpa_balance current_rpa_balance,
                   ast.txt_acct_status account_status
                   --,w.*
            FROM ln_acct_dtls a,
                 ci_custmast c,
                 sm_user_profile up,
                 ba_cc_brn_mast bm,
                 ba_acct_status ast,
                 ba_ccy_code cc,
                 mv_legal_dep_loans ldl,
                 v_ld_related_loans lrl,
                 (SELECT DISTINCT gd.cod_group,
                         gd.nam_group,
                         gd.cod_group_cycle,
                         gd.cod_acct_no
                  FROM ba_group_details gd
                  WHERE gd.flg_group_acc_attr = 'A') g,
                 (SELECT l.*
                  FROM w_ldr l
                  UNION
                  SELECT a.*
                  FROM w_act a) w
            WHERE w.cod_acct_no = a.cod_acct_no(+)
                  AND w.cod_acct_no = g.cod_acct_no(+)
                  AND w.cod_acct_no = ldl.account_no(+)
                  AND w.cod_acct_no = lrl.cod_acct_no(+)
                  AND a.cod_cust_id = c.cod_cust_id(+)
                  AND w.co_user_no = TRIM(up.cod_userno(+))
                  AND a.cod_ccy = cc.cod_ccy(+)
                  AND NVL(a.cod_acct_stat, 11) = ast.cod_acct_status(+)
                  AND NVL(up.cod_cc_brn, w.branch_id) = bm.cod_cc_brn
                  --
                  AND ast.cod_module(+) = 'LN'
            ORDER BY w.num, 1, 2, 4, 3;
       
       RETURN c_result;
  END;
  -- PR1050 - Calculation of bonuses for lawyers

END;
/
