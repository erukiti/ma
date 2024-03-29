/* 102�� */
#include "ma.h"
#include "str.h"

flist_t	flist=
	{
		{
		 "msgout"
,		 "disp_text"
,		 "disp_four"
,		 "disp_sixt"
,		 "sys_fin"
,		 "copyright"
,		 "end"
,		 "null_op"
,		 "wait_send"
,		 "modem_dtr"
,		 "brk_send"
,		 "where_seta"
,		 "where_setr"
,		 "where_sett"
,		 "where_set"
,		 "cs_norm"
,		 "pr_norm"
,		 "escape"
,		 "autoescape"
,		 "forth_chkpsp"
,		 "system"
,		 "sys_log"
,		 "infomation"
,		 "esc_chg"
,		 "one_chg"
,		 "timepas"
,		 "wfc_connect"
,		 "qlogout"
,		 "logout"
,		 "modem_send"
,		 "modem_tsend"
,		 "cron"
,		 "cron_day"
,		 "cron_month"
,		 "cron_year"
,		 "called_chg"
,		 "modem_init"
,		 "wfc_modem"
,		 "wfc_status"
,		 "cs_wfc"
,		 "wfc_dengon"
,		 "editor"
,		 "line_write"
,		 "line_list"
,		 "line_in"
,		 "line_start"
,		 "line_fin"
,		 "line_main"
,		 "line_name"
,		 "line_last"
,		 "BBS_add"
,		 "chat"
,		 "enter"
,		 "readquit"
,		 "vscan"
,		 "ascan"
,		 "tescan"
,		 "nscan"
,		 "mptr_day"
,		 "mptr_chk"
,		 "mptr_write"
,		 "mptr_last"
,		 "mptr_clr"
,		 "mail_rep"
,		 "mail_ent"
,		 "res"
,		 "post"
,		 "post_list"
,		 "art_del"
,		 "art_read"
,		 "number_plus"
,		 "number_minus"
,		 "number_number"
,		 "rf_chg"
,		 "number_kill"
,		 "note_mk"
,		 "note_info"
,		 "note_list"
,		 "note_list2"
,		 "board_first"
,		 "board_number"
,		 "board_plus"
,		 "board_minus"
,		 "mail_chk"
,		 "board_in"
,		 "news_prompt"
,		 "stage_in"
,		 "art_prof"
,		 "mask_set"
,		 "mask_chk"
,		 "ulist_disp"
,		 "user_set"
,		 "user_write"
,		 "user_disp"
,		 "user_chk"
,		 "user_def"
,		 "sign_up"
,		 "prof_write"
,		 "prof_myview"
,		 "prof_view"
,		 "file_dnload"
,		 "remote_exec"
,		 ""
		},
		{
		 (uint(*)(uarg_t,uarg_t))msgout
,		 (uint(*)(uarg_t,uarg_t))disp_text
,		 (uint(*)(uarg_t,uarg_t))disp_four
,		 (uint(*)(uarg_t,uarg_t))disp_sixt
,		 (uint(*)(uarg_t,uarg_t))sys_fin
,		 (uint(*)(uarg_t,uarg_t))copyright
,		 (uint(*)(uarg_t,uarg_t))end
,		 (uint(*)(uarg_t,uarg_t))null_op
,		 (uint(*)(uarg_t,uarg_t))wait_send
,		 (uint(*)(uarg_t,uarg_t))modem_dtr
,		 (uint(*)(uarg_t,uarg_t))brk_send
,		 (uint(*)(uarg_t,uarg_t))where_seta
,		 (uint(*)(uarg_t,uarg_t))where_setr
,		 (uint(*)(uarg_t,uarg_t))where_sett
,		 (uint(*)(uarg_t,uarg_t))where_set
,		 (uint(*)(uarg_t,uarg_t))cs_norm
,		 (uint(*)(uarg_t,uarg_t))pr_norm
,		 (uint(*)(uarg_t,uarg_t))escape
,		 (uint(*)(uarg_t,uarg_t))autoescape
,		 (uint(*)(uarg_t,uarg_t))forth_chkpsp
,		 (uint(*)(uarg_t,uarg_t))system
,		 (uint(*)(uarg_t,uarg_t))sys_log
,		 (uint(*)(uarg_t,uarg_t))infomation
,		 (uint(*)(uarg_t,uarg_t))esc_chg
,		 (uint(*)(uarg_t,uarg_t))one_chg
,		 (uint(*)(uarg_t,uarg_t))timepas
,		 (uint(*)(uarg_t,uarg_t))wfc_connect
,		 (uint(*)(uarg_t,uarg_t))qlogout
,		 (uint(*)(uarg_t,uarg_t))logout
,		 (uint(*)(uarg_t,uarg_t))modem_send
,		 (uint(*)(uarg_t,uarg_t))modem_tsend
,		 (uint(*)(uarg_t,uarg_t))cron
,		 (uint(*)(uarg_t,uarg_t))cron_day
,		 (uint(*)(uarg_t,uarg_t))cron_month
,		 (uint(*)(uarg_t,uarg_t))cron_year
,		 (uint(*)(uarg_t,uarg_t))called_chg
,		 (uint(*)(uarg_t,uarg_t))modem_init
,		 (uint(*)(uarg_t,uarg_t))wfc_modem
,		 (uint(*)(uarg_t,uarg_t))wfc_status
,		 (uint(*)(uarg_t,uarg_t))cs_wfc
,		 (uint(*)(uarg_t,uarg_t))wfc_dengon
,		 (uint(*)(uarg_t,uarg_t))editor
,		 (uint(*)(uarg_t,uarg_t))line_write
,		 (uint(*)(uarg_t,uarg_t))line_list
,		 (uint(*)(uarg_t,uarg_t))line_in
,		 (uint(*)(uarg_t,uarg_t))line_start
,		 (uint(*)(uarg_t,uarg_t))line_fin
,		 (uint(*)(uarg_t,uarg_t))line_main
,		 (uint(*)(uarg_t,uarg_t))line_name
,		 (uint(*)(uarg_t,uarg_t))line_last
,		 (uint(*)(uarg_t,uarg_t))BBS_add
,		 (uint(*)(uarg_t,uarg_t))chat
,		 (uint(*)(uarg_t,uarg_t))enter
,		 (uint(*)(uarg_t,uarg_t))readquit
,		 (uint(*)(uarg_t,uarg_t))vscan
,		 (uint(*)(uarg_t,uarg_t))ascan
,		 (uint(*)(uarg_t,uarg_t))tescan
,		 (uint(*)(uarg_t,uarg_t))nscan
,		 (uint(*)(uarg_t,uarg_t))mptr_day
,		 (uint(*)(uarg_t,uarg_t))mptr_chk
,		 (uint(*)(uarg_t,uarg_t))mptr_write
,		 (uint(*)(uarg_t,uarg_t))mptr_last
,		 (uint(*)(uarg_t,uarg_t))mptr_clr
,		 (uint(*)(uarg_t,uarg_t))mail_rep
,		 (uint(*)(uarg_t,uarg_t))mail_ent
,		 (uint(*)(uarg_t,uarg_t))res
,		 (uint(*)(uarg_t,uarg_t))post
,		 (uint(*)(uarg_t,uarg_t))post_list
,		 (uint(*)(uarg_t,uarg_t))art_del
,		 (uint(*)(uarg_t,uarg_t))art_read
,		 (uint(*)(uarg_t,uarg_t))number_plus
,		 (uint(*)(uarg_t,uarg_t))number_minus
,		 (uint(*)(uarg_t,uarg_t))number_number
,		 (uint(*)(uarg_t,uarg_t))rf_chg
,		 (uint(*)(uarg_t,uarg_t))number_kill
,		 (uint(*)(uarg_t,uarg_t))note_mk
,		 (uint(*)(uarg_t,uarg_t))note_info
,		 (uint(*)(uarg_t,uarg_t))note_list
,		 (uint(*)(uarg_t,uarg_t))note_list2
,		 (uint(*)(uarg_t,uarg_t))board_first
,		 (uint(*)(uarg_t,uarg_t))board_number
,		 (uint(*)(uarg_t,uarg_t))board_plus
,		 (uint(*)(uarg_t,uarg_t))board_minus
,		 (uint(*)(uarg_t,uarg_t))mail_chk
,		 (uint(*)(uarg_t,uarg_t))board_in
,		 (uint(*)(uarg_t,uarg_t))news_prompt
,		 (uint(*)(uarg_t,uarg_t))stage_in
,		 (uint(*)(uarg_t,uarg_t))art_prof
,		 (uint(*)(uarg_t,uarg_t))mask_set
,		 (uint(*)(uarg_t,uarg_t))mask_chk
,		 (uint(*)(uarg_t,uarg_t))ulist_disp
,		 (uint(*)(uarg_t,uarg_t))user_set
,		 (uint(*)(uarg_t,uarg_t))user_write
,		 (uint(*)(uarg_t,uarg_t))user_disp
,		 (uint(*)(uarg_t,uarg_t))user_chk
,		 (uint(*)(uarg_t,uarg_t))user_def
,		 (uint(*)(uarg_t,uarg_t))sign_up
,		 (uint(*)(uarg_t,uarg_t))prof_write
,		 (uint(*)(uarg_t,uarg_t))prof_myview
,		 (uint(*)(uarg_t,uarg_t))prof_view
,		 (uint(*)(uarg_t,uarg_t))file_dnload
,		 (uint(*)(uarg_t,uarg_t))remote_exec
,		 NULL
		},
		{
		 1
,		 1
,		 1
,		 1
,		 0
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 1
,		 1
,		 1
,		 1
,		 0
,		 1
,		 0
,		 1
,		 1
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 1
,		 1
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 0
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 0
,		 0
,		 1
,		 0
,		 0
,		 0
,		 1
,		 1
,		 0
,		 0
,		 0
,		 1
,		 0
,		 1
,		 0
,		 0
,		 0
,		 0
,		 0
,		 0
,		 0
,		 1
,		 1
,		 0
,		 1
,		 0
,		 0
,		 0
,		 1
,		 1
,		 0
,		 0
,		 1
,		 1
,		 0
,		 0
,		 0
,		 0
,		 0
,		 1

		}
	};
