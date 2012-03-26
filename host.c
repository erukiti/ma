#define	S_Data	1
#include "ma.h"
#include "board.h"
#include "str.h"
#include "mio.h"
#include "pool.h"

SHELL	void	sys_log(char *s)
{
	char	buf[LN_buf+1];

	sprt(buf,stredatime(getdatime())," ",s,NULL);
	add_text("syslog",buf);
}

void	sys_init()
{
	char	*p,buf[LN_buf+1];
	char	t;
	time_t	e,s;

	ch.frug.called=0;
	ch.frug.monitor=1;
	ch.frug.esc=0;
	ch.fp=NULL;
	sys.rs=FALSE;
/*	ch.env=NULL;*/

/*	debug_start();*/
	ed_init();
	sion_init();
	nowvar=varinit(LN_str,MAX_var);
	sys.var=varinit(LN_str,MAX_var);
	sys.result=varinit(LN_result,MAX_var);

	strcpy(sys.home,".");
	op_system();
	if (sion_sr("config","system")==0)
		{
		 exit(1);
		}
	sion_read(sys.var);
	sion_sr("config","result");
	sion_read(sys.result);

	sion_close();

	sys.execute=atoi(getvar("execute",sys.var))+1;
	sys.logon  =atoi(getvar("logon",sys.var));
	sys.post   =atoi(getvar("post",sys.var));

	sys.t_logon=atoi(getvar("tdlogon",sys.var));
	sys.t_post =atoi(getvar("tdpost",sys.var));

	strjncpy(sys.netname,getvar("netname",sys.var),LN_title);
	strjncpy(sys.netid,getvar("netid",sys.var),LN_netid);
	strjncpy(sys.home ,getvar("home",sys.var),LN_dirs);
	strjncpy(sys.temp ,getvar("temp",sys.var),LN_dirs);
	pool.max=atoi(getvar("poolmax",sys.var));

	copyright();

	p=getvar("opentime",sys.var);
	tzset();	/* 時空関数がまだ現れていないので呼ぶ必要あり */

	if (p==NULL)
		{
		 sys.opentime=DT_daytime;
		 sys.starttime=0;
		}else
		{
		 p=pull(buf,p);
		 t=atoi(buf);
		 p=pull(buf,p);
		 s=(t*60+atoi(buf))*60;
		 p=pull(buf,p);
		 t=atoi(buf);
		 p=pull(buf,p);
		 e=(t*60+atoi(buf))*60;
/*dbprintf("s:%ld,e:%ld",s,e);*/
		 sys.opentime=e+(e<s?DT_daytime:0)-s;
		 sys.starttime=DT_daytime-s;
/*dbprintf("o:%ld,s:%ld",sys.opentime,sys.starttime);*/
		}

	stack_init();
	where_init();
	user_init();
	art_init();
	pool_init();

	sys.rs=rs_init();
/*	sys_log("●システム起動");*/
}

SHELL	void	sys_fin()
{
	setvar("member",lpu(user.member,0),sys.var);
	setvar("execute",lpu(sys.execute,0),sys.var);
	setvar("logon",lpu(sys.logon,0),sys.var);
	setvar("post",lpu(sys.post,0),sys.var);
	setvar("poolmax",lpu(pool.max,0),sys.var);

	setvar("tdlogon",lpu(sys.t_logon,0),sys.var);
	setvar("tdpost" ,lpu(sys.t_post,0),sys.var);

	ulist_write();

	op_system();
	sion_sr("config","system");
	sion_write(NULL,sys.var);
	varfin(sys.result);
	varfin(sys.var);
	varfin(nowvar);
	sion_close();
/*	sys_log("●システム終了");*/
}

SHELL	void	end()
{
	rs_fin();
	art_fin();
	ed_fin();
	user_fin();
	sion_fin();
	pool_fin();

	msgout("終了します");
	exit(1);
}

main()
{
	sys_init();

	if (setjmp(ch.env)>0)
		{
		 ch.frug.called=0;
		 ch.frug.cd=0;
/*		 wfc_modem(0); の処理は中にwait_minute(2)があるから････*/
		 sys_log("! ぶっち");
		 msgout("回線切断されました。");
		}
	wfc_set();

	for(;;)
		{
		 shell();
		}
}

SHELL	void	infomation()
{
	lo_line(stredatime(getdatime()));
	prt("システム起動回数:",lpu(sys.execute,0),"\n",NULL);
	prt("書き込み量      :",lpu(sys.t_post,0) ,"/",lpu(sys.post,0),"\n",NULL);
	prt("ログイン回数    :",lpu(sys.t_logon,0),"/",lpu(sys.logon,0),"\n",NULL);
	lo_line1();
}

void	op_news()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\archive\\",stages->code,NULL);
	sion_fop(buf);
}


FILE	*op_map()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\archive\\",stages->code,".map",NULL);
	return fopr(buf);
}

FILE	*op_datime()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\archive\\",stages->code,".dat",NULL);
	return fopr(buf);
}

fileb	*op_art()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\archive\\",stages->code,".art",NULL);
	return fopb(buf,sizeof(art_t));
}

fileb	*op_note()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\archive\\",stages->code,".not",NULL);
	return fopb(buf,sizeof(notes_t));
}


void	op_system()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\system",NULL);
	sion_fop(buf);
}

void	op_user()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\user",NULL);
	sion_fop(buf);
}

void	op_files()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\file",NULL);
	sion_fop(buf);
}

void	op_stage()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\stage",NULL);
/* dbprt("--Stage:",buf,"\n",NULL); */
	sion_fop(buf);
}

void	op_style()
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\style\\",user.style,NULL);
/* dbprt("--Style:",buf,"\n",NULL); */
	sion_fop(buf);
}

FILE	*op_text(char *name)
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\text\\",name,NULL);
/*	return fopen(buf,"a+");*/
	return fop(buf);
}

FILE	*op_rtext(char *name)
{
	char	buf[LN_buf+1];

	sprt(buf,sys.home,"\\text\\",name,NULL);
	return fopen(buf,"r");
}

FILE	*op_temp(char *mode)
{
	char	buf[LN_buf+1];

	sprt(buf,sys.temp,"\\temp.tmp",NULL);
	return fopen(buf,mode);
}

void	name_pooldir(char *dir)
{
	sprt(dir,sys.home,"\\pool\\",NULL);
}

FILE	*op_pool(char *name)
{
	char	buf[LN_buf+1];

	name_pooldir(buf);
	strcat(buf,name);
	return !fchk(buf,"ab")?NULL:fopr(buf);
}

void	name_localdir(char *dir)
{
	sprt(dir,sys.home,"\\local\\",NULL);
}

FILE	*op_local(char *name)
{
	char	buf[LN_buf+1];

	name_localdir(buf);
	strcat(buf,name);
	return fopr(buf);
}

SHELL	void	esc_chg()
{
	ch.frug.esc ^= 1;
	msgout(ch.frug.esc==0?"エスケープシーケンスを有効にしました。":
							"エスケープシーケンスをカットしました。");
}

SHELL	void	one_chg()
{
	ch.frug.onekey ^= 1;
	msgout(ch.frug.onekey==0?"コマンドラインをラインモードにしました。":
							"コマンドラインをワンキーモードにしました。");
}

SHELL	void	timepas()
{
	if (ch.limit<timer_pas(ch.pass))
		{
		 msgout("活動停止しました。");
		 longjmp(ch.env,1);
		}
}

