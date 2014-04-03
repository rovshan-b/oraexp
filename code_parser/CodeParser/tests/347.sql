CREATE OR REPLACE TYPE "OZUMUZ"."NTT_VARCHAR2" AS TABLE OF VARCHAR2(4000);
/
CREATE OR REPLACE TYPE "OZUMUZ"."NUMBER_LIST" as table of number;
/
CREATE OR REPLACE TYPE "OZUMUZ"."STRING_LIST" as table of varchar2(32767);
/
CREATE OR REPLACE TYPE "OZUMUZ"."SYSTP4dfw5/4AHZLgQAoKFAEhlg==" AS TABLE OF VARCHAR2(50);
/
CREATE OR REPLACE TYPE "OZUMUZ"."SYSTP4dk9jIxmBeHgQAoKFAFI3g==" AS TABLE OF VARCHAR2(50);
/
CREATE OR REPLACE TYPE "OZUMUZ"."T_KEYWORDS_TBL" is table of t_keyword_rec;
/
CREATE OR REPLACE TYPE "OZUMUZ"."T_KEYWORD_REC" AS OBJECT (
      keyword varchar2(200), 
      subs_id number,
      variant_id number,
      agent_id number
   );
/







CREATE OR REPLACE PACKAGE "OZUMUZ"."BULKSMS_API" 
AS
   /******************************************************************************
      NAME:       BULKSMS_API
      PURPOSE:

      REVISIONS:
      Ver        Date        Author           Description
      ---------  ----------  ---------------  ------------------------------------
      1.0        1/13/2012      rdavudov       1. Created this package.
   ******************************************************************************/
   TYPE t_cursor IS REF CURSOR;

   FUNCTION check_login (pi_login IN VARCHAR2, pi_password IN VARCHAR2)
      RETURN NUMBER;

   FUNCTION check_balance (pi_login IN VARCHAR2, pi_lock IN NUMBER DEFAULT 0)
      RETURN NUMBER;

   FUNCTION check_schedule (pi_schedule IN VARCHAR2)
      RETURN NUMBER;

   PROCEDURE get_units (pi_login       IN     VARCHAR2,
                        pi_password    IN     VARCHAR2,
                        po_resp_code      OUT VARCHAR2,
                        po_units          OUT NUMBER);

   PROCEDURE add_bulk_sms (pi_login       IN     VARCHAR2,
                           pi_password    IN     VARCHAR2,
                           pi_title       IN     VARCHAR2,
                           pi_schedule    IN     VARCHAR2,
                           pi_message     IN     VARCHAR2,
                           pi_count       IN     NUMBER,
                           pi_controlid   IN     VARCHAR2,
                           po_resp_code      OUT VARCHAR2,
                           po_task_id        OUT NUMBER);

   PROCEDURE add_bulk_msisdn (pi_task_id   IN NUMBER,
                              pi_msisdn    IN VARCHAR2,
                              pi_message   IN VARCHAR2);

   PROCEDURE start_bulk_sms (pi_task_id IN NUMBER, po_resp_code OUT VARCHAR2);

   PROCEDURE get_delivery_report (pi_login       IN     VARCHAR,
                                  pi_password    IN     VARCHAR2,
                                  pi_task_id     IN     NUMBER,
                                  po_resp_code      OUT VARCHAR2,
                                  po_pending        OUT NUMBER,
                                  po_delivered      OUT NUMBER,
                                  po_failed         OUT NUMBER,
                                  po_removed        OUT NUMBER,
                                  po_error          OUT NUMBER);

   PROCEDURE get_detailed_report (pi_login       IN     VARCHAR,
                                  pi_password    IN     VARCHAR2,
                                  pi_task_id     IN     NUMBER,
                                  po_resp_code      OUT VARCHAR2,
                                  po_cursor         OUT t_cursor);
                                 

   
END BULKSMS_API;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_APP_MODULE" AS

  type t_cursor is ref cursor;
  procedure has_access(v_manager_id number, v_module_name nvarchar2, v_has_access out number);
  procedure get_all_modules(v_items_cursor out t_cursor);
  procedure get_all_modules_for_manager(v_man_id number, v_items_cursor out t_cursor);
  procedure get_top_level_modules(v_items_cursor out t_cursor);
  PROCEDURE ADD_APP_MODULE(V_MODULE_DISPLAY_NAME NVARCHAR2,V_MODULE_NAME NVARCHAR2,V_PARENT_MODULE_ID NUMBER,V_NEW_ROW_ID OUT NUMBER);

END PKG_APP_MODULE;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_APP_SETTINGS" AS

   procedure set_value(p_key varchar2, p_value varchar2);
   function get_value(p_key varchar2, p_default_value varchar2 := null) return varchar2;

END;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_APP_SETTTINGS" AS 

  function get_value(p_key varchar2) return varchar2;

END PKG_APP_SETTTINGS;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_ARAZFM_SMS" 
as
     type t_cursor is ref cursor;

   procedure truncIncomingSms(v_cursor out t_cursor);
   
    procedure betweenHour(v_cursor out t_cursor);
    
    procedure betweenTwoHour(v_cursor out t_cursor);
    
    procedure GetSMS(v_from number, v_cursor out t_cursor);
    procedure GetSMS(v_from number,v_to number,  v_cursor out t_cursor) ;
 
end pkg_ArazFM_sms;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_AUTOSMS" as
   
   procedure get_next_content(p_subs_id number, p_msisdn varchar2, p_max_days_back number, o_result out varchar2, o_content_id out number);
    
end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_BETTING" AS

  type t_cursor is ref cursor;
  procedure get_bettings_to_display(v_group_id number, v_items_cursor out t_cursor);
  procedure get_bettings_for_ext_display(v_group_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_BETTING(V_BETTING_NAME NVARCHAR2,V_START_DATE DATE,V_END_DATE DATE,V_SHORT_NUMBER_ID NUMBER, V_GROUP_ID NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE DELETE_BETTING(V_BETTING_ID NUMBER);
  PROCEDURE GET_BETTING_BY_ID(V_BETTING_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR);
  PROCEDURE UPDATE_BETTING(V_BETTING_ID NUMBER,V_BETTING_NAME NVARCHAR2,V_START_DATE DATE,V_END_DATE DATE,V_SHORT_NUMBER_ID NUMBER);
  procedure get_betting_name_by_id(v_betting_id number, v_betting_name out nocopy nvarchar2);
  procedure get_betting_winners(v_betting_id number, v_min_match_count number, v_max_res_count number, v_items_cursor out t_cursor);
  procedure get_betting_drawing_count(v_betting_id number, v_drawing_count out number);
  procedure get_betting_drawing_results(v_betting_id number, v_drawing_results out nocopy varchar2);
  procedure notify_by_sms(v_betting_id number, v_ids varchar2, v_msg_body nvarchar2);
  procedure get_bets_by_betting(v_group_id number, v_betting_id number, v_source_addr varchar2, v_items_cursor out t_cursor);
  function is_valid_bet_format(v_bet nvarchar2) return boolean;

END PKG_BETTING;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_BETTING_DRAWING" AS

  type t_cursor is ref cursor;
  procedure get_drawings_by_betting_id(v_betting_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_BETTING_DRAWING(V_BETTING_ID NUMBER,V_DRAWING_NAME NVARCHAR2,V_DRAWING_RESULT NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_BETTING_DRAWING(V_DRAWING_ID NUMBER,V_DRAWING_NAME NVARCHAR2,V_DRAWING_RESULT NUMBER);
  PROCEDURE DELETE_BETTING_DRAWING(V_DRAWING_ID NUMBER);
  function get_drawing_results_string(v_betting_id number) return varchar2;
  procedure update_betting_drawings_result(v_betting_id number);

END PKG_BETTING_DRAWING;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_BETTING_GROUP" AS

  type t_cursor is ref cursor;
  procedure get_groups_to_display(v_items_cursor out t_cursor);
  PROCEDURE ADD_BETTING_GROUP(V_GROUP_NAME NVARCHAR2,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_BETTING_GROUP(V_GROUP_ID NUMBER,V_GROUP_NAME NVARCHAR2);
  PROCEDURE DELETE_BETTING_GROUP(V_GROUP_ID NUMBER);
  PROCEDURE GET_BETTING_GROUP_BY_ID(V_GROUP_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR);
  procedure get_group_name_by_id(v_group_id number, v_group_name out nocopy nvarchar2);
  procedure get_betting_group_winners(v_group_id number, v_bettings varchar2, v_min_points number, v_max_res_count number, v_items_cursor out t_cursor);

END PKG_BETTING_GROUP;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_BLACK_LIST" AS

    procedure load_from_external(o_result out number);
    
    function is_in_black_list(p_msisdn varchar2) return number;

END PKG_BLACK_LIST;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_BULK_SMS2" as
  
  procedure add_number(p_bulk_sms_id number, p_msisdn varchar2, 
                       p_msg_body varchar2, p_check_creator_id number);
                       
  procedure add_list(p_bulk_sms_id number, p_list_id number, 
                       p_check_creator_id number);  
                       
  procedure update_number_count(p_bulk_sms_id number);
  function get_number_count(p_bulk_sms_id number) return number;                   
                       
  procedure start_sending(p_bulk_sms_id number, p_check_creator_id number, 
                          o_msg out varchar2);
  
  procedure add_group_number(p_group_id number, p_name varchar2, p_msisdn varchar2);
end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_CHGW" AS
   
   procedure set_charge_fail_date(p_msisdn varchar2, p_failed number);
   
END;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_DIAMETER" AS 

  const_generic_error constant number := -1;
  const_success constant number := 100;
  const_wrong_number constant number := 400;
  const_no_balance constant number := 401;
  const_wrong_tariff constant number := 402;
  const_srv_denied_for_msisdn constant number := 403;
  const_internal_error constant number := 500;
  const_network_error constant number := -2;

  procedure charge(p_msisdn varchar2, 
                   p_tariff_id number, 
                   p_client_id number,
                   p_charge_reason varchar2,
                   o_result out number,
                   o_text_result out varchar2,
                   p_msg_id number := -1);

END PKG_DIAMETER;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_EVSMS" as
   
   procedure get_next_content(p_subs_id number, p_msisdn varchar2, p_max_days_back number, o_result out varchar2, o_content_id out number);
   
   function get_next_code return number;
   
end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_EXTERNAL_ACCESS" AS

  type t_cursor is ref cursor;
  procedure can_view_module_from_ip(v_access_key varchar2, v_ip_address varchar2, v_can_view out number, v_module_name out nocopy varchar2, v_access_name out nocopy nvarchar2, v_ext_access_rec_id out number, v_ext_access_date out date, v_param1 out number);
  procedure check_external_user(v_login nvarchar2, v_password_hash nvarchar2, v_access_key varchar2, v_items_cursor out t_cursor);
  PROCEDURE GET_ALL_EXTERNAL_ACCESS(ITEMS_CURSOR OUT T_CURSOR);
  PROCEDURE ADD_EXTERNAL_ACCESS(V_ACCESS_NAME NVARCHAR2,V_ACCESSOR_NAME NVARCHAR2,V_ACCESSOR_USER NVARCHAR2,V_ACCESSOR_PASS NVARCHAR2,V_ACCESSOR_IP VARCHAR2,V_ACCESS_MODULE VARCHAR2,V_ACCESS_KEY VARCHAR2,V_APP_ID NUMBER,V_PARAM1 NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE DELETE_EXTERNAL_ACCESS(V_REC_ID NUMBER);
  PROCEDURE UPDATE_EXTERNAL_ACCESS_IP(V_REC_ID NUMBER,V_ACCESSOR_IP VARCHAR2);
  PROCEDURE UPDATE_EXTERNAL_ACCESS_P1(V_REC_ID NUMBER,V_PARAM1 NUMBER);

END PKG_EXTERNAL_ACCESS;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_EXTERNAL_API_ACCESS" AS

  procedure has_access(v_module_name varchar2, v_ip_address varchar2, v_has_access out number);

END PKG_EXTERNAL_API_ACCESS;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_HTTP" is
    function http_post (
      p_url_in in varchar2,
      v_param in varchar2,
      p_app in varchar2,
      p_proxy_in in varchar2 default null,
      p_no_proxy_domains_in  in varchar2 default null,
      p_username_in in varchar2 default null,
      p_password_in in varchar2 default null
    ) return varchar2;
    
    FUNCTION http_get (p_url_in                IN VARCHAR2,
                       v_param                 IN VARCHAR2,
                       p_app                   IN VARCHAR2,
                       p_proxy_in              IN VARCHAR2 DEFAULT NULL,
                       p_no_proxy_domains_in   IN VARCHAR2 DEFAULT NULL,
                       p_username_in           IN VARCHAR2 DEFAULT NULL,
                       p_password_in           IN VARCHAR2 DEFAULT NULL)
      RETURN VARCHAR2;
end;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_HTTP_HANDLER" 
as
  
  procedure add_http_request(p_id out number, 
    p_message_id in number, 
    p_destination_address in varchar2,
    p_short_number in varchar2,
    p_message_body in varchar2,    
    p_module_id in varchar2,
    p_host_name in varchar2 := 'BAKCELL1',
    p_link in varchar2 := null,
    p_method in varchar2 := 'POST'
  );
end pkg_http_handler;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_HTTP_PROCESSOR" as 

  procedure process_requests;

end pkg_http_processor;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_HTTP_RU" is
  function http_post_ru (
    p_url_in in varchar2,
    v_param in varchar2,
    p_app in varchar2,
    p_proxy_in in varchar2 default null,
    p_no_proxy_domains_in  in varchar2 default null,
    p_username_in in varchar2 default null,
    p_password_in in varchar2 default null
  ) return number;
end; 
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_INCOMING_MESSAGE" AS

  type t_cursor is ref cursor;
  procedure search_by_short_number(v_short_number_id number, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure search_all_by_short_number(v_short_number_id number, v_date_from date, v_date_to date, v_items_cursor out t_cursor);
  procedure search_by_short_number_and_app(v_short_number_id number, v_date_from date, v_date_to date, v_app_name varchar2, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure search_all_by_short_number_app(v_short_number_id number, v_date_from date, v_date_to date, v_app_name varchar2, v_items_cursor out t_cursor);
  procedure search_by_sender(v_source_addr varchar2, v_short_number_id number, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure search_all_by_sender(v_source_addr varchar2, v_short_number_id number, v_date_from date, v_date_to date, v_items_cursor out t_cursor);
  procedure search_all_by_sender(v_source_addr varchar2,  v_date_from date, v_date_to date,v_page_id number,v_total_count out number, v_items_cursor out t_cursor);
procedure search_by_short_number_all( v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
END PKG_INCOMING_MESSAGE;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_LIVE_SMS" AS

  type t_cursor is ref cursor;
  procedure get_live_sms_by_short_number(v_ext_access_rec_id number, v_short_number_id number, v_for_date date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure reply_to_sms(v_ext_access_rec_id number, v_short_number_id number, v_msg_id number, v_text nvarchar2);

END PKG_LIVE_SMS;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_LOGGER" AS
  
  procedure do_log(v_log_text varchar2, v_app_name varchar2, log_category varchar2);
  procedure do_log(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2, log_category varchar2);

  procedure log_debug(v_log_text varchar2, v_app_name varchar2);
  procedure log_debug(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2);
  
  procedure log_error(v_log_text varchar2, v_app_name varchar2);
  procedure log_error(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2);
  
  procedure log_debug_auto(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2);  
  
  procedure log_debug_auto(v_log_text varchar2, v_app_name varchar2);  
  
  procedure log_error_auto(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2);
  
  procedure log_error_auto(v_log_text varchar2, v_app_name varchar2);

END PKG_LOGGER;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_MANAGER_ACCESS" AS

  type t_cursor is ref cursor;
  procedure get_manager_ip_addresses(v_man_id number, v_items_cursor out t_cursor);
  procedure update_ip_address(v_rec_id number, v_ip_address varchar2);
  procedure delete_ip_address(v_rec_id number);
  PROCEDURE ADD_IP_ADDRESS(V_MANAGER_ID NUMBER,V_IP_ADDRESS VARCHAR2);

END PKG_MANAGER_ACCESS;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_OUTGOING_MESSAGE" AS

  type t_cursor is ref cursor;
  procedure search_by_date_range(v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure search_by_date_range(v_short_number varchar2,v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  procedure search_by_receiver(v_destination_addr varchar2, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  
  procedure search_by_receiver(v_source_addr varchar2,v_destination_addr varchar2, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);

END PKG_OUTGOING_MESSAGE;

/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL" AS

  type t_cursor is ref cursor;
  procedure get_polls_to_display(v_items_cursor out t_cursor);
  procedure get_poll_to_display(v_poll_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_POLL(V_POLL_NAME NVARCHAR2,V_POLL_DESC NVARCHAR2,V_POLL_TYPE NUMBER,V_NEW_ROW_ID OUT NUMBER);
  procedure get_most_active_numbers(v_poll_id number, v_phase_ids nvarchar2, v_max_count number, v_items_cursor out t_cursor);

END PKG_POLL;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_MEMBER" AS

  type t_cursor is ref cursor;
  PROCEDURE GET_POLL_MEMBERS_BY_POLL_ID(V_POLL_ID NUMBER, V_ITEMS_CURSOR OUT T_CURSOR);
  PROCEDURE ADD_POLL_MEMBER(V_POLL_ID NUMBER,V_MEMBER_NAME NVARCHAR2,V_MEMBER_DESC NVARCHAR2,V_IS_ACTIVE NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_POLL_MEMBER(V_MEMBER_ID NUMBER,V_POLL_ID NUMBER,V_MEMBER_NAME NVARCHAR2,V_MEMBER_DESC NVARCHAR2,V_IS_ACTIVE NUMBER);
  PROCEDURE DELETE_POLL_MEMBER(V_MEMBER_ID NUMBER);

END PKG_POLL_MEMBER;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_NUMBER" AS

  type t_cursor is ref cursor;
  procedure get_numbers_by_member_id(v_member_id number, v_items_cursor out t_cursor);
  procedure get_numbers_by_poll_id(v_poll_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_POLL_MEMBER_NUMBER(V_MEMBER_ID NUMBER,V_SHORT_NUMBER_ID NUMBER,V_VOTE_COUNT NUMBER,V_POLL_ID NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_POLL_MEMBER_NUMBER(V_NUMBER_ID NUMBER,V_MEMBER_ID NUMBER,V_SHORT_NUMBER_ID NUMBER,V_VOTE_COUNT NUMBER,V_POLL_ID NUMBER);
  PROCEDURE DELETE_POLL_MEMBER_NUMBER(V_NUMBER_ID NUMBER);

END PKG_POLL_NUMBER;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_PHASE" AS

  type t_cursor is ref cursor;
  procedure get_poll_phases_by_poll_id(v_poll_id number, v_items_cursor out t_cursor);
  procedure get_active_phase_by_poll_id(v_poll_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_POLL_PHASE(V_POLL_ID NUMBER,V_PHASE_NAME NVARCHAR2,V_PHASE_DESC NVARCHAR2,V_PHASE_START_DATE DATE,V_PHASE_END_DATE DATE,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_POLL_PHASE(V_PHASE_ID NUMBER,V_POLL_ID NUMBER,V_PHASE_NAME NVARCHAR2,V_PHASE_DESC NVARCHAR2,V_PHASE_START_DATE DATE,V_PHASE_END_DATE DATE);
  PROCEDURE DELETE_POLL_PHASE(V_PHASE_ID NUMBER);
  
  procedure change_poll_phase_time(v_poll_id number, v_phase_id number, v_start_time number, v_add number);

END PKG_POLL_PHASE;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_PROCESSING" AS

  procedure process_ongoing_polls;
  procedure process_ongoing_poll_phase(v_poll_id number, v_phase_id number, v_poll_type number, v_start_date date, v_end_date date);
  procedure process_numeric_poll_phase(v_poll_id number, v_phase_id number, v_start_date date, v_end_date date, v_max_id number);
  
  procedure process_text_poll_phase(v_poll_id number, v_phase_id number, v_start_date date, v_end_date date, v_max_id number);

END PKG_POLL_PROCESSING;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_RESULTS" AS

  type t_cursor is ref cursor;
  procedure get_member_votes_by_poll_phase(v_phase_id number, v_items_cursor out t_cursor);

END PKG_POLL_RESULTS;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_POLL_TEXT" AS

  type t_cursor is ref cursor;
  procedure get_texts_by_member_id(v_member_id number, v_items_cursor out t_cursor);
  PROCEDURE ADD_POLL_MEMBER_TEXT(V_MEMBER_ID NUMBER,V_TEXT_BODY NVARCHAR2,V_POLL_ID NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_POLL_MEMBER_TEXT(V_TEXT_ID NUMBER,V_MEMBER_ID NUMBER,V_TEXT_BODY NVARCHAR2,V_POLL_ID NUMBER);
  PROCEDURE DELETE_POLL_MEMBER_TEXT(V_TEXT_ID NUMBER);

END PKG_POLL_TEXT;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_QUICK_SMS" as

  procedure send(p_key varchar2, p_text varchar2, p_msisdn varchar2, p_operator_id number, o_result out number);
  
end;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_REPORT_ARAZ" 
as
 type t_cursor is ref cursor;
 
 procedure get_incoming_sms_by_arazfm(v_msisdn in varchar2, v_from date, v_to date,  v_items_cursor out t_cursor);
 procedure get_incoming_sms_by_arazfm( v_items_cursor out t_cursor);
 procedure get_incoming_sms_by_arazfm(v_msisdn in varchar2, v_from date, v_to date,  v_items_cursor out t_cursor, v_page_number in number, v_total_count out number) ;
 
 end pkg_report_araz;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_REQUEST_HANDLER" AS 

  c_call_proc_success constant number := 0;
  c_call_proc_retry constant number := 1;
  c_call_proc_error constant number := 1000;

  type t_handler_rec_type is record 
  (
    msg_id number,
    host_name smpp_incoming.host_name%type,
    a_num smpp_incoming.source_addr%type,
    b_num smpp_incoming.destination_addr%type,
    date_added smpp_incoming.date_added%type,
    msg_body smpp_incoming.msg_body%type,
    app_name handler_list.app_name%type,
    
    num_param1 handler_list.num_param1%type,
    num_param2 handler_list.num_param2%type,
    str_param1 handler_list.str_param1%type,
    str_param2 handler_list.str_param2%type
  );
  
  type t_proc_result_type is record 
  (
    num_result number := pkg_request_handler.c_call_proc_success,
    str_result varchar2(500),
    app_num_id number,
    sms_template varchar2(2000),
    ok_to_charge number := 1,
    tariff_id number,
    sms_priority number := 1
  );
  
  type t_charge_result_type is record 
  (
    num_result number := pkg_diameter.const_success,
    str_result varchar2(500)
  ); 

  procedure process_queue(p_queue_id number);
  function call_proc(p_proc_name varchar2, p_msg_info t_handler_rec_type) return t_proc_result_type;
  function charge_request(p_msisdn varchar2, 
                          p_tariff_id number, 
                          p_client_id number,
                          p_charge_reason varchar2,
                          p_msg_id number) return t_charge_result_type;
  function get_sms_tpl_for_charge_code(p_handler_id number, p_diam_resp_code number) return varchar2;
  
  procedure mark_item_for_retry(p_rec_id number, p_msisdn varchar2);
  procedure set_item_status(p_rec_id number, p_status varchar2, 
                            p_msg_id number, p_app_name varchar2,
                            p_app_num_id number, p_str_result varchar2,
                            p_msisdn varchar2);

END PKG_REQUEST_HANDLER;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_AUTOSMS" AS

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
   
   function is_member(p_msg_info pkg_request_handler.t_handler_rec_type) return number;

   procedure process_info_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure process_ad_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_price_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_price_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
                             
   procedure send_year_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_year_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_mileage_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_mileage_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_market_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_models(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_model_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                                                      
   procedure send_announcement_instructions(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure clear_preferences(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_detailed_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_more_content(p_msg_info pkg_request_handler.t_handler_rec_type);
   
   procedure post_subs_handler(p_msisdn varchar2, p_msg_body varchar2, p_host_name varchar2, p_short_number varchar2);

END;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_CENSORING" as 

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type);

end pkg_rh_censoring;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_EVSMS" AS

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
   
   function is_member(p_msg_info pkg_request_handler.t_handler_rec_type) return number;

   procedure process_info_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure process_ad_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_sell_type_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_building_type_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_region_list(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_announcement_instructions(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure clear_preferences(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure set_region_preference(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_detailed_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure send_more_content(p_msg_info pkg_request_handler.t_handler_rec_type);

END;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_FAKE_MOZOOK_ALT" as 

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type);

end pkg_rh_fake_mozook_alt;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_HTTP_SENDER" as 

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type);

end pkg_rh_http_sender;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_SMSC" as 
   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type);
end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_SUBSCRIPTION" as

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type);
                             
   procedure check_for_subscribe_request(p_cleaned_msg_body smpp_incoming.msg_body%type,
                                         p_msg_info pkg_request_handler.t_handler_rec_type, 
                                         o_result in out pkg_request_handler.t_proc_result_type);
   
   procedure process_unsubscribe_request(p_cleaned_msg_body smpp_incoming.msg_body%type,
                                         p_msg_info pkg_request_handler.t_handler_rec_type,
                                         o_result in out pkg_request_handler.t_proc_result_type);
                       
   function get_subs_keywords_as_table(p_short_number_id number) return t_keywords_tbl;
   
   procedure unsubscribe_all(p_msg_info pkg_request_handler.t_handler_rec_type);
   procedure unsubscribe(p_msg_info pkg_request_handler.t_handler_rec_type, p_variant_id number);
   
   --procedure send_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
   --                    o_result in out pkg_request_handler.t_proc_result_type);
                       
   function get_next_content(p_subs_id number, p_msisdn varchar2, p_alg_id number, o_content_id out varchar2) return varchar2;
   
   --either p_sch_id or (p_subs_id and p_msisdn) must be set
   function send_content(p_sch_id number, p_subs_id number := null, p_msisdn varchar2 := null, p_priority number := 0) return number;
   
   function set_preference(p_msisdn varchar2, p_pref_name varchar2, p_pref_val varchar2) return number;
   function add_preference(p_msisdn varchar2, p_pref_name varchar2, p_pref_val varchar2) return number;
   function get_preference(p_msisdn varchar2, p_pref_name varchar2, p_default_val varchar2) return varchar2;
   function has_preference(p_msisdn varchar2, p_pref_name varchar2) return number;
   procedure clear_preferences(p_msisdn varchar2, p_pref_name_like varchar2);
   
   function is_charged(p_msisdn varchar2, p_subs_id number) return number;
   function ensure_charged(p_msisdn varchar2, p_subs_id number, p_msg_id number) return number;
   procedure charge_customers(p_operator_id number, p_msisdn varchar2 := null, p_subs_id number := null, p_msg_id number := -1);
   
end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_RH_TEST" as 

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type);
  --procedure emulate_new_sms ;

end pkg_rh_test;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SEGMENT" AS

   function get_segment_id(p_msisdn varchar2) return number;
   
   procedure add_to_segment(p_msisdn varchar2, p_segment_id number);

END;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SHORT_NUMBER" AS

  type t_cursor is ref cursor;
  procedure get_all_short_numbers(v_items_cursor out t_cursor);
  procedure get_all_short_numbers(p_group_id in number, v_items_cursor out t_cursor);
  procedure get_mt_numbers(v_items_cursor out t_cursor);
  procedure set_is_active_now(v_short_number_id number, v_is_active_now number);
  procedure get_is_active_now(v_short_number_id number, v_is_active_now out number);
  procedure get_bulk_sms_numbers(p_login_name in varchar2, v_items_cursor out t_cursor);
  function get_number_id(p_short_number varchar2, p_host_name varchar2) return number;
END PKG_SHORT_NUMBER;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMPP" AS

  type t_cursor is ref cursor;
  procedure register_received_sms(v_host_name varchar2, v_in_date date, v_esm_class number, 
                                  v_source_addr varchar2,v_destination_addr varchar2, 
                                  v_msg_body nvarchar2, v_file_name varchar2,
                                  o_msg_id out number);
  procedure post_process_received_sms(v_msg_id number, v_host_name varchar2, v_in_date date, v_esm_class number, v_source_addr varchar2,v_destination_addr varchar2, v_msg_body nvarchar2, v_file_name varchar2);
  procedure get_pending_messages_to_send(v_host_name varchar2, v_items_cursor out t_cursor);
  procedure mark_message_as_processed(v_msg_id number, v_smsc_id varchar2);
  procedure mark_message_as_in_error(v_msg_id number, v_err_code varchar2);
  procedure set_message_delivery_info(v_smsc_id varchar2, v_delivered number, v_final_status varchar2, v_done_date date, v_host_name varchar2);

END PKG_SMPP;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMPP_HOST" AS

  type t_cursor is ref cursor;
  procedure get_smpp_hosts(v_items_cursor out t_cursor);

END PKG_SMPP_HOST;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMPP_POST_PROCESSING" AS

  procedure process_received_sms(v_msg_id number, v_host_name varchar2, v_in_date date, v_esm_class number, v_source_addr varchar2,v_destination_addr varchar2, v_msg_body nvarchar2, v_file_name varchar2);

END PKG_SMPP_POST_PROCESSING;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMSC" as
   
   type t_cursor is ref cursor;
   
   procedure send_sms(p_system_id varchar2, p_source_addr varchar2, p_destination_addr varchar2, 
                      p_msg_body nvarchar2, p_delivery_report number,
                      o_result out number,
                      o_new_row_id out number);
                      
   procedure get_inc_queue(p_system_ids varchar2, p_items_cursor out t_cursor);
   procedure mark_queue_item_as_delivered(p_item_id number);

end;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMSC_SMS_DELIVERY" is

procedure add_third_party_deliverable(v_tp_system_id varchar2,v_source_addr_alias varchar2,v_destination_addr varchar2,v_msg_body nvarchar2,v_smpp_incoming_id number);  

end pkg_smsc_sms_delivery;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMSC_SMS_SENDER" is

  procedure send_sms(v_system_id varchar2, v_source_addr_alias varchar2, v_destination_addr varchar2, v_msg_body nvarchar2, v_delivery_report number, v_priority number, v_app_str_param1 nvarchar2, v_new_row_id out number);

end pkg_smsc_sms_sender;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMS_ONLINE" as

  type t_cursor is ref cursor;
  procedure get_messages_by_short_number(v_ext_access_rec_id number, v_short_number_id number, v_last_msg_id number, v_items_cursor out t_cursor);

end pkg_sms_online;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMS_SENDER" AS

  g_max_sms_length constant number := 1000;

  procedure send_sms(v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, 
                     v_msg_body nvarchar2, v_delivery_report number, v_priority number, 
                     v_app_name varchar2);
                     
  procedure send_sms_at(v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, 
                     v_msg_body nvarchar2, v_delivery_report number, v_priority number, 
                     v_app_name varchar2, v_send_at date, v_app_num_param1 number := null, v_validity_period number := 72);
  
  procedure send_sms_template(v_host_name varchar2, v_source_addr varchar2, 
                              v_destination_addr varchar2, v_template nvarchar2, 
                              v_delivery_report number, v_priority number, v_app_name varchar2,
                              v_send_at date := sysdate);
  
  procedure send_sms(v_short_number_id number, v_numbers varchar2, v_msg_body nvarchar2, v_priority number, v_app_name varchar2);
  
  procedure send_sms(v_sms_id out number, v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, v_msg_body nvarchar2, v_delivery_report number, v_priority number,  v_app_name varchar2);
  
  procedure send_wap_push(v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, v_title nvarchar2, v_url nvarchar2, v_delivery_report number, v_priority number);

END PKG_SMS_SENDER;
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_SMS_TICKER" as

  type t_cursor is ref cursor;
  
  type t_pending_ret_type is record 
  (
     msg_id number,
     in_date date,
     source_addr varchar2(50),
     client varchar2(100),
     msg_body varchar2(500),
     actions varchar2(10)
  );
  

  procedure mark_message(p_msg_id number, p_status varchar2, p_client varchar2, p_no_charge number, o_result out varchar2);
  procedure add_to_black_list(p_msg_id number);
  procedure mark_as_processed(p_msg_id number, p_status varchar2, p_client varchar2);
  procedure send_response(p_msg_id number, p_tpl_id number);
  function charge(p_msg_id number) return number;
  procedure add_tpl;
  
  procedure get_pending_messages(o_cur out t_cursor);
  
  function has_pending_messages return number;
  
  procedure notify_of_pending_messages;
  
end;
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_UTIL" AS

  FUNCTION get_number_table(v_split_string VARCHAR2, v_splitter VARCHAR2) RETURN number_list;
  FUNCTION get_string_table(v_split_string VARCHAR2, v_splitter VARCHAR2) RETURN string_list;
  FUNCTION verify_numeric (check_this VARCHAR2) RETURN BOOLEAN;
  FUNCTION get_matching_symbol_count(v_orig nvarchar2, v_tocheck nvarchar2) return number; 
  FUNCTION md5_digest(v_string in varchar2) RETURN VARCHAR2;
  FUNCTION format_msisdn(v_msisdn varchar2) return varchar2;
  FUNCTION get_http_response(p_url varchar2, p_method varchar2 := 'GET', p_post_data varchar2:=null, p_post_content_type varchar2 := 'application/x-www-form-urlencoded') return varchar2;

END PKG_UTIL;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_CATEGORY" AS

  type t_cursor is ref cursor;
  PROCEDURE GET_WEB_PUSH_CATEGORIES(V_SITE_ID NUMBER, ITEMS_CURSOR OUT T_CURSOR);
  PROCEDURE GET_WEB_PUSH_CATEGORY_BY_ID(V_CAT_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR);
  PROCEDURE ADD_WEB_PUSH_CATEGORY(V_PARENT_CAT_ID NUMBER,V_CAT_NAME NVARCHAR2,V_SHORT_NUMBER_ID NUMBER,V_SITE_ID NUMBER,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_WEB_PUSH_CATEGORY(V_CAT_ID NUMBER,V_PARENT_CAT_ID NUMBER,V_CAT_NAME NVARCHAR2,V_SHORT_NUMBER_ID NUMBER);
  PROCEDURE DELETE_WEB_PUSH_CATEGORY(V_CAT_ID NUMBER);

END PKG_WEB_PUSH_CATEGORY;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_ELEMENT" AS

  type t_cursor is ref cursor;
  procedure get_by_cat_id(v_cat_id number, v_page_id number, v_total_count out number, v_items_cursor out t_cursor);
  PROCEDURE ADD_UPDATE_WEB_PUSH_ELEMENT(V_CAT_ID NUMBER, V_ELEM_ID NUMBER,V_ELEM_NAME NVARCHAR2,V_ELEM_CODE NVARCHAR2,V_NEW_ROW_ID OUT NUMBER);
  PROCEDURE UPDATE_WEB_PUSH_ELEMENT(V_ELEM_ID NUMBER,V_ELEM_NAME NVARCHAR2,V_ELEM_CODE NVARCHAR2);
  PROCEDURE UPDATE_WEB_PUSH_ELEMENT_CODE(V_ELEM_ID NUMBER,V_ELEM_CODE NVARCHAR2);
  PROCEDURE DELETE_WEB_PUSH_ELEMENT(V_ELEM_ID NUMBER);
  PROCEDURE get_web_push_element(v_elem_id number, v_items_cursor out t_cursor);

END PKG_WEB_PUSH_ELEMENT;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_FILE" AS

TYPE T_CURSOR IS REF CURSOR;
PROCEDURE ADD_UPDATE_WEB_PUSH_FILE(V_ELEM_ID NUMBER,V_RES_ID NUMBER,V_CONTENT_TYPE NVARCHAR2,V_FILE_NAME NVARCHAR2,V_NEW_ROW_ID OUT NUMBER);
PROCEDURE DELETE_WEB_PUSH_FILE(V_FILE_ID NUMBER);
PROCEDURE GET_FILE_INFO(V_ELEM_ID NUMBER, V_RES_ID NUMBER, V_REG_KEY varchar2, V_ITEMS_CURSOR OUT T_CURSOR);
PROCEDURE GET_FILE_INFO_WITHOUT_KEY(V_ELEM_ID NUMBER, V_RES_ID NUMBER, V_ITEMS_CURSOR OUT T_CURSOR);
FUNCTION GET_FILE_EXTENSION(V_ELEM_ID NUMBER) RETURN NVARCHAR2;

END PKG_WEB_PUSH_FILE; 
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_REQUEST" AS

  type t_cursor is ref cursor;
  procedure add_web_push_request(v_elem_id number, v_reg_key varchar2, v_site_id number, v_source_addr varchar2);
  procedure get_request_by_key(v_reg_key varchar2, v_items_cursor out t_cursor);
  procedure get_res_id_by_phone(p_phone_model varchar2, v_items_cursor out t_cursor);

END PKG_WEB_PUSH_REQUEST; 
 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_SCREEN_RES" AS

  type t_cursor is ref cursor;
  procedure get_screen_resolutions(v_elem_id number, v_items_cursor out t_cursor); 
  procedure get_screen_resolutions_for(v_elem_id number, v_items_cursor out t_cursor); 

END PKG_WEB_PUSH_SCREEN_RES;

 
/
CREATE OR REPLACE PACKAGE "OZUMUZ"."PKG_WEB_PUSH_SITE" AS

  type t_cursor is ref cursor;
  procedure get_all_web_push_sites(v_items_cursor out t_cursor);
  procedure get_site_name_by_id(v_site_id number, v_site_name out nocopy nvarchar2);
  function get_site_id_by_prefix(v_msg_body nvarchar2, v_elem_code out nvarchar2) return number;

END PKG_WEB_PUSH_SITE;

 
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."BULKSMS_API" 
AS
   /******************************************************************************
      NAME:       BULKSMS_API
      PURPOSE:

      REVISIONS:
      Ver        Date        Author           Description
      ---------  ----------  ---------------  ------------------------------------
      1.0        1/13/2012      rdavudov       1. Created this package body.
   ******************************************************************************/

   FUNCTION check_login (pi_login IN VARCHAR2, pi_password IN VARCHAR2)
      RETURN NUMBER
   IS
      v_rec_id   NUMBER;
      v_hash     VARCHAR2 (400);
   BEGIN
      SELECT user_id, pwd_hash
        INTO v_rec_id, v_hash
        FROM ui_users
       WHERE login = pi_login;

      IF (pi_password = v_hash)
      THEN
         RETURN 1;
      ELSE
         RETURN 0;
      END IF;
   EXCEPTION
      WHEN NO_DATA_FOUND
      THEN
         RETURN 0;
   END;

   FUNCTION check_balance (pi_login IN VARCHAR2, pi_lock IN NUMBER DEFAULT 0)
      RETURN NUMBER
   IS
      v_rec_id   NUMBER;
      v_units    NUMBER;
   BEGIN
      SELECT user_id
        INTO v_rec_id
        FROM ui_users
       WHERE login = pi_login;

      IF (pi_lock = 0)
      THEN
         SELECT balance
           INTO v_units
           FROM BULK_SMS2_BALANCE
          WHERE user_id = v_rec_id;
      ELSIF (pi_lock = 1)
      THEN
         SELECT balance
           INTO v_units
           FROM BULK_SMS2_BALANCE
          WHERE user_id = v_rec_id
         FOR UPDATE;
      END IF;

      RETURN v_units;
   EXCEPTION
      WHEN NO_DATA_FOUND
      THEN
         RETURN -1;
   END;

   FUNCTION check_schedule (pi_schedule IN VARCHAR2)
      RETURN NUMBER
   IS
   BEGIN
      RETURN 1;
   /*IF (TO_NUMBER (TO_CHAR (to_date(pi_schedule,'YYYY-MM-DD HH24:MI:SS'), 'HH24MI')) BETWEEN 900 AND 2000)
   THEN
      RETURN 1;
   ELSE
      RETURN 0;
   END IF;*/
   END;

   FUNCTION check_sender_name (pi_login         IN VARCHAR2,
                               pi_sender_name   IN VARCHAR2)
      RETURN NUMBER
   IS
      v_rec_id   NUMBER;
      v_id       NUMBER;
   BEGIN
      SELECT user_id
        INTO v_rec_id
        FROM ui_users
       WHERE login = pi_login;

      SELECT COUNT (sender_id)
        INTO v_id
        FROM BULK_SMS2_SENDER_NAMES
       WHERE     LOWER (TRIM (SENDER_NAME)) = LOWER (TRIM (pi_sender_name))
             AND USER_ID = v_rec_id;

      IF (v_id = 0)
      THEN
         RETURN -1;
      ELSE
         RETURN 1;
      END IF;
   EXCEPTION
      WHEN NO_DATA_FOUND
      THEN
         RETURN -1;
   END;

   PROCEDURE get_units (pi_login       IN     VARCHAR2,
                        pi_password    IN     VARCHAR2,
                        po_resp_code      OUT VARCHAR2,
                        po_units          OUT NUMBER)
   IS
      v_units   NUMBER;
   BEGIN
      IF (check_login (pi_login, pi_password) = 1)
      THEN
         v_units := check_balance (pi_login);

         IF (v_units > -1)
         THEN
            po_resp_code := '000';
            po_units := v_units;
         ELSE
            po_resp_code := '230';
         END IF;
      ELSE
         po_resp_code := '105';
      END IF;
   EXCEPTION
      WHEN NO_DATA_FOUND
      THEN
         po_resp_code := '105';
   END;

   PROCEDURE add_bulk_sms (pi_login       IN     VARCHAR2,
                           pi_password    IN     VARCHAR2,
                           pi_title       IN     VARCHAR2,
                           pi_schedule    IN     VARCHAR2,
                           pi_message     IN     VARCHAR2,
                           pi_count       IN     NUMBER,
                           pi_controlid   IN     VARCHAR2,
                           po_resp_code      OUT VARCHAR2,
                           po_task_id        OUT NUMBER)
   IS
      v_rec_id      NUMBER;
      v_units       NUMBER;
      v_controlid   VARCHAR2 (64);
      v_count       NUMBER;
      v_user_id     number;
   BEGIN
      IF (check_login (pi_login, pi_password) = 1)
      THEN
         IF (check_schedule (pi_schedule) = 1)
         THEN
            IF (check_sender_name (pi_login, pi_title) = 1)
            THEN
               IF (pi_controlid IS NULL)
               THEN
                  SELECT TO_CHAR (SYSTIMESTAMP, 'YYYYMMDDHH24MISSSSS')
                    INTO v_controlid
                    FROM DUAL;
               ELSE
                  v_controlid := pi_controlid;
               END IF;

               v_controlid := pi_login || '_' || v_controlid;

               BEGIN
                  SELECT rec_id
                    INTO v_rec_id
                    FROM bulk_sms2
                   WHERE api_control_id = v_controlid;

                  po_task_id := v_rec_id;
                  po_resp_code := '002';
               EXCEPTION
                  WHEN NO_DATA_FOUND
                  THEN
                     SELECT user_id
                       INTO v_rec_id
                       FROM ui_users
                      WHERE login = pi_login;

                     v_units := check_balance (pi_login);

                     IF (v_units < pi_count)
                     THEN
                        ROLLBACK;
                        po_resp_code := '118';
                        RETURN;
                     END IF;

                     select user_id into v_user_id from ui_users where login=pi_login;
                     
                     SELECT BULK_SMS2_SEQ.NEXTVAL INTO po_task_id FROM DUAL;

                     INSERT INTO BULK_SMS2 (REC_ID,
                                           --DISTRIBUTION_NAME,
                                           COMMENTS,
                                           START_AT,
                                           STATUS,
                                           sender_id,
                                           MSG_BODY,
                                           API_CONTROL_ID,
                                           creator_id)
                          VALUES (
                                    po_task_id,
                                    --pi_login
                                    --|| '_'
                                    --|| TO_CHAR (SYSDATE, 'YYYYMMDDHH24MISS'),
                                    '',
                                    TO_DATE (pi_schedule,
                                             'YYYY-MM-DD HH24:MI:SS'),
                                    0,
                                    (select max(sender_id) from bulk_sms2_sender_names where LOWER (TRIM (SENDER_NAME)) = LOWER (TRIM (pi_title))),
                                    pi_message,
                                    v_controlid,
                                    v_user_id);

                     po_resp_code := '000';
               END;
            ELSE
               po_resp_code := '235';
            END IF;
         ELSE
            po_resp_code := '232';
         END IF;
      ELSE
         po_resp_code := '105';
      END IF;
   END;

   PROCEDURE add_bulk_msisdn (pi_task_id   IN NUMBER,
                              pi_msisdn    IN VARCHAR2,
                              pi_message   IN VARCHAR2)
   IS
      v_exist   NUMBER := 0;
      v_black_list number;
   BEGIN
      if pi_msisdn like '99455590%' then --block all bakcell employees
         return;
      end if;
   
      /*SELECT COUNT (0)
        INTO v_exist
        FROM BULK_SMS2_NUMBERS
       WHERE BULK_SMS_ID = pi_task_id AND MSISDN = pi_msisdn;*/

      SELECT COUNT (*)
        INTO v_black_list
        FROM black_list
       WHERE for_bulk = 1 AND msisdn = pi_msisdn;


      IF v_exist = 0 and v_black_list = 0
      THEN
         INSERT INTO bulk_sms2_numbers (rec_id,
                                       bulk_sms_id,
                                       msisdn,
                                       msg_body)
              VALUES (bulk_sms2_numbers_seq.NEXTVAL,
                      pi_task_id,
                      pi_msisdn,
                      pi_message);
      END IF;
   END;

   PROCEDURE start_bulk_sms (pi_task_id IN NUMBER, po_resp_code OUT VARCHAR2)
   IS
      l_already_started   NUMBER := 0;
      v_user_id           NUMBER;
      v_balance           NUMBER;
      v_count             NUMBER;
      v_sms_count         NUMBER;
      l_dummy             varchar2(2000);
   BEGIN
      SELECT COUNT (0)
        INTO l_already_started
        FROM bulk_sms2
       WHERE rec_id = pi_task_id AND status = 1;

      IF l_already_started > 0
      THEN
         po_resp_code := '231';
         RETURN;
      END IF;

      SELECT COUNT (*)
        INTO v_sms_count
        FROM BULK_SMS2_NUMBERS
       WHERE bulk_sms_id = pi_task_id;

     select creator_id into v_user_id from bulk_sms2 where rec_id=pi_task_id;

      SELECT balance
        INTO v_balance
        FROM BULK_SMS2_BALANCE
       WHERE user_id = v_user_id
      FOR UPDATE;

      IF (v_sms_count > v_balance)
      THEN
         po_resp_code := '118';
         RETURN;
      END IF;

      /*UPDATE bulk_sms2
         SET status=1
       WHERE rec_id = pi_task_id;*/
      
      /*
      INSERT INTO smpp_outgoing
         SELECT smpp_outgoing_seq.NEXTVAL,
                source_addr,
                destination_addr,
                msg_body,
                delivery_report,
                deliver_at,
                deliver_at + 30,
                0,
                0,
                NULL,
                NULL,
                0, 
                pi_task_id,
                NULL,
                NULL,
                NULL,
                host_name,
                SYSDATE,
                NULL,
                NULL,
                'BULK_SMS_API',
                NULL,
                SYSDATE, null, 24
           FROM bulk_sms_numbers_api_view
          WHERE bulk_sms_id = pi_task_id;*/
          
          pkg_bulk_sms2.start_sending(pi_task_id, 
                                      v_user_id,
                                      l_dummy);
                                      
          pkg_logger.log_debug('Output from start_sending: '||l_dummy,'BULKSMS_API');


      po_resp_code := '000';
   END;

   PROCEDURE get_delivery_report (pi_login       IN     VARCHAR,
                                  pi_password    IN     VARCHAR2,
                                  pi_task_id     IN     NUMBER,
                                  po_resp_code      OUT VARCHAR2,
                                  po_pending        OUT NUMBER,
                                  po_delivered      OUT NUMBER,
                                  po_failed         OUT NUMBER,
                                  po_removed        OUT NUMBER,
                                  po_error          OUT NUMBER)
   IS
      v_bulk   BULK_SMS2%ROWTYPE;
   BEGIN
      IF (check_login (pi_login, pi_password) = 1)
      THEN
         BEGIN
            SELECT *
              INTO v_bulk
              FROM bulk_sms2
             WHERE rec_id = pi_task_id;

            SELECT COUNT (*)
              INTO po_pending
              FROM smpp_outgoing
             WHERE app_name in ('BULK_SMS_API','BULK_SMS2') AND app_num_param1 = pi_task_id
                   AND (final_status IS NULL
                        OR final_status IN ('expired', 'EXPIRED'))
                   AND status <> 2;

            SELECT COUNT (*)
              INTO po_delivered
              FROM smpp_outgoing
             WHERE     app_name in ('BULK_SMS_API','BULK_SMS2')
                   AND app_num_param1 = pi_task_id
                   AND final_status IN ('delivered', 'DELIVRD');

            SELECT COUNT (*)
              INTO po_failed
              FROM smpp_outgoing
             WHERE     app_name in ('BULK_SMS_API','BULK_SMS2')
                   AND app_num_param1 = pi_task_id
                   AND final_status IN ('undelivered', 'UNDELIV');

            SELECT COUNT (*)
              INTO po_error
              FROM smpp_outgoing
             WHERE     app_name in ('BULK_SMS_API','BULK_SMS2')
                   AND app_num_param1 = pi_task_id
                   AND final_status IS NULL
                   AND status = 2;

            po_resp_code := '000';
         EXCEPTION
            WHEN NO_DATA_FOUND
            THEN
               po_resp_code := '109';
         END;
      ELSE
         po_resp_code := '105';
      END IF;
   END;

   PROCEDURE get_detailed_report (pi_login       IN     VARCHAR,
                                  pi_password    IN     VARCHAR2,
                                  pi_task_id     IN     NUMBER,
                                  po_resp_code      OUT VARCHAR2,
                                  po_cursor         OUT t_cursor)
   IS
      v_bulk   BULK_SMS2%ROWTYPE;
   BEGIN
      IF (check_login (pi_login, pi_password) = 1)
      THEN
         BEGIN
            SELECT *
              INTO v_bulk
              FROM bulk_sms2
             WHERE rec_id = pi_task_id;

            OPEN po_cursor FOR
               SELECT destination_addr,
                      msg_body,
                      CASE
                         WHEN final_status IS NULL AND status = 2
                         THEN
                            5
                         WHEN (final_status IS NULL
                               OR final_status IN ('expired', 'EXPIRED'))
                              AND status <> 2
                         THEN
                            1
                         WHEN final_status IN ('delivered', 'DELIVRD')
                         THEN
                            2
                         WHEN final_status IN ('undelivered', 'UNDELIV')
                         THEN
                            3
                      END
                         status
                 FROM smpp_outgoing
                WHERE app_name in ('BULK_SMS_API','BULK_SMS2')
                      AND app_num_param1 = pi_task_id;

            po_resp_code := '000';
         EXCEPTION
            WHEN NO_DATA_FOUND
            THEN
               po_resp_code := '109';
         END;
      ELSE
         po_resp_code := '105';
      END IF;
   END;
END BULKSMS_API;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_APP_MODULE" AS

  procedure has_access(v_manager_id number, v_module_name nvarchar2, v_has_access out number) is
  begin 
    select count(0) into v_has_access from managers_modules 
    inner join app_modules on managers_modules.module_id=app_modules.module_id
    where app_modules.module_name=v_module_name and managers_modules.man_id=v_manager_id;
  end;
  
  procedure get_all_modules(v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select * from app_modules start with parent_module_id is null 
    connect by parent_module_id=prior module_id;
  end;
  
  procedure get_all_modules_for_manager(v_man_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select t.*, (select count(0) from managers_modules where man_id=v_man_id and module_id=t.module_id) as has_access
    from app_modules t start with parent_module_id is null 
    connect by parent_module_id=prior module_id;
  end;
  
  procedure get_top_level_modules(v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select module_id, module_display_name from app_modules where parent_module_id is null;
  end; 
  
  PROCEDURE ADD_APP_MODULE(V_MODULE_DISPLAY_NAME NVARCHAR2,V_MODULE_NAME NVARCHAR2,V_PARENT_MODULE_ID NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO APP_MODULES (MODULE_ID,MODULE_DISPLAY_NAME,MODULE_NAME,PARENT_MODULE_ID) VALUES (APP_MODULES_SEQ.NEXTVAL,V_MODULE_DISPLAY_NAME,V_MODULE_NAME,V_PARENT_MODULE_ID);
  SELECT APP_MODULES_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;

END PKG_APP_MODULE;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_APP_SETTINGS" AS

   procedure set_value(p_key varchar2, p_value varchar2) is
   begin
      update app_settings set val = p_value where key = p_key;
      
      if sql%rowcount = 0 then
         insert into app_settings (key, val) values (p_key, p_value);
      end if;
   end;
   
   function get_value(p_key varchar2, p_default_value varchar2 := null) return varchar2 is
      l_result app_settings.val%type;
   begin
      select val into l_result from app_settings where key = p_key;
      return l_result;
   exception when no_data_found then
      return p_default_value;
   end;

END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_APP_SETTTINGS" as

  function get_value(p_key varchar2) return varchar2 as
    l_result varchar2(1000);
  begin
    select val into l_result from app_settings where key=p_key;
    
    return l_result;
  end get_value;

end pkg_app_setttings;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_ARAZFM_SMS" 
as
 
    procedure truncIncomingSms(v_cursor out t_cursor) is 
    Begin
        open v_cursor for
        select msg_id, to_char(in_date, 'dd.mm.yyyy hh24:mi:ss') as in_date, msg_body, in_date in_date2
        from (
          select i.msg_id, i.host_name, i.in_date, i.source_addr, i.destination_addr, i.msg_body, i.date_added, i.app_name
          from smpp_incoming i,diameter_requests d
          where i.in_date  >=  trunc(sysdate,'hh') and i.in_date < sysdate 
           and   d.in_date  >=  trunc(sysdate,'hh') and d.in_date < sysdate
           and   msg_body is not null
           and i.msg_id=d.msg_id
           and i.APP_NAME=d.module_id
              and i.destination_addr='81033'
              and d.diameter_answer=100
        
        )
        order by in_date2 desc;
    end ;
    
    
    procedure betweenHour(v_cursor out t_cursor) is
    Begin 
          open v_cursor for
        select msg_id, to_char(in_date, 'dd.mm.yyyy hh24:mi:ss') as in_date, msg_body, in_date in_date2
        from (
          select i.msg_id, i.host_name, i.in_date, i.source_addr, i.destination_addr, i.msg_body, i.date_added, i.app_name
          from smpp_incoming i ,diameter_requests d
          where i.in_date  >= trunc(sysdate-1/24,'hh') and  i.in_date < trunc(sysdate,'hh') and
         d.in_date  >= trunc(sysdate-1/24,'hh') and  d.in_date < trunc(sysdate,'hh') and   msg_body is not null
              and destination_addr='81033'
              and i.msg_id=d.msg_id
              and i.APP_NAME=d.module_id
              and d.diameter_answer=100
        )
        order by in_date2 desc;
    end;
    
    
     procedure GetSMS(v_from number,v_to number,  v_cursor out t_cursor) is
    Begin 
          open v_cursor for
        select msg_id, to_char(in_date, 'dd.mm.yyyy hh24:mi:ss') as in_date, msg_body, in_date in_date2
        from (
          select i.msg_id, i.host_name, i.in_date, i.source_addr, i.destination_addr, i.msg_body, i.date_added, i.app_name
          from smpp_incoming i ,diameter_requests d
          where i.in_date  >= trunc(sysdate-v_from/24,'hh') and  i.in_date < trunc(sysdate-v_to/24,'hh') and
         d.in_date  >= trunc(sysdate-v_from/24,'hh') and  d.in_date < trunc(sysdate-v_to/24,'hh') and   msg_body is not null
              and destination_addr='81033'
              and i.msg_id=d.msg_id
              and i.APP_NAME=d.module_id
              and d.diameter_answer=100
        )
        order by in_date2 desc;
    end;
    
    
       procedure GetSMS(v_from number, v_cursor out t_cursor) is
    Begin 
          open v_cursor for
        select msg_id, to_char(in_date, 'dd.mm.yyyy hh24:mi:ss') as in_date, msg_body, in_date in_date2
        from (
          select i.msg_id, i.host_name, i.in_date, i.source_addr, i.destination_addr, i.msg_body, i.date_added, i.app_name
          from smpp_incoming i ,diameter_requests d
          where i.in_date  >= trunc(sysdate-v_from/24,'hh') and  i.in_date <sysdate and
         d.in_date  >= trunc(sysdate-v_from/24,'hh') and  d.in_date <sysdate  and   msg_body is not null
              and destination_addr='81033'
              and i.msg_id=d.msg_id
              and i.APP_NAME=d.module_id
              and d.diameter_answer=100
        )
        order by in_date2 desc;
    end;
    procedure betweenTwoHour(v_cursor out t_cursor) is
    Begin 
          open v_cursor for
        select msg_id, to_char(in_date, 'dd.mm.yyyy hh24:mi:ss') as in_date, msg_body, in_date in_date2
        from (
             select i.msg_id, i.host_name, i.in_date, i.source_addr, i.destination_addr, i.msg_body, i.date_added, i.app_name
          from smpp_incoming i ,diameter_requests d
          where  i.in_date  >= trunc(sysdate-2/24,'hh') and  i.in_date < trunc(sysdate-1/24,'hh') and
          d.in_date  >= trunc(sysdate-2/24,'hh') and  d.in_date < trunc(sysdate-1/24,'hh') and   msg_body is not null
              and destination_addr='81033'
              and i.msg_id=d.msg_id
              and i.APP_NAME=d.module_id
              and d.diameter_answer=100
        )
        order by in_date2 desc;
    end;
    
end pkg_ArazFM_sms;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_AUTOSMS" as
   
   procedure get_next_content(p_subs_id number, p_msisdn varchar2, p_max_days_back number, o_result out varchar2, o_content_id out number) as
      l_append varchar2(1500);
      l_has_market_preference number;     
         
      l_has_content number := 0;
      l_suffix varchar2(200) := 'Etrafli melumat almaq istediyiniz elanin kodunu 6090-a gonderin (pulsuz)';
   begin
      o_content_id := null;
      
      l_has_market_preference := pkg_rh_subscription.has_preference(p_msisdn, 'AUTOSMS_MARKET');
      
      for cc in (select * from (select * from (
                  select c.car_id, c.sms_title, c.uniqid, c.country_id, c.pinned, 
                  (case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name='AUTOSMS_PRICE_RANGE' 
                                     and substr(pref_val, 1, instr(pref_val,':')-1)>=c.price and substr(pref_val, instr(pref_val,':')+1)<=c.price) then 1 else 0 end) as price_sort_order,
                  (case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name='AUTOSMS_YEAR_RANGE' 
                                     and substr(pref_val, 1, instr(pref_val,':')-1)>=c.year_of_manuf and substr(pref_val, instr(pref_val,':')+1)<=c.year_of_manuf) then 1 else 0 end) as year_sort_order,
                  (case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name='AUTOSMS_MILEAGE_RANGE' 
                                     and substr(pref_val, 1, instr(pref_val,':')-1)>=c.mileage and substr(pref_val, instr(pref_val,':')+1)<=c.mileage) then 1 else 0 end) as mileage_sort_order,
                  (case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name='AUTOSMS_MODEL' and 
                                     (to_number(pref_val)=c.model_id or to_number(pref_val)=c.make_id)) then 1 else 0 end) as model_sort_order
                  from active_cars_view c 
                  where not exists (select 0 from subs_sent_content 
                                  where content_id='AutoSMS_'||c.car_id 
                                  and msisdn=p_msisdn 
                                  and sent_at > sysdate - p_max_days_back)
                  and (c.country_id = 1 or (l_has_market_preference is not null and exists (select 0 from subs_preferences where msisdn=p_msisdn and pref_name='AUTOSMS_MARKET' and to_number(pref_val)=c.country_id)))
                  order by c.pinned desc, price_sort_order desc, year_sort_order desc, mileage_sort_order desc, model_sort_order desc, dbms_random.value) t  where rownum <= 10) t2 
                  order by pinned desc, car_id)
      loop
         l_append := cc.sms_title || ' (kodu: ' || cc.car_id || ')' || chr(10) || chr(10);
         
         if l_has_market_preference = 1 then
            if cc.country_id = 2 then
               l_append := 'Gurcustanda '||l_append;
            elsif cc.country_id = 3 then
               l_append := 'Almaniyada '||l_append;
            end if;
         end if;
                 
         l_has_content := 1;

         if length(o_result || l_append || l_suffix) > 1000 then
            exit;
         end if;
      
         o_result := o_result || l_append;
         
         insert into subs_sent_content (content_id, msisdn, sent_at) 
         values ('AutoSMS_'||cc.car_id, p_msisdn, sysdate);
         
         update cars set sent_count = sent_count + 1 where car_id = cc.car_id;
      end loop;
      
      if l_has_content = 0 then
         o_result := '';
      else
         o_result := o_result || l_suffix;
      end if;
   end;
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_BETTING" AS

  procedure get_bettings_to_display(v_group_id number, v_items_cursor out t_cursor) is
  begin 
    open v_items_cursor for
    select * from
    (select b.betting_id, b.betting_name, b.start_date, b.end_date, b.date_added, sn.short_number_name,
      b.short_number_id from bettings b
     inner join short_numbers_view sn on b.short_number_id=sn.number_id
     where betting_group_id=v_group_id
     order by b.betting_id) t
    where rownum<=30;
  end;
  
  procedure get_bettings_for_ext_display(v_group_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for 
    select betting_id, betting_name from bettings 
    where betting_group_id=v_group_id
    order by betting_id;
  end;
  
  PROCEDURE ADD_BETTING(V_BETTING_NAME NVARCHAR2,V_START_DATE DATE,V_END_DATE DATE,V_SHORT_NUMBER_ID NUMBER,V_GROUP_ID NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO BETTINGS (BETTING_ID,BETTING_NAME,START_DATE,END_DATE,SHORT_NUMBER_ID,BETTING_GROUP_ID) VALUES (BETTINGS_SEQ.NEXTVAL,V_BETTING_NAME,V_START_DATE,V_END_DATE,V_SHORT_NUMBER_ID,V_GROUP_ID);
  SELECT BETTINGS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  PROCEDURE DELETE_BETTING(V_BETTING_ID NUMBER) IS
  BEGIN
  DELETE FROM BETTINGS WHERE BETTING_ID=V_BETTING_ID;
  END;
  
  PROCEDURE GET_BETTING_BY_ID(V_BETTING_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR) IS
  BEGIN
  OPEN ITEMS_CURSOR FOR SELECT BETTING_ID,BETTING_NAME,START_DATE,END_DATE,BETTING_STATUS,DATE_ADDED,SHORT_NUMBER_ID FROM BETTINGS WHERE BETTING_ID=V_BETTING_ID;
  END;
  
  PROCEDURE UPDATE_BETTING(V_BETTING_ID NUMBER,V_BETTING_NAME NVARCHAR2,V_START_DATE DATE,V_END_DATE DATE,V_SHORT_NUMBER_ID NUMBER) IS
  BEGIN
  UPDATE BETTINGS SET BETTING_ID=V_BETTING_ID,BETTING_NAME=V_BETTING_NAME,START_DATE=V_START_DATE,END_DATE=V_END_DATE,SHORT_NUMBER_ID=V_SHORT_NUMBER_ID WHERE BETTING_ID=V_BETTING_ID;
  END;
  
  procedure get_betting_name_by_id(v_betting_id number, v_betting_name out nocopy nvarchar2) is
  begin
    select betting_name into v_betting_name from bettings where betting_id=v_betting_id;
  end;
  
  procedure get_betting_winners(v_betting_id number, v_min_match_count number, v_max_res_count number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select * from (
    select t.msg_id, t.source_addr, t.in_date, t.msg_body, 
    pkg_util.get_matching_symbol_count(b.drawings_result, msg_body) as match_count
    from smpp_incoming t inner join bettings_view b on 
    (t.host_name=b.host_name and t.destination_addr=b.short_number and t.in_date between b.start_date and b.end_date
    and t.is_processed=1 and t.app_name='BETTING' and t.app_num_param1=v_betting_id)
    where 
    b.betting_id=v_betting_id
    and
    pkg_util.get_matching_symbol_count(b.drawings_result, msg_body)>=v_min_match_count
    order by msg_id
    ) tt where rownum<=v_max_res_count;
  end; 
  
  procedure get_betting_drawing_count(v_betting_id number, v_drawing_count out number) is
  begin
    select count(0) into v_drawing_count from betting_drawings where betting_id=v_betting_id;
  end;
  
  procedure get_betting_drawing_results(v_betting_id number, v_drawing_results out nocopy varchar2) is
  begin
    select drawings_result into v_drawing_results from bettings where betting_id=v_betting_id;
  end;
  
  procedure notify_by_sms(v_betting_id number, v_ids varchar2, v_msg_body nvarchar2) is
  l_source_number varchar2(50);
  l_host_name varchar2(10);
  cursor numbers_cursor is select source_addr from smpp_incoming where
    msg_id in (select * from table(pkg_util.get_number_table(v_ids, ',')) t);
  begin
    select short_number, host_name into l_source_number, l_host_name from bettings_view where
    betting_id=v_betting_id;
    
    for c in numbers_cursor
    loop
      pkg_sms_sender.send_sms(l_host_name, l_source_number, c.source_addr, v_msg_body, 0, 1, 'BETTING');
    end loop;
    
    --update smpp_incoming set is_processed=1, app_name='BETTING', app_num_param1=v_betting_id where msg_id in (select * from table(pkg_util.get_number_table(v_ids, ',')) t);
    --commit;
  end;
  
  procedure get_bets_by_betting(v_group_id number, v_betting_id number, v_source_addr varchar2, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select to_char(t.in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, t.msg_body from smpp_incoming t
    inner join bettings_view b on 
    (t.host_name=b.host_name and t.destination_addr=b.short_number and t.in_date between b.start_date and b.end_date
    and t.is_processed=1 and t.app_name='BETTING' and t.app_num_param1=v_betting_id)
    where
    t.source_addr=v_source_addr
    and
    b.betting_id=v_betting_id
    and b.betting_group_id=v_group_id
    order by t.msg_id;
  end;
  
  function is_valid_bet_format(v_bet nvarchar2) return boolean is
  begin
    IF LTRIM (TRANSLATE (v_bet, '012', '000'), '0') IS NOT NULL
    THEN
       RETURN FALSE; 
    ELSE
       RETURN TRUE; 
    END IF;
  end;

END PKG_BETTING;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_BETTING_DRAWING" AS

  procedure get_drawings_by_betting_id(v_betting_id number, v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for
    select * from betting_drawings where betting_id=v_betting_id order by drawing_id;
  END get_drawings_by_betting_id;
  
  PROCEDURE ADD_BETTING_DRAWING(V_BETTING_ID NUMBER,V_DRAWING_NAME NVARCHAR2,V_DRAWING_RESULT NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO BETTING_DRAWINGS (DRAWING_ID,BETTING_ID,DRAWING_NAME,DRAWING_RESULT) VALUES (BETTING_DRAWINGS_SEQ.NEXTVAL,V_BETTING_ID,V_DRAWING_NAME,V_DRAWING_RESULT);
  update_betting_drawings_result(V_BETTING_ID);
  SELECT BETTING_DRAWINGS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  PROCEDURE UPDATE_BETTING_DRAWING(V_DRAWING_ID NUMBER,V_DRAWING_NAME NVARCHAR2,V_DRAWING_RESULT NUMBER) IS
  l_betting_id number;
  BEGIN
  UPDATE BETTING_DRAWINGS SET DRAWING_NAME=V_DRAWING_NAME,DRAWING_RESULT=V_DRAWING_RESULT WHERE DRAWING_ID=V_DRAWING_ID;
  select betting_id into l_betting_id from betting_drawings where drawing_id=v_drawing_id;
  update_betting_drawings_result(l_betting_id);
  END;
  
  PROCEDURE DELETE_BETTING_DRAWING(V_DRAWING_ID NUMBER) IS
  l_betting_id number;
  BEGIN
  select betting_id into l_betting_id from betting_drawings where drawing_id=v_drawing_id;
  DELETE FROM BETTING_DRAWINGS WHERE DRAWING_ID=V_DRAWING_ID;
  update_betting_drawings_result(l_betting_id);
  END;
  
  procedure update_betting_drawings_result(v_betting_id number) is
  begin
    update bettings set drawings_result=get_drawing_results_string(v_betting_id) where betting_id=v_betting_id;
  end;
  
  function get_drawing_results_string(v_betting_id number) return varchar2 is
  cursor drawing_cursor is select drawing_result from betting_drawings where betting_id=v_betting_id order by drawing_id;
  l_result varchar2(50):='';
  l_curr_res varchar2(1);
  begin
    for c in drawing_cursor
    loop
      if c.drawing_result=0 then
        l_curr_res:='X';
      elsif c.drawing_result=3 then
        l_curr_res:='0';
      elsif c.drawing_result=4 then
        l_curr_res:='_';
      else
        l_curr_res:=c.drawing_result;
      end if;
      l_result:=l_result || l_curr_res;
    end loop;
    
    return l_result;
  end;

END PKG_BETTING_DRAWING;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_BETTING_GROUP" AS

  procedure get_groups_to_display(v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for
    select * from
    (select * from betting_groups order by date_added desc) t
    where rownum<=30;
  END get_groups_to_display;
  
  PROCEDURE ADD_BETTING_GROUP(V_GROUP_NAME NVARCHAR2,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO BETTING_GROUPS (GROUP_ID,GROUP_NAME) VALUES (BETTING_GROUPS_SEQ.NEXTVAL,V_GROUP_NAME);
  SELECT BETTING_GROUPS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  PROCEDURE UPDATE_BETTING_GROUP(V_GROUP_ID NUMBER,V_GROUP_NAME NVARCHAR2) IS
  BEGIN
  UPDATE BETTING_GROUPS SET GROUP_ID=V_GROUP_ID,GROUP_NAME=V_GROUP_NAME WHERE GROUP_ID=V_GROUP_ID;
  END;
  
  PROCEDURE DELETE_BETTING_GROUP(V_GROUP_ID NUMBER) IS
  BEGIN
  DELETE FROM BETTING_GROUPS WHERE GROUP_ID=V_GROUP_ID;
  END;
  
  PROCEDURE GET_BETTING_GROUP_BY_ID(V_GROUP_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR) IS
  BEGIN
  OPEN ITEMS_CURSOR FOR SELECT GROUP_ID,GROUP_NAME,DATE_ADDED FROM BETTING_GROUPS WHERE GROUP_ID=V_GROUP_ID;
  END;
  
  procedure get_group_name_by_id(v_group_id number, v_group_name out nocopy nvarchar2) is
  begin
    select group_name into v_group_name from betting_groups where group_id=v_group_id;
  end;
  
  procedure get_betting_group_winners(v_group_id number, v_bettings varchar2, v_min_points number, v_max_res_count number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select * from (
    select t.source_addr, sum(pkg_util.get_matching_symbol_count(b.drawings_result, msg_body)) as sum_match_count
    from smpp_incoming t inner join bettings_view b on 
    (t.host_name=b.host_name and t.destination_addr=b.short_number and t.in_date between b.start_date and b.end_date)
    where 
    b.betting_id in (select * from table(pkg_util.get_number_table(v_bettings, ',')) t)
    group by t.source_addr
    having sum(pkg_util.get_matching_symbol_count(b.drawings_result, msg_body))>=v_min_points
    order by sum(pkg_util.get_matching_symbol_count(b.drawings_result, msg_body)) desc
    ) tt where rownum<=v_max_res_count;
  end; 

END PKG_BETTING_GROUP;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_BLACK_LIST" AS

  procedure load_from_external(o_result out number) AS
   l_black_list_count number;
  BEGIN
        o_result := 0;
        
        SELECT COUNT (0)
        INTO l_black_list_count
        FROM black_list_external
        where length(msisdn)=12;
        
        if l_black_list_count < 150000 then
           return;
        end if;
        
        delete from black_list where is_manual=0;
    
        insert into black_list (rec_id, msisdn)
        select black_list_seq.nextval, msisdn from (
        select distinct msisdn from black_list_external where msisdn not in
        (select msisdn from black_list where is_manual = 1 ) and length(msisdn)=12) t;
        
        o_result := SQL%ROWCOUNT;
  END load_from_external;
  
  function is_in_black_list(p_msisdn varchar2) return number is
     l_result number;
  begin
     select case when exists (select 0 from black_list where msisdn = p_msisdn) then 1 else 0 end into l_result from dual;
     
     return l_result;
  end;

END PKG_BLACK_LIST;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_BULK_SMS2" as

  function already_started(p_bulk_sms_id number) return number is 
     l_already_started number:=0;
  begin
     select count(0) into l_already_started from bulk_sms2 where rec_id=p_bulk_sms_id and status=1;
     
     return l_already_started;
  end;
  
  function is_in_black_list(p_msisdn varchar2) return number is
     l_in_black_list number;
  begin
     select case when exists (select 0 from black_list where for_bulk=1 and msisdn=p_msisdn) then 1
            else 0 end into l_in_black_list from dual;
            
     return l_in_black_list;
  end;
  
  function check_creator(p_bulk_sms_id number, p_check_creator_id number) return number is
     l_creator number;
  begin
     if p_check_creator_id is not null then
        select creator_id into l_creator
        from bulk_sms2 where rec_id=p_bulk_sms_id;
        
        if l_creator!=p_check_creator_id then
           return 0;
        else
           return 1;
        end if;
     else
        return 1;
     end if;
  end;
  
  procedure add_number(p_bulk_sms_id number, p_msisdn varchar2, 
                       p_msg_body varchar2, p_check_creator_id number) is
     l_number_exists number:=0;
  begin
     
     if already_started(p_bulk_sms_id)>0 then
       RAISE_APPLICATION_ERROR (-20021, 'Sending already started');
       return;
     end if;
     
     if p_msisdn like '99455590%' then --block all bakcell employees
        return;
     end if;
  
     if check_creator(p_bulk_sms_id, p_check_creator_id) = 0 then
        return;
     end if;

     if is_in_black_list(p_msisdn) > 0 then
       return;
     end if;
     
     select case when exists (select 0 from bulk_sms2_numbers where bulk_sms_id=p_bulk_sms_id and msisdn=p_msisdn)
                 then 1 else 0 end 
     into l_number_exists from dual;
     
     if l_number_exists=0 then
       insert into bulk_sms2_numbers (rec_id, bulk_sms_id, msisdn, msg_body) values 
       (bulk_sms2_numbers_seq.nextval, p_bulk_sms_id, p_msisdn, p_msg_body);
     end if;
  end;
  
  procedure add_list(p_bulk_sms_id number, p_list_id number, p_check_creator_id number) is
     l_exists number;
  begin
     if already_started(p_bulk_sms_id)>0 then
       RAISE_APPLICATION_ERROR (-20021, 'Sending already started');
       return;
     end if;
     
     if check_creator(p_bulk_sms_id, p_check_creator_id) = 0 then
        return;
     end if;
     
     select case when exists (select 0 from bulk_sms2_numbers where bulk_sms_id=p_bulk_sms_id and list_id=p_list_id)
                 then 1 else 0 end 
     into l_exists from dual;
     
     if l_exists = 0 then
        insert into bulk_sms2_numbers (rec_id, bulk_sms_id, list_id) values 
        (bulk_sms2_numbers_seq.nextval, p_bulk_sms_id, p_list_id);
     end if;
  end;
  
  procedure update_number_count(p_bulk_sms_id number) is
     l_count number;
  begin
     l_count := get_number_count(p_bulk_sms_id);
     update bulk_sms2 set number_count = l_count where rec_id=p_bulk_sms_id;
  end;
  
  function get_number_count(p_bulk_sms_id number) return number is
     l_count number;
     l_offset number;
     l_max_count number;
  begin
     select count(0) into l_count
     from (select b.rec_id, msisdn, nvl(bn.msg_body, b.msg_body) as msg_body, b.start_at, s.sender_name, b.validity_period
                from bulk_sms2_numbers bn 
                inner join bulk_sms2 b on bn.bulk_sms_id=b.rec_id
                inner join bulk_sms2_sender_names s on b.sender_id=s.sender_id
                where b.rec_id=p_bulk_sms_id and nvl(bn.msg_body, b.msg_body) is not null and bn.msisdn is not null
                and b.is_deleted=0
                union all
               select distinct b.rec_id, ln.msisdn, replace(b.msg_body, '{ad}', ln.full_name) as msg_body, b.start_at, s.sender_name, b.validity_period
               from bulk_sms2_list_numbers ln
               inner join bulk_sms2_lists l on ln.list_id=l.list_id
               inner join bulk_sms2_numbers bn on bn.list_id=l.list_id
               inner join bulk_sms2 b on bn.bulk_sms_id=b.rec_id
               inner join bulk_sms2_sender_names s on b.sender_id=s.sender_id
               where b.rec_id=p_bulk_sms_id and replace(b.msg_body, '{ad}', ln.full_name) is not null and ln.msisdn is not null
               and not exists (select 0 from bulk_sms2_numbers bn2 where bulk_sms_id=b.rec_id and bn2.msisdn = ln.msisdn)
               and not exists (select 0 from black_list where for_bulk=1 and black_list.msisdn=ln.msisdn)
               and b.is_deleted=0) t;
     select nvl(offset,0), nvl(max_count,0) into l_offset, l_max_count from bulk_sms2 where rec_id=p_bulk_sms_id;
     if l_max_count > 0 then
        return least(l_count-l_offset, l_max_count);
     else
        return greatest(l_count-l_offset, 0);
     end if;
  end;
  
  procedure start_sending(p_bulk_sms_id number, 
                          p_check_creator_id number,
                          o_msg out varchar2) is
     l_already_started number;
     l_creator number;
     l_current_balance number;
     l_send_count number;
     l_host_name varchar2(20);
     
     l_offset number;
     l_max_count number;
     l_batch_size number;
     l_batch_interval number;
     
     l_start_at date;
     
     l_current_offset number := 0;
     l_sent_count number := 0;
     l_current_start_at date;
  begin
     select status into l_already_started from bulk_sms2 where rec_id=p_bulk_sms_id;
     if l_already_started = 1 then
        rollback;
        return;
     end if;
     
     select creator_id into l_creator
     from bulk_sms2 where rec_id=p_bulk_sms_id;
        
     if p_check_creator_id is not null then
        if l_creator!=p_check_creator_id then
           return;
        end if;
     end if;
     
     select nvl(max(h.host_name),'BAKCELL1') into l_host_name from bulk_sms2_hosts bh
     inner join smpp_hosts h on bh.host_id=h.host_id
     where bh.user_id = l_creator;
     
     select nvl(max(balance), 0) into l_current_balance from bulk_sms2_balance
     where user_id=l_creator;
     
     l_send_count := get_number_count(p_bulk_sms_id);
     
     if l_send_count > l_current_balance then
        o_msg := 'You are out of balance for starting this task. Required balance: '||l_send_count;
        return;
     end if;
     
     if l_send_count = 0 then 
        o_msg := 'Nothing to send.';
        return;
     end if;
     
     update bulk_sms2_balance set balance = balance - l_send_count
     where user_id = l_creator;
     
     if sql%rowcount != 1 then
        o_msg := 'Internal application error. Could not start task.';
        return;
     end if;
     
     if l_send_count > 0 then
        update bulk_sms2 set status = 1 where rec_id=p_bulk_sms_id and status = 0;
        if sql%rowcount != 1 then
           o_msg := 'Internal application error. Failed to set task status.';
           return;
        end if;
     end if;
     
     select nvl(offset,0), nvl(max_count,0), nvl(batch_size,0), nvl(batch_interval,0), start_at into
     l_offset, l_max_count, l_batch_size, l_batch_interval, l_start_at from bulk_sms2
     where rec_id = p_bulk_sms_id;
     
     l_current_start_at := l_start_at;
     
     for cc in (select b.rec_id, msisdn, nvl(bn.msg_body, b.msg_body) as msg_body, b.start_at, s.sender_name, b.validity_period, bn.rec_id as num_seq
                from bulk_sms2_numbers bn 
                inner join bulk_sms2 b on bn.bulk_sms_id=b.rec_id
                inner join bulk_sms2_sender_names s on b.sender_id=s.sender_id
                where b.rec_id=p_bulk_sms_id and nvl(bn.msg_body, b.msg_body) is not null and bn.msisdn is not null
                and b.is_deleted=0
                union all
               select distinct b.rec_id, ln.msisdn, replace(b.msg_body, '{ad}', ln.full_name) as msg_body, b.start_at, s.sender_name, b.validity_period, ln.rec_id as num_seq
               from bulk_sms2_list_numbers ln
               inner join bulk_sms2_lists l on ln.list_id=l.list_id
               inner join bulk_sms2_numbers bn on bn.list_id=l.list_id
               inner join bulk_sms2 b on bn.bulk_sms_id=b.rec_id
               inner join bulk_sms2_sender_names s on b.sender_id=s.sender_id
               where b.rec_id=p_bulk_sms_id and replace(b.msg_body, '{ad}', ln.full_name) is not null and ln.msisdn is not null
               and not exists (select 0 from bulk_sms2_numbers bn2 where bulk_sms_id=b.rec_id and bn2.msisdn = ln.msisdn)
               and not exists (select 0 from black_list where for_bulk=1 and black_list.msisdn=ln.msisdn)
               and b.is_deleted=0
               order by num_seq)
     loop
        l_current_offset := l_current_offset + 1;
        
        continue when l_current_offset < l_offset;
        
        exit when (l_max_count>0 and l_sent_count >= l_max_count);
        
        
        l_sent_count := l_sent_count + 1;
        pkg_sms_sender.send_sms_at(l_host_name, 
                                cc.sender_name, 
                                cc.msisdn, 
                                cc.msg_body, 
                                1, 
                                0,  
                                'BULK_SMS2',
                                l_current_start_at,
                                cc.rec_id,
                                cc.validity_period);
                                
        if l_batch_size > 0 and l_batch_interval > 0 then
           if mod(l_sent_count, l_batch_size) = 0 then
              l_current_start_at := l_current_start_at + 1/24*l_batch_interval;
           end if;
        end if;
     
     end loop;
     
     update bulk_sms2 set number_count=l_send_count where rec_id=p_bulk_sms_id;
     
     o_msg := 'Task started successfully';
     
     --commit;
  end;
  
  procedure add_group_number(p_group_id number, p_name varchar2, p_msisdn varchar2) is
     l_exists number;
  begin
     select case when exists (select 0 from bulk_sms2_list_numbers where list_id=p_group_id and msisdn=p_msisdn) then 1 else 0 end
     into l_exists from dual;
     
     if l_exists = 0 then
        insert into bulk_sms2_list_numbers (rec_id, list_id, full_name, msisdn) values (bulk_sms2_list_numbers_seq.nextval, p_group_id, p_name, p_msisdn);
     end if;
  end;
  
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_CHGW" AS

   procedure set_charge_fail_date(p_msisdn varchar2, p_failed number) is
      l_exists number;
   begin
      if p_failed = 0 then
         delete from chgw_stats where msisdn = p_msisdn;
      else
         select case when exists (select 0 from chgw_stats where msisdn = p_msisdn) then 1 else 0 end into l_exists from dual;
         
         if l_exists = 0 then
            insert into chgw_stats (msisdn, charge_fail_date) values (p_msisdn, sysdate);
         else
            update chgw_stats set charge_fail_date = sysdate where msisdn = p_msisdn;
         end if;
      end if;
   end;
   
END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_DIAMETER" as

  const_app_name constant varchar2(20) := 'PKG_DIAMETER';
  
  /*
  procedure charge_bakcell(p_gateway_url varchar2,
                           p_msisdn varchar2, 
                           p_short_number_id number,
                           p_tariff_name varchar2,
                           o_result out number,
                           o_text_result out varchar2) is
    l_post_data varchar2(500);
    l_response varchar2(32767); 
    l_code varchar2(10);
    l_underscore_pos number;
  begin
    l_post_data := 'msisdn='||p_msisdn||'&tariff='||p_tariff_name||'&content='||p_short_number_id;
    pkg_logger.log_debug('url='||p_gateway_url||', post data='||l_post_data, const_app_name, p_msisdn);
    l_response := pkg_util.get_http_response(p_gateway_url, 'POST', l_post_data);
    
    pkg_logger.log_debug('response='||l_response, const_app_name, p_msisdn);
    
    l_underscore_pos := instr(l_response, '_');
    
    if l_underscore_pos=0 then 
      o_result := const_generic_error;
      o_text_result := 'Hormetli abunechi, SMS sorgunuz qebul edilmedi. Xahish edirik, Bakcell mushteri xidmetlerine muraciet edesiniz. Balansinizdan hech bir mebleg chixilmamishdir.';
      return;
    end if;
    
    
    l_code := substr(trim(l_response), 1, l_underscore_pos-1);
    
    pkg_logger.log_debug('code='||l_code, const_app_name, p_msisdn);
    
    o_result := to_number(l_code);
  end;
  
  procedure charge_nar(p_transaction_id number,
                       p_gateway_url varchar2,
                           p_msisdn varchar2, 
                           p_short_number_id number,
                           p_tariff_name varchar2,
                           o_result out number,
                           o_text_result out varchar2) is
    l_request varchar2(32767);
    l_result varchar2(32767);
    l_ix1 number;
    l_ix2 number;
    
    l_service_xml varchar2(1000);
    l_retval varchar2(50);
  begin
    l_request := '<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns1="http://ws.cube.azerfon.az">
                   <SOAP-ENV:Body>
                   <ns1:chargeSubscriber>
                       <ns1:username>navigator</ns1:username>
                       <ns1:password>navigat0r</ns1:password>
                       <ns1:msisdn>'||substr(p_msisdn,-9)||'</ns1:msisdn>
                       <ns1:service>'||p_tariff_name||'</ns1:service>
                       <ns1:quantity>1</ns1:quantity>
                       <ns1:minQuantity>1</ns1:minQuantity>
                       <ns1:transactionId>'||p_transaction_id||'</ns1:transactionId>
                       <ns1:description>'||p_short_number_id||'</ns1:description>
                   </ns1:chargeSubscriber>
                   </SOAP-ENV:Body>
                </SOAP-ENV:Envelope>';
    pkg_logger.log_debug('url='||p_gateway_url||', post data='||l_request, const_app_name, p_msisdn);
    
    l_result := pkg_util.get_http_response(p_gateway_url, 'POST', l_request, 'text/xml');
    
    pkg_logger.log_debug('response='||l_result, const_app_name, p_msisdn);
    
    l_ix1 := instr(l_result, '<ns:return>')+length('<ns:return>');
    l_ix2 := instr(l_result, '</ns:return>');
    
    if l_ix1=0 then 
      o_result := const_generic_error;
      o_text_result := 'Error parsing xml (l_result)';
      return;
    end if;
    
    l_service_xml := substr(l_result, l_ix1, l_ix2-l_ix1);
    
    pkg_logger.log_debug('service xml='||l_service_xml, const_app_name, p_msisdn);
    
    l_ix1 := instr(l_service_xml, '&lt;retval>')+length('&lt;retval>');
    l_ix2 := instr(l_service_xml, '&lt;/retval>');
    
    if l_ix1=0 then 
      o_result := const_generic_error;
      o_text_result := 'Error parsing xml (l_service_xml)';
      return;
    end if;
    
    l_retval := substr(l_service_xml, l_ix1, l_ix2-l_ix1);
    
    pkg_logger.log_debug('ret val='||l_retval, const_app_name, p_msisdn);
    
    o_result := to_number(trim(l_retval));
    o_text_result := to_char(o_result);
    
          
--          Return Value Return Message
--            -2200001 Invalid username or password
--            -2200002 Your CUBE account is suspended. Please contact your Account Manager
--         +  -2000000 Provided MSISDN is invalid or not found
--         +  -2000001 Subscriber status is not allowed to use services
--            -2000002 DB API initialization error. Please contact IT Technical Support.
--            -2000003 Duplicate Transaction ID
--            -2000004 General Database Error. Please contact IT Technical Support.
--            -2222222 General Web Service Error. Please contact IT Technical Support.
--            -2222223 Invalid or missing parameter
--            -2200003 Invalid Service
--            -2200004 Deactive Service. Please contact your Account Manager
--         +  -1000400 Insufficient account balance to execute the operation
--            -2000010 Invalid Cube Transaction ID
--            1 Operation successful
--            >0 Operation successful
--            <0 Other error. Please contact IT Technical Support.
--            0 API Error, please refer to Return Message
            
            
     if o_result in (-2200001,-2200002,-2000002,-2000003,-2000004,-2222222,-2222223,-2200003,-2200004, -2000010) then
        o_result := 500;
     elsif o_result = -2000000 then
        o_result := 400;
     elsif o_result = -2000001 then
        o_result := 402;
     elsif o_result = -1000400 then
        o_result := 401;
     elsif o_result >= 1 then
        o_result := 100;
     else
        o_result := 500;
     end if;
     
     if o_result in (-2200002,-2000003,-2222223,-2200003,-2200004) then
       pkg_logger.log_error('We have suspicious response code in NAR charging. Have to check configuration: '||o_result, const_app_name, p_msisdn);
     end if;
    
--    example response:
--    <?xml version='1.0' encoding='UTF-8'?>
--    <soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/">
--    <soapenv:Body>
--       <ns:chargeSubscriberResponse xmlns:ns="http://ws.cube.azerfon.az">
--           <ns:return>&lt;?xml version="1.0" encoding="UTF-8"?>&#xd;
--                      &lt;response>&#xd;
--                        &lt;retval>-1000400&lt;/retval>&#xd;
--                        &lt;retmsg>Insufficient account balance to execute the operation.&lt;/retmsg>&#xd;
--                        &lt;quantity>&lt;/quantity>&#xd;
--                        &lt;cube-transaction-id>&lt;/cube-transaction-id>&#xd;
--                      &lt;/response>
--            </ns:return>
--       </ns:chargeSubscriberResponse>
--    </soapenv:Body>
--    </soapenv:Envelope>
    
  end;
  
  procedure charge_azercell(p_gateway_url varchar2,
                           p_msisdn varchar2, 
                           p_short_number_id number,
                           p_tariff_name varchar2,
                           o_result out number,
                           o_text_result out varchar2) is
    l_gateway_url varchar2(1000) := p_gateway_url;
    l_response varchar2(32767); 
    l_opening_tag varchar2(50) := '<ResponseCode>';
    l_closing_tag varchar2(50) := '</ResponseCode>';
    l_opening_tag_ix number;
    l_closing_tag_ix number;
    
    l_code varchar2(50);
    l_numeric_code number;
  begin
    l_gateway_url := replace(l_gateway_url, '{MSISDN}', p_msisdn);
    l_gateway_url := replace(l_gateway_url, '{TARIFF}', p_tariff_name);
    pkg_logger.log_debug('url='||l_gateway_url, const_app_name, p_msisdn);
    
    
    l_gateway_url := 'http://localhost/index.php?app=http_proxy&url=' || utl_url.escape(l_gateway_url, TRUE);
    
    l_response := pkg_util.get_http_response(l_gateway_url);
    
    pkg_logger.log_debug('response='||l_response, const_app_name, p_msisdn);
    
    l_opening_tag_ix := instr(l_response, l_opening_tag);
    l_closing_tag_ix := instr(l_response, l_closing_tag);
    
    if l_opening_tag_ix=0 or l_closing_tag_ix=0 then 
      o_result := const_generic_error;
      o_text_result := 'Tags not found in response';
      return;
    end if;
    
    l_code := substr(l_response, l_opening_tag_ix+length('<ResponseCode>'), l_closing_tag_ix-l_opening_tag_ix-length('</ResponseCode>')+1);
    
    pkg_logger.log_debug('code='||l_code, const_app_name, p_msisdn);
    
    if pkg_util.verify_numeric(l_code)=false then
       o_result := const_generic_error;
       o_text_result := 'Parsed response code is not numeric';
       return;
    end if;
    
    l_numeric_code := to_number(l_code);
    
    if l_numeric_code in (6015, 2001, 2002, 8019, 6017) then
       o_result := 100;
       o_text_result := 'Success';
    elsif l_numeric_code = 6018 then
       o_result := 401;
       o_text_result := 'Cdr not generated';
    else
       o_result := 500;
       o_text_result := l_code;
    end if;
  end;*/
  
  procedure do_charge(
                   p_msisdn varchar2, 
                   p_tariff_id number, 
                   p_client_id number,
                   p_charge_reason varchar2,
                   o_result out number,
                   o_text_result out varchar2,
                   p_msg_id number) as
    l_gateway_url varchar2(1000) := 'http://127.0.0.1/crit/index.php?app=chgw';
    l_response varchar2(32767);
    
    l_opening_tag varchar2(50) := '<code>';
    l_closing_tag varchar2(50) := '</code>';
    l_opening_tag_ix number;
    l_closing_tag_ix number;
    
    l_code varchar2(50);
    l_numeric_code number;
  begin
    o_result := const_generic_error;
    
    l_gateway_url := l_gateway_url || '&client=' || p_client_id || '&tariff=' || p_tariff_id || '&msisdn=' || p_msisdn || '&reason=' || utl_url.escape(p_charge_reason) || '&mo=' || p_msg_id;
    
    select utl_http.request(l_gateway_url) into l_response from dual;
    
    --pkg_logger.log_debug_auto(l_response, const_app_name, p_msisdn);
    
    l_opening_tag_ix := instr(l_response, l_opening_tag);
    l_closing_tag_ix := instr(l_response, l_closing_tag);
    
    if l_opening_tag_ix=0 or l_closing_tag_ix=0 then 
      o_result := const_generic_error;
      o_text_result := 'Tags not found in response';
      return;
    end if;
    
    l_code := substr(l_response, l_opening_tag_ix+length(l_opening_tag), l_closing_tag_ix-l_opening_tag_ix-length(l_closing_tag)+1);
    
    if pkg_util.verify_numeric(l_code)=false then
       o_result := const_generic_error;
       o_text_result := 'Parsed response code is not numeric';
       return;
    end if;
    
    o_result := to_number(l_code);
    o_text_result := l_code;
   
  exception when utl_http.HTTP_SERVER_ERROR or utl_http.TOO_MANY_REQUESTS or 
                 utl_http.TRANSFER_TIMEOUT or utl_http.REQUEST_FAILED then 
      o_result := const_network_error;
      o_text_result := substr(sqlerrm,1,500);
      pkg_logger.log_error_auto(sqlerrm, const_app_name, p_msisdn);
  when others then
    o_result := const_generic_error;
    o_text_result := substr(sqlerrm,1,500);
    
    pkg_logger.log_error_auto(sqlerrm, const_app_name, p_msisdn);
  end do_charge;
  
  procedure charge(p_msisdn varchar2, 
                   p_tariff_id number, 
                   p_client_id number,
                   p_charge_reason varchar2,
                   o_result out number,
                   o_text_result out varchar2,
                   p_msg_id number) is
  begin
    
    do_charge(p_msisdn, p_tariff_id, p_client_id, p_charge_reason, o_result, o_text_result, p_msg_id);
    
  end;

end pkg_diameter;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_EVSMS" as
   
   procedure get_next_content(p_subs_id number, p_msisdn varchar2, p_max_days_back number, o_result out varchar2, o_content_id out number) as
      l_append varchar2(1500);
      
      l_sell_type_pref number;
      l_building_type_pref number;
      
      l_has_content number := 0;
      l_suffix varchar2(200) := 'Etrafli melumat almaq istediyiniz elanin kodunu 6010-a gonderin (pulsuz)';
   begin
      o_content_id := null;
      
      l_sell_type_pref := to_number(pkg_rh_subscription.get_preference(p_msisdn, 'EVSMS_SELL_TYPE', '-1'));
      l_building_type_pref := to_number(pkg_rh_subscription.get_preference(p_msisdn, 'EVSMS_BUILDING_TYPE', '-1'));
      
      for cc in (select * from (select * from (
                  select re.re_id, re.re_code, re.sms_title, re.sms_body, re.uniqid, re.re_pinned, re.re_sell_type_id, --reg.reg_sms_text,
                  --(case when re.re_sell_type_id = l_sell_type_pref then 1 else 0 end) as sell_type_sort_order,
                  (case when re.re_building_type_id = l_building_type_pref then 1 else 0 end) as building_type_sort_order,
                  (case when exists (
                        select rereg.region_id from real_estates_regions rereg inner join evsms_regions ereg on rereg.region_id=ereg.reg_id 
                        where rereg.re_id = re.re_id
                        and (rereg.region_id in (select to_number(pref_val) as pref_reg_id from subs_preferences where msisdn = p_msisdn and pref_name='EVSMS_REGION')
                              or ereg.parent_id in (select to_number(pref_val) as pref_reg_id from subs_preferences where msisdn = p_msisdn and pref_name='EVSMS_REGION'))
                        ) then 1 else 0 end) as region_sort_order
                  from active_real_estates re 
                  where (l_sell_type_pref=-1 or re.re_sell_type_id=l_sell_type_pref)
                  --and (l_building_type_pref=-1 or re.re_building_type_id=l_building_type_pref)
                  and not exists (select 0 from subs_sent_content 
                                  where content_id='EvSMS_'||re.re_id 
                                  and msisdn=p_msisdn 
                                  and sent_at > sysdate - p_max_days_back)
                  order by re.re_pinned desc, region_sort_order desc, building_type_sort_order desc, --sell_type_sort_order desc, region_sort_order desc, building_type_sort_order desc, 
                  dbms_random.value) t  where rownum <= 10) t2 order by re_pinned desc, re_code)
      loop
         l_append := cc.sms_title || ' (kodu: ' || cc.re_code || ')' || chr(10);
         
         if l_sell_type_pref = -1 then
            if cc.re_sell_type_id = 1 then
               l_append := 'Satilir : '||l_append;
            else
               l_append := 'Kiraye : '||l_append;
            end if;
         end if;
         
         l_has_content := 1;

         if length(o_result || l_append || l_suffix) > 1000 then
            exit;
         end if;
      
         o_result := o_result || l_append;
         
         insert into subs_sent_content (content_id, msisdn, sent_at) 
         values ('EvSMS_'||cc.re_id, p_msisdn, sysdate);
         
         update real_estates set re_sent_count = re_sent_count + 1 where re_id = cc.re_id;
      end loop;
      
      if l_has_content = 0 then
         o_result := '';
      else
         o_result := o_result || l_suffix;
      end if;
   end;
   
   --range for codes is 100-999
   function get_next_code return number is
      l_next_code number;
   begin
      select nvl(max(re_code), 99)+1 into l_next_code from real_estates where re_code is not null and re_status in ('active', 'pending');
           
      --if l_next_code > 999 then
      --   return 100;
      --end if;
      
      return l_next_code;
   end;
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_EXTERNAL_ACCESS" AS

  procedure can_view_module_from_ip(v_access_key varchar2, v_ip_address varchar2, v_can_view out number, v_module_name out nocopy varchar2, v_access_name out nocopy nvarchar2, v_ext_access_rec_id out number, v_ext_access_date out date, v_param1 out number) is
  begin
    select count(0) into v_can_view
    from external_access where 
    access_key=v_access_key and
    (accessor_ip=v_ip_address or accessor_ip is null);    
    
    if v_can_view>0 then
      select app_id, access_module, access_name, rec_id , date_added, param1
      into v_can_view, v_module_name, v_access_name, v_ext_access_rec_id, v_ext_access_date, v_param1
      from external_access where 
      access_key=v_access_key and
      (accessor_ip=v_ip_address or accessor_ip is null);  
    end if;
    
  end;
  
  procedure check_external_user(v_login nvarchar2, v_password_hash nvarchar2, v_access_key varchar2, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select access_module from external_access where
    accessor_user=v_login and 
    accessor_pass=v_password_hash and 
    access_key=v_access_key;
  end;
  
  PROCEDURE GET_ALL_EXTERNAL_ACCESS(ITEMS_CURSOR OUT T_CURSOR) IS
  BEGIN
  OPEN ITEMS_CURSOR FOR 
  SELECT REC_ID,ACCESS_NAME,ACCESSOR_NAME,ACCESSOR_USER,ACCESSOR_PASS,
  ACCESSOR_IP,ACCESS_MODULE,ACCESS_KEY,APP_ID,PARAM1 FROM EXTERNAL_ACCESS ORDER BY ACCESS_NAME;
  END;
  
  PROCEDURE ADD_EXTERNAL_ACCESS(V_ACCESS_NAME NVARCHAR2,V_ACCESSOR_NAME NVARCHAR2,V_ACCESSOR_USER NVARCHAR2,V_ACCESSOR_PASS NVARCHAR2,V_ACCESSOR_IP VARCHAR2,V_ACCESS_MODULE VARCHAR2,V_ACCESS_KEY VARCHAR2,V_APP_ID NUMBER,V_PARAM1 NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO EXTERNAL_ACCESS (REC_ID,ACCESS_NAME,ACCESSOR_NAME,ACCESSOR_USER,ACCESSOR_PASS,ACCESSOR_IP,ACCESS_MODULE,ACCESS_KEY,APP_ID,PARAM1) VALUES (EXTERNAL_ACCESS_SEQ.NEXTVAL,V_ACCESS_NAME,V_ACCESSOR_NAME,V_ACCESSOR_USER,V_ACCESSOR_PASS,V_ACCESSOR_IP,V_ACCESS_MODULE,V_ACCESS_KEY,V_APP_ID,V_PARAM1);
  SELECT EXTERNAL_ACCESS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  PROCEDURE DELETE_EXTERNAL_ACCESS(V_REC_ID NUMBER) IS
  BEGIN
    --delete from bulk_sms
    --where external_rec_id=V_REC_ID;

    DELETE FROM EXTERNAL_ACCESS WHERE REC_ID=V_REC_ID;
  END;
  
  PROCEDURE UPDATE_EXTERNAL_ACCESS_IP(V_REC_ID NUMBER,V_ACCESSOR_IP VARCHAR2) IS
  BEGIN
  UPDATE EXTERNAL_ACCESS SET ACCESSOR_IP=V_ACCESSOR_IP WHERE REC_ID=V_REC_ID; 
  END;
  
  PROCEDURE UPDATE_EXTERNAL_ACCESS_P1(V_REC_ID NUMBER,V_PARAM1 NUMBER) IS
  BEGIN
  UPDATE EXTERNAL_ACCESS SET PARAM1=V_PARAM1 WHERE REC_ID=V_REC_ID; 
  END;

END PKG_EXTERNAL_ACCESS;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_EXTERNAL_API_ACCESS" AS

  procedure has_access(v_module_name varchar2, v_ip_address varchar2, v_has_access out number) AS
  BEGIN
    select count(0) into v_has_access from external_api_access 
    where module_name=v_module_name and ip_address=v_ip_address;
  END has_access;

END PKG_EXTERNAL_API_ACCESS;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_HTTP" 
IS
   FUNCTION http_post (p_url_in                IN VARCHAR2,
                       v_param                 IN VARCHAR2,
                       p_app                   IN VARCHAR2,
                       p_proxy_in              IN VARCHAR2 DEFAULT NULL,
                       p_no_proxy_domains_in   IN VARCHAR2 DEFAULT NULL,
                       p_username_in           IN VARCHAR2 DEFAULT NULL,
                       p_password_in           IN VARCHAR2 DEFAULT NULL)
      RETURN VARCHAR2
   IS
      l_http_req       UTL_HTTP.req;
      l_http_resp      UTL_HTTP.resp;
      l_my_scheme      VARCHAR2 (256);
      l_my_realm       VARCHAR2 (256);
      l_my_proxy       BOOLEAN;

      v_param_length   NUMBER := LENGTH (v_param);

      v_msg            VARCHAR2 (100);
   BEGIN
      
      -- When going through a firewall, pass requests through this host.
      -- Specify sites inside the firewall that don't need the proxy host.
      IF (p_proxy_in IS NOT NULL) AND (p_no_proxy_domains_in IS NOT NULL)
      THEN
         UTL_HTTP.set_proxy (p_proxy_in, p_no_proxy_domains_in);
      END IF;

      -- Ask UTL_HTTP not to raise an exception for 4xx and 5xx status codes,
      -- rather than just returning the text of the error page.
      UTL_HTTP.set_response_error_check (FALSE);

      -- Begin the post request
      l_http_req := UTL_HTTP.begin_request (p_url_in, 'POST');

      --utl_http.SET_PERSISTENT_CONN_SUPPORT(l_http_req, TRUE);

      -- Set the HTTP request headers
      UTL_HTTP.set_header (l_http_req, 'User-Agent', 'Mozilla/4.0');
      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Type',
                   VALUE   => 'application/x-www-form-urlencoded');
      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Length',
                   VALUE   => v_param_length);
      UTL_HTTP.WRITE_TEXT (r => l_http_req, data => v_param);

      -- Specify a user ID and password for pages that require them.
      IF p_username_in IS NOT NULL
      THEN
         UTL_HTTP.
          set_authentication (l_http_req, p_username_in, p_password_in);
      END IF;

      -- Process the request and get the response.
      BEGIN
         l_http_resp := UTL_HTTP.get_response (l_http_req);
      --UTL_HTTP.SET_TRANSFER_TIMEOUT(l_http_req, 30);
      --UTL_HTTP.set_persistent_conn_support (l_http_req, false);
      EXCEPTION
         WHEN UTL_HTTP.TOO_MANY_REQUESTS
         THEN
            UTL_HTTP.end_response (l_http_resp);
      END;

      pkg_logger.
       log_debug ('HTTP: status code: ' || l_http_resp.status_code, p_app);
      pkg_logger.
       log_debug ('HTTP: reason phrase: ' || l_http_resp.reason_phrase,
                  p_app);

      -- Look for client-side error and report it.
      IF (l_http_resp.status_code >= 400)
         AND (l_http_resp.status_code <= 499)
      THEN
         -- Detect whether the page is password protected,
         -- and we didn't supply the right authorization.
         -- Note the use of utl_http.HTTP_UNAUTHORIZED, a predefined
         -- utl_http package global variable
         IF (l_http_resp.status_code = UTL_HTTP.HTTP_UNAUTHORIZED)
         THEN
            UTL_HTTP.get_authentication (l_http_resp,
                                         l_my_scheme,
                                         l_my_realm,
                                         l_my_proxy);

            IF (l_my_proxy)
            THEN
               pkg_logger.
                log_debug ('HTTP: Web proxy server is protected.', p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supply the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the proxy server.',
                  p_app);
            ELSE
               pkg_logger.
                log_debug ('HTTP: Web page ' || p_url_in || ' is protected.',
                           p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supplied the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the Web page.',
                  p_app);
            END IF;
         ELSE
            pkg_logger.log_debug ('HTTP: Check the URL.', p_app);
         END IF;

         UTL_HTTP.end_response (l_http_resp);
         RETURN 'NOK1';
      -- Look for server-side error and report it.
      ELSIF (l_http_resp.status_code >= 500)
            AND (l_http_resp.status_code <= 599)
      THEN
         pkg_logger.log_debug ('HTTP: Check if the Web site is up.', p_app);
         --UTL_HTTP.END_REQUEST (l_http_req);
         UTL_HTTP.end_response (l_http_resp);
         RETURN 'NOK2';
      END IF;

      BEGIN
         UTL_HTTP.read_text (r => l_http_resp, DATA => v_msg);
         --UTL_HTTP.END_REQUEST (l_http_req);
         UTL_HTTP.end_response (l_http_resp);
         RETURN NVL (v_msg, ' ');
      EXCEPTION
         WHEN UTL_HTTP.end_of_body
         THEN
            -- UTL_HTTP.end_response (l_http_resp);
            NULL;
      END;

      UTL_HTTP.end_response (l_http_resp);
      RETURN 'Sorgunuz gebul edildi. Teshekkur edirik, wap.clicks.az!';
   EXCEPTION
      WHEN OTHERS
      THEN
         pkg_logger.log_error (SQLERRM, p_app);
         pkg_logger.log_error (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, p_app);
         --         UTL_HTTP.END_REQUEST (l_http_req);
         --       UTL_HTTP.end_response (l_http_resp);
         RETURN 'Hormetli abunechi, SMS sorgunuz ishlenmekdedir, cavab SMS-i mueyyen muddetden sonra size chatdirilacaq. Muveqqeti narahatchiliga gore uzr isteyirik.';
   END http_post;


   FUNCTION http_get (p_url_in                IN VARCHAR2,
                      v_param                 IN VARCHAR2,
                      p_app                   IN VARCHAR2,
                      p_proxy_in              IN VARCHAR2 DEFAULT NULL,
                      p_no_proxy_domains_in   IN VARCHAR2 DEFAULT NULL,
                      p_username_in           IN VARCHAR2 DEFAULT NULL,
                      p_password_in           IN VARCHAR2 DEFAULT NULL)
      RETURN VARCHAR2
   IS
      l_http_req       UTL_HTTP.req;
      l_http_resp      UTL_HTTP.resp;
      l_my_scheme      VARCHAR2 (256);
      l_my_realm       VARCHAR2 (256);
      l_my_proxy       BOOLEAN;

      v_param_length   NUMBER := LENGTH (v_param);

      v_msg            VARCHAR2 (100);
   BEGIN
      -- When going through a firewall, pass requests through this host.
      -- Specify sites inside the firewall that don't need the proxy host.
      IF (p_proxy_in IS NOT NULL) AND (p_no_proxy_domains_in IS NOT NULL)
      THEN
         UTL_HTTP.set_proxy (p_proxy_in, p_no_proxy_domains_in);
      END IF;

      -- Ask UTL_HTTP not to raise an exception for 4xx and 5xx status codes,
      -- rather than just returning the text of the error page.
      UTL_HTTP.set_response_error_check (FALSE);

      -- Begin the post request
      l_http_req := UTL_HTTP.begin_request (p_url_in);

      --utl_http.SET_PERSISTENT_CONN_SUPPORT(l_http_req, TRUE);

      -- Set the HTTP request headers
      UTL_HTTP.set_header (l_http_req, 'User-Agent', 'Mozilla/4.0');
/*      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Type',
                   VALUE   => 'application/x-www-form-urlencoded');*/
      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Length',
                   VALUE   => v_param_length);
--      UTL_HTTP.WRITE_TEXT (r => l_http_req, data => v_param);

      -- Specify a user ID and password for pages that require them.
      IF p_username_in IS NOT NULL
      THEN
         UTL_HTTP.
          set_authentication (l_http_req, p_username_in, p_password_in);
      END IF;

      -- Process the request and get the response.
      BEGIN
         l_http_resp := UTL_HTTP.get_response (l_http_req);
      --UTL_HTTP.SET_TRANSFER_TIMEOUT(l_http_req, 30);
      --UTL_HTTP.set_persistent_conn_support (l_http_req, false);
      EXCEPTION
         WHEN UTL_HTTP.TOO_MANY_REQUESTS
         THEN
            UTL_HTTP.end_response (l_http_resp);
      END;

      pkg_logger.
       log_debug ('HTTP: status code: ' || l_http_resp.status_code, p_app);
      pkg_logger.
       log_debug ('HTTP: reason phrase: ' || l_http_resp.reason_phrase,
                  p_app);

      -- Look for client-side error and report it.
      IF (l_http_resp.status_code >= 400)
         AND (l_http_resp.status_code <= 499)
      THEN
         -- Detect whether the page is password protected,
         -- and we didn't supply the right authorization.
         -- Note the use of utl_http.HTTP_UNAUTHORIZED, a predefined
         -- utl_http package global variable
         IF (l_http_resp.status_code = UTL_HTTP.HTTP_UNAUTHORIZED)
         THEN
            UTL_HTTP.get_authentication (l_http_resp,
                                         l_my_scheme,
                                         l_my_realm,
                                         l_my_proxy);

            IF (l_my_proxy)
            THEN
               pkg_logger.
                log_debug ('HTTP: Web proxy server is protected.', p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supply the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the proxy server.',
                  p_app);
            ELSE
               pkg_logger.
                log_debug ('HTTP: Web page ' || p_url_in || ' is protected.',
                           p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supplied the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the Web page.',
                  p_app);
            END IF;
         ELSE
            pkg_logger.log_debug ('HTTP: Check the URL.', p_app);
         END IF;

         UTL_HTTP.end_response (l_http_resp);
         RETURN 'NOK1';
      -- Look for server-side error and report it.
      ELSIF (l_http_resp.status_code >= 500)
            AND (l_http_resp.status_code <= 599)
      THEN
         pkg_logger.log_debug ('HTTP: Check if the Web site is up.', p_app);
         --UTL_HTTP.END_REQUEST (l_http_req);
         UTL_HTTP.end_response (l_http_resp);
         RETURN 'NOK2';
      END IF;

      BEGIN
         UTL_HTTP.read_text (r => l_http_resp, DATA => v_msg);
         --UTL_HTTP.END_REQUEST (l_http_req);
         UTL_HTTP.end_response (l_http_resp);
         RETURN NVL (v_msg, ' ');
      EXCEPTION
         WHEN UTL_HTTP.end_of_body
         THEN
            -- UTL_HTTP.end_response (l_http_resp);
            NULL;
      END;

      UTL_HTTP.end_response (l_http_resp);
      RETURN 'Sorgunuz gebul edildi. Teshekkur edirik, wap.clicks.az!';
   EXCEPTION
      WHEN OTHERS
      THEN
         pkg_logger.log_error (SQLERRM, p_app);
         pkg_logger.log_error (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, p_app);
         --         UTL_HTTP.END_REQUEST (l_http_req);
         --       UTL_HTTP.end_response (l_http_resp);
         RETURN 'Hormetli abunechi, SMS sorgunuz ishlenmekdedir, cavab SMS-i mueyyen muddetden sonra size chatdirilacaq. Muveqqeti narahatchiliga gore uzr isteyirik.';
   END http_get;
END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_HTTP_HANDLER" 
as
  G_APP  varchar2(50) := 'PKG_HTTP_HANDLER';
  
   procedure add_http_request(p_id out number, 
    p_message_id in number, 
    p_destination_address in varchar2,
    p_short_number in varchar2,
    p_message_body in varchar2,    
    p_module_id in varchar2,
    p_host_name in varchar2 := 'BAKCELL1',
    p_link in varchar2 := null,
    p_method in varchar2 := 'POST'
  )
  is    
    l_link varchar2(500) := p_link;
    l_param varchar2(500);
  begin
    pkg_logger.log_debug('message_id='||p_message_id || 
      ' short_number=' || p_short_number || 
      ' message_body=' || p_message_body, 
      G_APP, p_destination_address);
 
    
    pkg_logger.log_debug('message_id='||p_message_id || 
      ' url=' || l_link || ' param=' || l_param, 
      G_APP, p_destination_address);
    
    INSERT INTO OZUMUZ.HTTP_REQUESTS (N,SMS_ID,REQUEST,METHOD,IS_PROCESSED,R_DATE,APP,PROXY_IN,NO_PROXY_DOMAINS_IN,USER_NAME,PASSWORD, msisdn, host_name, short_number)
    VALUES (HTTP_REQUEST_N_SEQ.NEXTVAL,p_message_id,l_link,p_method, 0,sysdate,p_module_id,NULL,NULL,NULL,NULL,p_destination_address, p_host_name, p_short_number)
    returning N into p_id;
  end add_http_request; 
  
end pkg_http_handler;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_HTTP_PROCESSOR" as

  g_app constant varchar2(20) := 'PKG_HTTP_PROCESSOR';  
  const_lock_id constant number := 1001;
  
  function get_short_number_from_req(p_req_text varchar2, p_req_method varchar2, p_app_name varchar2) return varchar2 is
    l_result varchar2(50);
    l_txt_from varchar2(50):='&sms_number=';
    l_txt_to varchar2(50):='&sended_text=';
    l_ix1 number;
    l_ix2 number;
  begin
    if p_req_method='POST' then
      l_ix1:=instr(p_req_text, l_txt_from)+length(l_txt_from);
      l_ix2:=instr(p_req_text, l_txt_to);
      l_result := substr(p_req_text, l_ix1, l_ix2-l_ix1);
    elsif p_app_name='ISB' then
      l_result := '9707';
    else
      l_txt_from := '&dest=';
      l_result := substr(p_req_text, instr(p_req_text, l_txt_from)+length(l_txt_from));
    end if;
    
    return l_result;
  end;

  procedure do_process_requests is
    l_post_data varchar2(2000);
    l_url varchar2(2000);
    l_response varchar2(2000);
    l_short_number varchar2(50);
  begin 
    UTL_HTTP.set_wallet('file:/u01/app/oracle/admin/nvgr/wallet', 'MyWallet!');
    
    for cur_req in (SELECT * FROM http_requests where is_processed = 0 and retry_count<25 and next_retry_date<=sysdate and rownum <= 250)
    loop
       if cur_req.method='POST' then
         l_post_data := substr(cur_req.request, instr(cur_req.request, '?')+1);
         l_url := substr(cur_req.request, 1, instr(cur_req.request, '?')-1);
       else
         l_post_data := null;
         l_url := cur_req.request;
       end if;
       
       pkg_logger.log_debug_auto('processing http_request '||cur_req.n, g_app, cur_req.msisdn);
       
       begin
          l_response := pkg_util.get_http_response(l_url, cur_req.method, l_post_data);
          update http_requests set is_processed=1, response=substr(l_response, 1, 4000) where n=cur_req.n;
          
          pkg_logger.log_debug_auto('prequest processed ('||cur_req.n||'). response: '||substr(l_response, 1, 1500), g_app, cur_req.msisdn);
               
          if l_response is not null and cur_req.app!='MOZOOK' and cur_req.app!='SEHIYYE' then
             l_short_number := cur_req.short_number;
             if l_short_number is null then
               l_short_number := get_short_number_from_req(cur_req.request, cur_req.method, cur_req.app);
             end if;
             
             pkg_sms_sender.send_sms(cur_req.host_name, l_short_number, cur_req.msisdn, l_response, 0, 1, g_app);
          end if;
          
       exception when others then
          rollback;
          pkg_logger.log_error(SQLERRM, g_app); 
          pkg_logger.log_error(DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
          
          update http_requests set next_retry_date=sysdate+(1/24/60*10), retry_count=retry_count+1 
          where n=cur_req.n;
       end;
       
       commit;
    end loop;
  end;

  procedure process_requests is 
    l_lock_res number;
    l_dummy number;
  begin
    l_lock_res := DBMS_LOCK.REQUEST(const_lock_id, DBMS_LOCK.X_MODE, 1, FALSE); 
    if l_lock_res!=0 then --another copy executing
      return;
    end if;
    
    do_process_requests;
    
    l_dummy := DBMS_LOCK.RELEASE(const_lock_id);
    
  exception when others then
    pkg_logger.log_error(SQLERRM, g_app); 
    pkg_logger.log_error(DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
    commit;
        
    l_dummy := DBMS_LOCK.RELEASE(const_lock_id);
  end;

end pkg_http_processor;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_HTTP_RU" 
IS
   FUNCTION http_post_ru (p_url_in                IN VARCHAR2,
                          v_param                 IN VARCHAR2,
                          p_app                   IN VARCHAR2,
                          p_proxy_in              IN VARCHAR2 DEFAULT NULL,
                          p_no_proxy_domains_in   IN VARCHAR2 DEFAULT NULL,
                          p_username_in           IN VARCHAR2 DEFAULT NULL,
                          p_password_in           IN VARCHAR2 DEFAULT NULL)
      RETURN NUMBER
   IS
      l_http_req       UTL_HTTP.req;
      l_http_resp      UTL_HTTP.resp;
      l_my_scheme      VARCHAR2 (256);
      l_my_realm       VARCHAR2 (256);
      l_my_proxy       BOOLEAN;

      v_param_length   NUMBER := LENGTH (v_param);

      v_msg            VARCHAR2 (100);
   BEGIN
      -- When going through a firewall, pass requests through this host.
      -- Specify sites inside the firewall that don't need the proxy host.
      IF (p_proxy_in IS NOT NULL) AND (p_no_proxy_domains_in IS NOT NULL)
      THEN
         UTL_HTTP.set_proxy (p_proxy_in, p_no_proxy_domains_in);
      END IF;

      -- Ask UTL_HTTP not to raise an exception for 4xx and 5xx status codes,
      -- rather than just returning the text of the error page.
      UTL_HTTP.set_response_error_check (FALSE);

      -- Begin the post request
      l_http_req := UTL_HTTP.begin_request (p_url_in, 'POST');

      -- Set the HTTP request headers
      UTL_HTTP.set_header (l_http_req, 'User-Agent', 'Mozilla/4.0');
      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Type',
                   VALUE   => 'application/x-www-form-urlencoded');
      UTL_HTTP.
       SET_HEADER (r       => l_http_req,
                   name    => 'Content-Length',
                   VALUE   => v_param_length);
      UTL_HTTP.WRITE_TEXT (r => l_http_req, data => v_param);

      -- Specify a user ID and password for pages that require them.
      IF p_username_in IS NOT NULL
      THEN
         UTL_HTTP.
          set_authentication (l_http_req, p_username_in, p_password_in);
      END IF;

      -- Process the request and get the response.
      BEGIN
         l_http_resp := UTL_HTTP.get_response (l_http_req);
      EXCEPTION
         WHEN UTL_HTTP.TOO_MANY_REQUESTS
         THEN
            UTL_HTTP.end_response (l_http_resp);
      END;

      pkg_logger.
       log_debug ('HTTP: status code: ' || l_http_resp.status_code, p_app);
      pkg_logger.
       log_debug ('HTTP: reason phrase: ' || l_http_resp.reason_phrase,
                  p_app);

      -- Look for client-side error and report it.
      IF l_http_resp.status_code = 200
      THEN
         RETURN 200;
      ELSIF (l_http_resp.status_code >= 400)
            AND (l_http_resp.status_code <= 499)
      THEN
         -- Detect whether the page is password protected,
         -- and we didn't supply the right authorization.
         -- Note the use of utl_http.HTTP_UNAUTHORIZED, a predefined
         -- utl_http package global variable
         IF (l_http_resp.status_code = UTL_HTTP.HTTP_UNAUTHORIZED)
         THEN
            UTL_HTTP.get_authentication (l_http_resp,
                                         l_my_scheme,
                                         l_my_realm,
                                         l_my_proxy);

            IF (l_my_proxy)
            THEN
               pkg_logger.
                log_debug ('HTTP: Web proxy server is protected.', p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supply the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the proxy server.',
                  p_app);
            ELSE
               pkg_logger.
                log_debug ('HTTP: Web page ' || p_url_in || ' is protected.',
                           p_app);
               pkg_logger.
                log_debug (
                     'HTTP: Please supplied the required '
                  || l_my_scheme
                  || ' authentication username/password for realm '
                  || l_my_realm
                  || ' for the Web page.',
                  p_app);
            END IF;
         ELSE
            pkg_logger.log_debug ('HTTP: Check the URL.', p_app);
         END IF;

         UTL_HTTP.end_response (l_http_resp);
         RETURN l_http_resp.status_code;
      -- Look for server-side error and report it.
      ELSIF (l_http_resp.status_code >= 500)
            AND (l_http_resp.status_code <= 599)
      THEN
         pkg_logger.log_debug ('HTTP: Check if the Web site is up.', p_app);
         UTL_HTTP.end_response (l_http_resp);
         RETURN l_http_resp.status_code;
      ELSE
         RETURN 0;
      END IF;

      UTL_HTTP.end_response (l_http_resp);
   EXCEPTION
      WHEN OTHERS
      THEN
         pkg_logger.log_error (SQLERRM, p_app);
         pkg_logger.log_error (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, p_app);
         RETURN 0;
   END http_post_ru;
END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_INCOMING_MESSAGE" AS
  g_page_size number:=20;

  procedure search_by_short_number(v_short_number_id number, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) AS
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  l_date_to date:=v_date_to+1;
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and l_date_to order by in_date    
    trunc(in_date) between v_date_from and v_date_to order by in_date    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_incoming where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and l_date_to;
    trunc(in_date) between v_date_from and v_date_to;
    
  END search_by_short_number;
  
  procedure search_all_by_short_number(v_short_number_id number, v_date_from date, v_date_to date, v_items_cursor out t_cursor) AS
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  l_date_to date:=v_date_to+1;
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    select to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and l_date_to order by in_date;
    trunc(in_date) between v_date_from and l_date_to order by in_date;
        
  END search_all_by_short_number;
  
  procedure search_by_short_number_and_app(v_short_number_id number, v_date_from date, v_date_to date, v_app_name varchar2, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) AS
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to 
    trunc(in_date) between v_date_from and v_date_to 
    and is_processed=1 and app_name=v_app_name
    order by in_date
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count 
    from smpp_incoming 
    where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to 
    trunc(in_date) between v_date_from and v_date_to 
    and is_processed=1 and app_name=v_app_name;
    
  END search_by_short_number_and_app;
  
  procedure search_all_by_short_number_app(v_short_number_id number, v_date_from date, v_date_to date, v_app_name varchar2, v_items_cursor out t_cursor) AS
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    select 
    to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to 
    trunc(in_date) between v_date_from and v_date_to 
    and is_processed=1 and app_name=v_app_name
    order by in_date;
    
  END search_all_by_short_number_app;
  
  procedure search_by_sender(v_source_addr varchar2, v_short_number_id number, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) is
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where 
    source_addr=v_source_addr and
    destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to 
    trunc(in_date) between v_date_from and v_date_to 
    order by in_date
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_incoming where 
    source_addr=v_source_addr and
    destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to;
    trunc(in_date) between v_date_from and v_date_to;
    
  END search_by_sender;
  
  procedure search_all_by_sender(v_source_addr varchar2, v_short_number_id number, v_date_from date, v_date_to date, v_items_cursor out t_cursor) is
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  BEGIN
    select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
    
    open v_items_cursor for
    select 
    to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where 
    source_addr=v_source_addr and
    destination_addr=l_short_number and host_name=l_host_name and
    --in_date between v_date_from and v_date_to 
    trunc(in_date) between v_date_from and v_date_to 
    order by in_date;
        
  END search_all_by_sender;

procedure search_all_by_sender(v_source_addr varchar2,  v_date_from date, v_date_to date,v_page_id number,v_total_count out number, v_items_cursor out t_cursor) is
Begin
  open v_items_cursor for
  Select * from (
    Select a.*, rownum rnum From (
 select
  to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where 
    trunc(in_date) between v_date_from and v_date_to and is_processed=1 and source_addr=v_source_addr
    order by in_date
     ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
     select count(0) into v_total_count from smpp_incoming where 
    trunc(in_date) between v_date_from and v_date_to and is_processed=1 and source_addr=v_source_addr;

End search_all_by_sender;

procedure search_by_short_number_all( v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) as
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  l_date_to date:=v_date_to+1;
Begin
 open v_items_cursor for
 Select * from (
    Select a.*, rownum rnum From (
    select to_char(in_date, 'yyyy-mm-dd hh24:mi:ss') as in_date, pkg_util.format_msisdn(source_addr) as source_addr, msg_body,destination_addr
    from smpp_incoming where is_processed=1 and 
    trunc(in_date) between v_date_from and l_date_to order by in_date
     ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_incoming where 
    trunc(in_date) between v_date_from and l_date_to and is_processed=1;
     
end  search_by_short_number_all;

END PKG_INCOMING_MESSAGE;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_LIVE_SMS" AS
    g_page_size number:=30;
        
    procedure get_live_sms_by_short_number(v_ext_access_rec_id number, v_short_number_id number, v_for_date date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) is
    l_short_number varchar2(50);
    l_host_name varchar2(10);
    l_from_date date;
    l_to_date date;
    l_ext_access_date date;
    begin
      select date_added into l_ext_access_date from external_access where rec_id=v_ext_access_rec_id;
    
      l_from_date :=trunc(v_for_date);
      l_to_date   :=trunc(v_for_date)+1;
    
      select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
      
      open v_items_cursor for 
      Select * from (
      Select a.*, rownum rnum From (
      
      select 
      (select count(0) from smpp_incoming si1 where 
                                                si1.destination_addr=smpp_incoming.destination_addr and si1.host_name=smpp_incoming.host_name 
                                                and si1.in_date >= l_ext_access_date
                                                and si1.msg_id <= smpp_incoming.msg_id) as rec_counter,
      msg_id, pkg_util.format_msisdn(source_addr) as source_addr, destination_addr, 
      msg_body, in_date,
      is_processed, app_str_param1 from smpp_incoming where
      destination_addr=l_short_number and host_name=l_host_name 
      and in_date between l_from_date and l_to_date
      and in_date >= l_ext_access_date
      order by in_date, msg_id desc 
      
      ) a where rownum <= v_page_id*g_page_size
      ) where rnum >= (v_page_id-1)*g_page_size;
      
      select count(0) into v_total_count from smpp_incoming where
      destination_addr=l_short_number and host_name=l_host_name and 
      in_date between l_from_date and l_to_date
      and in_date >= l_ext_access_date;
    end;
    
    procedure reply_to_sms(v_ext_access_rec_id number, v_short_number_id number, v_msg_id number, v_text nvarchar2) is
    l_short_number varchar2(50);
    l_host_name varchar2(10);
    l_ext_access_date date;
    l_destination_addr varchar2(50);
    begin
       select date_added into l_ext_access_date from external_access where rec_id=v_ext_access_rec_id;
       select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
       
       update smpp_incoming set is_processed=1, app_name='LIVE_SMS2', app_str_param1=v_text, date_processed=sysdate
       where msg_id=v_msg_id and host_name=l_host_name and destination_addr=l_short_number
       and is_processed=0 and in_date > l_ext_access_date;
       
       if SQL%ROWCOUNT = 1 then
          select source_addr into l_destination_addr from smpp_incoming where msg_id = v_msg_id;
          pkg_sms_sender.send_sms(l_host_name, l_short_number, l_destination_addr, v_text, 0, 0, 'LIVE_SMS2');
       end if;
       
    end;
    
END PKG_LIVE_SMS;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_LOGGER" AS

  procedure do_log(v_log_text varchar2, v_app_name varchar2, log_category varchar2) is
  begin
    do_log(v_log_text, v_app_name, null, log_category);
  end;
  
  procedure do_log(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2, log_category varchar2) is
  --burda avtonom tranzaksiya istifade etmeyin
  begin
    insert into app_log (log_id, log_category, log_date, log_text, app_name, msisdn)
    values (log_seq.nextval, log_category, sysdate, v_log_text, v_app_name, v_msisdn);
  exception when others then --loga insert getmediyine gore rollback edib sakit durmaq olmaz
     null; --helelik null qoyuram, amma fix etmek lazimdir
  end;

  procedure log_debug(v_log_text varchar2, v_app_name varchar2) is
  begin
    log_debug(v_log_text, v_app_name, null);
  end;
  
  procedure log_debug(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2) AS
  BEGIN
    do_log(v_log_text, v_app_name, v_msisdn, 'DEBUG');
  END log_debug;    
  
  procedure log_error(v_log_text varchar2, v_app_name varchar2) is
  begin
    do_log(v_log_text, v_app_name, null, 'ERROR');
  end;
  
  procedure log_error(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2) is
  begin
    do_log(v_log_text, v_app_name, v_msisdn, 'ERROR');
  end;
  
  procedure log_debug_auto(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2) AS  
  pragma autonomous_transaction;
  BEGIN        
    do_log(v_log_text, v_app_name, v_msisdn, 'DEBUG');
    commit;
  END log_debug_auto;
  
  procedure log_debug_auto(v_log_text varchar2, v_app_name varchar2) AS  
  pragma autonomous_transaction;
  BEGIN        
    do_log(v_log_text, v_app_name, null, 'DEBUG');
    commit;
  END log_debug_auto;
  
  procedure log_error_auto(v_log_text varchar2, v_app_name varchar2, v_msisdn varchar2) is
  pragma autonomous_transaction;
  begin
    do_log(v_log_text, v_app_name, v_msisdn, 'ERROR');
    commit;
  end;
  
  procedure log_error_auto(v_log_text varchar2, v_app_name varchar2) is
  pragma autonomous_transaction;
  begin
    do_log(v_log_text, v_app_name, null, 'ERROR');
    commit;
  end;

END PKG_LOGGER;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_MANAGER_ACCESS" AS

  procedure get_manager_ip_addresses(v_man_id number, v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for
    select rec_id, ip_address from manager_access where manager_id=v_man_id;
  END get_manager_ip_addresses;
  
  procedure update_ip_address(v_rec_id number, v_ip_address varchar2) is
  begin
    update manager_access set ip_address=v_ip_address where rec_id=v_rec_id;
  end;
  
  procedure delete_ip_address(v_rec_id number) is
  begin
    delete from manager_access where rec_id=v_rec_id;
  end;
  
  PROCEDURE ADD_IP_ADDRESS(V_MANAGER_ID NUMBER,V_IP_ADDRESS VARCHAR2) IS
  BEGIN 
  INSERT INTO MANAGER_ACCESS (REC_ID,MANAGER_ID,IP_ADDRESS) VALUES (MANAGER_ACCESS_SEQ.NEXTVAL,V_MANAGER_ID,V_IP_ADDRESS);
  END;



END PKG_MANAGER_ACCESS;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_OUTGOING_MESSAGE" AS
  g_page_size number:=20; 

  procedure search_by_date_range(v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) AS
  l_date_to date:=v_date_to+1;
  BEGIN
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    date_added, source_addr, host_name, pkg_util.format_msisdn(destination_addr) as destination_addr, msg_body, status, delivery_report, done_date, final_status
    from smpp_outgoing where date_added between v_date_from and l_date_to order by date_added
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_outgoing where date_added between v_date_from and l_date_to;
    
  END search_by_date_range;
  
  procedure search_by_date_range(v_short_number varchar2,v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) as
  l_date_to date:=v_date_to+1;
  Begin
  
     open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    date_added, source_addr, host_name, pkg_util.format_msisdn(destination_addr) as destination_addr, msg_body, status, delivery_report, done_date, final_status
    from smpp_outgoing where date_added between v_date_from and l_date_to and source_addr=v_short_number order by date_added
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_outgoing where date_added between v_date_from and l_date_to and source_addr=v_short_number;
  end search_by_date_range;

  procedure search_by_receiver(v_destination_addr varchar2, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) AS
  l_date_to date:=v_date_to+1;
  BEGIN
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    date_added, source_addr, host_name, pkg_util.format_msisdn(destination_addr) as destination_addr, msg_body, status, delivery_report, done_date, final_status
    from smpp_outgoing where destination_addr=v_destination_addr and date_added between v_date_from and l_date_to order by date_added
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_outgoing where destination_addr=v_destination_addr and date_added between v_date_from and l_date_to;
  END search_by_receiver;

  procedure search_by_receiver(v_source_addr varchar2,v_destination_addr varchar2, v_date_from date, v_date_to date, v_page_id number, v_total_count out number, v_items_cursor out t_cursor)
  as
  l_date_to date:=v_date_to+1;
  BEGIN
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    date_added, source_addr, host_name, pkg_util.format_msisdn(destination_addr) as destination_addr, msg_body, status, delivery_report, done_date, final_status
    from smpp_outgoing where source_addr=v_source_addr and  destination_addr=v_destination_addr and date_added between v_date_from and l_date_to order by date_added
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum >= (v_page_id-1)*g_page_size;
    
    select count(0) into v_total_count from smpp_outgoing where source_addr=v_source_addr and destination_addr=v_destination_addr and date_added between v_date_from and l_date_to;
  END search_by_receiver;

END PKG_OUTGOING_MESSAGE;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL" AS

  procedure get_polls_to_display(v_items_cursor out t_cursor) is
  begin 
    open v_items_cursor for
    select * from 
    (select poll_id, poll_name, poll_desc, poll_type, date_added from polls order by date_added desc
    ) t
    where rownum<=30;
  end;
  
  procedure get_poll_to_display(v_poll_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select poll_id, poll_name, poll_desc, poll_type, date_added from polls where poll_id=v_poll_id;
  end;
  
  PROCEDURE ADD_POLL(V_POLL_NAME NVARCHAR2,V_POLL_DESC NVARCHAR2,V_POLL_TYPE NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
    if V_POLL_TYPE not between 1 and 2 then
      return;
    end if;
    INSERT INTO POLLS (POLL_ID,POLL_NAME,POLL_DESC,POLL_TYPE) VALUES (POLLS_SEQ.NEXTVAL,V_POLL_NAME,V_POLL_DESC,V_POLL_TYPE);
    SELECT POLLS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  procedure get_most_active_numbers(v_poll_id number, v_phase_ids nvarchar2, v_max_count number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select * from (select i.source_addr, sum(mn.vote_count) as total_vote_count, count(0) as total_sms_count from poll_member_numbers mn 
    inner join short_numbers_view sn on mn.short_number_id=sn.number_id
    inner join smpp_incoming i on (sn.short_number=i.destination_addr and sn.host_name=i.host_name)
    where poll_id=v_poll_id 
    and i.app_name='POLL' and app_num_param1 in(select * from table(pkg_util.get_number_table(v_phase_ids, ',')))
    group by source_addr order by total_vote_count desc, total_sms_count desc) where rownum<=v_max_count;
  end;


END PKG_POLL;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_MEMBER" AS


PROCEDURE GET_POLL_MEMBERS_BY_POLL_ID(V_POLL_ID NUMBER, V_ITEMS_CURSOR OUT T_CURSOR) IS
BEGIN
  OPEN V_ITEMS_CURSOR FOR
  SELECT MEMBER_ID,MEMBER_NAME,MEMBER_DESC,IS_ACTIVE FROM POLL_MEMBERS WHERE POLL_ID=V_POLL_ID ORDER BY MEMBER_NAME;
END;

PROCEDURE ADD_POLL_MEMBER(V_POLL_ID NUMBER,V_MEMBER_NAME NVARCHAR2,V_MEMBER_DESC NVARCHAR2,V_IS_ACTIVE NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
BEGIN
INSERT INTO POLL_MEMBERS (MEMBER_ID,POLL_ID,MEMBER_NAME,MEMBER_DESC,IS_ACTIVE) VALUES (POLL_MEMBERS_SEQ.NEXTVAL,V_POLL_ID,V_MEMBER_NAME,V_MEMBER_DESC,V_IS_ACTIVE);
SELECT POLL_MEMBERS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
END;

PROCEDURE UPDATE_POLL_MEMBER(V_MEMBER_ID NUMBER,V_POLL_ID NUMBER,V_MEMBER_NAME NVARCHAR2,V_MEMBER_DESC NVARCHAR2,V_IS_ACTIVE NUMBER) IS
BEGIN
UPDATE POLL_MEMBERS SET MEMBER_ID=V_MEMBER_ID,POLL_ID=V_POLL_ID,MEMBER_NAME=V_MEMBER_NAME,MEMBER_DESC=V_MEMBER_DESC,IS_ACTIVE=V_IS_ACTIVE WHERE MEMBER_ID=V_MEMBER_ID;
END;

PROCEDURE DELETE_POLL_MEMBER(V_MEMBER_ID NUMBER) IS
BEGIN
DELETE FROM POLL_MEMBERS WHERE MEMBER_ID=V_MEMBER_ID;
END;


END PKG_POLL_MEMBER;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_NUMBER" AS


procedure get_numbers_by_member_id(v_member_id number, v_items_cursor out t_cursor) is
begin
  open v_items_cursor for
  select * from poll_member_numbers_view where member_id=v_member_id;
end;
 
procedure get_numbers_by_poll_id(v_poll_id number, v_items_cursor out t_cursor) is
begin
  open v_items_cursor for
  select * from poll_member_numbers_view where poll_id=v_poll_id;
end;

PROCEDURE ADD_POLL_MEMBER_NUMBER(V_MEMBER_ID NUMBER,V_SHORT_NUMBER_ID NUMBER,V_VOTE_COUNT NUMBER,V_POLL_ID NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
BEGIN
INSERT INTO POLL_MEMBER_NUMBERS (NUMBER_ID,MEMBER_ID,SHORT_NUMBER_ID,VOTE_COUNT,POLL_ID) VALUES (POLL_MEMBER_NUMBERS_SEQ.NEXTVAL,V_MEMBER_ID,V_SHORT_NUMBER_ID,V_VOTE_COUNT,V_POLL_ID);
SELECT POLL_MEMBER_NUMBERS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
END;

PROCEDURE UPDATE_POLL_MEMBER_NUMBER(V_NUMBER_ID NUMBER,V_MEMBER_ID NUMBER,V_SHORT_NUMBER_ID NUMBER,V_VOTE_COUNT NUMBER,V_POLL_ID NUMBER) IS
BEGIN
UPDATE POLL_MEMBER_NUMBERS SET NUMBER_ID=V_NUMBER_ID,MEMBER_ID=V_MEMBER_ID,SHORT_NUMBER_ID=V_SHORT_NUMBER_ID,VOTE_COUNT=V_VOTE_COUNT,POLL_ID=V_POLL_ID WHERE NUMBER_ID=V_NUMBER_ID;
END;

PROCEDURE DELETE_POLL_MEMBER_NUMBER(V_NUMBER_ID NUMBER) IS
BEGIN
DELETE FROM POLL_MEMBER_NUMBERS WHERE NUMBER_ID=V_NUMBER_ID;
END;



END PKG_POLL_NUMBER;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_PHASE" AS

  procedure get_poll_phases_by_poll_id(v_poll_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for select 
    phase_id,
    poll_id,
    phase_name,
    phase_desc,
    phase_start_date,
    phase_end_date,
    case 
      when sysdate between phase_start_date and phase_end_date then 1
      else 0
    end as is_current
    from poll_phases where poll_id=v_poll_id
    order by phase_start_date;
  end;
  
  procedure get_active_phase_by_poll_id(v_poll_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for select 
    phase_id,
    poll_id,
    phase_name,
    phase_desc,
    phase_start_date,
    phase_end_date,
    1 as is_current
    from poll_phases where poll_id=v_poll_id and sysdate between phase_start_date and phase_end_date;
  end;
  
  PROCEDURE ADD_POLL_PHASE(V_POLL_ID NUMBER,V_PHASE_NAME NVARCHAR2,V_PHASE_DESC NVARCHAR2,V_PHASE_START_DATE DATE,V_PHASE_END_DATE DATE,V_NEW_ROW_ID OUT NUMBER) IS
  BEGIN
  INSERT INTO POLL_PHASES (PHASE_ID,POLL_ID,PHASE_NAME,PHASE_DESC,PHASE_START_DATE,PHASE_END_DATE) VALUES (POLL_PHASES_SEQ.NEXTVAL,V_POLL_ID,V_PHASE_NAME,V_PHASE_DESC,V_PHASE_START_DATE,V_PHASE_END_DATE);
  SELECT POLL_PHASES_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
  END;
  
  PROCEDURE UPDATE_POLL_PHASE(V_PHASE_ID NUMBER,V_POLL_ID NUMBER,V_PHASE_NAME NVARCHAR2,V_PHASE_DESC NVARCHAR2,V_PHASE_START_DATE DATE,V_PHASE_END_DATE DATE) IS
  BEGIN
  UPDATE POLL_PHASES SET PHASE_ID=V_PHASE_ID,POLL_ID=V_POLL_ID,PHASE_NAME=V_PHASE_NAME,PHASE_DESC=V_PHASE_DESC,PHASE_START_DATE=V_PHASE_START_DATE,PHASE_END_DATE=V_PHASE_END_DATE WHERE PHASE_ID=V_PHASE_ID;
  END;
  
  PROCEDURE DELETE_POLL_PHASE(V_PHASE_ID NUMBER) IS
  BEGIN
  DELETE FROM POLL_PHASES WHERE PHASE_ID=V_PHASE_ID;
  END;
  
  procedure change_poll_phase_time(v_poll_id number, v_phase_id number, v_start_time number, v_add number) is
  begin
    if v_add not in (-1, -5, -10, -30, -60, 1, 5, 10, 30, 60, 1000) then
      return;
    end if;
    
    if v_start_time=1 then
      update poll_phases set phase_start_date=phase_start_date+v_add/24/60 
      where poll_id=v_poll_id and phase_id=v_phase_id;
    elsif v_add=1000 then
      update poll_phases set phase_end_date=sysdate 
      where poll_id=v_poll_id and phase_id=v_phase_id;
    else
      update poll_phases set phase_end_date=phase_end_date+v_add/24/60 
      where poll_id=v_poll_id and phase_id=v_phase_id;
    end if;
  end;
  

END PKG_POLL_PHASE;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_PROCESSING" AS

  g_app varchar2(100):='PKG_POLL_PROCESSING';

  procedure process_ongoing_polls AS
  cursor l_polls_cursor is
    select poll_id, phase_id, poll_type, phase_start_date, phase_end_date from ongoing_poll_phases_view;
  BEGIN
    pkg_logger.log_debug('Starting to process ongoing polls', g_app); 
    for c in l_polls_cursor
    loop
      pkg_logger.log_debug('processing poll phase: ' || c.phase_id, g_app);
      process_ongoing_poll_phase(c.poll_id, c.phase_id, c.poll_type, c.phase_start_date, c.phase_end_date);
      commit;
    end loop;
    pkg_logger.log_debug('Finished processing ongoing polls', g_app);
    commit;
  END process_ongoing_polls;
  
  procedure process_ongoing_poll_phase(v_poll_id number, v_phase_id number, v_poll_type number, v_start_date date, v_end_date date) is
  l_max_msg_id number;
  l_has_records number;
  begin
    
    select count(*) into l_has_records from smpp_incoming where rownum=1;
    if l_has_records<1 then
      return;
    end if;
  
    select max(msg_id) into l_max_msg_id from smpp_incoming;
    pkg_logger.log_debug('max smpp_incoming msg_id id is :' || l_max_msg_id, g_app);
     
    pkg_logger.log_debug('poll type is: ' || v_poll_type, g_app);
    if v_poll_type=1 then --poll by numbers
      process_numeric_poll_phase(v_poll_id, v_phase_id, v_start_date, v_end_date, l_max_msg_id);
    elsif v_poll_type=2 then --poll by text
      process_text_poll_phase(v_poll_id, v_phase_id, v_start_date, v_end_date, l_max_msg_id);
    end if; 
  end;
  
  procedure process_numeric_poll_phase(v_poll_id number, v_phase_id number, v_start_date date, v_end_date date, v_max_id number) is
  l_did_some_work number:=0;
  cursor l_member_votes_cursor is
    select mn.member_id, sum(mn.vote_count) as new_vote_count from poll_member_numbers mn 
    inner join short_numbers_view sn on mn.short_number_id=sn.number_id
    inner join smpp_incoming i on (sn.short_number=i.destination_addr and sn.host_name=i.host_name)
    where poll_id=v_poll_id and i.is_processed=0 and msg_id<=v_max_id and i.in_date between v_start_date and v_end_date
    group by member_id;
  begin
    for c in l_member_votes_cursor
    loop
      pkg_logger.log_debug('member and new vote count: ' || c.member_id || ' - ' || c.new_vote_count, g_app);
      
      update poll_results set vote_count=vote_count+c.new_vote_count where poll_phase_id=v_phase_id and poll_member_id=c.member_id;
      if SQL%ROWCOUNT<1 then
        pkg_logger.log_debug('its first time, inserting a new record into poll_results table', g_app);
        insert into poll_results (result_id, poll_phase_id, poll_member_id, vote_count)
        values (poll_results_seq.nextval, v_phase_id, c.member_id, c.new_vote_count);
      end if;
      commit;
         
      --mark messages as processed in smpp_incoming table
      pkg_logger.log_debug('member votes processed', g_app);
      l_did_some_work:=1;
    end loop;
    
    if l_did_some_work>0 then
      pkg_logger.log_debug('marking messages as processed. max id: ' || v_max_id, g_app);
      update smpp_incoming set is_processed=1, date_processed=sysdate, app_name='POLL', app_num_param1=v_phase_id 
      where msg_id in 
      (select i.msg_id from poll_member_numbers mn 
      inner join short_numbers_view sn on mn.short_number_id=sn.number_id
      inner join smpp_incoming i on (sn.short_number=i.destination_addr and sn.host_name=i.host_name)
      where poll_id=v_poll_id and i.is_processed=0 and msg_id<=v_max_id and i.in_date between v_start_date and v_end_date
      );
        
      commit;
    end if;
  end;
  
  procedure process_text_poll_phase(v_poll_id number, v_phase_id number, v_start_date date, v_end_date date, v_max_id number) is
  l_did_some_work number:=0;
  cursor l_member_votes_cursor is
    select mt.member_id, sum(mn.vote_count) as new_vote_count from  poll_member_texts mt
    inner join poll_member_numbers mn on mn.poll_id=mt.poll_id
    inner join short_numbers_view sn on mn.short_number_id=sn.number_id
    inner join smpp_incoming i on (sn.short_number=i.destination_addr and sn.host_name=i.host_name and upper(mt.text_body)=upper(i.msg_body))
    where mt.poll_id=v_poll_id and i.is_processed=0 and msg_id<=v_max_id and i.in_date between v_start_date and v_end_date
    group by mt.member_id;
  begin
    for c in l_member_votes_cursor
    loop
      pkg_logger.log_debug('member and new vote count: ' || c.member_id || ' - ' || c.new_vote_count, g_app);
      
      update poll_results set vote_count=vote_count+c.new_vote_count where poll_phase_id=v_phase_id and poll_member_id=c.member_id;
      if SQL%ROWCOUNT<1 then
        pkg_logger.log_debug('its first time, inserting a new record into poll_results table', g_app);
        insert into poll_results (result_id, poll_phase_id, poll_member_id, vote_count)
        values (poll_results_seq.nextval, v_phase_id, c.member_id, c.new_vote_count);
      end if;
      commit;
         
      --mark messages as processed in smpp_incoming table
      pkg_logger.log_debug('member votes processed', g_app);
      l_did_some_work:=1;
    end loop;
    
    if l_did_some_work>0 then
      pkg_logger.log_debug('marking messages as processed. max id: ' || v_max_id, g_app);
      update smpp_incoming set is_processed=1, date_processed=sysdate, app_name='POLL', app_num_param1=v_phase_id 
      where msg_id in 
      (select i.msg_id from  poll_member_texts mt
        inner join poll_member_numbers mn on mn.poll_id=mt.poll_id
        inner join short_numbers_view sn on mn.short_number_id=sn.number_id
        inner join smpp_incoming i on (sn.short_number=i.destination_addr and sn.host_name=i.host_name and upper(mt.text_body)=upper(i.msg_body))
        where mt.poll_id=v_poll_id and i.is_processed=0 and msg_id<=v_max_id and i.in_date between v_start_date and v_end_date
      );
        
      commit;
    end if;
    
  end;
    
END PKG_POLL_PROCESSING;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_RESULTS" AS

  procedure get_member_votes_by_poll_phase(v_phase_id number, v_items_cursor out t_cursor) is
  l_total_votes number;
  begin
    select sum(vote_count) into l_total_votes from poll_results where poll_phase_id=v_phase_id;
  
    open v_items_cursor for
    select poll_members.member_name, poll_results.vote_count,
    to_char(poll_results.vote_count, '999,999,999,999') as vote_count_formatted, 
    round(poll_results.vote_count*100/l_total_votes, 2) as count_in_percents  from poll_results 
    inner join poll_phases on poll_results.poll_phase_id=poll_phases.phase_id
    inner join poll_members on (poll_phases.poll_id=poll_members.poll_id and poll_members.member_id=poll_results.poll_member_id)
    where poll_results.poll_phase_id=v_phase_id
    order by vote_count desc;
  end;

END PKG_POLL_RESULTS;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_POLL_TEXT" AS

procedure get_texts_by_member_id(v_member_id number, v_items_cursor out t_cursor) is
begin
  open v_items_cursor for
  select * from poll_member_texts where member_id=v_member_id;
end;

PROCEDURE ADD_POLL_MEMBER_TEXT(V_MEMBER_ID NUMBER,V_TEXT_BODY NVARCHAR2,V_POLL_ID NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
BEGIN
INSERT INTO POLL_MEMBER_TEXTS (TEXT_ID,MEMBER_ID,TEXT_BODY,POLL_ID) VALUES (POLL_MEMBER_TEXTS_SEQ.NEXTVAL,V_MEMBER_ID,V_TEXT_BODY,V_POLL_ID);
SELECT POLL_MEMBER_TEXTS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
END;

PROCEDURE UPDATE_POLL_MEMBER_TEXT(V_TEXT_ID NUMBER,V_MEMBER_ID NUMBER,V_TEXT_BODY NVARCHAR2,V_POLL_ID NUMBER) IS
BEGIN
UPDATE POLL_MEMBER_TEXTS SET TEXT_ID=V_TEXT_ID,MEMBER_ID=V_MEMBER_ID,TEXT_BODY=V_TEXT_BODY,POLL_ID=V_POLL_ID WHERE TEXT_ID=V_TEXT_ID;
END;

PROCEDURE DELETE_POLL_MEMBER_TEXT(V_TEXT_ID NUMBER) IS
BEGIN
DELETE FROM POLL_MEMBER_TEXTS WHERE TEXT_ID=V_TEXT_ID;
END;



END PKG_POLL_TEXT;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_QUICK_SMS" as

  procedure send(p_key varchar2, p_text varchar2, p_msisdn varchar2, p_operator_id number, 
                 o_result out number) is
     l_host_name smpp_hosts.host_name%type;
     l_source_addr quick_sms_clients.source_addr%type;
     l_current_balance number;
     l_manager_id number;
     l_is_bulk number;
     l_host_pack_id number;
  begin
     select q.source_addr, q.ui_user_id, q.is_bulk_sms, q.pack_id,
     (select nvl(max(balance), 0) from bulk_sms2_balance where user_id=q.ui_user_id) as current_balance
     into l_source_addr, l_manager_id, l_is_bulk, l_host_pack_id, l_current_balance
     from quick_sms_clients q
     where client_key=p_key;
     
     if l_is_bulk = 1 and l_current_balance < 1 then
        o_result := 4;
        return;
     end if;
     
     begin
        select host_name into l_host_name from smpp_hosts
        where host_id=(select host_id from host_packs_hosts where operator_id=p_operator_id and pack_id=l_host_pack_id);
     exception when no_data_found then
        o_result := 4;
        return;
     end;
     
     pkg_sms_sender.send_sms(l_host_name, l_source_addr, 
                             p_msisdn, p_text, 1, 1, 
                             'QUICK_SMS_'||p_key);
                     
     if l_is_bulk = 1 then        
        update bulk_sms2_balance set balance=balance-1 where user_id=l_manager_id;
     end if;
                                                         
     o_result := 0;
  exception when no_data_found then
     o_result := 1;
  end;
  
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_REPORT_ARAZ" 
 is
    g_page_size number:=20;
 procedure get_incoming_sms_by_arazfm(v_msisdn in varchar2, v_from date, v_to date,  v_items_cursor out t_cursor) 
 is
 Begin
 open v_items_cursor for
 select * from (
   select i.source_addr , i.destination_addr  , i.in_date   , i.msg_body from smpp_incoming i ,diameter_requests d
   where i.destination_addr='81033' and i.in_date between v_from and v_to and i.is_processed=1
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
   and d.diameter_answer=100
   );
 end get_incoming_sms_by_arazfm;
 
 procedure get_incoming_sms_by_arazfm( v_items_cursor out t_cursor)
 is
 Begin
 
 open v_items_cursor for
 select * from (
     select i.source_addr , i.destination_addr  , i.in_date   , i.msg_body from smpp_incoming i ,diameter_requests d
   where i.destination_addr='81033'   and i.is_processed=1
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
   and d.diameter_answer=100
   );
 
 end get_incoming_sms_by_arazfm;
 
 procedure get_incoming_sms_by_arazfm(v_msisdn in varchar2, v_from date, v_to date, v_items_cursor out t_cursor, v_page_number in number, v_total_count out number) 
 is
 Begin
 
 if v_msisdn is null then 
 open v_items_cursor for
 select * from (
 select  a.*, rownum rnum  from (
   select i.source_addr  , i.in_date   , i.msg_body from smpp_incoming i,diameter_requests d
   where i.destination_addr='81033' and i.is_processed=1
   and i.in_date between trunc(v_from) and trunc(v_to) 
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
  and d.diameter_answer=100
   order by in_date desc
   ) a where rownum <= v_page_number*g_page_size
    ) where rnum >= (v_page_number-1)*g_page_size;
    
    select count(*) into v_total_count from (
      select i.source_addr  , i.in_date   , i.msg_body from smpp_incoming i,diameter_requests d
   where i.destination_addr='81033' and i.is_processed=1
   and i.in_date between trunc(v_from) and trunc(v_to) 
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
   and d.diameter_answer=100
   order by in_date desc
    );
    
    else 
    open v_items_cursor for 
    select * from (
 select  a.*, rownum rnum  from (
  select i.source_addr  , i.in_date   , i.msg_body from smpp_incoming i,diameter_requests d
   where i.destination_addr='81033' and i.is_processed=1
   and i.in_date between trunc(v_from) and trunc(v_to) 
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
   and d.diameter_answer=100
   
   and i.source_addr=v_msisdn order by in_date desc
   ) a where rownum <= v_page_number*g_page_size
    ) where rnum >= (v_page_number-1)*g_page_size;
    
    select count(*) into v_total_count from (
       select i.source_addr  , i.in_date   , i.msg_body from smpp_incoming i,diameter_requests d
   where i.destination_addr='81033' and i.is_processed=1
   and i.in_date between trunc(v_from) and trunc(v_to) 
   and i.app_name=D.MODULE_ID
   and i.msg_id=D.MSG_ID
   and d.diameter_answer=100
   
   and i.source_addr=v_msisdn
   
  order by in_date desc
    );
    
    end if;
 end get_incoming_sms_by_arazfm;
 end pkg_report_araz;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_REQUEST_HANDLER" as

  g_app constant varchar2(20) := 'PKG_REQUEST_HANDLER';  
  
  const_status_pending constant varchar2(20) := 'pending';
  const_status_done constant varchar2(20) := 'done';
  const_status_error constant varchar2(20) := 'error';
  
  const_retry_after_minutes constant number := 5; 
  const_max_retry_count constant number := 10;
  
  
  const_lock_id number := 1000;

  procedure do_process_queue(p_queue_id number) as
    l_rec t_handler_rec_type;
    
    l_proc_result t_proc_result_type;
    l_charge_result t_charge_result_type;
    
    l_item_done number;
    
    l_sms_tpl_to_send sms_templates.tpl_text%type;
    
    l_is_already_processed number;
  
  begin
    for cur_queue in (select * from (select h.proc_name, h.tariff_id, 
                      h.handler_id,
                      h.charge_reason, h.app_name, h.client_id,
                      --sn.number_id as charge_short_number_id, --sh.operator_id,
                      h.retry_on_network_error, h.num_param1, h.num_param2, h.str_param1, h.str_param2, 
                      h.send_charge_fail_msg, q.rec_id, q.msg_id, i.host_name, 
                      i.source_addr as a_num, i.destination_addr as b_num, i.date_added,
                      trim(decode(rl.source_chars, null, i.msg_body, translate(i.msg_body, rl.source_chars, rl.dest_chars))) as msg_body, 
                      q.status, q.retry_count
                      from handler_queue q inner join smpp_incoming i on (q.msg_id=i.msg_id and i.is_processed=0)
                      inner join handler_list h on (
                        regexp_like(i.host_name, h.host_name) and
                        regexp_like(i.source_addr, h.a_num) and
                        regexp_like(i.destination_addr, h.b_num) and
                        h.handler_status='a' and sysdate between h.valid_from and h.valid_to
                      ) 
                      inner join smpp_hosts sh on i.host_name=sh.host_name
                      inner join short_numbers sn on (sn.host_id=sh.host_id and sn.short_number=i.destination_addr)
                      inner join handler_queues queue_names on (regexp_like(i.host_name, queue_names.host_name) and queue_names.queue_id=p_queue_id)
                      --left join diameter_tariffs t on (t.tariff_id=h.tariff_id and t.operator_id=sh.operator_id)
                      left join chr_replacement_list  rl on (h.chr_replacement_rule_id=rl.rec_id)
                      left join segment_numbers segnum on (h.segment_id=segnum.segment_id and segnum.msisdn=i.source_addr)
                      where q.next_retry_date<=sysdate
                      and status = const_status_pending and insert_date > (sysdate-2) and i.in_date > (sysdate-2)
                      and (regexp_like(trim(decode(rl.source_chars, null, i.msg_body, translate(i.msg_body, rl.source_chars, rl.dest_chars))), h.content, 'in') or (i.msg_body is null and h.content='.*'))
                      order by decode(segnum.segment_id, null, 1, 0), h.sort_order) t where rownum<=250)
    loop
      begin
          select case when exists (select 0 from handler_queue where 
                                   msg_id=cur_queue.msg_id and 
                                   status != const_status_pending and insert_date > (sysdate-3)) then 1
                 else 0 end into l_is_already_processed from dual;
                 
          if l_is_already_processed = 0 then 
          
              --savepoint start_req_processing;
      
              l_item_done := 0;
              l_sms_tpl_to_send := null;
            
              pkg_logger.log_debug_auto('processing sms with id:'||cur_queue.msg_id, g_app, cur_queue.a_num); 
            
              l_rec.msg_id := cur_queue.msg_id;
              l_rec.host_name := cur_queue.host_name;
              l_rec.a_num := cur_queue.a_num;
              l_rec.b_num := cur_queue.b_num;
              l_rec.date_added := cur_queue.date_added;
              l_rec.msg_body := cur_queue.msg_body; 
              l_rec.app_name := cur_queue.app_name; 
              l_rec.num_param1 := cur_queue.num_param1;
              l_rec.num_param2 := cur_queue.num_param2;
              l_rec.str_param1 := cur_queue.str_param1;
              l_rec.str_param2 := cur_queue.str_param2;
              
              --eger cari veziyyet = gozleme hali ve before_charge_proc varsa
              if cur_queue.proc_name is not null then
                l_proc_result := call_proc(cur_queue.proc_name, l_rec); --before_charge_proc chagiririq
                pkg_logger.log_debug('result of first proc:'||l_proc_result.num_result||','||l_proc_result.str_result||', ok_to_charge='||l_proc_result.ok_to_charge, g_app, cur_queue.a_num); 
                
                if l_proc_result.num_result = pkg_request_handler.c_call_proc_retry then --eger chagrilan prosedur birazdan yeniden chagrilmag isteyirse
                    if cur_queue.retry_count > const_max_retry_count then
                       set_item_status(cur_queue.rec_id, const_status_error,
                                     cur_queue.msg_id, cur_queue.app_name, 
                                     l_proc_result.app_num_id, l_proc_result.str_result,
                                     cur_queue.a_num);
                    else
                       mark_item_for_retry(cur_queue.rec_id, cur_queue.a_num);
                    end if;
                elsif l_proc_result.num_result >= pkg_request_handler.c_call_proc_error then
                    set_item_status(cur_queue.rec_id, l_proc_result.num_result,
                                      cur_queue.msg_id, cur_queue.app_name, 
                                      l_proc_result.app_num_id, l_proc_result.str_result,
                                      cur_queue.a_num);
                end if;
              end if;
              
              --eger cari veziyyet = gozleme hali ve 1-ci prosedur ugurla bitdise (ve ya proc_name boshdursa)
              if l_proc_result.num_result = pkg_request_handler.c_call_proc_success then
                  
                  l_sms_tpl_to_send := l_proc_result.sms_template;
                  --eger charge etmek lazimdirsa ve 1-ci chagrilan prosedur buna etiraz etmirse
                  if (cur_queue.tariff_id is not null or l_proc_result.tariff_id is not null) 
                      and l_proc_result.ok_to_charge = 1 then 
                      --charge etmeye chalishiriq
                      l_charge_result := charge_request(cur_queue.a_num, 
                                                        nvl(l_proc_result.tariff_id, cur_queue.tariff_id), 
                                                        cur_queue.client_id,
                                                        cur_queue.charge_reason,
                                                        cur_queue.msg_id);
                      pkg_logger.log_debug('result of charge proc:'||l_charge_result.num_result, g_app, cur_queue.a_num); 
                  end if;
                  
                  if l_charge_result.num_result = pkg_diameter.const_success then --eger charge normal kechdise ve ya charge etmeye ehtiyac yoxdursa       
                      set_item_status(cur_queue.rec_id, const_status_done,
                                      cur_queue.msg_id, cur_queue.app_name, 
                                      l_proc_result.app_num_id, l_proc_result.str_result,
                                      cur_queue.a_num);
                  elsif l_charge_result.num_result = pkg_diameter.const_network_error and cur_queue.retry_on_network_error = 1 then --shebeke ile bagli sehv oldusa birazdan yeniden yoxlayacayiq
                      rollback; --to start_req_processing; --rollback work done by first procedure
                      
                      l_sms_tpl_to_send := null;
                      if cur_queue.retry_count > const_max_retry_count then
                        set_item_status(cur_queue.rec_id, const_status_error,
                                      cur_queue.msg_id, cur_queue.app_name, 
                                      l_proc_result.app_num_id, 'Rolled back:'||l_proc_result.str_result,
                                      cur_queue.a_num);
                      else
                        mark_item_for_retry(cur_queue.rec_id, cur_queue.a_num);
                      end if;
                  else --diger bir sehv oldusa statusu error kimi qeyd edirik
                      rollback; --to start_req_processing; --rollback work done by first proc
                                      
                      set_item_status(cur_queue.rec_id, const_status_error,
                                      cur_queue.msg_id, cur_queue.app_name, 
                                      l_proc_result.app_num_id, 'Rolled back:'||l_proc_result.str_result,
                                      cur_queue.a_num);
                      
                      if cur_queue.send_charge_fail_msg = 1 then                                  
                        l_sms_tpl_to_send := get_sms_tpl_for_charge_code(cur_queue.handler_id, l_charge_result.num_result);
                      else
                        l_sms_tpl_to_send := null;
                      end if;
                  end if; 
              end if; 
              
              if l_sms_tpl_to_send is not null then
                pkg_sms_sender.send_sms_template(cur_queue.host_name, cur_queue.b_num, 
                                  cur_queue.a_num, l_sms_tpl_to_send, 
                                  0, l_proc_result.sms_priority, cur_queue.app_name);
              end if;
              
              commit;
              
         end if;
      exception when others then
        --NO ROLLBACKS HERE (we could have already executed procedure and done charging)
        pkg_logger.log_error_auto(substr(SQLERRM||dbms_utility.format_error_backtrace,1,2000), g_app||'.LOOP', cur_queue.a_num); 
        pkg_logger.log_error_auto(DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
      end;
    end loop;
  end do_process_queue;
  
  procedure process_queue(p_queue_id number) is
    l_lock_res number;
    l_dummy number;
  begin
    const_lock_id := const_lock_id + p_queue_id;
    l_lock_res := DBMS_LOCK.REQUEST(const_lock_id, DBMS_LOCK.X_MODE, 1, FALSE); 
    if l_lock_res!=0 then --another copy executing
      return;
    end if;
    
    do_process_queue(p_queue_id);
    
    l_dummy := DBMS_LOCK.RELEASE(const_lock_id);
    
  exception when others then
    pkg_logger.log_error(SQLERRM, g_app); 
    pkg_logger.log_error(DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
    commit;
        
    l_dummy := DBMS_LOCK.RELEASE(const_lock_id);
  end;
  
  function call_proc(p_proc_name varchar2, p_msg_info t_handler_rec_type) return t_proc_result_type is
    l_result number;
    l_anonymous_block varchar2(1500);
    
    l_proc_result t_proc_result_type;
  begin
    l_anonymous_block := '
      declare
        l_rec pkg_request_handler.t_handler_rec_type;
        o_result pkg_request_handler.t_proc_result_type;
      begin
        l_rec.msg_id := :msg_id;
        l_rec.host_name := :host_name;
        l_rec.a_num := :a_num;
        l_rec.b_num := :b_num;
        l_rec.date_added := :date_added;
        l_rec.msg_body := :msg_body;
        l_rec.app_name := :app_name;
        l_rec.num_param1 := :num_param1;
        l_rec.num_param2 := :num_param2;
        l_rec.str_param1 := :str_param1;
        l_rec.str_param2 := :str_param2;
        
        '||p_proc_name||'(l_rec, o_result);
        
        :num_result := o_result.num_result;
        :str_result := o_result.str_result;
        :app_num_id := o_result.app_num_id;
        :sms_template := o_result.sms_template;
        :ok_to_charge := o_result.ok_to_charge;
        :tariff_id := o_result.tariff_id;
        :sms_priority := o_result.sms_priority;
      end;
    ';
    
    pkg_logger.log_debug('calling:'||l_anonymous_block, g_app, p_msg_info.a_num); 
    
    execute immediate l_anonymous_block using in p_msg_info.msg_id,
                                              in p_msg_info.host_name,
                                              in p_msg_info.a_num,
                                              in p_msg_info.b_num,
                                              in p_msg_info.date_added,
                                              in p_msg_info.msg_body,
                                              in p_msg_info.app_name,
                                              in p_msg_info.num_param1,
                                              in p_msg_info.num_param2,
                                              in p_msg_info.str_param1,
                                              in p_msg_info.str_param2,
                                              out l_proc_result.num_result,
                                              out l_proc_result.str_result,
                                              out l_proc_result.app_num_id,
                                              out l_proc_result.sms_template,
                                              out l_proc_result.ok_to_charge,
                                              out l_proc_result.tariff_id,
                                              out l_proc_result.sms_priority;
                                              
    return l_proc_result;
                                              
  exception when others then
    pkg_logger.log_error_auto(substr(SQLERRM||dbms_utility.format_error_backtrace,1,2000), 'PKG_REQUEST_HANDLER.CALL_PROC', p_msg_info.a_num);  
    
    l_proc_result.num_result := c_call_proc_error;
    l_proc_result.str_result := substr(sqlerrm, 1, 500);
    
    return l_proc_result;
  end;
  
  function charge_request(p_msisdn varchar2, 
                          p_tariff_id number, 
                          p_client_id number,
                          p_charge_reason varchar2,
                          p_msg_id number) return t_charge_result_type is
    l_charge_result t_charge_result_type;
  begin
    --we can determine operator id from p_short_number_id
    
    pkg_diameter.charge(p_msisdn, 
                        p_tariff_id, 
                        p_client_id,
                        p_charge_reason,
                        l_charge_result.num_result,
                        l_charge_result.str_result,
                        p_msg_id);
  
    return l_charge_result;
    
  end;
  
  function get_sms_tpl_for_charge_code(p_handler_id number, p_diam_resp_code number) return varchar2 is
    l_result sms_templates.tpl_text%type;
  begin
    select h.sms_tpl_id into l_result from handler_charge_templates h 
    where diam_resp_code=p_diam_resp_code 
    and (handler_id=p_handler_id or handler_id is null) and rownum<2
    order by (case when handler_id is null then 2 else 1 end);
    
    if l_result is not null then
      l_result := l_result || '#';
    end if;
    
    return l_result;
  exception when no_data_found then
    return null;
  end;
  
  procedure mark_item_for_retry(p_rec_id number, p_msisdn varchar2) is 
  begin
    pkg_logger.log_debug('marking item for retry:'||p_rec_id, g_app, p_msisdn); 
    
    update handler_queue set next_retry_date = sysdate + 1/24/60*const_retry_after_minutes,
                             last_retry_date = sysdate,
                             retry_count = retry_count + 1
                             where rec_id=p_rec_id;
  end;
  
  procedure set_item_status(p_rec_id number, p_status varchar2, 
                            p_msg_id number, p_app_name varchar2,
                            p_app_num_id number, p_str_result varchar2,
                            p_msisdn varchar2) is
  begin
    pkg_logger.log_debug('setting item status:'||p_rec_id||','||p_status, g_app, p_msisdn); 
    
    update smpp_incoming s set 
            is_processed = 1, 
            s.app_name = p_app_name, 
            s.app_num_param1=p_app_num_id,
            s.app_str_param1=p_str_result
            where msg_id = p_msg_id;
            
    update handler_queue set status=p_status where rec_id = p_rec_id;
  end;

end pkg_request_handler;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_AUTOSMS" AS

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_is_member number;
      l_is_subs_keyword number;
   begin
   
      o_result.ok_to_charge := 0;
      
      l_is_member := is_member(p_msg_info);
   
      if lower(p_msg_info.msg_body)='info' then
         process_info_request(p_msg_info, l_is_member, o_result);
         return;
      elsif lower(p_msg_info.msg_body) like 'elan%' or lower(p_msg_info.msg_body) like 'avto%' then
         process_ad_request(p_msg_info, l_is_member, o_result);
         return;
      end if;
      
      if l_is_member = 0 then
         
         select case when exists (select 0 from subs_keywords where subs_id = p_msg_info.num_param1 and lower(keyword_text) = lower(p_msg_info.msg_body)) then 1 else 0 end 
         into l_is_subs_keyword from dual;
         
         if l_is_subs_keyword = 1 then
            ozumuz.pkg_rh_subscription.process_request(p_msg_info, o_result);
         end if;
         
         return;
      end if;
      
      if p_msg_info.msg_body = '3' then
         send_price_ranges(p_msg_info, o_result);
      elsif p_msg_info.msg_body in ('20','21','22','23','24') then
         set_price_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '4' then
         send_models(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '5' then
         send_year_ranges(p_msg_info, o_result);
      elsif p_msg_info.msg_body in ('30','31','32','33') then
         set_year_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '6' then
         send_mileage_ranges(p_msg_info, o_result);
      elsif p_msg_info.msg_body in ('40','41','42','43') then
         set_mileage_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body in ('51','52') then
         set_market_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '8' then
         send_announcement_instructions(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '9' then
         clear_preferences(p_msg_info, o_result);
      elsif pkg_util.verify_numeric(p_msg_info.msg_body) then
         declare
            l_code number;
         begin
            l_code := to_number(p_msg_info.msg_body);
            if l_code >= 100 and l_code < 700 then
               set_model_pref(p_msg_info, o_result);
            elsif l_code >=1000 then
               send_detailed_info(p_msg_info, o_result);
            end if;
         end;
      end if;
   end;
   
   function is_member(p_msg_info pkg_request_handler.t_handler_rec_type) return number is
      l_subs_id number := p_msg_info.num_param1;
      l_is_member number;
   begin
      select case when exists (select 0 from subs_customers sc
                              inner join subs_list_variants v on sc.subs_variant_id=v.variant_id
                              where sc.msisdn=p_msg_info.a_num and sc.status='a' and v.subs_id=l_subs_id) then 1 else 0 end into l_is_member from dual;
                              
      return l_is_member;
   end;
   
   procedure process_ad_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      o_result.sms_template:='60#';
   end;
   
   procedure process_info_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      
     if p_is_member = 1 then   
        o_result.sms_template:='53#';
     else
        o_result.sms_template:='52#';
     end if;
   end;
   
   procedure send_price_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sent number;
   begin
      o_result.sms_template := '54#';
   end;  
   
   procedure set_price_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_val_to_set varchar2(50);
      l_set number;
   begin
      select decode(p_msg_info.msg_body, '20', '0:10000', '21', '10000:20000', '22', '20000:30000', '23', '30000:40000', '40000:10000000')
      into l_val_to_set from dual;
      
      l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_PRICE_RANGE', l_val_to_set);
      o_result.sms_template := '60#';
      
      if l_set = 1 then
         send_more_content(p_msg_info);
      end if;
   end;   
   
   procedure send_year_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sent number;
   begin
      o_result.sms_template := '55#';
   end;  
   
   procedure set_year_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_val_to_set varchar2(50);
      l_set number;
   begin
      select decode(p_msg_info.msg_body, '30', '0:2000', '31', '2000:2005', '32', '2006:2011', '2011:2999')
      into l_val_to_set from dual;
      
      l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_YEAR_RANGE', l_val_to_set);
      o_result.sms_template := '60#';
      
      if l_set = 1 then
         send_more_content(p_msg_info);
      end if;
   end;                        
   
   procedure send_mileage_ranges(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sent number;
   begin
      o_result.sms_template := '56#';
   end;  
   
   procedure set_mileage_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_val_to_set varchar2(50);
      l_set number;
   begin
      select decode(p_msg_info.msg_body, '40', '0:30000', '41', '30000:60000', '42', '60000:100000', '100000:10000000')
      into l_val_to_set from dual;
      
      l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_MILEAGE_RANGE', l_val_to_set);
      o_result.sms_template := '60#';
      
      if l_set = 1 then
         send_more_content(p_msg_info);
      end if;
   end; 
   
   procedure set_market_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_set number;
   begin
      if p_msg_info.msg_body = '51' then
         l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_MARKET', '2');
         o_result.sms_template := '64#';
      else
         l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_MARKET', '3');
         o_result.sms_template := '65#';
      end if;
      
      if l_set = 1 then
         send_more_content(p_msg_info);
      end if;
   end;          
   
   procedure send_models(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sms_text varchar2(1000);
      l_append varchar2(1500);
      l_prefix varchar2(500) := 'Hormetli Abunechi, secdiyiniz avtomobil MARKAlarinin kodunu 6090-a gonderin ve gundelik hemen MARKAya aid elanlari elde edin :';
   begin
      for cc in (with t as (
                  select make_id, make_name, null as parent_id from car_makes
                  union all
                  select model_id, model_name, make_id as parent_id from car_models where show_in_pref=1)
                  select make_id, make_name, parent_id,
                  connect_by_root make_id as root_id
                  from t
                  connect by prior make_id = parent_id
                  start with parent_id is null)
      loop
         l_append := chr(10) || cc.make_name || ' (kodu: ' || cc.make_id || ')';
         
         if length(l_prefix || l_sms_text || l_append) > 1000 then
            pkg_sms_sender.send_sms(p_msg_info.host_name, p_msg_info.b_num, p_msg_info.a_num, l_prefix||l_sms_text, 0, 1,  'AUTOSMS');
            l_sms_text := '';
         end if;

         l_sms_text := l_sms_text || chr(10) || cc.make_name || ' (kodu: ' || cc.make_id || ')';
      end loop;
      
      if l_sms_text is not null then
         pkg_sms_sender.send_sms(p_msg_info.host_name, p_msg_info.b_num, p_msg_info.a_num, l_prefix||l_sms_text, 0, 1,  'AUTOSMS');
      end if;
   end;              
                             
   procedure set_model_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_set number;
   begin
      l_set := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'AUTOSMS_MODEL', p_msg_info.msg_body);
      o_result.sms_template := '60#';
      
      if l_set = 1 then
         send_more_content(p_msg_info);
      end if;
   end;                          
                                  
   procedure send_announcement_instructions(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      o_result.sms_template := '62#';
   end;                          
                             
   procedure clear_preferences(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      pkg_rh_subscription.clear_preferences(p_msg_info.a_num, 'AUTOSMS_%');
      o_result.sms_template := '59#';
   end;                          
                                                      
   procedure send_detailed_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
                             
      l_is_charged number;
                                   
      l_code number;
      l_car_id number;
      l_sms_body varchar2(1000);
      l_uniqid varchar2(100);
      
      l_has_files number;
      l_pic_text varchar2(200);
      
      l_tail app_settings.val%type;
   begin
      l_code := to_number(p_msg_info.msg_body);
      
      l_is_charged := pkg_rh_subscription.ensure_charged(p_msg_info.a_num, p_msg_info.num_param1, p_msg_info.msg_id);
      
      if l_is_charged = 0 then
         o_result.sms_template := '66#';
         return;
      end if;
      
      begin
         --dovr yarananda ve bir neche elanin kodu eyni olduqda bu select duz ishlemeyecek
         select car_id, sms_body, uniqid into l_car_id, l_sms_body, l_uniqid from active_cars_view
         where car_id = l_code;
         
         select case when exists (select 0 from cars_files where car_id = l_car_id) then 1 else 0 end into l_has_files from dual;
         
         if l_has_files = 1 then
            l_pic_text := 'Shekil: http://autosms.az/asite/i.php?id='||l_car_id||'_'||l_uniqid;
         end if;
         
         l_sms_body := replace(l_sms_body, '{pic}', l_pic_text);
         
         l_tail := pkg_app_settings.get_value('AUTOSMS_DETAILED_INFO_TAIL');
         if l_tail is not null and length(l_sms_body || ' ' || chr(10) || l_tail) <= pkg_sms_sender.g_max_sms_length then
            l_sms_body := l_sms_body || ' ' || chr(10) || l_tail;
         end if;
         
         update cars set request_count = request_count + 1 where car_id = l_car_id;
         
         o_result.sms_template := '99#[[MSG]]='||l_sms_body;
      exception when no_data_found then
         o_result.sms_template := '43#';
      end;
   end;                          
   
   procedure send_more_content(p_msg_info pkg_request_handler.t_handler_rec_type) is
      l_dummy number;
   begin
      l_dummy := pkg_rh_subscription.send_content(null, p_msg_info.num_param1, p_msg_info.a_num, 1);
   end;
   
   procedure post_subs_handler(p_msisdn varchar2, p_msg_body varchar2, p_host_name varchar2, p_short_number varchar2) is
   begin
      pkg_sms_sender.send_sms_template(p_host_name, p_short_number, 
                              p_msisdn, '63#', 
                              0, 1, 'PKG_RH_AUTOSMS',
                              sysdate + 1);
   end;
END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_CENSORING" as

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type) as
  begin
    o_result.sms_template := '1#';
  end process_request;

end pkg_rh_censoring;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_EVSMS" AS

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_is_member number;
   begin
   
      o_result.ok_to_charge := 0;
      
      l_is_member := is_member(p_msg_info);
   
      if lower(p_msg_info.msg_body)='info' then
         process_info_request(p_msg_info, l_is_member, o_result);
         return;
      elsif lower(p_msg_info.msg_body) like 'elan%' then
         process_ad_request(p_msg_info, l_is_member, o_result);
         return;
      end if;
      
      if l_is_member = 0 then
         return;
      end if;
      
      if p_msg_info.msg_body in ('3','4') then
         set_sell_type_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body in ('5','6') then
         set_building_type_pref(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '7' then
         send_region_list(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '8' then
         send_announcement_instructions(p_msg_info, o_result);
      elsif p_msg_info.msg_body = '9' then
         clear_preferences(p_msg_info, o_result);
      elsif pkg_util.verify_numeric(p_msg_info.msg_body) then
         declare
            l_code number;
         begin
            l_code := to_number(p_msg_info.msg_body);
            if l_code >= 20 and l_code < 100 then
               set_region_preference(p_msg_info, o_result);
            elsif l_code >=100 then
               send_detailed_info(p_msg_info, o_result);
            end if;
         end;
      end if;
   end;
   
   function is_member(p_msg_info pkg_request_handler.t_handler_rec_type) return number is
      l_subs_id number := p_msg_info.num_param1;
      l_is_member number;
   begin
      select case when exists (select 0 from subs_customers sc
                              inner join subs_list_variants v on sc.subs_variant_id=v.variant_id
                              where sc.msisdn=p_msg_info.a_num and sc.status='a' and v.subs_id=l_subs_id) then 1 else 0 end into l_is_member from dual;
                              
      return l_is_member;
   end;
   
   procedure process_ad_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      o_result.sms_template:='99#[[MSG]]=Isteyiniz qebul olundu'; --real template ile evez etmek lazimdir
   end;
   
   procedure process_info_request(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             p_is_member number,
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      
     if p_is_member = 1 then   
        o_result.sms_template:='34#';
     else
        o_result.sms_template:='35#';
     end if;
   end;
   
   procedure set_sell_type_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sent number;
   begin
      if p_msg_info.msg_body = '3' then
         l_sent := pkg_rh_subscription.set_preference(p_msg_info.a_num, 'EVSMS_SELL_TYPE', '1');
         o_result.sms_template := '36#';
      elsif p_msg_info.msg_body = '4' then
         l_sent := pkg_rh_subscription.set_preference(p_msg_info.a_num, 'EVSMS_SELL_TYPE', '2');
         o_result.sms_template := '37#';
      end if;
      
      if l_sent = 1 then
         send_more_content(p_msg_info);
      end if;
   end;                          
                             
   procedure set_building_type_pref(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sent number;
   begin
      if p_msg_info.msg_body = '5' then
         l_sent := pkg_rh_subscription.set_preference(p_msg_info.a_num, 'EVSMS_BUILDING_TYPE', '1');
         o_result.sms_template := '38#';
      elsif p_msg_info.msg_body = '6' then
         l_sent := pkg_rh_subscription.set_preference(p_msg_info.a_num, 'EVSMS_BUILDING_TYPE', '2');
         o_result.sms_template := '39#';
      end if;
      
      if l_sent = 1 then
         send_more_content(p_msg_info);
      end if;
   end;                          
                             
   procedure send_region_list(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_sms_text varchar2(1000);
      l_append varchar2(1500);
      l_prefix varchar2(500) := 'Elan almaq istediyiniz rayonun kodunu 6010-a gondern (pulsuz) :';
   begin
      for cc in (select reg_code, reg_sms_text as reg_name from evsms_regions order by reg_code)
      loop
         l_append := chr(10) || cc.reg_name || ' (kodu: ' || cc.reg_code || ')';
         
         if length(l_prefix || l_sms_text || l_append) > 1000 then
            pkg_sms_sender.send_sms(p_msg_info.host_name, p_msg_info.b_num, p_msg_info.a_num, l_prefix||l_sms_text, 0, 1,  'EVSMS');
            l_sms_text := '';
         end if;

         l_sms_text := l_sms_text || chr(10) || cc.reg_name || ' (kodu: ' || cc.reg_code || ')';
      end loop;
      
      if l_sms_text is not null then
         pkg_sms_sender.send_sms(p_msg_info.host_name, p_msg_info.b_num, p_msg_info.a_num, l_prefix||l_sms_text, 0, 1,  'EVSMS');
      end if;
   end;                          
                             
   procedure send_announcement_instructions(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      o_result.sms_template := '40#';
   end;                          
                             
   procedure clear_preferences(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
   begin
      pkg_rh_subscription.clear_preferences(p_msg_info.a_num, 'EVSMS_%');
      o_result.sms_template := '41#';
   end;                          
                             
   procedure set_region_preference(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
      l_reg_id number; 
      l_sent number;                      
   begin
      begin
         select reg_id into l_reg_id from evsms_regions where reg_code=to_number(p_msg_info.msg_body);
      exception when no_data_found then
         o_result.sms_template := '44#';
         return;
      end;
   
      l_sent := pkg_rh_subscription.add_preference(p_msg_info.a_num, 'EVSMS_REGION', l_reg_id);
      o_result.sms_template := '42#';
      
      if l_sent = 1 then
         send_more_content(p_msg_info);
      end if;
   end;                          
                             
   procedure send_detailed_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
                             o_result in out pkg_request_handler.t_proc_result_type) is
                             
      l_is_charged number;
                                   
      l_code number;
      l_re_id number;
      l_sms_body varchar2(1000);
      l_uniqid varchar2(100);
      
      l_has_files number;
      l_pic_text varchar2(200);
      
      l_tail app_settings.val%type;
   begin
      l_code := to_number(p_msg_info.msg_body);
      
      l_is_charged := pkg_rh_subscription.ensure_charged(p_msg_info.a_num, p_msg_info.num_param1, p_msg_info.msg_id);
      
      if l_is_charged = 0 then
         o_result.sms_template := '45#';
         return;
      end if;
      
      begin
         --dovr yarananda ve bir neche elanin kodu eyni olduqda bu select duz ishlemeyecek
         select re_id, sms_body, uniqid into l_re_id, l_sms_body, l_uniqid from real_estates 
         where re_code = l_code and re_status='active';
         
         select case when exists (select 0 from real_estates_files where real_estate_id = l_re_id) then 1 else 0 end into l_has_files from dual;
         
         if l_has_files = 1 then
            l_pic_text := 'Shekil: http://evsms.az/site/i.php?id='||l_re_id||'_'||l_uniqid;
         end if;
         
         l_sms_body := replace(l_sms_body, '{pic}', l_pic_text);
         
         l_tail := pkg_app_settings.get_value('EVSMS_DETAILED_INFO_TAIL');
         if l_tail is not null and length(l_sms_body || ' ' || chr(10) || l_tail) <= pkg_sms_sender.g_max_sms_length then
            l_sms_body := l_sms_body || ' ' || chr(10) || l_tail;
         end if;
         
         update real_estates set re_request_count = re_request_count + 1 where re_id = l_re_id;
         
         o_result.sms_template := '99#[[MSG]]='||l_sms_body;
      exception when no_data_found then
         o_result.sms_template := '43#';
      end;
   end;                          
   
   procedure send_more_content(p_msg_info pkg_request_handler.t_handler_rec_type) is
      l_dummy number;
   begin
      l_dummy := pkg_rh_subscription.send_content(null, p_msg_info.num_param1, p_msg_info.a_num, 1);
   end;
END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_FAKE_MOZOOK_ALT" as

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type) as
    l_msg_body varchar2(254);
    --l_text varchar2(500);
  begin
    --l_text := trim(lower(p_msg_info.msg_body));
    
    l_msg_body:= 'Hormetli abunechi Siz yemek reseptlerine abune oldunuz! Gundelik 3 qepik odemekle her gun yeni reseptler haqda melumat alacaqsiniz. Sechdiyiniz her resept haqda etrafli melumat da 3 qepik deyerindedir.';
       
    pkg_sms_sender.send_sms(p_msg_info.host_name, p_msg_info.b_num, 
                              p_msg_info.a_num, l_msg_body, 
                              0, 1, 
                              'RH_FAKE');
                              
    o_result.num_result := pkg_request_handler.c_call_proc_success;
  end process_request;

end pkg_rh_fake_mozook_alt;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_HTTP_SENDER" as

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type) as
    l_url varchar2(3000);
    l_operator_id number;
  begin
    l_url := p_msg_info.str_param1;
    
    --linkde butun parametrlerin olmasi vacib deyil. hansilari tapsa onlari replace edecek
    l_url := replace(l_url, '{msisdn}', p_msg_info.a_num);
    l_url := replace(l_url, '{short_number}', p_msg_info.b_num);
    l_url := replace(l_url, '{msg_id}', p_msg_info.msg_id);
    l_url := replace(l_url, '{msg_body}', urlencode(p_msg_info.msg_body));
    
    if instr(l_url, '{operator_id}')>0 then
      select operator_id into l_operator_id from smpp_hosts where host_name=p_msg_info.host_name;
      l_url := replace(l_url, '{operator_id}', l_operator_id);
    end if;
    
    pkg_http_handler.add_http_request(o_result.app_num_id, 
            p_msg_info.msg_id, 
            p_msg_info.a_num,
            p_msg_info.b_num,
            p_msg_info.msg_body,    
            p_msg_info.app_name,
            p_msg_info.host_name,
            l_url,
            nvl(p_msg_info.str_param2, 'POST')
      );
  
    o_result.num_result := pkg_request_handler.c_call_proc_success;
    o_result.str_result := 'Success';
  end;

end pkg_rh_http_sender;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_SMSC" as 
  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type) is
  begin
     insert into smsc_inc_queue (item_id, smpp_incoming_id, smpp_outgoing_id, delivered)
     values (smsc_inc_queue_seq.nextval, p_msg_info.msg_id, null, 0);
     
     update smpp_incoming set system_id = p_msg_info.str_param1 where msg_id = p_msg_info.msg_id;
  end;
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_SUBSCRIPTION" as

   g_app_name varchar2(50) := 'SUBSCRIPTION';

   procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type,
                             o_result in out pkg_request_handler.t_proc_result_type) is
     l_cleaned_msg_body smpp_incoming.msg_body%type := p_msg_info.msg_body;
     
      l_short_number_id number;
      l_sent number;
     -- l_host_name number; 
      l_results varchar2(2000);
   begin
   
      o_result.ok_to_charge := 0;
      o_result.num_result := pkg_request_handler.c_call_proc_success;
       
      /*
        we can get following types of requests:
         1 - subscribe. in this case request text will contain keyword from subs_keywords table
         2 - unsubscribe. in this case request will contain text stop if user has only single subscription
             or stop [SUBS_NAME] or stop all if user has several subscriptions
             
      */
      
      --first clean up msg body
      l_cleaned_msg_body := lower(trim(translate(l_cleaned_msg_body, '.,:$%^&()!', ' ')));
      --replace multiple spaces inside string with single space
      l_cleaned_msg_body := REGEXP_REPLACE(l_cleaned_msg_body, '( ){2,}', ' ');
       
      --now we are ready to analyze msg body
      begin
      
        if l_cleaned_msg_body = '0' or l_cleaned_msg_body like '%stop%' then
            process_unsubscribe_request(l_cleaned_msg_body, p_msg_info, o_result);
        --elsif l_cleaned_msg_body like '%info%' then
        --    send_info(p_msg_info, o_result);
        elsif lower(trim(p_msg_info.msg_body)) in ('2','yene', 'daha') then
           l_short_number_id := pkg_short_number.get_number_id(p_msg_info.b_num, p_msg_info.host_name);
             
           for cc in (select  s.subs_id, sl.nocontent_tpl_id from subs_list s 
                      inner join subs_list_variants sl on sl.subs_id= s.subs_id
                      where sl.short_number_id=l_short_number_id  and sl.status='a')
           loop
              l_sent := send_content(null, cc.subs_id, p_msg_info.a_num, 1);
           end loop;
           
           if l_sent = 1 then
              o_result.ok_to_charge := 1;
           end if;
           
        else
            check_for_subscribe_request(l_cleaned_msg_body, p_msg_info, o_result);
        end if;
      exception when others then
         --rollback;
         o_result.ok_to_charge := 0;
         o_result.num_result := pkg_request_handler.c_call_proc_error;
         --o_result.str_result := substr(sqlerrm||DBMS_UTILITY.FORMAT_ERROR_BACKTRACE,1,300);
         pkg_logger.log_error_auto(substr(sqlerrm||DBMS_UTILITY.FORMAT_ERROR_BACKTRACE,1,500), g_app_name, p_msg_info.a_num);
         commit;
      end;
   end;
   
   function get_subs_keywords_as_table(p_short_number_id number) return t_keywords_tbl is
      l_res t_keywords_tbl;
      
      function create_rec(p_keyword_text varchar2, p_subs_id number, p_variant_id number, p_agent_id number) return t_keyword_rec is
         l_rec t_keyword_rec;
        l_short_number_id number; 
      begin
          
         l_rec := t_keyword_rec(p_keyword_text, p_subs_id, p_variant_id, p_agent_id);
         
         return l_rec;
          
      end;
      
   begin
      
      l_res := t_keywords_tbl();
      
      for cc1 in (select keyword_text, subs_keywords.subs_id, agent_id from subs_keywords
        -- where subs_keywords.subs_id=(select subs_id from subs_list where short_number=p_short_number and status='a'))
       inner join subs_list on subs_keywords.subs_id=subs_list.subs_id where subs_list.status='a')
                  --       and subs_list.short_number=p_short_number)
      
      loop
         for cc2 in (select variant_id, day_count, is_default from subs_list_variants
                     where subs_id=cc1.subs_id and status='a' and short_number_id=p_short_number_id)
         loop
          
            l_res.extend;
            l_res(l_res.count) := create_rec(cc1.keyword_text || to_char(cc2.day_count), cc1.subs_id, cc2.variant_id, cc1.agent_id);
            
            l_res.extend;
            l_res(l_res.count) := create_rec(cc1.keyword_text || ' ' || to_char(cc2.day_count), cc1.subs_id, cc2.variant_id, cc1.agent_id);
            
            if cc2.is_default = 1 then
               l_res.extend;
               l_res(l_res.count) := create_rec(cc1.keyword_text, cc1.subs_id, cc2.variant_id, cc1.agent_id);
            end if;
         end loop;
      end loop;              
      
      return l_res;
   end;
     
   procedure check_for_subscribe_request(p_cleaned_msg_body smpp_incoming.msg_body%type,
                                         p_msg_info pkg_request_handler.t_handler_rec_type, 
                                         o_result in out pkg_request_handler.t_proc_result_type) is
      l_keywords_tbl t_keywords_tbl;
      l_short_number_id number;
      l_already_subscribed number;
      l_requested_variant_id number;
      l_subs_id number;
      l_day_count number;
      l_day_count_text varchar2(50);
      l_tariff_id number;
      l_trial_period number;
      l_ever_subscribed number;
      l_subs_name subs_list.subs_name%type;
      l_sql_rowcount number;
      l_next_charge_date date;
      l_tpl_to_send varchar2(2000);
      l_agent_id number; 
      l_dummy number;
      l_welcome_message subs_list.welcome_message%type;
      l_welcome_message2 subs_list.welcome_message2%type;
      l_post_subs_handler subs_list.post_subs_handler%type;
      l_already_member_message subs_list.already_member_message%type;
   begin
      l_short_number_id := pkg_short_number.get_number_id(p_msg_info.b_num, p_msg_info.host_name);
      l_keywords_tbl := get_subs_keywords_as_table(l_short_number_id);
      
      begin
      
         select t1.subs_id, t1.variant_id, t1.agent_id into l_subs_id, l_requested_variant_id, l_agent_id
         from table(l_keywords_tbl) t1
         inner join subs_list_variants v on t1.variant_id=v.variant_id
         where v.short_number_id=l_short_number_id
         and lower(t1.keyword)=p_cleaned_msg_body
         and rownum<=1; 
      exception when no_data_found then
         l_requested_variant_id := null;
      end;
            
      if l_requested_variant_id is null then
         o_result.sms_template := '10#'; 
         return;
      end if;

      select welcome_message, welcome_message2, already_member_message, post_subs_handler into
             l_welcome_message, l_welcome_message2, l_already_member_message, l_post_subs_handler
      from subs_list where subs_id = l_subs_id;
      
      --check if already subscribed
      select case when exists 
                (select 0 from SUBS_CUSTOMERS 
                 where subs_variant_id=l_requested_variant_id 
                 and msisdn=p_msg_info.a_num and status='a') then 1
                 else 0
              end
      into l_already_subscribed from dual;
      
      if l_already_subscribed = 1 then
         if l_already_member_message is null then
            o_result.sms_template := '11#';
         else
            o_result.sms_template := l_already_member_message;
         end if;
         
         return;
      end if;
      
      --check if already subscribed to another variant of this subscription
      update subs_customers set subs_variant_id = l_requested_variant_id
      where msisdn=p_msg_info.a_num 
      and subs_variant_id in (select variant_id from subs_list_variants 
                              where subs_id=l_subs_id and status='a')
      and status='a';
      
      l_sql_rowcount := sql%rowcount;
      
      select v.day_count, v.trial_period, decode(s.tariff_id, null, v.tariff_id, s.tariff_id), s.subs_name 
      into l_day_count, l_trial_period, l_tariff_id, l_subs_name
      from subs_list_variants v 
      inner join subs_list s on v.subs_id=s.subs_id
      left join segment_numbers n on (n.msisdn=p_msg_info.a_num)
      left join subs_list_vars_for_segments s on (v.variant_id=s.variant_id and s.segment_id=n.segment_id)
      where v.variant_id=l_requested_variant_id;
                              
      if l_sql_rowcount > 0 then --sucessfully switched to another variant of same subscription
         select day_count into l_day_count from subs_list_variants where variant_id=l_requested_variant_id;
         o_result.sms_template := '12#[[day_count]]='||l_day_count;
         return;
      end if;
      
      --check if ever subscribed to some variant of this subscription (both active and inactive)
      select case when exists (select 0 from subs_customers where msisdn=p_msg_info.a_num 
                              and subs_variant_id in (select variant_id from subs_list_variants 
                              where subs_id=l_subs_id)) then 1 else 0 end 
             into l_ever_subscribed from dual;
             
      if l_ever_subscribed = 1 then
         l_trial_period := 0;
      end if;
      
      if l_trial_period > 0 then
         l_next_charge_date := sysdate+l_trial_period;
      else
         --l_next_charge_date := sysdate+l_day_count;
         l_next_charge_date := sysdate;
      end if;
      
         
      insert into subs_customers (rec_id, subs_variant_id, subs_date, last_charge_date, 
                                  next_charge_date, msisdn, host_name, agent_id)
      values (subs_customers_seq.nextval, l_requested_variant_id, sysdate, sysdate, 
                                  l_next_charge_date, p_msg_info.a_num, p_msg_info.host_name, l_agent_id);
                                  
      
      if l_day_count = 1 then
         l_day_count_text := 'gundelik';
      else
         l_day_count_text := l_day_count || ' gunluk';
      end if;
      
      o_result.sms_template := null;
      if l_trial_period = 0 then
         l_tpl_to_send := l_welcome_message;
      
         --if ok to charge = 1 then next-charge-date block above also must be modified
         --o_result.ok_to_charge := 1;
         o_result.ok_to_charge := 0;
         o_result.tariff_id := l_tariff_id;
      else
         l_tpl_to_send := l_welcome_message;
      
         o_result.ok_to_charge := 0;
      end if;
      
      if l_tpl_to_send is not null then
        pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num, 
                              p_msg_info.a_num, l_tpl_to_send, 
                              1, 1, 'SUBSCRIPTION');
                             
        l_dummy := send_content(null, l_subs_id, p_msg_info.a_num, 1);
        
        if l_welcome_message2 is not null then
           pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num, 
                              p_msg_info.a_num, l_welcome_message2, 
                              1, 1, 'SUBSCRIPTION', sysdate + 1/24/60); --send one minute later
        end if;
      end if;
      
      if l_post_subs_handler is not null then
         begin
            execute immediate l_post_subs_handler using in p_msg_info.a_num, in p_msg_info.msg_body, p_msg_info.host_name, p_msg_info.b_num;
         exception when others then
            pkg_logger.log_error_auto('Post subs handler error: '||substr(sqlerrm||DBMS_UTILITY.FORMAT_ERROR_BACKTRACE,1,500), g_app_name, p_msg_info.a_num);
         end;
      end if;
   end;
   
   procedure process_unsubscribe_request(p_cleaned_msg_body smpp_incoming.msg_body%type,
                                         p_msg_info pkg_request_handler.t_handler_rec_type,
                                         o_result in out pkg_request_handler.t_proc_result_type) is
      l_subs_name varchar2(200);
      l_subs_count number;
      l_variant_id number;
      l_short_number_id number;
   begin
      select count(0) into l_subs_count from subs_customers where msisdn=p_msg_info.a_num and status='a';
      
      if l_subs_count = 0 then --does not have any active subscription
         o_result.sms_template := '14#';
         return;
      end if;
      
      if l_subs_count >= 1 then --there is only single active subscription
         unsubscribe_all(p_msg_info);
         return;
      end if;
      /*
      --l_subs_count > 1
      l_subs_name := lower(trim(substr(p_cleaned_msg_body, length('stop')+1)));
      
      if l_subs_name is null or length(l_subs_name)=0 then
         o_result.sms_template := '21#';
         return;
      end if;
      
      if l_subs_name = 'all' then
         unsubscribe_all(p_msg_info);
         return;
      end if;
      
      --user wants to unsubscribe from subscription name specified in l_subs_name
      l_short_number_id := pkg_short_number.get_number_id(p_msg_info.b_num, p_msg_info.host_name);
      begin
         select t1.variant_id into l_variant_id
         from table(pkg_rh_subscription.get_subs_keywords_as_table(l_short_number_id)) t1
         inner join subs_list_variants v on t1.variant_id=v.variant_id and v.short_number_id=l_short_number_id
         --inner join diameter_tariffs dt on v.tariff_id=dt.tariff_id
        -- where dt.operator_id=l_operator_id
         and lower(t1.keyword)=l_subs_name
         and t1.variant_id in (select subs_variant_id from subs_customers where msisdn=p_msg_info.a_num and status='a')
         and rownum<=1;
         
         unsubscribe(p_msg_info, l_variant_id);
      exception when no_data_found then
         o_result.sms_template := '15#[[subs_name]]='||l_subs_name;
      end;*/
   end;
   
   procedure unsubscribe_all(p_msg_info pkg_request_handler.t_handler_rec_type) is
      l_short_number_id number;
   begin
      l_short_number_id := pkg_short_number.get_number_id(p_msg_info.b_num, p_msg_info.host_name);
      for cc in (select subs_variant_id from subs_customers sc inner join subs_list_variants v
                 on  sc.subs_variant_id = v.variant_id
                 where msisdn=p_msg_info.a_num and sc.status='a' and v.status='a'
                 and v.short_number_id=l_short_number_id)
      loop
         unsubscribe(p_msg_info, cc.subs_variant_id);
      end loop;
   end;
   
   procedure unsubscribe(p_msg_info pkg_request_handler.t_handler_rec_type, p_variant_id number) is
      l_subs_name varchar2(200);
      
      l_goodbye_message subs_list.goodbye_message%type;
   begin
      update subs_customers set status='d', unsubs_date=sysdate where msisdn=p_msg_info.a_num and subs_variant_id=p_variant_id;
--      select s.subs_name into l_subs_name from subs_list_variants v
--      inner join subs_list s on v.subs_id=v.subs_id where v.variant_id=p_variant_id;
      select subs_name, goodbye_message into l_subs_name, l_goodbye_message from subs_list where subs_id=(select subs_id from subs_list_variants where variant_id=p_variant_id);
      
      if l_goodbye_message is null then
         pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num,
                              p_msg_info.a_num, '16#[[subs_name]]='||l_subs_name,
                              0, 1, 'SUBSCRIPTION');
      else
         pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num,
                              p_msg_info.a_num, l_goodbye_message,
                              0, 1, 'SUBSCRIPTION');
      end if;
   end;
   
   /*
    procedure send_info(p_msg_info pkg_request_handler.t_handler_rec_type, 
                       o_result in out pkg_request_handler.t_proc_result_type) is
     l_msg_body varchar2(500);
     l_variant_list varchar2(500);
     l_short_number_id number;
   begin
     l_short_number_id := pkg_short_number.get_number_id(p_msg_info.b_num, p_msg_info.host_name);
                
     for cc in (select subs_id, subs_name, subs_desc from subs_list  where 
                subs_id in(select subs_id from subs_list_variants where short_number_id=l_short_number_id
                 and status='a' 
                ))
                            
     loop
        l_msg_body := '17#[[subs_name]]='||cc.subs_name||'[[subs_desc]]='||cc.subs_desc;
        l_variant_list := '';
        for cc2 in (select v.day_count, t.tariff_price from subs_list_variants v 
                    inner join
                    diameter_tariffs t on v.tariff_id=t.tariff_id where v.short_number_id=l_short_number_id and v.subs_id=cc.subs_id and v.status='a')
        loop
           l_variant_list := l_variant_list || chr(13) || chr(10) ||
           'Gun sayi: '||cc2.day_count||', Qiymet: '||to_char (cc2.tariff_price,'0.99')||' AZN, Qoshulmaq uchun: '||cc.subs_name||cc2.day_count||' yazib gonderin';
        end loop;
       
        l_msg_body := l_msg_body || '[[variants]]='||l_variant_list;
        
        pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num, 
                              p_msg_info.a_num, l_msg_body, 
                              0, 1, 'SUBSCRIPTION');
     end loop;
    
     l_msg_body := '';
     for cc in (select v.day_count, s.subs_name, c.next_charge_date, s.info_message 
                from subs_customers c 
                inner join subs_list_variants v on c.subs_variant_id=v.variant_id
                inner join subs_list s on v.subs_id=s.subs_id
                where msisdn=p_msg_info.a_num and c.status='a' and v.status='a' and s.status='a')
     loop
        if cc.info_message is null then
           l_msg_body := l_msg_body ||chr(13)||chr(10)||'Movzu: '||cc.subs_name||', Gun sayi: '||cc.day_count||
                      ', Bitme tarixi: '||to_char(cc.next_charge_date,'yyyy-mm-dd');
        else
           l_msg_body := l_msg_body ||chr(13)||chr(10)||cc.info_message;
        end if;
     end loop;
     
     if l_msg_body is not null and length(l_msg_body)>0 then
        pkg_sms_sender.send_sms_template(p_msg_info.host_name, p_msg_info.b_num, 
                              p_msg_info.a_num, '19#[[list]]='||l_msg_body, 
                              0, 1, 'SUBSCRIPTION');
                              
     end if;
   end;   */
   
   function get_next_content(p_subs_id number, p_msisdn varchar2, p_alg_id number, o_content_id out varchar2) return varchar2 is
      l_result varchar2(1000);
      l_custom_selector subs_list.custom_selector%type;
      l_max_days_back number;
   begin
   
      if p_alg_id = 1 then
          select content_id, content_body into o_content_id, l_result from (
            select cnt.content_id, b.content_body from subs_customers c
            inner join subs_list_variants v on (c.subs_variant_id=v.variant_id and v.status='a')
            inner join subs_list l on (v.subs_id=l.subs_id and l.status='a')
            inner join subs_list_content lcnt on (lcnt.subs_id=v.subs_id)
            inner join subs_content cnt on (lcnt.content_id=cnt.content_id)
            inner join subs_content_body b on (lcnt.content_id=b.content_id and b.lang_id=1)
            where v.subs_id=p_subs_id and c.msisdn=p_msisdn and c.status='a'
            and sysdate-cnt.date_added<=l.selection_max_days_back
            and not exists (select 0 from subs_sent_content where content_id=to_char(b.content_id) and msisdn=c.msisdn)
            order by cnt.content_id) t where rownum<=1;
      elsif p_alg_id = 2 then 
          select content_id, content_body into o_content_id, l_result from (
            select cnt.content_id, b.content_body from subs_customers c
            inner join subs_list_variants v on (c.subs_variant_id=v.variant_id and v.status='a')
            inner join subs_list l on (v.subs_id=l.subs_id and l.status='a')
            inner join subs_list_content lcnt on (lcnt.subs_id=v.subs_id)
            inner join subs_content cnt on (lcnt.content_id=cnt.content_id)
            inner join subs_content_body b on (lcnt.content_id=b.content_id and b.lang_id=1)
            where v.subs_id=p_subs_id and c.msisdn=p_msisdn and c.status='a'
            and sysdate-cnt.date_added<=l.selection_max_days_back
            and not exists (select 0 from subs_sent_content where content_id=to_char(b.content_id) and msisdn=c.msisdn)
            order by dbms_random.value) t where rownum<=1;
      else --custom selector
          select custom_selector, selection_max_days_back into l_custom_selector, l_max_days_back
          from subs_list where subs_id = p_subs_id;
          
          execute immediate l_custom_selector using in p_subs_id, in p_msisdn, in l_max_days_back, out l_result, out o_content_id;
      end if;
      
      return l_result;
   exception when no_data_found then     
      return null;
   end;  
   
   function send_content(p_sch_id number, p_subs_id number := null, p_msisdn varchar2 := null, p_priority number := 0) return number is 
      l_out_of_content number := 0;
      l_text_to_send varchar2(2000);
      l_content_to_send varchar2(2000); 
      l_content_id varchar2(150);
      
      
   begin
         for cc in (select c.msisdn, c.host_name, v.tpl_id, l.selection_algorithm_id, v.subs_id, sn.short_number
                     from subs_list_variants v
                     inner join subs_customers c on v.variant_id=c.subs_variant_id
                     inner join subs_list l on v.subs_id=l.subs_id
                     inner join short_numbers sn on v.short_number_id=sn.number_id
                     where v.status='a' and c.status='a' and l.status='a'  and
                     (
                        (p_sch_id is null or v.sch_id=p_sch_id)
                        and
                        ((p_subs_id is null and p_msisdn is null) or (v.subs_id=p_subs_id and c.msisdn=p_msisdn))
                     )
                   )
      loop
      
         l_content_to_send := get_next_content(cc.subs_id, cc.msisdn, cc.selection_algorithm_id, l_content_id);
         
         if l_content_to_send is null then
            l_out_of_content := 1;
         else
            if cc.tpl_id is null then
               l_text_to_send := l_content_to_send;
            else
               l_text_to_send := to_char(cc.tpl_id)||'#[[MSG]]='||l_content_to_send;               
            end if;
             
            if l_content_id is not null then
               insert into subs_sent_content (content_id, msisdn, sent_at)
               values (l_content_id, cc.msisdn, sysdate);
            end if;
            
            if cc.tpl_id is null then
               pkg_sms_sender.send_sms(cc.host_name, cc.short_number, cc.msisdn, l_text_to_send, 0, p_priority, 'SUBSCRIPTION');
            else
               pkg_sms_sender.send_sms_template(cc.host_name, cc.short_number, cc.msisdn, l_text_to_send, 0, p_priority, 'SUBSCRIPTION');
            end if;
            
         end if;
      end loop;
      
      if l_out_of_content = 1 then
         /*send_email('alarm@nms.az',
                    'rbaghirov@nms.az;aslan@nms.az;anar@nms.az',
                    'out of content',
                    'we are out of content on some subscriptions');*/
         --pkg_sms_sender.send_sms('BAKCELL1', '6010', '994556744159', 'Out of content', 0, 1, 'SUBSCRIPTION');
         pkg_sms_sender.send_sms('BAKCELL1', '6010', '994553241802', 'Out of content', 0, 1, 'SUBSCRIPTION');
         null;
      end if;
       
      return abs(l_out_of_content-1);
   end;
   
   function set_preference(p_msisdn varchar2, p_pref_name varchar2, p_pref_val varchar2) return number is
      l_curr_val subs_preferences.pref_val%type;
   begin
      l_curr_val := get_preference(p_msisdn, p_pref_name, null);
      
      if p_pref_val = l_curr_val then
         return 0;
      end if;
   
      update subs_preferences set pref_val = p_pref_val where msisdn = p_msisdn and pref_name = p_pref_name;
      if sql%rowcount = 0 then
         return add_preference(p_msisdn, p_pref_name, p_pref_val);
      end if;
      
      return 1;
   end;
   
   function add_preference(p_msisdn varchar2, p_pref_name varchar2, p_pref_val varchar2) return number is
      l_pref_exists number;
   begin
      select case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name = p_pref_name and pref_val = p_pref_val) then 1 else 0 end into l_pref_exists from dual;
   
      if l_pref_exists = 0 then
         insert into subs_preferences (msisdn, pref_name, pref_val)
         values (p_msisdn, p_pref_name, p_pref_val);
         
         return 1;
      else
         return 0;
      end if;
   end;
   
   function get_preference(p_msisdn varchar2, p_pref_name varchar2, p_default_val varchar2) return varchar2 is
      l_result subs_preferences.pref_val%type;
   begin
      select pref_val into l_result from subs_preferences where msisdn = p_msisdn and pref_name = p_pref_name;
      
      return l_result;
   exception when no_data_found then
      return p_default_val;
   end;
   
   function has_preference(p_msisdn varchar2, p_pref_name varchar2) return number is
      l_result number;
   begin
      select case when exists (select 0 from subs_preferences where msisdn = p_msisdn and pref_name = p_pref_name) then 1 else 0 end into l_result from dual;
      
      return l_result;
   end;
   
   procedure clear_preferences(p_msisdn varchar2, p_pref_name_like varchar2) is
   begin
      delete from subs_preferences where msisdn = p_msisdn and pref_name like p_pref_name_like;
   end;
   
   function is_charged(p_msisdn varchar2, p_subs_id number) return number is
      l_result number;
   begin
      select case when exists (select 0 from active_subs_customers_view where msisdn = p_msisdn and subs_id = p_subs_id and next_charge_date > sysdate) then 1 else 0 end into l_result from dual;
      
      return l_result;
   end;
   
   function ensure_charged(p_msisdn varchar2, p_subs_id number, p_msg_id number) return number is
      l_result number;
   begin
      l_result := is_charged(p_msisdn, p_subs_id);
      
      if l_result = 1 then
         return l_result;
      end if;
      
      --try to charge
      charge_customers(null, p_msisdn, p_subs_id, p_msg_id);
      
      l_result := is_charged(p_msisdn, p_subs_id);
      
      return l_result;
   end;

   procedure charge_customers(p_operator_id number, p_msisdn varchar2, p_subs_id number, p_msg_id number) is --either operator id or (msisdn and subs id and msg_id) must be provided
      l_text_to_send varchar2(200);
      l_result number;
      l_text_result varchar2(500);
   begin
      for cur_customers in (select v.REC_ID, v.MSISDN, HOST_NAME, SHORT_NUMBER,  decode(s.tariff_id, null, v.tariff_id, s.tariff_id) as TARIFF_ID, DAY_COUNT, SUBS_NAME, SUBS_ID, v.VARIANT_ID, 
                            OPERATOR_ID, LAST_CHARGE_DATE, NEXT_CHARGE_DATE from active_subs_customers_view v
                            left join segment_numbers n on (n.msisdn=v.msisdn)
                            left join subs_list_vars_for_segments s on (v.variant_id=s.variant_id and s.segment_id=n.segment_id)
                            where next_charge_date <= trunc(sysdate+1)
                            and
                            (
                              (p_operator_id is null or v.operator_id=p_operator_id)
                              and
                              (p_msisdn is null or (v.msisdn = p_msisdn and v.subs_id = p_subs_id))
                            ))
      loop
         pkg_diameter.charge(cur_customers.msisdn,
                            cur_customers.tariff_id,
                            1,
                            'SMS_'||cur_customers.short_number||'_'||cur_customers.subs_name||'_EMobile',
                            l_result,
                            l_text_result,
                            p_msg_id);
      
         if l_result = pkg_diameter.const_success then
            update subs_customers set last_charge_date = sysdate, next_charge_date = sysdate + cur_customers.day_count where rec_id=cur_customers.rec_id;
            commit;
         end if;
      end loop;
   end;
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_RH_TEST" as

  procedure process_request(p_msg_info pkg_request_handler.t_handler_rec_type, o_result in out pkg_request_handler.t_proc_result_type) as
  begin
    o_result.num_result := pkg_request_handler.c_call_proc_success;
    o_result.str_result := 'Success';
    o_result.app_num_id := 1;
    o_result.sms_template := '1#[[PARAM1]]=Ali[[PARAM2]]=100';
    o_result.ok_to_charge := 1;
  end process_request; 
  
  /*procedure emulate_new_sms is
  begin
    pkg_smpp.register_received_sms('BAKCELL1', sysdate, 1, '994552297703', '8171', 'Salam', dbms_random.string('X', 20));
    pkg_smpp.register_received_sms('BAKCELL1', sysdate, 1, '994552297707', '8171', 'Salam', dbms_random.string('X', 20));
    pkg_request_handler.process_queue(1);
  end;*/

end pkg_rh_test;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SEGMENT" AS

   function get_segment_id(p_msisdn varchar2) return number is
      l_segment_id number;
   begin
      select segment_id into l_segment_id from segment_numbers where msisdn = p_msisdn;
      
      return l_segment_id;
   exception when no_data_found then
      return null;
   end;
   
   procedure add_to_segment(p_msisdn varchar2, p_segment_id number) is
   begin
      insert into segment_numbers (rec_id, segment_id, msisdn)
      values (segment_numbers_seq.nextval, p_segment_id, p_msisdn);
   end;

END;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SHORT_NUMBER" AS

  procedure get_all_short_numbers(v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for
    select rownum row_number, number_id,short_number, short_number_name, incoming_price, round(incoming_price*1.18,6) incoming_price_with_vat,
    outgoing_price 
    from short_numbers_view
    where is_active_now=1
     order by number_id;
  END get_all_short_numbers;  
  
  procedure get_all_short_numbers(p_group_id in number, v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for        
    select g.group_name, rownum number_id, sn.short_number, 
      round(incoming_price,2) incoming_price, round(incoming_price*1.18,2) incoming_price_with_vat, 
      round(outgoing_price,2) outgoing_price, round(outgoing_price*1.18,2) outgoing_price_with_vat
    from short_number_group g join short_number_group_numbers gn on g.id=gn.group_id and g.id=p_group_id
      join short_numbers sn on gn.short_number_id=sn.number_id    
    order by sn.number_id;
  END get_all_short_numbers;
  
  procedure get_bulk_sms_numbers(p_login_name in varchar2, v_items_cursor out t_cursor) AS
  BEGIN
    /*open v_items_cursor for
    select * from (
      select rownum row_number, number_id, short_number_name, 'short_number' number_type 
      from short_numbers_view
      where is_active_now=1 and host_name='BAKCELL2'
      union
      select rownum row_number, c.id, name, 'custom_sender_name' number_type 
      from custom_sender_name c, managers m
      where c.status=2 and c.manager_id=m.man_id and m.login=p_login_name
      union
      select rownum row_number, number_id, short_number_name, 'short_number' number_type 
      from short_numbers_view
      where is_active_now=1 and host_name='BAKCELL1' and p_login_name in (
        select login from managers where man_email like '%@nvgr.az')
      )
    order by short_number_name;*/
    null;
  END get_bulk_sms_numbers; 
  
  procedure get_mt_numbers(v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for        
    select rownum id, bc.name,       
      round(bc.price,2) price, round(bc.price_with_vat, 2) price_with_vat
    from billing_category bc 
    order by bc.id;
  END get_mt_numbers;
  
  procedure set_is_active_now(v_short_number_id number, v_is_active_now number) is
  begin
    update short_numbers set is_active_now=v_is_active_now where number_id=v_short_number_id;
    commit;
  end;
  
  function get_number_id(p_short_number varchar2, p_host_name varchar2) return number is
    l_result number;
  begin
    select number_id into l_result from short_numbers sn
    inner join smpp_hosts sh on sn.host_id=sh.host_id
    where short_number=p_short_number and host_name=p_host_name;
    
    return l_result;
  end;

  procedure get_is_active_now(v_short_number_id number, v_is_active_now out number) is
  begin
    select is_active_now into v_is_active_now from short_numbers where number_id=v_short_number_id;
  end;
END PKG_SHORT_NUMBER;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMPP" 
AS
   g_app   VARCHAR2 (100) := 'PKG_SMPP';

   PROCEDURE register_received_sms (v_host_name           VARCHAR2,
                                    v_in_date             DATE,
                                    v_esm_class           NUMBER,
                                    v_source_addr         VARCHAR2,
                                    v_destination_addr    VARCHAR2,
                                    v_msg_body            NVARCHAR2,
                                    v_file_name           VARCHAR2,
                                    o_msg_id    out       number)
   IS
      l_file_already_processed   NUMBER := 0;
      l_new_msg_id               NUMBER;
      --mins                       NUMBER := 30;
      --qty                        NUMBER := 30;
      --tmp_qty                    NUMBER;
   BEGIN
      o_msg_id := -1;
      pkg_logger.
       log_debug (
            'SMS received from '
         || v_source_addr
         || ' to '
         || v_destination_addr,
         g_app,
         v_source_addr);

     /* Formatted on 12/9/2010 12:18:01 AM (QP5 v5.149.1003.31008) */
SELECT CASE
          WHEN EXISTS
                  (SELECT msg_id
                     FROM smpp_incoming
                    WHERE file_name = v_file_name) THEN 1
          WHEN EXISTS
                  (SELECT msisdn
                     FROM black_list
                    WHERE msisdn = v_source_addr) THEN 2
          --WHEN EXISTS 
          --        (select 0 from smpp_incoming where in_date>sysdate-1 --scan last one day
          --        and host_name=v_host_name and source_addr=v_source_addr and destination_addr=v_destination_addr
          --        and msg_body=v_msg_body and dup_last_susp_date>sysdate-1/24/60*5) THEN 3 --suspected to be a duplicate
          ELSE 0
       END
  INTO l_file_already_processed
  FROM DUAL;

      --tmp_qty := 0;



      IF l_file_already_processed in (0, 2)
      THEN
         SELECT smpp_incoming_seq.NEXTVAL INTO l_new_msg_id FROM DUAL;
         
         o_msg_id := l_new_msg_id;

         INSERT INTO smpp_incoming (msg_id,
                                    host_name,
                                    in_date,
                                    esm_class,
                                    source_addr,
                                    destination_addr,
                                    msg_body,
                                    is_processed,
                                    file_name)
              VALUES (l_new_msg_id,
                      v_host_name,
                      v_in_date,
                      v_esm_class,
                      v_source_addr,
                      v_destination_addr,
                      v_msg_body,
                      l_file_already_processed,
                      v_file_name);

         COMMIT;
         --pkg_logger.log_debug ('Inserted into smpp_incoming table', g_app,v_source_addr);

         post_process_received_sms (l_new_msg_id,
                                       v_host_name,
                                       v_in_date,
                                       v_esm_class,
                                       v_source_addr,
                                       v_destination_addr,
                                       v_msg_body,
                                       v_file_name);
     ELSIF l_file_already_processed = 1 THEN
        SELECT msg_id INTO o_msg_id
                     FROM smpp_incoming
                    WHERE file_name = v_file_name;
     END IF;
   END;

   PROCEDURE post_process_received_sms (v_msg_id              NUMBER,
                                        v_host_name           VARCHAR2,
                                        v_in_date             DATE,
                                        v_esm_class           NUMBER,
                                        v_source_addr         VARCHAR2,
                                        v_destination_addr    VARCHAR2,
                                        v_msg_body            NVARCHAR2,
                                        v_file_name           VARCHAR2)
   IS
   BEGIN
      --pkg_logger.log_debug ('before processing', g_app);
      pkg_smpp_post_processing.process_received_sms (v_msg_id,
                                                     v_host_name,
                                                     v_in_date,
                                                     v_esm_class,
                                                     v_source_addr,
                                                     v_destination_addr,
                                                     v_msg_body,
                                                     v_file_name);
   EXCEPTION
      WHEN OTHERS
      THEN
         pkg_logger.log_error (SQLERRM, g_app);
         pkg_logger.log_error (dbms_utility.format_error_stack, g_app);
         pkg_logger.log_error (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
   END;

   PROCEDURE get_pending_messages_to_send (v_host_name          VARCHAR2,
                                           v_items_cursor   OUT t_cursor)
   IS
   BEGIN
      OPEN v_items_cursor FOR
         SELECT *
           FROM (  SELECT msg_id,
                          source_addr,
                          destination_addr,
                          msg_body,
                          delivery_report,
                          app_name,
                          app_str_param1,
                          app_str_param2,
                          validity_period
                     FROM smpp_outgoing
                    WHERE     status = 0
                          AND (SYSDATE BETWEEN send_at AND send_until)
                          AND host_name = v_host_name
                 ORDER BY priority DESC, msg_id) t
          WHERE ROWNUM <= 250;
   END;

   PROCEDURE mark_message_as_processed (v_msg_id NUMBER, v_smsc_id VARCHAR2)
   IS
   BEGIN
      UPDATE smpp_outgoing
         SET status = 1, smsc_id = v_smsc_id, sent_at = SYSDATE
       WHERE msg_id = v_msg_id;

      COMMIT;
   END;

   PROCEDURE mark_message_as_in_error (v_msg_id NUMBER, v_err_code VARCHAR2)
   IS
   BEGIN
      UPDATE smpp_outgoing
         SET status = 2, err_code = v_err_code
       WHERE msg_id = v_msg_id;

      COMMIT;
   END;

   PROCEDURE set_message_delivery_info (v_smsc_id         VARCHAR2,
                                        v_delivered       NUMBER,
                                        v_final_status    VARCHAR2,
                                        V_DONE_DATE       date,
                                        v_host_name varchar2)
   IS
      l_msg_id smpp_outgoing.msg_id%type;
      l_system_id smpp_outgoing.system_id%type;
   begin
   
      UPDATE smpp_outgoing
         set DELIVERED = V_DELIVERED,
             FINAL_STATUS = V_FINAL_STATUS,
             DONE_DATE = V_DONE_DATE
       WHERE smsc_id = v_smsc_id --and host_name=v_host_name
       returning msg_id, system_id into l_msg_id, l_system_id;
       
       if l_system_id is not null then --to be delivered to third party
          insert into smsc_inc_queue (item_id, smpp_incoming_id, smpp_outgoing_id, delivered)
          values (smsc_inc_queue_seq.nextval, null, l_msg_id, 0);
       end if;

      COMMIT;

      /*pkg_pp_third_party.check_for_tp_delivery_report (v_smsc_id,
                                                       v_delivered,
                                                       v_final_status,
                                                       v_done_date);*/
      commit;
      EXCEPTION when OTHERS then
         rollback;
         PKG_LOGGER.LOG_ERROR (SQLERRM, G_APP);
         pkg_logger.log_error (dbms_utility.format_error_stack, g_app);
         PKG_LOGGER.LOG_ERROR (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, G_APP);
         raise;
   
   END;
END PKG_SMPP;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMPP_HOST" AS

  procedure get_smpp_hosts(v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for
    select host_name from smpp_hosts;
  END get_smpp_hosts;

END PKG_SMPP_HOST;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMPP_POST_PROCESSING" 
AS
   g_app   VARCHAR2 (100) := 'PKG_SMPP_POST_PROCESSING';

   PROCEDURE process_received_sms (v_msg_id              NUMBER,
                                   v_host_name           VARCHAR2,
                                   v_in_date             DATE,
                                   v_esm_class           NUMBER,
                                   v_source_addr         VARCHAR2,
                                   v_destination_addr    VARCHAR2,
                                   v_msg_body            NVARCHAR2,
                                   v_file_name           VARCHAR2)
   IS 
    l_res number := 0;
    l_error_answer varchar2(250);
   BEGIN
       /*
       if(v_destination_addr='8102') then
               pkg_logger.log_debug ('Checking for ANS request', g_app, v_source_addr);
               l_res := pkg_pp_ans_new.check_for_ans(
                      v_msg_id,
                      v_host_name,
                      v_destination_addr,
                      v_source_addr,
                      v_msg_body,
                      'ANS');
                pkg_logger.log_debug ('Result: ' || l_res, g_app, v_source_addr);  
            
            return;
       end if;    */
   
       insert into handler_queue(rec_id, msg_id) 
       values(handler_queue_seq.nextval, v_msg_id);
       commit;  
       
      
 
       pkg_logger.log_debug ('Successfully finished post processing',
                            g_app,
                            v_source_addr);
   EXCEPTION
      WHEN OTHERS
      THEN
         pkg_logger.log_error (SQLERRM, g_app);
         pkg_logger.log_error (dbms_utility.format_error_stack, g_app);
         pkg_logger.log_error (DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
   END;
END PKG_SMPP_POST_PROCESSING;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMSC" as

   c_success constant number := 0;
   c_invalid_source_addr constant number := 1;
   c_out_of_balance constant number := 1024;
   c_generic_error constant number := 100;

   procedure send_sms(p_system_id varchar2, p_source_addr varchar2, p_destination_addr varchar2, 
                      p_msg_body nvarchar2, p_delivery_report number,
                      o_result out number,
                      o_new_row_id out number) 
   is                   
       l_sender_name_exists number;
       l_host_name smpp_hosts.host_name%type;
       
       l_manager_id number;
       l_current_balance number;
   begin
      
       begin      
          select u.user_id,
          (select nvl(max(balance), 0) from bulk_sms2_balance where user_id=u.user_id) as current_balance 
          into l_manager_id, l_current_balance from bulk_sms2_sender_names v 
          inner join ui_users u on v.user_id=u.user_id
          where u.login = p_system_id and sender_name = p_source_addr;
       exception when no_data_found then
          o_result := c_invalid_source_addr;
          return;
       end;
       
       if l_manager_id is not null and l_current_balance < 1 then
          o_result := c_out_of_balance;
          return;
       end if;
       
       select nvl(max(h.host_name),'NAR1') into l_host_name from bulk_sms2_hosts bh
       inner join smpp_hosts h on bh.host_id=h.host_id
       where bh.user_id = l_manager_id;
   
       insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, 
                                  delivery_report, send_at, send_until, app_name, priority, system_id)
       values (smpp_outgoing_seq.nextval, l_host_name, p_source_addr, p_destination_addr, substr(p_msg_body, 1, 254), 
               p_delivery_report, sysdate, sysdate+10, 'SMSC', 1, p_system_id)
       returning msg_id into o_new_row_id;
       
       if l_manager_id is not null then        
         update bulk_sms2_balance set balance=balance-1 where user_id=l_manager_id;
       end if;
       
       o_result := c_success;
   exception when others then
       o_result := c_generic_error;
       
       pkg_logger.log_error(sqlerrm, 'SMSC');
   end;
   
   procedure get_inc_queue(p_system_ids varchar2, p_items_cursor out t_cursor) is
   begin
      open p_items_cursor for
      select q1.item_id, 0 as is_delivery_report, i.system_id, i.source_addr as src_addr, i.destination_addr as dest_addr, i.msg_body, null as final_status, null as done_date, i.msg_id 
      from smsc_inc_queue q1 
      inner join smpp_incoming i on q1.smpp_incoming_id=i.msg_id 
      where q1.delivered=0 and i.system_id in (select column_value from table(pkg_util.get_string_table(p_system_ids, ',')))
      union all
      select q2.item_id, 1 as is_delivery_report, o.system_id, o.destination_addr, o.source_addr, o.msg_body, final_status, done_date, o.msg_id 
      from smsc_inc_queue q2 
      inner join smpp_outgoing o on q2.smpp_outgoing_id=o.msg_id 
      where q2.delivered=0 and o.final_status is not null and system_id in (select column_value from table(pkg_util.get_string_table(p_system_ids, ',')))
      order by item_id;
   end;
   
   procedure mark_queue_item_as_delivered(p_item_id number) is
   begin
      update smsc_inc_queue set delivered = 1 where item_id = p_item_id;
   end;
   
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMSC_SMS_DELIVERY" is

procedure add_third_party_deliverable(v_tp_system_id varchar2,v_source_addr_alias varchar2,v_destination_addr varchar2,v_msg_body nvarchar2,v_smpp_incoming_id number) is
begin
insert into third_party_deliverables 
(del_id,tp_system_id,source_addr,destination_addr,msg_body,date_added,is_delivery_report,smpp_incoming_id) 
values 
(third_party_deliverables_seq.nextval,v_tp_system_id,v_source_addr_alias,v_destination_addr,v_msg_body,sysdate,0,v_smpp_incoming_id);
end;

  
end pkg_smsc_sms_delivery;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMSC_SMS_SENDER" as

 g_app varchar2(100):='PKG_SMSC_SMS_SENDER';

 procedure send_sms(v_system_id varchar2, v_source_addr_alias varchar2, v_destination_addr varchar2, v_msg_body nvarchar2, v_delivery_report number, v_priority number,  v_app_str_param1 nvarchar2, v_new_row_id out number) is
  l_send_at date:=sysdate;
  l_send_until date:=sysdate+30;
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  l_short_number_id number;
  begin
    pkg_logger.log_debug('SMSC-SMS from' || v_system_id || ', source number ' || v_source_addr_alias || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
  
    select snv.host_name, snv.short_number, snv.number_id 
    into l_host_name, l_short_number, l_short_number_id 
    from short_numbers_view snv
    inner join third_parties_numbers_view tpnv 
    on (snv.number_id=tpnv.short_number_id and tpnv.tp_system_id=v_system_id and snv.short_number_alias=v_source_addr_alias);
    
    insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, delivery_report, app_name, app_str_param1, send_at, send_until, priority)
    values
    (smpp_outgoing_seq.nextval, l_host_name, l_short_number, v_destination_addr, v_msg_body, v_delivery_report, 'SMSC_THIRD_PARTY', v_app_str_param1, l_send_at, l_send_until, v_priority);
    
    commit;
    
    select smpp_outgoing_seq.currval into v_new_row_id from dual;
    
    pkg_logger.log_debug('SMSC-SMS inserted from ' || l_short_number || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr); 
  end;
  
end pkg_smsc_sms_sender;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMS_ONLINE" as

  procedure get_messages_by_short_number(v_ext_access_rec_id number, v_short_number_id number, v_last_msg_id number, v_items_cursor out t_cursor) as
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  l_from_date date;
  l_for_date date;
  l_ext_access_date date;
  begin
      select date_added into l_ext_access_date from external_access where rec_id=v_ext_access_rec_id;
    
      l_for_date :=trunc(sysdate);
    
      select short_number, host_name into l_short_number, l_host_name from short_numbers_view where number_id=v_short_number_id;
      
      open v_items_cursor for 
      select msg_id, pkg_util.format_msisdn(source_addr) as source_addr, destination_addr, msg_body, in_date from smpp_incoming where
      is_processed=0 and destination_addr=l_short_number and host_name=l_host_name 
      and in_date >= l_for_date
      and in_date >= l_ext_access_date
      and msg_id>v_last_msg_id
      order by msg_id;
  end get_messages_by_short_number;

end pkg_sms_online;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMS_SENDER" AS
  
  g_app varchar2(100):='PKG_SMS_SENDER';
  
  procedure send_sms(v_host_name varchar2, v_source_addr varchar2, 
                       v_destination_addr varchar2, v_msg_body nvarchar2, v_delivery_report number, 
                       v_priority number,  v_app_name varchar2) is
  l_send_at date:=sysdate;
  l_send_until date:=sysdate+30;
  begin
    
    if v_msg_body is not null and length(v_msg_body) > 0 and pkg_black_list.is_in_black_list(v_destination_addr) = 0 then
        insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, delivery_report, app_name, send_at, send_until, priority, app_num_param1)
        values (smpp_outgoing_seq.nextval, v_host_name, v_source_addr, v_destination_addr, substr(v_msg_body, 1, g_max_sms_length), v_delivery_report, v_app_name, l_send_at, l_send_until, v_priority, null);
        --pkg_logger.log_debug('SMS inserted from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
    --else 
    --    pkg_logger.log_debug('SMS message body is null or empty from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
    end if; 

  end;
  
  procedure send_sms_at(v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, 
                     v_msg_body nvarchar2, v_delivery_report number, v_priority number, 
                     v_app_name varchar2, v_send_at date, v_app_num_param1 number, v_validity_period number) is
  l_send_at date:=v_send_at;
  l_send_until date:=v_send_at+30;
  begin
    
    if v_msg_body is not null and length(v_msg_body) > 0 and pkg_black_list.is_in_black_list(v_destination_addr) = 0 then
        insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, delivery_report, app_name, send_at, send_until, priority, app_num_param1, validity_period)
        values (smpp_outgoing_seq.nextval, v_host_name, v_source_addr, v_destination_addr, substr(v_msg_body, 1, g_max_sms_length), v_delivery_report, v_app_name, l_send_at, l_send_until, v_priority, v_app_num_param1, v_validity_period);
        --pkg_logger.log_debug('SMS inserted from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
    --else 
    --    pkg_logger.log_debug('SMS message body is null or empty from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
    end if; 

  end;
  
  function get_template_text(p_tpl_id number, p_msisdn varchar2) return varchar2 is
    l_segment_id number;
    l_template_text varchar2(1500);
  begin
  
    begin
       select decode(s.tpl_text, null, t.tpl_text, s.tpl_text) into l_template_text from sms_templates t 
       left join segment_numbers n on (n.msisdn=p_msisdn)
       left join sms_templates_for_segments s on (t.tpl_id=s.tpl_id and s.segment_id=n.segment_id)
       where t.tpl_id=p_tpl_id;
    exception when no_data_found then
       raise_application_error(-20002, 'Sms template with id '||p_tpl_id||' not found');
    end;    
    
    return l_template_text;
  end;
  
  --sms template must be in following format: TPL_ID#[[PLACEHOLDER1]]=val1 [[PLACEHOLDER2]]=val2
  procedure send_sms_template(v_host_name varchar2, v_source_addr varchar2, 
                              v_destination_addr varchar2, v_template nvarchar2, 
                              v_delivery_report number, v_priority number, v_app_name varchar2,
                              v_send_at date) is
    l_template_id number;
    l_template_text varchar2(1500);
    
    l_opening_pos number:=0;
    l_closing_pos number:=0;
    l_eq_pos number:=0;
    l_next_opening_pos number:=0;
    l_placeholder_name varchar2(50);
    l_placeholder_value varchar2(1000);     
  begin
    if not regexp_like(v_template, '^\d+\s*#(\s*\[\[[A-Z0-9_]+\]\]\s*=\s*[^[]+)*$', 'in') then
      raise_application_error(-20001, 'v_template parameter must be in following format: TPL_ID#[[PLACEHOLDER1]]=val1 [[PLACEHOLDER2]]=val2. Your input:'||v_template);
    end if;
    
    l_template_id := to_number(trim(substr(v_template, 1, instr(v_template, '#')-1)));
    
    /*
    begin
      select tpl_text into l_template_text from sms_templates where tpl_id=l_template_id;
    exception when no_data_found then
      raise_application_error(-20002, 'Sms template with id '||l_template_id||' not found');
    end;
    */
    
    l_template_text := get_template_text(l_template_id, v_destination_addr);
       
    l_opening_pos := instr(v_template, '[[', l_opening_pos+2);
    l_next_opening_pos := instr(v_template, '[[', l_opening_pos+2);
    l_closing_pos := instr(v_template, ']]', l_closing_pos+2);
    l_eq_pos := instr(v_template, '=', l_eq_pos+1);
    
    while l_opening_pos != 0 and l_opening_pos!=(length(v_template)+1)
    loop     
      l_placeholder_name := substr(v_template, l_opening_pos+2, l_closing_pos-l_opening_pos-2);
      if l_next_opening_pos = 0 then
        l_next_opening_pos := length(v_template)+1;
      end if;
      l_placeholder_value := substr(v_template, l_eq_pos+1, l_next_opening_pos-l_eq_pos-1);
      
      l_template_text := replace(l_template_text, '[['||l_placeholder_name||']]', l_placeholder_value);
    
      l_opening_pos := l_next_opening_pos;
      l_next_opening_pos := instr(v_template, '[[', l_opening_pos+2);
      l_closing_pos := instr(v_template, ']]', l_closing_pos+2);
      l_eq_pos := instr(v_template, '=', l_eq_pos+1);
    end loop;
    
    send_sms_at(v_host_name, v_source_addr, v_destination_addr, l_template_text, v_delivery_report, v_priority,  v_app_name, v_send_at);
  end;
  
  procedure send_sms(v_short_number_id number, v_numbers varchar2, v_msg_body nvarchar2, v_priority number, v_app_name varchar2) is
  l_short_number varchar2(50);
  l_host_name varchar2(10);
  cursor numbers_cursor is (select * from table(pkg_util.get_string_table(v_numbers, ',')) t);
  begin    
    select host_name, short_number into l_host_name, l_short_number from short_numbers_view where number_id=v_short_number_id;
    --pkg_logger.log_debug('send_sms bulk started - from ' || l_short_number || ' with body: ' || v_msg_body || ' to ' || v_numbers, v_app_name); 
    
    for c in numbers_cursor 
    loop
      send_sms(l_host_name, l_short_number,  c.column_value, v_msg_body, 0, v_priority,  v_app_name);
    end loop;
    
    --pkg_logger.log_debug('send_sms bulk finished - from ' || l_short_number || ' with body: ' || v_msg_body || ' to ' || v_numbers, v_app_name); 
  end; 
  
  procedure send_wap_push(v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, v_title nvarchar2, v_url nvarchar2, v_delivery_report number, v_priority number) is
  l_send_at date:=sysdate;
  l_send_until date:=sysdate+30;
  begin
    
    if  pkg_black_list.is_in_black_list(v_destination_addr) = 0 then
       insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, delivery_report, app_name, send_at, send_until, priority, app_str_param1)
       values
       (smpp_outgoing_seq.nextval, v_host_name, v_source_addr, v_destination_addr, substr(v_title, 1, g_max_sms_length), v_delivery_report, 'WAP_PUSH', l_send_at, l_send_until, v_priority, v_url);
    end if;
    
    --pkg_logger.log_debug('WAP push inserted from ' || v_source_addr || ' to ' || v_destination_addr || ' with url: ' || v_url, g_app, v_destination_addr); 
    
  end;

  procedure send_sms(v_sms_id out number, v_host_name varchar2, v_source_addr varchar2, v_destination_addr varchar2, v_msg_body nvarchar2, v_delivery_report number, v_priority number,  v_app_name varchar2) is
  l_send_at date:=sysdate;
  l_send_until date:=sysdate+30;
  begin
    if v_msg_body is not null and length(v_msg_body) > 0 and pkg_black_list.is_in_black_list(v_destination_addr) = 0 then
        select smpp_outgoing_seq.nextval into v_sms_id from dual;
    
        insert into smpp_outgoing (msg_id, host_name, source_addr, destination_addr, msg_body, delivery_report, app_name, send_at, send_until, priority)
        values(v_sms_id, v_host_name, v_source_addr, v_destination_addr, substr(v_msg_body, 1, g_max_sms_length), v_delivery_report, v_app_name, l_send_at, l_send_until, v_priority);
    
        --pkg_logger.log_debug(v_sms_id || ' SMS inserted from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);
    --else
    --    pkg_logger.log_debug('SMS message body is null or empty from ' || v_source_addr || ' to ' || v_destination_addr || ' with body: ' || v_msg_body, g_app, v_destination_addr);    
    end if;     
  end;
  

END PKG_SMS_SENDER;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_SMS_TICKER" as

  procedure mark_message(p_msg_id number, p_status varchar2, p_client varchar2, p_no_charge number, o_result out varchar2) is
      l_charged number;
      l_already_processed number;
  begin
     select case when exists (select 0 from smpp_incoming where msg_id=p_msg_id and is_processed=1) then 1
            else 0
            end into l_already_processed from dual;
            
     if l_already_processed = 1 then
       o_result := 'This sms was already processed';
       return;
     end if;
  
     if p_status='good' then --24
       l_charged := charge(p_msg_id);
       if l_charged = 100 or (l_charged = 500 and p_no_charge = 1) then 
          send_response(p_msg_id, 24);
          mark_as_processed(p_msg_id, p_status, p_client);
          insert into sms_ticker_queue (rec_id, msg_id, msg_body, client_id)
          values (sms_ticker_queue_seq.nextval, p_msg_id,
                 (select msg_body from smpp_incoming where msg_id=p_msg_id),
                 (select client_id from sms_ticker_clients where title=p_client));
          o_result := 'Sms accepted';
       elsif l_charged = 401 then
          send_response(p_msg_id, 26);
          mark_as_processed(p_msg_id, 'no_balance', p_client);
          o_result := 'Insufficient balance';
       else
          send_response(p_msg_id, 25);
          mark_as_processed(p_msg_id, 'charge_failed', p_client);
          o_result := 'Failed to charge';
       end if;
     elsif p_status='bad_words' then
       send_response(p_msg_id, 22); 
       mark_as_processed(p_msg_id, p_status, p_client);
       o_result := 'Sent message about bad words';
     elsif p_status='wrong_spelling' then
       send_response(p_msg_id, 23);
       mark_as_processed(p_msg_id, p_status, p_client);
       o_result := 'Sent message about bad spelling';
     elsif p_status='block' then
       add_to_black_list(p_msg_id);
       mark_as_processed(p_msg_id, p_status, p_client);
       o_result := 'Subscriber was blocked';
     elsif p_status='ignore' then
       mark_as_processed(p_msg_id, p_status, p_client);
       o_result := 'Message ignored';
     else
       o_result := 'Unknown command';
       mark_as_processed(p_msg_id, p_status, p_client);
     end if;
     
     commit;
  end;
  
  procedure add_to_black_list(p_msg_id number) is
    l_msisdn varchar2(50);
  begin
    select source_addr into l_msisdn from smpp_incoming where msg_id=p_msg_id;
    insert into sms_ticker_black_list (msisdn) values (l_msisdn);
  exception when others 
     then null;
  end;
  
  procedure mark_as_processed(p_msg_id number, p_status varchar2, p_client varchar2) is
  begin
    update smpp_incoming set is_processed=1, app_str_param1=p_status, app_str_param2=p_client,
    date_processed=sysdate, app_name='SMS_TICKER' where msg_id=p_msg_id;
  end;
  
  procedure send_response(p_msg_id number, p_tpl_id number) is
     l_host_name varchar2(10);
     l_short_number varchar2(50);
     l_msisdn varchar2(50);
  begin
     select host_name, source_addr, destination_addr into l_host_name, l_msisdn, l_short_number
     from smpp_incoming where msg_id=p_msg_id;
     
     pkg_sms_sender.send_sms_template(l_host_name, l_short_number, 
                              l_msisdn, to_char(p_tpl_id)||'#', 
                              0, 1, 'SMS_TICKER');
  end;
  
  function charge(p_msg_id number) return number is
     l_host_name varchar2(10);
     l_msisdn varchar2(50);
     l_short_number_id number;
     l_tariff_id number;
     
     l_result number;
     l_text_result varchar2(500);
  begin     
      select i.host_name, i.source_addr, sn.number_id into l_host_name, l_msisdn, l_short_number_id
      from smpp_incoming i
      inner join smpp_hosts h on i.host_name=h.host_name
      inner join short_numbers sn on (sn.host_id=h.host_id and sn.short_number=i.destination_addr)
      where msg_id=p_msg_id;
      
      if l_msisdn = '994552297703' or l_msisdn = '994558114663' then
         return 100;
      end if;
     
     if l_host_name like 'BAKCELL%' then
        l_tariff_id := 39;
     else
        l_tariff_id := 1;
     end if;
  
     pkg_diameter.charge(l_msisdn, 
                  l_tariff_id, 
                  1,
                  'SMS_TICKER',
                  l_result,
                  l_text_result,
                  p_msg_id);
                  
     return l_result;
  end;
  
  procedure add_tpl is
  begin
      for cc in (select client_id from sms_ticker_clients where current_status='a')
      loop
         insert into sms_ticker_queue 
         select sms_ticker_queue_seq.nextval, null, tpl_text, cc.client_id, sysdate
         from sms_ticker_templates
         where to_date(to_char(sysdate, 'yyyy-mm-dd ') || to_char(tpl_time,'hh24:mi'), 'yyyy-mm-dd hh24:mi')<sysdate
         and to_date(to_char(sysdate, 'yyyy-mm-dd ') || to_char(tpl_time,'hh24:mi'), 'yyyy-mm-dd hh24:mi')>sysdate-1/24/60*20;
      end loop;
  end;
  
  procedure get_pending_messages(o_cur out t_cursor) is
  begin
      open o_cur for 
      select i.msg_id, in_date, i.source_addr, c.title as client, msg_body, 
      ' ' as actions from smpp_incoming i
      inner join smpp_hosts h on i.host_name=h.host_name
      inner join short_numbers sn on (sn.host_id=h.host_id and sn.short_number=i.destination_addr)
      inner join sms_ticker_clients_numbers cn on (cn.number_id=sn.number_id and (cn.regexp is null or (cn.regexp is not null and regexp_like(i.msg_body, cn.regexp))))
      inner join sms_ticker_clients c on cn.client_id=c.client_id
      where in_date>trunc(sysdate-1/24*1 /*max last 1 hour*/ ) 
      and is_processed=0 
      and i.source_addr not in (select msisdn from sms_ticker_black_list)
      and not regexp_like(trim(i.msg_body), '^((sk\d+)).*$', 'in')
      order by i.msg_id;
  end;
  
  function has_pending_messages return number is
     l_result number := 0;
     pending_rec t_pending_ret_type;
     l_cur t_cursor;
  begin
     get_pending_messages(l_cur);
     fetch l_cur into pending_rec;
     if l_cur%found then
        l_result := 1;
     end if;
     close l_cur;
     
     return l_result;
  exception when others then
     if l_cur%isopen then
        close l_cur;
     end if;
     
     return 0;
  end;
  
  procedure notify_of_pending_messages is
     l_has_pending number;
  begin
     l_has_pending := has_pending_messages;
     if l_has_pending = 0 then
        return;
     end if;
     
     pkg_sms_sender.send_sms('BAKCELL1', '9013', '994558493105', 'You have pending message(s) to accept', 0, 1, 'SMS_TICKER');
     pkg_sms_sender.send_sms('BAKCELL1', '9013', '994558114663', 'You have pending message(s) to accept', 0, 1, 'SMS_TICKER');
  end;
  
end;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_UTIL" AS

  FUNCTION get_number_table(v_split_string VARCHAR2, v_splitter VARCHAR2) RETURN number_list 
  IS
    v_split_len NUMBER(1) := LENGTH(v_splitter);
    v_string VARCHAR2(32000) := v_split_string;
    v_result number_list := number_list();
    idx NUMBER;
    val VARCHAR2(100);
  BEGIN
       IF LENGTH(v_string) > 0
       THEN
           LOOP
               idx := INSTR(v_string, v_splitter, 1);
               IF idx > 0 THEN
                  val := trim(SUBSTR(v_string, 1, idx-1));
                  IF val IS NOT NULL
                  THEN
                      v_result.EXTEND;
                      v_result(v_result.LAST) := val;
                  END IF;
               ELSE
                  val := trim(SUBSTR(v_string, 1));
                  IF val IS NOT NULL
                  THEN
                      v_result.EXTEND;
                      v_result(v_result.LAST) := val;
                  END IF;
                  EXIT;
               END IF;
               v_string := SUBSTR(v_string, idx + v_split_len);
           END LOOP;
       END IF;

       RETURN v_result;
  END; 
  
  FUNCTION get_string_table(v_split_string VARCHAR2, v_splitter VARCHAR2) RETURN string_list 
  IS
    v_split_len NUMBER(1) := LENGTH(v_splitter); 
    v_string VARCHAR2(32000) := v_split_string;
    v_result string_list := string_list();
    idx NUMBER;
    val VARCHAR2(100); 
  BEGIN
       IF LENGTH(v_string) > 0
       THEN
           LOOP
               idx := INSTR(v_string, v_splitter, 1);
               IF idx > 0 THEN
                  val := trim(SUBSTR(v_string, 1, idx-1));
                  IF val IS NOT NULL
                  THEN
                      v_result.EXTEND;
                      v_result(v_result.LAST) := val;
                  END IF;
               ELSE
                  val := trim(SUBSTR(v_string, 1));
                  IF val IS NOT NULL
                  THEN
                      v_result.EXTEND;
                      v_result(v_result.LAST) := val;
                  END IF;
                  EXIT;
               END IF;
               v_string := SUBSTR(v_string, idx + v_split_len);
           END LOOP;
       END IF;

       RETURN v_result;
  END;
  
  FUNCTION verify_numeric (check_this VARCHAR2)
  RETURN BOOLEAN
  IS
  BEGIN
    IF LTRIM (TRANSLATE (check_this, '123456789', '000000000'), '0') IS NOT NULL
    THEN
       RETURN FALSE; -- check_this is not numeric
    ELSE
       RETURN TRUE; -- check_this is numeric
    END IF;
  END verify_numeric;
  
  FUNCTION get_matching_symbol_count(v_orig nvarchar2, v_tocheck nvarchar2) return number is
  l_res number:=0;
  l_length number;
  l_counter number:=1;
  l_chr_orig nvarchar2(1);
  l_chr_check nvarchar2(1);
  begin
    if length(v_orig)!=length(v_tocheck) then
      return 0;
    end if;
    
    l_length:=length(v_orig);
    
    if v_tocheck like v_orig then
      return l_length;
    end if;
    
    while l_counter<=l_length
    loop
      l_chr_orig:=substr(v_orig, l_counter, 1);
      l_chr_check:=substr(v_tocheck, l_counter, 1);
      if l_chr_orig is not null and l_chr_check is not null and (l_chr_check=l_chr_orig or l_chr_orig='_') then
        l_res:=l_res+1;
      end if;
      l_counter:=l_counter+1;
    end loop;
    
    return l_res;
  end;
  
  function md5_digest (
    v_string IN varchar2
    ) return varchar2 IS
    --
    -- Return an MD5 hash of the input string.
    --
    BEGIN
    return lower(dbms_obfuscation_toolkit.md5 (
    input => utl_raw.cast_to_raw(v_string)
    ));
    END md5_digest;
    
    FUNCTION format_msisdn(v_msisdn varchar2) return varchar2 is
    l_res varchar2(50);
    begin
       if length(v_msisdn) not between 12 and 20 then
          return v_msisdn;
       end if;
       
       l_res:='(' || substr(v_msisdn, 1, 3) || '-' || substr(v_msisdn, 4, 2) || ') ' || 
             substr(v_msisdn, 6, 3) || '-' || substr(v_msisdn, 9, 2) || '-' || substr(v_msisdn, 11);
             
       return l_res;
    end;
     
  FUNCTION get_http_response(p_url varchar2, p_method varchar2 := 'GET', p_post_data varchar2:=null, p_post_content_type varchar2 := 'application/x-www-form-urlencoded') return varchar2 is
    req   utl_http.req;
    resp  utl_http.resp;

    l_result varchar2(32767); 
  BEGIN
  
    --utl_http.set_proxy('proxy.my-company.com', 'corp.my-company.com');
  
    req := utl_http.begin_request(url => p_url, method => p_method);
    utl_http.set_transfer_timeout(60);
    utl_http.set_header(req, 'User-Agent', 'Mozilla/4.0');
    utl_http.set_header(req, 'Connection', 'close');
    
    if p_method = 'POST' then
      utl_http.set_header(req, 'Content-Type', p_post_content_type);
      utl_http.set_header(req, 'Content-Length', length(p_post_data));
      utl_http.write_text(req, p_post_data);      
    end if;
    
    resp := utl_http.get_response(req);
    
    utl_http.read_text(resp, l_result);

    utl_http.end_response(resp);
    
    return l_result;
  EXCEPTION
    WHEN utl_http.end_of_body THEN
      utl_http.end_response(resp);
      
    return l_result;
  END;

END PKG_UTIL;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_CATEGORY" AS

PROCEDURE GET_WEB_PUSH_CATEGORIES(V_SITE_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR) IS
BEGIN
OPEN ITEMS_CURSOR FOR 
SELECT 
C.CAT_ID,C.PARENT_CAT_ID,C.CAT_NAME, SN.SHORT_NUMBER FROM 
WEB_PUSH_CATEGORIES C
INNER JOIN SHORT_NUMBERS SN
ON C.SHORT_NUMBER_ID=SN.NUMBER_ID
WHERE SITE_ID=V_SITE_ID ORDER BY CAT_ID;
END;

PROCEDURE GET_WEB_PUSH_CATEGORY_BY_ID(V_CAT_ID NUMBER,ITEMS_CURSOR OUT T_CURSOR) IS
BEGIN
OPEN ITEMS_CURSOR FOR SELECT CAT_ID,PARENT_CAT_ID,CAT_NAME,SHORT_NUMBER_ID,HOST_NAME,SHORT_NUMBER FROM WEB_PUSH_CATEGORIES_VIEW WHERE CAT_ID=V_CAT_ID;
END;

PROCEDURE ADD_WEB_PUSH_CATEGORY(V_PARENT_CAT_ID NUMBER,V_CAT_NAME NVARCHAR2,V_SHORT_NUMBER_ID NUMBER,V_SITE_ID NUMBER,V_NEW_ROW_ID OUT NUMBER) IS
BEGIN
INSERT INTO WEB_PUSH_CATEGORIES (CAT_ID,PARENT_CAT_ID,CAT_NAME,SHORT_NUMBER_ID,SITE_ID) VALUES (WEB_PUSH_CATEGORIES_SEQ.NEXTVAL,V_PARENT_CAT_ID,V_CAT_NAME,V_SHORT_NUMBER_ID,V_SITE_ID);
SELECT WEB_PUSH_CATEGORIES_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
END;

PROCEDURE UPDATE_WEB_PUSH_CATEGORY(V_CAT_ID NUMBER,V_PARENT_CAT_ID NUMBER,V_CAT_NAME NVARCHAR2,V_SHORT_NUMBER_ID NUMBER) IS
BEGIN
UPDATE WEB_PUSH_CATEGORIES SET PARENT_CAT_ID=V_PARENT_CAT_ID,CAT_NAME=V_CAT_NAME,SHORT_NUMBER_ID=V_SHORT_NUMBER_ID WHERE CAT_ID=V_CAT_ID;
END;

PROCEDURE DELETE_WEB_PUSH_CATEGORY(V_CAT_ID NUMBER) IS
BEGIN
DELETE FROM WEB_PUSH_CATEGORIES WHERE CAT_ID=V_CAT_ID;
END;



END PKG_WEB_PUSH_CATEGORY; 
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_ELEMENT" AS
  g_page_size number:=24;

  procedure get_by_cat_id(v_cat_id number, v_page_id number, v_total_count out number, v_items_cursor out t_cursor) AS
  BEGIN   
    open v_items_cursor for
    Select * from (
    Select a.*, rownum rnum From (
    
    select 
    elem_id, elem_name, elem_code, date_added
    from web_push_elements where cat_id=v_cat_id order by date_added desc
    
    ) a where rownum <= v_page_id*g_page_size
    ) where rnum > (v_page_id-1)*g_page_size; 
    
    select count(0) into v_total_count from web_push_elements where cat_id=v_cat_id;
    
  END;
  
  PROCEDURE ADD_UPDATE_WEB_PUSH_ELEMENT(V_CAT_ID NUMBER, V_ELEM_ID NUMBER,V_ELEM_NAME NVARCHAR2,V_ELEM_CODE NVARCHAR2,V_NEW_ROW_ID OUT NUMBER) IS
  L_NEW_ID NUMBER;
  L_ELEM_CODE NVARCHAR2(160):=TRIM(V_ELEM_CODE);
  BEGIN
    IF V_ELEM_ID=0 THEN
      SELECT WEB_PUSH_ELEMENTS_SEQ.NEXTVAL INTO L_NEW_ID FROM DUAL;
      IF L_ELEM_CODE IS NULL THEN
        L_ELEM_CODE:=L_NEW_ID;
      END IF;
      INSERT INTO WEB_PUSH_ELEMENTS (ELEM_ID,CAT_ID,ELEM_NAME,ELEM_CODE) VALUES (L_NEW_ID,V_CAT_ID,V_ELEM_NAME,L_ELEM_CODE);
      SELECT WEB_PUSH_ELEMENTS_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
    ELSE
      UPDATE_WEB_PUSH_ELEMENT(V_ELEM_ID, V_ELEM_NAME, V_ELEM_CODE);
      SELECT V_ELEM_ID INTO V_NEW_ROW_ID  FROM DUAL;
    END IF;
  END;
  
  PROCEDURE UPDATE_WEB_PUSH_ELEMENT(V_ELEM_ID NUMBER,V_ELEM_NAME NVARCHAR2,V_ELEM_CODE NVARCHAR2) IS
  BEGIN
  UPDATE WEB_PUSH_ELEMENTS SET ELEM_NAME=V_ELEM_NAME,ELEM_CODE=TRIM(V_ELEM_CODE) WHERE ELEM_ID=V_ELEM_ID;
  END;
  
  PROCEDURE UPDATE_WEB_PUSH_ELEMENT_CODE(V_ELEM_ID NUMBER,V_ELEM_CODE NVARCHAR2) IS
  BEGIN
  UPDATE WEB_PUSH_ELEMENTS SET ELEM_CODE=TRIM(V_ELEM_CODE) WHERE ELEM_ID=V_ELEM_ID;
  END;
  
  PROCEDURE DELETE_WEB_PUSH_ELEMENT(V_ELEM_ID NUMBER) IS
  BEGIN
  DELETE FROM WEB_PUSH_ELEMENTS WHERE ELEM_ID=V_ELEM_ID;
  END;
  
  PROCEDURE get_web_push_element(v_elem_id number, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for select elem_name, elem_code from web_push_elements where elem_id=v_elem_id;
  end;


END PKG_WEB_PUSH_ELEMENT;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_FILE" AS

g_app varchar2(100):='PKG_WEB_PUSH_FILE'; 

PROCEDURE ADD_UPDATE_WEB_PUSH_FILE(V_ELEM_ID NUMBER,V_RES_ID NUMBER,V_CONTENT_TYPE NVARCHAR2,V_FILE_NAME NVARCHAR2,V_NEW_ROW_ID OUT NUMBER) IS
L_COUNT NUMBER;
BEGIN
SELECT COUNT(0) INTO L_COUNT FROM WEB_PUSH_FILES WHERE ELEM_ID=V_ELEM_ID AND RES_ID=V_RES_ID;
IF L_COUNT=0 THEN
  INSERT INTO WEB_PUSH_FILES (FILE_ID,ELEM_ID,RES_ID,CONTENT_TYPE,FILE_NAME) VALUES (WEB_PUSH_FILES_SEQ.NEXTVAL,V_ELEM_ID,V_RES_ID,V_CONTENT_TYPE,V_FILE_NAME);
  SELECT WEB_PUSH_FILES_SEQ.CURRVAL INTO V_NEW_ROW_ID  FROM DUAL;
ELSE
  UPDATE WEB_PUSH_FILES SET CONTENT_TYPE=V_CONTENT_TYPE,FILE_NAME=V_FILE_NAME WHERE ELEM_ID=V_ELEM_ID AND RES_ID=V_RES_ID;
  SELECT FILE_ID INTO V_NEW_ROW_ID  FROM WEB_PUSH_FILES WHERE ELEM_ID=V_ELEM_ID AND RES_ID=V_RES_ID;
END IF;
END;

PROCEDURE DELETE_WEB_PUSH_FILE(V_FILE_ID NUMBER) IS
BEGIN
DELETE FROM WEB_PUSH_FILES WHERE FILE_ID=V_FILE_ID;
END;

PROCEDURE GET_FILE_INFO(V_ELEM_ID NUMBER, V_RES_ID NUMBER, V_REG_KEY varchar2, V_ITEMS_CURSOR OUT T_CURSOR) IS
BEGIN
  open v_items_cursor for 
  select content_type, file_name from web_push_files 
  where elem_id=v_elem_id and res_id=v_res_id
  and
  elem_id in (select elem_id from web_push_requests where req_key=v_reg_key and expiry_date>=sysdate);
END;

PROCEDURE GET_FILE_INFO_WITHOUT_KEY(V_ELEM_ID NUMBER, V_RES_ID NUMBER, V_ITEMS_CURSOR OUT T_CURSOR) IS
BEGIN
  open v_items_cursor for 
  select content_type, file_name from web_push_files 
  where elem_id=v_elem_id and res_id=v_res_id;
END;

FUNCTION GET_FILE_EXTENSION(V_ELEM_ID NUMBER) RETURN NVARCHAR2 IS
l_file_name nvarchar2(250);
l_ext nvarchar2(10);
BEGIN
    SELECT FILE_NAME INTO l_file_name FROM WEB_PUSH_FILES WPF WHERE
    ELEM_ID=V_ELEM_ID AND RES_ID=(SELECT MIN(RES_ID) FROM WEB_PUSH_FILES WHERE ELEM_ID=WPF.ELEM_ID AND RES_ID!=7);
    
    l_ext:=SUBSTR(l_file_name, INSTR(l_file_name, '.', -1, 1));
    RETURN l_ext;
EXCEPTION 
    WHEN OTHERS THEN
    pkg_logger.log_error(SQLERRM, g_app);
    pkg_logger.log_error(DBMS_UTILITY.FORMAT_ERROR_BACKTRACE, g_app);
    RETURN NULL;
END;


END PKG_WEB_PUSH_FILE; 
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_REQUEST" AS
 
  g_app varchar2(100):='PKG_WEB_PUSH_REQUEST'; 

  procedure add_web_push_request(v_elem_id number, v_reg_key varchar2, v_site_id number, v_source_addr varchar2) is
  l_exp_time number:=1;
  begin
    pkg_logger.log_debug('Adding web push request for element ' || v_elem_id || ' with key ' || v_reg_key, g_app, v_source_addr); 
    insert into web_push_requests (req_id, elem_id, req_date, expiry_date, req_key, site_id)
    values (web_push_requests_seq.nextval, v_elem_id, sysdate, sysdate+l_exp_time, v_reg_key, v_site_id);
    
    commit;
  end;
  
  procedure get_request_by_key(v_reg_key varchar2, v_items_cursor out t_cursor) is
  begin
    open v_items_cursor for
    select elem_id from web_push_requests where req_key=v_reg_key and expiry_date>=sysdate;
  end;
  
  procedure get_res_id_by_phone(p_phone_model varchar2, v_items_cursor out t_cursor) is
    cursor cur_res_id is
      select w.res_id
      from web_push_screen_res w, mob_ser_desc m, vendor_and_user_agent v
      where upper(m.mob_ven||m.mob_ser) like upper(p_phone_model||'%')
      and upper(trim(to_char(w.width)||'x'||to_char(w.height)))=replace(upper(m.mob_dimen),' ') 
      and upper(v.user_agent) = upper(m.mob_ven)
      and upper(w.phone_model) = upper(v.vendor);
    str_res_id cur_res_id%rowtype;
    
    v_res_id integer;
  begin
    open cur_res_id;
    fetch cur_res_id into str_res_id;
    
    if cur_res_id%notfound then
      v_res_id := 6;
    else 
      v_res_id := str_res_id.res_id;
    end if;
    close cur_res_id;
    open v_items_cursor for
      select v_res_id "RES_ID" from dual;
  end;

END PKG_WEB_PUSH_REQUEST;
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_SCREEN_RES" AS

  procedure get_screen_resolutions(v_elem_id number, v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor
    for select res_id, width, height, phone_model,
    (select elem_id from web_push_files where elem_id=v_elem_id and res_id=web_push_screen_res.res_id) as elem_id 
    from web_push_screen_res
    order by phone_model, width, height; 
  END get_screen_resolutions;
  
  procedure get_screen_resolutions_for(v_elem_id number, v_items_cursor out t_cursor) is 
  begin
    open v_items_cursor
    for select sr.res_id, sr.width, sr.height
    from web_push_screen_res sr inner join web_push_files f
    on sr.res_id=f.res_id
    where f.elem_id=v_elem_id and f.res_id!=7 
    order by width, height; 
  end;

END PKG_WEB_PUSH_SCREEN_RES; 
/
CREATE OR REPLACE PACKAGE BODY "OZUMUZ"."PKG_WEB_PUSH_SITE" AS

  procedure get_all_web_push_sites(v_items_cursor out t_cursor) AS
  BEGIN
    open v_items_cursor for select * from web_push_sites where alias_of_id is null;
  END get_all_web_push_sites;
  
  procedure get_site_name_by_id(v_site_id number, v_site_name out nocopy nvarchar2) is
  begin
     select site_name into v_site_name from web_push_sites
     where site_id=v_site_id;
  end;
  
  function get_site_id_by_prefix(v_msg_body nvarchar2, v_elem_code out nvarchar2) return number is
  l_site_id number:=1; --default 1
  l_first_letter nvarchar2(1);
  l_site_exists number;
  begin
  pkg_logger.log_debug(v_msg_body, 'PKG_WEB_PUSH_SITE', '');
     v_elem_code:=v_msg_body;
     
     if length(v_msg_body)<2 then
        return l_site_id;
     end if;

     l_first_letter:=substr(v_msg_body, 1, 1);
     if pkg_util.verify_numeric(l_first_letter) then
        return l_site_id;
     end if;
     
     select count(0) into l_site_exists from web_push_sites 
     where site_prefix=upper(l_first_letter);
     if l_site_exists!=1 then
        return l_site_id;
     end if;
     
     select site_id into l_site_id from web_push_sites 
     where site_prefix=upper(l_first_letter);
     
     v_elem_code:=substr(v_msg_body, 2);
     return l_site_id;
  end;

END PKG_WEB_PUSH_SITE;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."CHECK_MT_FRAUD" as
begin
 --if we have more than 30 charge requests for same number, add that number to blacklist for next 30 minutes
 for cc in (select msisdn, count(0) from diameter_requests
            where in_date>sysdate-1/24/2 
            group by msisdn
            having count(0)>=30)
 loop
    update black_list set for_vas=1, out_date=sysdate+1/2 where msisdn=cc.msisdn and out_date<sysdate+1/2;
    if sql%rowcount = 0 then
      insert into black_list (msisdn, out_date) values (cc.msisdn, sysdate+1/24/2);
    end if;
    
    commit;
 end loop;
end;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."COMPILE_INVALID_PACKAGES" AS
cursor c_inv_packages is 
  select
     owner       ,
     object_type ,
     object_name 
  from
     all_objects
  where
     status != 'VALID' and object_type='PACKAGE BODY'
  order by
     owner,
     object_type;
BEGIN
  for c in c_inv_packages
  loop
    dbms_output.put_line('Compiling: ' || c.object_name);
    execute immediate 'alter package ' || c.object_name || ' compile body';
  end loop;
END COMPILE_INVALID_PACKAGES;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."CREATE_RIGHTS" (p_base_name varchar2, p_display_name varchar2,
                                          p_index number := 1, p_add number := 1,
                                          p_edit number := 1, p_delete number := 1, p_view number := 0) as
begin
   if p_index = 1 then
      insert into ui_rights (right_id, right_name, right_desc, for_everyone)
      values
      (ui_rights_seq.nextval, p_base_name||':index', p_display_name||' - Index', 0);
   end if;
   
   if p_add = 1 then
      insert into ui_rights (right_id, right_name, right_desc, for_everyone)
      values
      (ui_rights_seq.nextval, p_base_name||':add', p_display_name||' - Add', 0);
   end if;
   
   if p_edit = 1 then
      insert into ui_rights (right_id, right_name, right_desc, for_everyone)
      values
      (ui_rights_seq.nextval, p_base_name||':edit', p_display_name||' - Edit', 0);
   end if;
   
   if p_delete = 1 then
      insert into ui_rights (right_id, right_name, right_desc, for_everyone)
      values
      (ui_rights_seq.nextval, p_base_name||':delete', p_display_name||' - Delete', 0);
   end if;
   
   if p_view = 1 then
      insert into ui_rights (right_id, right_name, right_desc, for_everyone)
      values
      (ui_rights_seq.nextval, p_base_name||':view', p_display_name||' - View', 0);
   end if;
end;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."DROP_ALL_OLD_PARTITIONS" AS
BEGIN
   drop_old_partitions('APP_LOG', 1);
   drop_old_partitions('HANDLER_QUEUE', 1);
   drop_old_partitions('HTTP_REQUESTS', 1);
   drop_old_partitions('SMPP_INCOMING', 5);
   drop_old_partitions('SMPP_OUTGOING', 5);
   drop_old_partitions('SUBS_SENT_CONTENT', 5);
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."DROP_OLD_PARTITIONS" (p_table_name varchar2, p_months_to_keep number) AS
BEGIN
   if p_months_to_keep < 1 then
      raise_application_error(-20001, 'Must keep data for at least 1 month');
   end if;
   
   FOR cc IN (SELECT partition_name, high_value --
                  FROM user_tab_partitions
                 WHERE table_name = p_table_name) LOOP
                 
        if cc.partition_name = 'P1' then
           continue;
        end if;
        
        EXECUTE IMMEDIATE
           'BEGIN
               IF sysdate >= ADD_MONTHS(' || cc.high_value || ', '|| p_months_to_keep ||') THEN
                  dbms_output.put_line(''Dropping parititon '|| cc.partition_name ||' '');
                  EXECUTE IMMEDIATE ''ALTER TABLE '|| p_table_name ||' DROP PARTITION '|| cc.partition_name || ' UPDATE GLOBAL INDEXES '';
               END IF;
            END;';
    END LOOP;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."FIX_DOLLAR" 
is
begin
  update smpp_incoming
  set msg_body=substr(msg_body, 4)
  where in_date >= trunc(sysdate-30)
  and substr(msg_body, 1,1) = '$'
  and ascii(substr(msg_body, 2,1)) = 1
  and ascii(substr(msg_body, 3,1)) = 1
  and ascii(substr(msg_body, 4,1)) != 1;
end;
 
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."INCREASE_CAR_PIC_COUNTER" (p_car_id number) AS
BEGIN
   update cars set pic_view_count = pic_view_count + 1 where car_id = p_car_id;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."INCREASE_RE_PIC_COUNTER" (p_re_id number) AS
BEGIN
   update real_estates set re_pic_view_count = re_pic_view_count + 1 where re_id = p_re_id;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."RUN_REPORT_EVSMS_OUTOFCONTENT" AS
BEGIN
   begin
      execute immediate 'drop table report_evsms_outofcontent';
   exception when others then null; end;
   
   execute immediate '
   create table report_evsms_outofcontent as
   select * from (
   select c.msisdn, 
   (select count(0) from active_real_estates re where (p.pref_val is null or re.re_sell_type_id=to_number(p.pref_val)) 
    and not exists (select 0 from subs_sent_content where content_id = ''EvSMS_''||re.re_id and msisdn=c.msisdn)) as remaining_count
   from active_subs_customers_view c
   left join subs_preferences p on (p.msisdn=c.msisdn and p.pref_name=''EVSMS_SELL_TYPE'')
   where c.subs_id=1) where remaining_count <= 20
   ';
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."SEND_EMAIL" (
  v_From      VARCHAR2,
  v_Recipients VARCHAR2,
  v_Subject   VARCHAR2,
  v_Body      VARCHAR2
) as
  v_Mail_Host VARCHAR2(30) := 'mail.nms.az';
  v_Mail_Conn utl_smtp.Connection;
  crlf        VARCHAR2(2)  := chr(13)||chr(10);
BEGIN
 v_Mail_Conn := utl_smtp.Open_Connection(v_Mail_Host, 25);
 utl_smtp.Helo(v_Mail_Conn, v_Mail_Host);
 utl_smtp.Mail(v_Mail_Conn, v_From);
 for cc in (select column_value from table(pkg_util.get_string_table(v_Recipients, ';')))
 loop
  utl_smtp.Rcpt(v_Mail_Conn, cc.column_value);
 end loop;
 utl_smtp.Data(v_Mail_Conn,
   'Date: '   || to_char(sysdate, 'Dy, DD Mon YYYY hh24:mi:ss') || crlf ||
   'From: '   || v_From || crlf ||
   'Subject: '|| v_Subject || crlf ||
   'To: '     || v_Recipients || crlf ||
   crlf ||
   v_Body
 );
 utl_smtp.Quit(v_mail_conn);
--EXCEPTION
 --WHEN utl_smtp.Transient_Error OR utl_smtp.Permanent_Error then
   --raise_application_error(-20000, 'Unable to send mail: '||sqlerrm);
   --null;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."SMPPALERT" IS
tmpVar NUMBER;
BEGIN
  pkg_sms_sender.send_sms('1', '994556671666', 'OK!', '1', 'DOWNLOAD');
END SMPPALERT; 
 
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."WEB_ADD_CAR" (p_model_id number, p_price number, p_tt_id number, p_ft_id number, p_color_id number,
                                        p_power number, p_year number, p_mileage number, p_credit number,
                                        p_comments varchar2, p_contact_person varchar2, p_contact_number varchar2,
                                        o_new_id out number) AS
BEGIN
   INSERT INTO CARS (CAR_ID, STATUS, MODEL_ID, PRICE, TT_ID, FT_ID, COLOR_ID, HORSE_POWER, 
             YEAR_OF_MANUF, MILEAGE, CREDIT, COUNTRY_ID, COMMENTS, 
             CONTACT_PERSON, CONTACT_NUMBER, SMS_TITLE, SMS_BODY, ADDED_BY, UNIQID)
   VALUES (cars_seq.nextval, 'pending', p_model_id, p_price, p_tt_id, p_ft_id, p_color_id, p_power, p_year, p_mileage, p_credit, 1, 
    substr(p_comments, 1, 250), substr(p_contact_person, 1, 50), substr(p_contact_number, 1, 50), null, null, 'web_user', DBMS_RANDOM.string('l',15)) RETURNING CAR_ID INTO o_new_id;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."WEB_ADD_CAR_FILE" (p_car_id number, p_file_name varchar2, p_content_type varchar2, o_new_id out number) AS
   --declarations
BEGIN
    insert into ozumuz.files (file_id, file_name, content_type, uniqid)
    values (files_seq.nextval, p_file_name, p_content_type, DBMS_RANDOM.string('l',15))
    returning file_id into o_new_id;
                
    insert into ozumuz.cars_files (rec_id, car_id, file_id)
    values (cars_files_seq.nextval, p_car_id, o_new_id);
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."WEB_CHECK_PIN" (p_subs_id number, p_pin_code char, o_result out number) AS
   l_is_member number;
   l_msisdn varchar2(50);
   l_dummy number;
BEGIN
   select max(msisdn) into l_msisdn from pin_codes where pin_code = p_pin_code and date_added > sysdate - 1/12;
   
   if l_msisdn is null then
      o_result := 2;
      return;
   end if;
   
   select case when exists (select 0 from active_subs_customers_view where msisdn=l_msisdn and subs_id=p_subs_id) then 1 else 0 end into l_is_member from dual;
   
   if l_is_member = 1 then
      o_result := 3;
      return;
   end if;
   
   delete from pin_codes where msisdn = l_msisdn and pin_code = p_pin_code;
   
   pkg_smpp.register_received_sms ('BAKCELL1', sysdate, 0, l_msisdn, '6090', '1', 'E'||DBMS_RANDOM.VALUE, l_dummy);
   
   o_result := 1;
END;
/
CREATE OR REPLACE PROCEDURE "OZUMUZ"."WEB_SEND_PIN" (p_subs_id number, p_msisdn varchar2, o_response_code out number) AS
   l_is_member number;
   l_pin number;
BEGIN
   select case when exists (select 0 from active_subs_customers_view where msisdn=p_msisdn and subs_id=p_subs_id) then 1 else 0 end into l_is_member from dual;
   
   if l_is_member = 1 then
      o_response_code := 2;
      return;
   end if;
   
   select max(pin_code) into l_pin from pin_codes where msisdn = p_msisdn and date_added > sysdate - 1/12;
   
   if l_pin is null then
      l_pin := trunc(DBMS_RANDOM.VALUE(1000, 9999));
      insert into pin_codes (msisdn, pin_code) values (p_msisdn, l_pin);
   end if;
   
   pkg_sms_sender.send_sms('BAKCELL1', '6090', p_msisdn, 
                     'PIN: '||l_pin, 0, 1, 
                     'WEB_SEND_PIN');
   o_response_code := 1;
END;
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."BITOR" (x IN NUMBER, y IN NUMBER) RETURN NUMBER AS
BEGIN
  RETURN (x + y - BITAND(x, y));
END;
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."MD5" (p_value varchar2) return varchar2 as
  v_result dbms_obfuscation_toolkit.varchar2_checksum;
begin
  dbms_obfuscation_toolkit.md5 (
      input_string => p_value, checksum_string => v_result
  );
  return v_result;
end;
 
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."TO_ASCII" (p_str varchar2) RETURN NUMBER AS
BEGIN  
   return REGEXP_REPLACE(p_str,'[^[\x80-\xFF]]',' ');
END;
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."TO_STRING" (
              nt_in        IN ntt_varchar2,
              delimiter_in IN VARCHAR2 DEFAULT ','
              ) RETURN VARCHAR2 IS
  
     v_idx PLS_INTEGER;
     v_str VARCHAR2(32767);
     v_dlm VARCHAR2(10);
  
  BEGIN
  
     v_idx := nt_in.FIRST;
     WHILE v_idx IS NOT NULL LOOP
        v_str := v_str || v_dlm || nt_in(v_idx);
        v_dlm := delimiter_in;
        v_idx := nt_in.NEXT(v_idx);
     END LOOP;
   
     RETURN v_str;
  
  END to_string;
 
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."URLENCODE" ( p_str in varchar2 ) return varchar2
as
    l_tmp   varchar2(6000 char);
    l_bad   varchar2(100 char) default ' >%}\~];?@&<#{|^[`/:=$+''"';
    l_char  char(1 char);
begin
    for i in 1 .. nvl(length(p_str),0) loop
        l_char :=  substr(p_str,i,1);
        if ( instr( l_bad, l_char ) > 0 )
        then
            l_tmp := l_tmp || '%' ||
                            to_char( ascii(l_char), 'fmXX' );
        else
            l_tmp := l_tmp || l_char;
        end if;
    end loop;
  
    return l_tmp;
end;
/
CREATE OR REPLACE FUNCTION "OZUMUZ"."URLENCODE2" ( p_str in varchar2 ) return varchar2
as
    l_tmp   varchar2(6000);
    l_bad   varchar2(100) default ' >%}\~];?@&<#{|^[`/:=$+''"';
    l_char  char(1);
begin
    for i in 1 .. nvl(length(p_str),0) loop
        l_char :=  substr(p_str,i,1);
        if ( instr( l_bad, l_char ) > 0 )
        then
            l_tmp := l_tmp || '%' ||
                            to_char( ascii(l_char), 'fmXX' );
        else
            l_tmp := l_tmp || l_char;
        end if;
    end loop;
  
    return l_tmp;
end;
 
/
