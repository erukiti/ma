#include <malloc.h>
#include "ma.h"
#include "mio.h"
#include "str.h"

bool	wfc_greet()
{
	uint	i;
	uint	u;
	char	buf[LN_buf+1];

	killbuf();
	ch.frug.esc=FALSE;
	putz(ESC_csron ESC_norm);
	ch.frug.esc=TRUE;
	disp_text("connect");

	for (i=0;i<5;++i)
		{
		 putz("login: ");
		 guide_up("login: ");
		 getstri(buf,LN_id);
		 u=ulist_search(buf);
		 if (u==ERROR)
		 	{
		 	 msgout(IC_err "そのようなIDは存在しません。");
		 	 continue;
		 	}

		 if (u==0)
		 	{
		 	 msgout(IC_act "お名前を入力してください。");
		 	 getstr(buf);
		 	 user_guest(buf);
		 	 return TRUE;
		 	}
		 putz("Password:");
		 getpass(buf);
		 if (!ulist_chkpass(buf,u))
		 	{
		 	 msgout(IC_err "Passwordが違います。");
			 sprt(buf,"!!Password間違い(",ulist_getid(u),")",NULL);
		 	 sys_log(buf);
		 	 ulist_incwp(u);
		 	 continue;
		 	}
		 i=ulist_getwp(u);
		 user_set(u);
		 ulist_setwp(u,i);
		 return TRUE;
		}
	return FALSE;
}

void	wfc_nowtime()
{
/* !! */
	disp_text(lpu(gethour(gettime()),0));
}

bool	wfc_birthday(uint i)
{
	struct tm	tm;

	if (i==0 ||i>user.member)
		return FALSE;

	setdatime(getdate(),&tm);
	return tm.tm_mon ==ulist_getbirthm(i) &&
			tm.tm_mday ==ulist_getbirthd(i);
}

void	wfc_today()
{
	FILE	*fp;
	char	day[16];
	char	buf[LN_buf+1],*s;
	struct tm	tm;

	setdatime(getdate(),&tm);


	disp_text(dwstr[tm.tm_wday]);


	fp = op_text("flower.tbl");
	if (fp==NULL)
		error("error");

	sprt(day,lpu(tm.tm_mon,2),"-",lpu(tm.tm_mday,2),NULL);
	while(fgets(buf,sizeof(buf),fp))
		{
		 if (strncmp(day,buf,5)==0)
		 	{
		 	 s=buf+6;
		 	 while(!isspace(*s)) ++s;
		 	 *s++='\0';
		 	 while(isspace(*s)) ++s;

			 *(strchr(s,'\n'))='\0';

			 prt(IC_ok "今日の花言葉は"FC_str2_s ,buf+6,
			 	FC_str2_e  "で、",s,"という意味です。\n"
			 	,NULL);
			 break;
		 	}
		}
	fclose (fp);
	disp_text(day);

	if (wfc_birthday(user.number))
		{
		 msgout("今日はあなたの誕生日です。");
		}
}

void	wfc_welcome()
{
	char	buf[LN_buf+1];
	uint	wp;
	time_t	t;
	uint	i;

	sprt(buf,"Welcome to the ",sys.netname,NULL);
	lo_line(buf);
	disp_text("logging");

	lo_line2();

	t=atol(getvar("lastlog",user.var));
	i=atoi(getvar("times",user.var));
	if (getdate()!=t/DT_daytime)
		i=1; else
		++i;
	setvar("times",lpu(i,0),user.var);
	if (t==0)
		msgout( "初めてのログインです。"); else
		{
		 prt("前回のログイン日時は",stredatime(t),
		 	"でした。\n",NULL);
		 prt(IC_ok "今日",lpu(i,0), "回目のログインです。\n"
		 	,NULL);
		 prt(IC_ok "このログインまでに"
		 	,strptime(getdatime()-t),"経過してます。\n",NULL);
		}

	if (user.number>0)
		{
		 wp=ulist_getwp(user.number);
		 if (wp>0)
			prt(IC_err  "その間、",lpu(wp,0),
				"回のパスワード間違いがありました。\n",NULL);
		 ulist_clrwp(user.number);
		}

	prt("\n今日は",strdate(getdate()),"です。\n"
		,NULL);
	wfc_today();
	msgout("");

	prt("ただいまの時刻は",strtime(getdatime())
		,"です。\n",NULL);
	wfc_nowtime();
	msgout("");

	lo_line1();
}


void	log_fin()
{
	char	buf[LN_buf+1];

	user_write();
	sprt(buf,nps(ch.result,5),DC_list
		,lpu(gethour(ch.login),-2) ,DC_norm
		,lpu(getmin(ch.login),2) ,DC_area
		,lpu(gethour(gettime()),-2),DC_norm
		,lpu(getmin(gettime()),2)," "
		,nps(user_handle(),LN_handle)
		,FC_str2_s ,nps(ch.dengon,LN_dengon),FC_str2_e ,NULL);
	add_text("caller",buf);
}

SHELL	void	wfc_connect(char *s)
{
	char	buf[LN_buf+1];

	strcpy(ch.result,(*s=='\0'||s==NULL)?
		getvar(lpu(getrnumber(0),0),sys.result):s);

	sprt(buf,"--着信 [",ch.result,"]",NULL);
	sys_log(buf);

	if (!wfc_greet())
		{
		 wfc_set();
		 return;
		}

	atescape(log_fin);

	timer_res(ch.pass);
	sprt(buf,"| login[",user_id(),"]",NULL);
	sys_log(buf);
	ch.login =getdatime();
	*ch.dengon ='\0';

	++sys.logon;
	++sys.t_logon;
	if (user.number>0)
		++ult(user.number)->mlogin;

	wfc_welcome();

	setvar("lastlog",lpu(ch.login,0),user.var);

	where_sett("main");
	where_exec(where_comnum("\\login"));

	user_chk(0);

	stage_in("mail");
	note_mk("mail");
	mptr_clr();
	mail_chk(0);
}

SHELL	void	wfc_dengon(char *s)
{
	if (*s!='\0')
		{
		 if (*ch.dengon=='\0')
			strjncpy(ch.dengon,s,LN_dengon);
		 return;
		}

	msgout(IC_act "伝言板に書き込むメッセージを入力してください。");
	msgout("    無ければリターンのみ(でもなるべく入れてね)。");
	getrstr(ch.dengon);
}

void	wfc_set()
{
/*dbprt("Wfc_set:",lpu(where.max_escape,0),"\n",NULL);*/
	ch.frug.discut=FALSE;
	autoescape(0);
	stack_del();
	strcpy(user.style,"wfc");
	where_set("wfc");
	ch.frug.monitor=1;

}

SHELL	void	logout()
{
	putz(IC_dact "ログアウトしますか？");
	if (!ynq())
		return;
	wfc_dengon("");
	qlogout();
}

SHELL	void	qlogout()
{
	sys_log("| logout");
	msgout(IC_atn"お友達のネットですぅ～");
	disp_text("bbslist");
	msgout("");
	disp_text("fortune");
	ch.frug.cd=0;
	escape();
	wait_send();
	wfc_set();
}

SHELL	void	modem_send(const char *s)
{
	char	buf[LN_buf+1];

	if (!dsr())
		{
		 msgout(IC_nok "モデムが接続されていません。");
		 return;
		}

	if (s!=NULL&*s!='\0')
		strcpy(buf,s); else
		{
		 msgout(IC_act "モデムに送るコマンドを入力してください。");
		 getstr(buf);
		}
	prt("Modem command:[",buf,"]\n",NULL);
	strcat(buf,"\r");
	rsputs(buf);
	wait_minute(1);
}

SHELL	void	modem_tsend(const char *s)
{
	if (isopentime(gettime()))
		{
		 msgout("着信処理を行います。");
		 modem_send(s);
		}
}

SHELL	void	cron()
{
	struct	tm	tmo,tmn;
	time_t	t;

	t=atol(getvar("nowday",sys.var));
	setdatime(t,&tmo);
	t=getdate();
	setdatime(t,&tmn);

 	setvar("nowday",lpu(t,0),sys.var);
	where_exec(where_comnum("day"));

	if (tmo.tm_mon!=tmn.tm_mon)
		where_exec(where_comnum("month"));

	if (tmo.tm_year!=tmn.tm_year)
		where_exec(where_comnum("year"));

	where_setr();
}

SHELL	void	cron_day()
{
/* 一日毎の処理 */
	char	buf[LN_buf];

	msgout("\r" IC_ok "日付変更処理を行っています。");

	sys_log ("------------");
	sprt(buf,"書き込み量  :",lpu(sys.t_post,0),NULL);
	sys_log(buf);
	sprt(buf,"ログイン回数:",lpu(sys.t_logon,0),NULL);
	sys_log(buf);
	sprt(buf,"------------",NULL);
	sys_log(buf);

	sys.t_logon=0;
	sys.t_post=0;

	sprt(buf,"  --- ",strdate(getdate())," ---",NULL);
	add_text("caller",buf);
}

SHELL	void	cron_month()
{
/* 月ごとの処理 */
	uint	i;

	msgout( IC_ok "月初めの処理を行っています。");
	op_user();
	for(i=1;i<=user.member;++i)
		{
		 ult(i)->post +=ulist_getmpost (i);
		 ult(i)->login+=ulist_getmlogin(i);
		 ulist_clrmpost (i);
		 ulist_clrmlogin(i);

		 ulist_set(i,nowvar);
		}
	sion_close();
}

SHELL	void	cron_year()
{
/* 年毎の処理 */
	msgout( IC_ok "年初めの処理を行っています。");
}

bool	wfc_rsgets(char *s)
{
#if 1
	char	*p;
	uint	c;

	p=s;
	s+=strlen(s);

	for(;;)
		{
		 c=rsgetc();
/*prt("[",cps(c),":",lpx(c,4),"]\n",NULL);*/
		 switch(c)
		 	{
		  case ERROR:
		  	 break;
		  case '!':
		  	 strcpy(p,"!");
		  	 return TRUE;
		  case '\r':
		  	 *s='\0';
/*		  	 if (*p=='\0')
		  	 	return FALSE;*/
		  	 return TRUE;
		  case '\n':
		  	 continue;
		  default:
		  	 *s++=(char)(c&0xff);
		  	 if (p-s>=LN_buf)
		  	 	s=p;
		  	 continue;
		 	}
		 break;
		}
	*s='\0';
	return FALSE;
#else
	char	*p;
	char	c;

	p=s;
	s+=strlen(s);

	while(rsloc()>0)
		{
		 c=rsgetc();
		 switch(c)
		 	{
		  case '!':
		  	 strcpy(p,"!");
		  	 return TRUE;
		  case '\r':
		  	 *s='\0';
/*			 if (*p=='\0')
			 	return FALSE;*/
			 return TRUE;
		  case '\n':
		  	 continue;
		  	}
		 *s++=c;
		 if (p-s>=LN_buf)
		 	s=p;
		}
	*s='\0';
	return FALSE;
#endif
}

SHELL	void	cs_wfc()
{
	char	buf[LN_buf+1];
	char	recbuf[LN_buf+1];
	time_t	t,d;
	uint	i,j,k,l;

/*	killbuf();*/
/*	rsbufs(FLWSIG_ON2);*/

/*	wfc_set();*/
	timer_res(t);
	--t;
	*recbuf='\0';
	j=ERROR;
	k=0;
	l=0;
	d=atol(getvar("nowday",sys.var));
/* prt("Stackavail:",lpx(stackavail(),4),"\n",NULL); */

	for(;;)
		{
		 int28h();
		 i=rsstat();
		 if (i!=j|| k!=rsloc()|| l!=rssloc())
		 	{
			 putz(ESC_save ESC_csroff ESC_locate0);
		 	 modem_chk(i);
		 	 j=i;
		 	 k=rsloc();
		 	 l=rssloc();
			 putz(ESC_unsave ESC_csron);
		 	}
		 if (timer_pas(t)>0)
		 	{
			 if (isopentime(gettime()))
			 	putz(SC_open ESC_csroff "\r･"); else
			 	putz(ESC_csroff "\r ");
			 prt(stredatime(getdatime())," %"  ESC_norm ESC_csron,NULL);
			 if (d!=getdate())
			 	{
			 	 where_sett("cron");
			 	 return;
			 	}
			 timer_res(t);
			}
		 if (kbhit()!=0)
		 	{
		 	 buf[0]=getche();
		 	 buf[1]='\0';
		 	 if (buf[0]=='\r')
		 	 	buf[0]='\0';
			 break;
			}
		 if (wfc_rsgets(recbuf))
			{
dbprt("[",recbuf,"]",NULL);
		  	 if (!isdigit(*recbuf)&&*recbuf!='!')
		  	 	{
dbmsgout("");
		  	 	 *recbuf='\0';
		  	 	 continue;
		  	 	}
		 	 prt("[",recbuf,"]",NULL);
		 	 strcpy(buf,recbuf);
		 	 break;
		 	}
		}
	msgout("");
	where_shell(buf);
}

SHELL	void	called_chg(uint u)
{
	ch.frug.called = u>0?1:0;
}

SHELL	void	modem_init()
{
	char	buf[LN_buf+1];
	time_t	t;

	ch.frug.esc=FALSE;
	ch.frug.cd=FALSE;
	called_chg(0);

	msgout(IC_ok "Modemを初期化します。");
	modem_dtr(0);
	timer_res(t);
	while(dcd())
		{
		 if (timer_pas(t)>10)
		 	{
		 	 msgout("cd not clear");
			 longjmp(ch.env,1);
			}
		}

	modem_dtr(1);
	wait_minute(1);

	if (!dsr())
		{
		 msgout(IC_nok "モデムが接続されていません。");
		 return;
		}
	killbuf();
	modem_send(" ");
	modem_send("ATZ");
	wfc_rsgets(buf);
	modem_send(getvar("modem",sys.var));
}

SHELL	void	wfc_modem()
{
	time_t	t;

	ch.frug.cd= TRUE;
	timer_res(t);
	while(!dcd())
		{
		 if (timer_pas(t)>10)
		 	{
		 	 ch.frug.cd=FALSE;
		 	 msgout("CD信号がonになりません。");
			 longjmp(ch.env,1);
			}
		}
	wait_minute(1);
}

SHELL	void	wfc_status()
{
	char	recbuf[LN_buf+1];

	lo_line("Modem Status");
	*recbuf='\0';
	for(;;)
		{
		 if (kbhit()!=0)
		 	{
			 getch();
		 	 msgout("中断しました。");
		 	 return;
		 	}
		 if (rsloc()!=0)
		 	{
			 if (wfc_rsgets(recbuf))
			 	{
			 	 if (isdigit(*recbuf)||strcmp(recbuf,"OK")==SAME)
			 	 	break;
			 	 msgout(recbuf);
			 	 *recbuf='\0';
			 	}
			}
		}
dbmsgout(recbuf);
	lo_line1();
}
