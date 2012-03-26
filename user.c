#include "ma.h"
#include "str.h"	/* !! */

ulist_t	lerge *_ult(uint n)
{
/*	if (n>user.member)
		error(lpu(n,0));*/
	return &user.list[n];
}

void	ulist_free()
{
	lfree(user.list);
}

void	ulist_write()
{
	uint	i;

	op_user();
	for(i=1;i<user.member;++i)
		ulist_set(i,nowvar);
	sion_close();
}

void	ulist_set(uint i,var_t *v)
{
	sion_sr("user",lpu(i,0));
	clrvar(v);
	sion_read(v);
/* varview(v);*/

	setvar("post"  ,lpu(ulist_getpost  (i),0),v);
	setvar("mpost" ,lpu(ulist_getmpost (i),0),v);
	setvar("login" ,lpu(ulist_getlogin (i),0),v);
	setvar("mlogin",lpu(ulist_getmlogin(i),0),v);
	setvar("wp"    ,lpu(ulist_getwp    (i),0),v);

	sion_write(NULL,v);
}

void	ulist_rep(uint i,var_t *v)
{
	char	*p;
	struct tm	tm;

	p=getvar("memo",v);
	if (p==NULL)
		p="メモなし。";
	lstrcpy(ult(i)->memo  ,nps(p,LN_memo));
	lstrcpy(ult(i)->hoby  ,nps(getvar("hoby",v),LN_memo));
	lstrcpy(ult(i)->id    ,nps(getvar("id",v),0));
	lstrcpy(ult(i)->pass  ,nps(getvar("pass",v),0));
	lstrcpy(ult(i)->handle,nps(getvar("handle",v),LN_handle));
	ult(i)->mpost =atoi(getvar("mpost",v));
	ult(i)->post  =atoi(getvar("post",v));
	ult(i)->mlogin=atoi(getvar("mlogin",v));
	ult(i)->login =atoi(getvar("login",v));

	dtotm(getvar("birth",v),&tm);
	ult(i)->birthm=tm.tm_mon;
	ult(i)->birthd=tm.tm_mday;

	ult(i)->wp    =atoi(getvar("wp",v));
}

void	ulist_init()
{
	uint	i;

	user.list=lmalloc(sizeof(ulist_t) *(MAX_user+1));
	if (user.list==NULL)
		error(lpu(farcoreleft(),0));

	op_user();
	for (i=0;i<=user.member;++i)
		{
		 if (sion_sr("user",lpu(i,0))==0)
		 	continue;
		 sion_read(nowvar);
		 ulist_rep(i,nowvar);
		}
	sion_close();
}

void	user_fin()
{
	varfin(user.defs);
	varfin(user.var);
	varfin(user.mptr);
	ulist_free();
}

void	user_init()
{
	user.defs=varinit(LN_str,MAX_var);
	user.var=varinit(LN_str,MAX_var);
	user.mptr=varinit(LN_longdig,MAX_var);

	user.member=atoi(getvar("member",sys.var));

	op_system();
	sion_sr("def","signup");
	sion_read(user.defs);
	sion_close();

	ulist_init();
}

void	user_rep(uint id)
{
	uint	a;

	user.number= id;
	strjncpy(user.id,getvar("id",user.var),LN_id);
	strjncpy(user.style,getvar("style",user.var),LN_key);
	strjncpy(user.handle,getvar("handle",user.var),LN_handle);
	ch.limit= atoi(getvar("limit",user.var))*60;
	if (ch.limit==0 )/* ||ゴールデンアワーならば････ */
		ch.limit=60*10;
	a=atoi(getvar("mode",user.var));
	ch.frug.esc   =!(a&1);
	ch.frug.onekey=a&2;

	ulist_rep(id,user.var);
}

uint	ulist_search(char *s)
{
	uint	i;

	if (isdigit(*s))
		{
		 i=atoi(s);
		 return (i>user.member)?ERROR:i;
		}
	if (stricmp("guest",s)==SAME)
		return 0;

	for (i=1;i<=user.member;++i)
		{
/*dbprintf(" %d:[%s]\n",i,ulist_getid(i));*/
		 if (stricmp(ulist_getid(i),s)==SAME)
		 	return i;
		}
	return ERROR;
}

uint	ulist_fsearch(const char *s)
{
	uint	i,c;
	char	buf[LN_buf+1];

	if (s!=NULL)
		strcpy(buf,s); else
		*buf='\0';

	for(;;)
		{
		 if (*buf=='\0')
		 	{
		 	 msgout(IC_act 
		 	 	"IDを入力してください。(retでリスト /qで戻る /oでlogout)");
		 	 getstr(buf);
		 	}
		 if (stricmp(buf,"/Q")==SAME)
		 	return ERROR;
		 if (stricmp(buf,"/O")==SAME)
		 	return 0;
		 if (isdigit(*buf))
		 	{
		 	 i=atoi(buf);
		 	 if (i<=user.member)
		 		return i;
		 	 msgout(IC_nok "そのような番号の人はいません。");
		 	 *buf='\0';
		 	 continue;
		 	}

		 c=ulist_disp(buf);
		 if (c!=ERROR&&c!=0)
		 	return c;
		 if (c==0)
		 	msgout(IC_nok"そのような人は存在しません。"); else
			msgout(IC_nok
				"以上がその条件に相当します。一人にしぼってください");
		 *buf='\0';
		}
}

char	*ulist_getnid(uint i)
{
	char	*p;

	p=getring(LN_owner+1);
	sprt(p,ulist_getid(i),"@",sys_getnid(),NULL);
	return p;
}

char	*user_getnid()
{
	char	*p;

	p=getring(LN_owner+1);
	sprt(p,user_id(),"@",sys_getnid(),NULL);
	return p;
}

SHELL	uint	ulist_disp(char *s)
{
	uint	i,c,j,f;

	f=(*s=='\0'||strcmp(s,"?")==SAME);
	if (user.member==0)
		{
		 msgout(IC_err "メンバーがいません。");
		 return 0;
		}

	c=0;
	lo_line("user list");
	for (i=1;i<=user.member;++i)
		{
		 if (!f&& strfcmp(s,ulist_getid(i))!=SAME)
		 	continue;
		 ++c;
		 j=i;
		 prt(lpu(i,-4),DC_norm FC_code_s 
		 	,nps(ulist_getid(i),LN_id),FC_code_e 
		 	,nps(ulist_gethandle(i),LN_handle),FC_str2_s 
		 	,nps(ulist_getmemo(i),34),FC_str2_e  "\n",NULL);
		}
	lo_line1();
	if (c==0)
		return 0;
	if (c==1)
		return j; else
		return ERROR;
}

void	user_guest(char *s)
{
	char 	buf[LN_buf+1];

	op_user();
	sion_sr("user",lpu(0,0));
	sion_read(user.var);
	sion_sr("mptr",lpu(0,0));
	sion_read(user.mptr);
	sion_close();

	sprt(buf,nps(s,LN_handle-6),"(旅人)",NULL);
	setvar("handle",buf,user.var);

	user_rep(0);
}

SHELL	void	user_set(uint id)
{
	if (id>user.member)
		return ;

	op_user();
	sion_sr("user",lpu(id,0));
	sion_read(user.var);
	sion_sr("mptr",lpu(id,0));
	sion_read(user.mptr);
	sion_close();

	user_rep(id);
}

SHELL	void	user_write()
{
	if (user.number==0)
		return ;
	op_user();

	setvar("mode",lpu((!ch.frug.esc)?1:0|(ch.frug.onekey)?2:0,0),user.var);

	setvar("post"  ,lpu(ulist_getpost  (user.number),0),user.var);
	setvar("mpost" ,lpu(ulist_getmpost (user.number),0),user.var);
	setvar("login" ,lpu(ulist_getlogin (user.number),0),user.var);
	setvar("mlogin",lpu(ulist_getmlogin(user.number),0),user.var);
	setvar("wp"    ,lpu(ulist_getwp    (user.number),0),user.var);

	sion_sr("user",lpu(user.number,0));
	sion_write(NULL,user.var);
	sion_sr("mptr",lpu(user.number,0));
	sion_write(NULL,user.mptr);
	sion_close();

	user_rep(user.number);
}

SHELL	void	user_disp()
{
	char	buf[LN_buf+1];
	uint	a,i;

	sprt(buf,lpu(user.number,-4)," [",user_id(),"]",NULL);
	lo_line(buf);

	prt("ハンドル "DC_norm ,user_handle(),"\n",NULL);
	prt("端末     "DC_norm ,
		isonekey()?"ワンキー入力":"ライン入力","\n",NULL);

	i=atol(getvar("lastlog",user.var))!=0;
	if (i)
		{
		 prt("コネクト "DC_norm ,ch.result,"\n",NULL);
		 prt("経過時間 "DC_norm ,strptime(timer_pas(ch.pass)),"。"
		 "活動限界まで",strptime(ch.limit-timer_pas(ch.pass)),"です。\n",NULL);
		}
	if (user.number>0)
		{
		 if (i)
		 	{
		 	 prt("ログイン "DC_norm 
		 	 						,stredatime(ch.login),"\n",NULL);

			 a=ulist_getmpost(user.number);
			 prt("ポスト数 "DC_norm "(今月)",lpu(a,0),"/(全体)"
			 	,lpu(ulist_getpost(user.number)+a,0),"\n",NULL);

			 a=ulist_getmlogin(user.number);
			 prt("ログイン "DC_norm "(今月)",lpu(a,0),"/(全体)"
			 	,lpu(ulist_getlogin(user.number)+a,0),"\n",NULL);
			}

		 lo_line2();
		 prt("氏名     "DC_norm ,getvar("name",user.var),"\n",NULL);
		 prt("住所     "DC_norm ,getvar("adrs",user.var),"\n",NULL);
		 prt("電話番号 "DC_norm ,getvar("tel",user.var),"\n",NULL);
		 prt("誕生日   "DC_norm ,getvar("birth",user.var),"\n",NULL);

		 lo_line2();
		 prof_myview();
		}
	lo_line1();
}

SHELL	void	user_chk(uint u)
{
	char	i,f;
	char	*p,*s;

	f=0;
	if (user.number==0)
		return;

	for (i=0;i<varsize(user.defs);++i)
		{
		 p=mkkey(i,user.defs);
		 if (p==NULL)
		 	break;
		 if (toupper(*getvar(p,user.defs))!='T')
			s=getvar(p,user.var); else
		 	{
		 	 if (u==0)
		 		continue;
		 	}
		 if (s==NULL||*s=='\0'||u==1)
		 	{
		 	 if (f==0)
		 	 	{
		 	 	 f=1;
		 	 	 msgout("");
		 	 	 msgout(IC_ok"二三、あなたについてお聞きしたい事が有ります。");
		 	 	}
		 	 user_def(p);
		 	}
		}
/* varview(user.var); */
}

void	sign_id(char *p)
{
	uint	u;

	for(;;)
		{
		 getrstri(p,LN_id);
		 if (isdigit(*p))
		 	{
		 	 msgout(IC_err "IDの先頭に数字を指定できません。");
		 	 continue;
		 	}
		 u=ulist_search(p);
		 if (u==ERROR)
		 	return;
		 if (*p=='\0')
		 	 msgout(IC_err "そのIDは利用できません。"); else
	  		 msgout(IC_err 
	  		 	"既にそのIDを持つ人が存在しています。");
		}
}

void	sign_pass(char *p)
{
	char	buf[LN_buf+1];

	strcpy(buf,getvar("pass",user.var));
	if (*buf!='\0')
		{
		 msgout(IC_act "現時点でのパスワードを入力してください。");
		 getpass(buf);
		 if (!ulist_chkpass(buf,user.number))
			{
			 msgout(IC_err 
			 	"あなたはパスワード変更が出来ません。");
			 return ;
			}
		}

	for(;;)
		{
		 msgout("パスワードを入力してください。");
		 getpass(p);
		 msgout(IC_act "確認のためにもう一度入力してください。");
		 getpass(buf);
		 if (strcmp(p,buf)==SAME)
		 	return;
		 msgout(IC_err "パスワードが食い違っています。");
		}
}

void	user_sel(char *s,char *p)
{
	op_system();
	sion_sr("select",s);
	sion_read(nowvar);
	sion_close();

	dsd_select(p,nowvar);
}

void	user_mark(char *s,char *p)
{
	op_system();
	sion_sr("select",s);
	sion_read(nowvar);
	sion_close();

	dsd_mark(p,nowvar,TRUE);
}


SHELL	void	user_def(char *s)
{
	char	*p,*msgptr;
	uint	i;
	char	c,buf[LN_buf+1];

	msgptr = pull(buf,getvar(s,user.defs));
	i=atoi(buf);
	c=toupper(*buf);
	if (c!='T')
		{
		 strcpy(buf,getvar(s,user.var));
		 p=buf;
		}

	msgout(msgptr);
	switch(c)
		{
	 case 'I':
/* IDを取得している状態からはこれを呼び出さないようにしなければならない */
		 sign_id(p);
		 break;
	 case 'P':
	  	 sign_pass(p);
		 break;
	 case 'S':
	 	 user_sel(s,p);
	 	 break;
	 case 'M':
	 	 user_mark(s,p);
	 	 break;
	 case 'T':
	 	 disp_text(s);
	 	 return;

	 default:
	  	 if (i==0 ||LN_str<i)
	  	 	i=LN_str;
	  	 getrstri(p,i);
	  	}
	setvar(s,p,user.var);
}

void	user_add()
{
	++user.member;
}

SHELL	void	sign_up()
{
	clrvar(user.mptr);
	clrvar(user.var);
	user.number=user.member+1;
	ult(user.number)->mpost =0;
	ult(user.number)->post  =0;
	ult(user.number)->mlogin=0;
	ult(user.number)->login =0;
	setvar("mpost","0",user.var);
	setvar("post","0",user.var);
	setvar("mlogin","0",user.var);
	setvar("login","0",user.var);

	for(;;)
		{
		 setvar("pass","",user.var);
		 msgout(IC_act "今からサインアップを行います。");

		 user_chk(1);
	 	 user_rep(user.member+1);
		 user_disp();
		 msgout(IC_act "以上でよろしいでしょうか？");
		 if (ynq())
		 	break;
		 msgout(IC_act "中止しますか？");
		 if (ynq())
		 	{
		 	 user_guest("");
		 	 return;
		 	}
		}
	user_add();
	user_write();
	sys_log("| User登録");
	where_seta();
}
/*	msgout("では確認の意味も込めて新しいIDでログインします。");
	quits("WFC");
	login();
*/

void	prof_read(uint i)
{
	if (i==0||i>user.member)
		return;

	op_user();
	if (sion_sr("user",lpu(i,0))==2)
		sion_pageout(); else
		msgout(IC_nok "プロフィールは登録されていません。");
	sion_close();
}

SHELL	void	prof_view()
{
	uint	i;
	char	buf[LN_buf+1];

	msgout(IC_act "プロフィールをみます。");
	i=ulist_fsearch("");
	if (i==ERROR)
		return;
	sprt(buf,ulist_gethandle(i),"さん",NULL);
	lo_line(buf);
	prt(IC_atn ,lpu(ulist_getbirthm(i),-2),"月"
			,lpu(ulist_getbirthd(i),-2),"日生まれ\n",NULL);
	prt(IC_atn "      "FC_str2_s ,ulist_getmemo(i),
		FC_str2_e "\n",NULL);
	prt(IC_atn "趣味は"FC_str2_s ,ulist_gethoby(i),
		FC_str2_e "\n",NULL);
	lo_line2();
	prof_read(i);
	lo_line1();
}

SHELL	void	prof_myview()
{
	prof_read(user.number);
}

void	prof_writeafter(uint f)
{
	if (!f)
		{
		 msgout(IC_stop "書き込みを中止しました。");
		 return;
		}
	op_user();
	sion_sr("user",lpu(user.number,0));
	sion_write(op_temp("r"),user.var);
	msgout(IC_ok "書き込みをしました。");
	sys_log("| prof書き込み");
	sion_close();
}

SHELL	void	prof_write()
{
	if (user.number==0)
		{
		 msgout(IC_err 
		 	"ゲストさんはプロフィールを登録する事はできません。");
		 return;
		}
	prof_writeafter(editor());
}

char	__retpcode[]={"\r"};

char	getpass(char *s)
{
	char	i;

	*s='\0';

	i=_ma_getstr(s,LN_pass,F_mask,__retpcode);
	msgout("");
	return i;
}

