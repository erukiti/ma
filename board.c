#include "ma.h"
#include "board.h"
#include "pool.h"
#include "str.h"	/* !! */


/* board kernel code */

void	art_init()
{
	uint	i;

	board.var     =varinit(LN_str,MAX_var);
	board.s       =(stage_t *)malloc(sizeof(stage_t));
	stages->map   =lmalloc(sizeof(*stages->map)*MAX_map);
	stages->datime=lmalloc(sizeof(*stages->datime)*MAX_map);
	stages->kind  =varinit(LN_str,MAX_var);

	if (board.s==NULL ||stages->map==NULL ||stages->datime==NULL)
		error("");

	for (i=0;i<MAX_notes;++i)
		{
		 note.s[i]=lmalloc(sizeof(notes_t));
		 if (note.s[i]==NULL)
		 	error(lpu(i,0));
		 lmemset(note.s[i],0,sizeof(notes_t));
		}
}

void	art_fin()
{
	uint	i;

	for (i=0;i<MAX_notes;++i)
		lfree(note.s[i]);

	varfin(stages->kind);
	lfree(stages->datime);
	lfree(stages->map);
	free(board.s);
	varfin(board.var);
}

	/* Stage総括 */

void	stage_mk()
{
	uint	i;
	num_s	l;

/* dbmsgout("!.stage_mk"); */

	ch.frug.discut=TRUE;	/* 強制回線切断禁止状態 */

	stages->serial=0;
	stages->num=0;
	stages->frug=TRUE;

/* !!  ↓ 動的ノートならば、という処理に変更すべき。 */
	if (stricmp(stages->code,"mail")!=SAME)
		putz(IC_dact "唯今、書き込みindexを作成しています。\r");
				/* 電報に変える */
	op_news();
	i=sion_first();
	for(;;)
		{
		 if (i==0)
		 	break;
		 if (stricmp(sion.type,stages->code)!=SAME)
		 	continue;
		 l=atol(sion.name);
		 if (l>stages->serial)
		 	stages->serial=l;
		 sion_read(nowvar);

/*		 art.num= チェック用*/
		 strcpy(art.title,getvar("title",nowvar));
		 strcpy(art.kind ,getvar("kind",nowvar));
		 art.file   =atoi(getvar("file",nowvar));
		 art.res    =atol(getvar("res",nowvar));
		 art.to     =atoi(getvar("to",nowvar));
		 art.owner  =atoi(getvar("owner",nowvar));
		 art.datime =atol(getvar("datime",nowvar));
		 art.isdel  =atoi(getvar("isdel",nowvar));
		 art.srn    =sion.num;

		 stage_add(l);
		 i=sion_next();
		}
	sion_close();
	if (stricmp(stages->code,"mail")!=SAME)
		msgout(IC_dok "書き込みindexを作成しました。        ");
			/* 電報に変える */
	ch.frug.discut=FALSE; /* 回線切断許可 */
}

num_s	stage_add(num_s l)
{
	num_m	i;

	if (l==NT_errors)
		l=++stages->serial;

	i=stages->num++;
/* dbprt("stage->...[",lpu(i,0)
,"]=",lpu(l,0),"/",lpu(art.datime,0),"\n",NULL); */
	stages->frug=TRUE;
	stages->map[i]=l;
	stages->datime[i]=art.datime;
	art_write(i);
	return l;
}

void	stage_out()
{
	struct stat	st;
	FILE	*fp;

	op_news();
	fstat(fileno(sion.fp),&st);
	sion_close();

	op_stage();
	sion_sr("stage",stages->code);

	clrvar(nowvar);
	setvar("name"  ,stages->name,nowvar);
	setvar("num"   ,lpu(stages->num,0),nowvar);
	setvar("serial",lpu(stages->serial,0),nowvar);
	setvar("datime",lpu(st.st_mtime,0),nowvar);

	sion_write(NULL,nowvar);
	sion_close();

	fcloseb(stages->fba);
	if (stages->frug)
		{
/* dbmsgout("stages->frugがTRUEなのでmap,datimeを出力する。"); */
		 stages->frug=FALSE;
		 fp=op_map();
		 clearerr(fp);
		 lfwrite(stages->map   ,sizeof(*stages->map   )*(stages->num),fp);
		 fclose(fp);
		 fp=op_datime();
		 clearerr(fp);
		 lfwrite(stages->datime,sizeof(*stages->datime)*(stages->num),fp);
		 fclose(fp);
		}
}

SHELL	void	stage_in(const char *s)
{
	time_t	t;
	struct stat	st;
	FILE	*fp;

	atescape(stage_out);
	strjncpy(stages->code,s,LN_key);
	stages->fba=op_art();
	stages->frug=FALSE;
	t=atol(getvar(s,user.mptr));
	mptr_set(t);

	op_stage();
	sion_sr("stage",s);

	sion_read(nowvar);
	strjncpy(stages->name,getvar("name",nowvar),LN_str);

	stages->num=atol(getvar("num",nowvar));
	stages->serial=atol(getvar("serial",nowvar));

	t=atol(getvar("datime",nowvar));

	if (sion_sr("kind",s)==0)
		clrvar(stages->kind); else
		sion_read(stages->kind);

	sion_close();

	op_news();
	fstat(fileno(sion.fp),&st);
	sion_close();
	if (t!=st.st_mtime)
		stage_mk(); else
		{
		 fp=op_map();
		 if (fsize(fp)!=sizeof(*stages->map)*(stages->num))
		 	{
		 	 stage_mk();
		 	 return;
		 	}
		 lfread(stages->map,sizeof(*stages->map)*(stages->num),fp);
		 fclose(fp);
		 fp=op_datime();
		 if (fsize(fp)!=sizeof(*stages->datime)*(stages->num))
		 	{
		 	 stage_mk();
		 	 return;
		 	}
		 lfread(stages->datime,sizeof(*stages->datime)*(stages->num),fp);
		 fclose(fp);
		 return;
		}
}

	/* note 回り */

void	mkfirst_news()
{
	uint	i;

/* dbmsgout("!.mkfirst_news"); */
	for (i=0;i<varsize(stages->kind);++i)
		{
 		 note.s[i]->no=i;
		 lstrcpy(note.s[i]->name,nps(getnvar(i,stages->kind),LN_title));
		 lstrcpy(note.s[i]->kind,nps(mkkey(i,stages->kind),0));
		 note.s[i]->num=0;
		 note.s[i]->number=0;
		 note.s[i]->frug=TRUE;
		}
/* dbprt("note.num=",lpu(i,0),"\n",NULL); */
	note.num=i;
	note.number=0;
	for (;i<MAX_notes;++i)
		{
		 note.s[i]->no=ERROR;
		 note.s[i]->frug=TRUE;
		}
}

void	mkfirst_mail()
{
	note.s[0]->no=0;
	note.s[0]->num=0;
	note.s[0]->number=0;
	lstrcpy(note.s[0]->name,"あなた宛のメールです。");

	note.s[1]->no=1;
	note.s[1]->num=0;
	note.s[1]->number=0;
	lstrcpy(note.s[1]->name,"あなたの出したメールです。");

	note.num=2;
	note.number=0;
	note.s[2]->no=ERROR;
}

void	write_news(num_m m)
{
	char	buf[LN_buf+1];
	uint	i;

	pull(buf,art.kind);
	i=varnum(buf,stages->kind);
	if (i==ERROR)
		error(buf);
	note.s[i]->chain[note.s[i]->number++]=m;
	note.s[i]->frug=TRUE;

/* dbprt("write_news:",buf," ",lpu(i,0),"番ボード,",lpu(m,0),"\n",NULL); */
}

void	write_mail(num_m m)
{
/*dbprt("write_mail ",lpu(art.owner,0),"->",lpu(art.to,0),NULL); */

	if (user.number ==art.to &&!art.isdel)
		{
		 note.s[0]->chain[note.s[0]->number++]=m;
dbmsgout(" your mail");
		} else
		{
		 if (user.number !=art.owner)
dbmsgout(" other mail"); else
		 	{
			 note.s[1]->chain[note.s[1]->number++]=m;
dbmsgout(" you mail");
			}
		}
}

void	note_mkafter()
{
	uint	i;
	fileb	*fb;

/* dbmsgout("! note_mkafter"); */
	if (note.dynamic)
		return;
	fb=op_note();
	for (i=0;i<MAX_notes;++i)
		{
		 if (note.s[i]->frug)
		 	{
		 	 note.s[i]->frug=FALSE;
/* dbprt("  note_mkafter :",lpu(i,0),"\n",NULL); */
/*dbprt("note.s[i]->no=",lpu(note.s[i]->no,0),":",fps(note.s[i]->name,0)
  ,"\n",NULL);*/
  			 lwriteb(note.s[i],i,fb);
		 	}
		}
	fcloseb(fb);
}

SHELL	void	note_mk(char *s)
{
	uint	i;
	fileb	*fb;

	for(i=0;;++i)
		{
		 if (*nlist[i].name=='\0')
		 	 error(s);
		 if (stricmp(nlist[i].name,s)==SAME)
		 	break;
		}
	note.readend=nlist[i].readend;
	note.mkfirst=nlist[i].mkfirst;
	note.write  =nlist[i].write;
	note.dynamic=nlist[i].dynamic;
	atescape(note_mkafter);

/* dbprt("! stages->frug:",lpu(!stages->frug,0),"\n",NULL); */
	if (!note.dynamic &&!stages->frug)
		{
		 fb=op_note();
/* dbprt("! fb->max:",lpu(fb->max,0),"\n",NULL); */
		 if (fb->max == MAX_notes)
		 	{
			 for(i=0;i<MAX_notes;++i)
			 	{
			 	 lreadb(note.s[i],i,fb);
			 	 note.s[i]->frug=FALSE;
			 	 if (note.s[i]->no !=i)
			 	 	break;
			 	}
/* dbprt("note.mk:",lpu(i,0),",",lpu(note.s[i]->no,0),"\n",NULL); */
			 note.num=i;
			 note.number=0;
			 fcloseb(fb);
/* dbprt("2)note.s[0]->frug:",lpu(note.s[0]->frug,0),"\n",NULL); */
			 return;
			}
		 fcloseb(fb);
		}
/*dbmsgout("note.mkfirst");*/
	note.mkfirst();

	for(i=0;i<stages->num;++i)
		{
		 art_read(i);
		 note.write(i);
		}
}

SHELL	void	note_info()
{
	char	buf[LN_buf+1];

	sprt(buf,lpu(notes->no+1,-2),DC_norm 
		,delspc(fps(notes->name,LN_title)),FC_num_s 
		,lpu(getdisps(board.number),-4),DC_per 
		,lpu(notes->number,-4),FC_num_e ,NULL);
	lo_line(buf);
}

SHELL	void	note_list()
{
	uint	i;

	lo_line("ボードリスト");
	for (i=0;i<note.num;++i)
		{
		 prt(lpu(i+1,-2),DC_norm FC_str2_s
		 	,fps(note.s[i]->name,LN_title),FC_code_s
		 	,fps(note.s[i]->kind,26),NULL);
		 prt(FC_code_e FC_str2_e FC_num_s 
		 	,lpu(note.s[i]->number-note.s[i]->num,-4),DC_norm
		 	,lpu(note.s[i]->number,-4),FC_num_e "\n" ,NULL);
		}
	lo_line1();
}

SHELL	void	note_list2()
{
	uint	i;
	uint	u,v;

	prt(IC_ok,strhdatime(mptr_get()),"以降の未読です。\n",NULL);

	lo_line("未読一覧");
	v=0;
	for (i=0;i<note.num;++i)
		{
		 u=note.s[i]->number- note.s[i]->num;
		 if (u<=0)
		 	continue;
		 v+=u;
		 prt(lpu(i+1,-2),DC_norm FC_str2_s
			,fps(note.s[i]->name,LN_title),FC_code_s
			,fps(note.s[i]->kind,26)
			,FC_code_e FC_str2_e  " 未読"
			,lpu(u,0),"件\n",NULL);
		}
	if (v==0)
		msgout(IC_nok "未読はありません。" ); else
		{
		 lo_line2();
		 prt(IC_ok "未読合計 " ,lpu(v,0), "件\n" ,NULL);
		}
	lo_line1();
}

SHELL	void	board_first()
{
	note.number=0;
}

SHELL	void	board_number()
{
	board.number=0;
	note.number=getrnumber(note.number)-1;
}

SHELL	bool	board_plus()
{
	if (note.number+1>=note.num)
		return FALSE;
	++note.number;
	return TRUE;
}

SHELL	bool	board_minus()
{
	board.number=0;
	if (note.number==0)
		return FALSE;
	--note.number;
	return TRUE;
}

#define	mail_count(i)	note.s[i]->number

SHELL	uint	mail_chk(uint i)
{
/*	mptr_chk();*/
	if (mail_count(i) ==0)
		{
		 msgout((i==0)? IC_nok "あなた宛のメールは届いておりません。":
		 				IC_nok "あなたはメールを出していません。");
		 return FALSE;
		}
/*	prt("  ●",lpu(note.s[i]->number - note.s[i]->num,0),*/
	prt(IC_ok ,lpu(mail_count(i),0),
			 (i==0)?"通のメールが届いております。\n":
			 		"通のメールを出しています。\n",NULL);
	board.number=0;
	note.number=i;

	board_in("mail");
	return TRUE;
}

SHELL	void	board_in(char *s)
{
	number_mptr();
	if (note.number<note.num)
		where_sett(s); else
		{
		 msgout(IC_err  "そんなボードは存在しません。");
		 note.number=0;
		}
}


/* num_n 関連 */

SHELL	void	number_kill()
{
	board.number=notes->number;
}

void	number_mptr()
{
	board.number=notes->num;
}

SHELL	void	number_plus()
{
	if (board.number<notes->number)
		++board.number;
}

SHELL	void	number_minus()
{
	if (board.number)
		--board.number;
}

SHELL	void	number_number()
{
	board.number=getnotes(getrnumber(getdisps(board.number)));
}

SHELL	void	rf_chg(uint i)
{
	board.readfrug=(i>0);
}

num_n	art_chk(num_n n)
{
	return (n< notes->number)?n:NT_error;
}

/* art Index */

SHELL	void	art_read(num_m m)
{
	if (m==NT_errorm)
		{
		 m=getmap(board.number);
/*dbprt("art_read ",lpu(board.number,0),":",lpu(m,0),"\n",NULL);*/
		 if (m==NT_errorm)
		 	{
		 	 board.number=notes->number;
			 return;
			}
		}

	readb(&art,m,stages->fba);
}

void	art_write(num_m m)
{
	if (m==NT_errorm ||m >=stages->num)
		error("");

	writeb(&art,m,stages->fba);
}

void	art_varmk()
{
	clrvar(board.var);
	setvar("to"    ,lpu(art.to,0)    ,board.var);
	setvar("owner" ,lpu(art.owner,0) ,board.var);
	setvar("kind"  ,art.kind         ,board.var);
	setvar("file"  ,lpu(art.file,0)  ,board.var);
	setvar("res"   ,lpu(art.res,0)   ,board.var);
	setvar("title" ,art.title        ,board.var);
	setvar("datime",lpu(art.datime,0),board.var);
	setvar("isdel" ,lpu(art.isdel,0) ,board.var);
}

SHELL	void	art_del(uint f)
{
	num_n	i;
	num_m	j;

	if (user.number==0)
		return;

	if (!board.readfrug)
		i=board.number; else
		i=board.number-1;
	j=getmap(i);
	art_read(j);

	if (art_chk(i)==NT_error)
		{
		 msgout(IC_err "削除はできません。");
		 return;
		} 

	if (art.to!=user.number &&art.owner!=user.number)
		{
		 msgout(IC_err 
		 	"あなたには、このアーティクル操作の権限はありません。");
		 return;
		}

	if (art.isdel&&f==1)
		{
		 msgout(IC_nok "既に削除されています。");
		 return;
		}
	if (!art.isdel&&f==0)
		{
		 msgout(IC_nok "その書き込みは削除されていません。");
		 return;
		}
	if (f==2)
		f=(art.isdel)?0:1;
	art.isdel=f;

	op_news();
	art_varmk();
/*	sion_sr(stages->code,lpu(getserial(j),0));*/
	sion_srn(art.srn);
	sion_write(NULL,board.var);
	sion_close();
	art_write(j);
}



/* 書き込み */

void	art_postout()
{
	num_s	l;

	op_news();

	art.srn=sion_max();			/* !! */
/* dbprt("sion_max",lpu(sion_max(),0),"\n",NULL); */
	l=stage_add(NT_errors);		/* mapに登録 */
	note.write(getmserial(l));	/* noteに登録 */

	art_varmk();
	sion_sr(stages->code,lpu(l,0));
	sion_write(op_temp("r"),board.var);	/* !! */
	sion_close();
}

void	post_after(uint f)
{
	char	buf[LN_buf+1];

	if (!f)
		{
		 msgout(IC_stop "書き込みを中止しました。");
		 return;
		}

/* dbprt("! Post:[",art.kind,"]\n",NULL); */
	delspc(art.kind);
	kind_sel(art.kind);
	if (*art.kind=='\0')
		msgout(IC_stop "書き込みを中止しました。"); else
		{
		 art_postout();

		 msgout(IC_act "書き込みをしました。");
		 ++sys.t_post;
		 ++sys.post;
		 if (user.number>1)
			++ult(user.number)->mpost;
		 sprt(buf,"| 書き込み(",art.title,")",NULL);
		 sys_log(buf);
		 return;
		}
}

void	art_post(num_s res,void (*func)(uint))
{
	char	buf[LN_buf+1];

/* dbprt("art_post:[",art.kind,"]\n",NULL); */
	art.file=ERROR;
	msgout(IC_act "タイトルをいれてねっ！");
	getstri(buf,LN_title);
	if (*buf!='\0')
		strcpy(art.title,buf);
	prt(IC_ok "タイトル:『",art.title,"』で書き込みしま～す。\n",NULL);


	art.owner=user.number;
	art.datime=getdatime();
	art.isdel=FALSE;
	art.res=res;

	func(editor());
}

SHELL	void	res()
{
	num_n	u;
/* 	char	*p; */
	char	buf[LN_title-4+1];

/* dbprt("res1:[",art.kind,"]\n",NULL); */
	u=getnotes(getrnumber(board.number+(board.readfrug?0:1)));
	if (art_chk(u)==NT_error)
		{
		 msgout(IC_err "そんな番号にレスはできません。");
		 return;
		}
	art_read(getmap(u));
/* dbprt("res2 kind:[",art.kind,"]\n",NULL); */
/* dbprt("res2 title:[",art.title,"]\n",NULL); */

	if (strncmp(art.title,"Re*",3)!=SAME)
		{
		 strjncpy(buf,art.title,LN_title-4);
/*		 sprt(art.title,"Re*:",buf); */
		 strcpy(art.title,"Re*:");
		 strcat(art.title,buf); 
		}
/* dbprt("res3 kind:[",art.kind,"]\n",NULL); */
	art.file=ERROR;

	art_post(getserial(getmap(u)),post_after);
}

SHELL	void	post()
{
	art.file=ERROR;
	lstrcpy(art.kind,notes->kind);
	strcpy(art.title,"えとえとえっとぉ～");
	art_post(NT_errors,post_after);
}

SHELL	void	post_list()
{
	art.file=ERROR;
	lstrcpy(art.kind,"");
	strcpy(art.title,"えとえとえっとぉ～");
	art_post(NT_errors,post_after);
}

void	mail_after(uint f)
{
	if (!f)
		{
		 msgout(IC_stop "メール送信を中止しました。");
		 return;
		}

	msgout(IC_ok "メールを送りました。");
	art_postout();

	sys_log("| mail送信");
}

SHELL	void	mail_rep()
{
	num_n	u;
	char	*p;

	u=getnotes(getrnumber(board.number+(board.readfrug?0:-1)));
	if (art_chk(u)==NT_error)
		{
		 msgout(IC_err "そんな番号にリプライはできません。");
		 return;
		}
	art_read(getmap(u));
	art.file=ERROR;
	art.to=art.owner;
	*art.kind='\0';
	prt(IC_act ,ulist_gethandle(art.to), "さんにお手紙をかえします。\n"
		,NULL);

	if (strncmp(art.title,"Re*",3)!=SAME)
		{
		 p=nps(art.title,LN_title-4);
		 sprt(art.title,"Re*:",p,NULL);
		}
	art_post(NT_errors,mail_after);
}

SHELL	void	mail_ent(uint u)
{
	if (u==0)
		{
		 msgout(IC_act "誰にメールを出しますか？");
		 u=ulist_fsearch(NULL);
		}
	if (u==0)
		{
		 wfc_dengon("ごめんよっ、急いでるンでぃっ！");
		 qlogout();
		 return;
		}
	if (u==ERROR)
		{
		 msgout(IC_stop "メール送信を中止しました。");
		 return ;
		}
	art.file=ERROR;
	*art.kind='\0';
	art.to=u;
	strcpy(art.title,"えとえとえっとぉ～");
	art_post(NT_errorm,mail_after);
}

/* 未読ポインタ関連 */

SHELL	void	mptr_day(const char *s)
{
	char	buf[LN_buf+1];
	time_t	t;
	num_m	i;

	strcpy(buf,(s==NULL)?"":s);
	for(;;)
		{
		 if (*buf=='\0')
		 	{
			 msgout(IC_act "未読ポインターを設定します。ex.75/02/18,-23 etc");
			 getstr(buf);
			}
		 t=(*buf=='-')?date_s(atoi(buf+1)):dtos(buf);
		 prt(IC_act "未読ポインターを" FC_time_s ,stredatime(t)
					 ,FC_time_e  "に設定します。\n" ,NULL);
		 break;
/*		 msgout("以上でよろしいでしょうか？");
		 if (ynq())
		 	break;*/
		}
/* dbprt("mptr_day: i=",lpu(stages->num,0),NULL); */
	for (i=stages->num;i>0;--i)
		{
		 if (t>stages->datime[i-1])
		 	break;
		}
/* dbprt(" -> ",lpu(i,0),"\n",NULL); */
	mptr_set(t);
	mptr_chk();
	if (i==stages->num)
		msgout(IC_nok "未読がありません。"); else
		note_list2();
}

SHELL	void	mptr_chk()
{
	uint			i,j;
	num_m	lerge	*p;


	for(i=0;i<note.num;++i)
		{
		 p=note.s[i]->chain;
		 for(j=note.s[i]->number;j>0;--j)
		 	{
		 	 if (stages->datime[p[j-1]] <mptr_get())
		 	 	break;
		 	}
		 note.s[i]->num=j;
		}
}

SHELL	void	mask_chk()
{
	uint	i;

	for(i=0;i<note.num;++i)
		{
/* dbprt("mask_chk[",fps(note.s[i]->kind,0),"]",NULL); */
		 if (dsd_chk(fps(note.s[i]->kind,LN_dsd),getvar("mask",user.var)))
		 	{
/*dbprt("Mask!",fps(note.s[i]->name,0),"\n",NULL);*/
		 	 note.s[i]->num=note.s[i]->number;
		 	}
		}
}

SHELL	void	mask_set()
{
	char	buf[LN_buf+1];

	strcpy(buf,getvar("mask",user.var));/* !!ちょっとフマン！(笑) */

	msgout(IC_act "マスクするボードに印をつけてください。");
	msgout("    Nスキャン、/Nスキャン時に有効です。");
	dsd_mark(buf,stages->kind,TRUE);
	setvar("mask",buf,user.var);
}

SHELL	void	mptr_write()
{
	setvar(stages->code,lpu(mptr_get(),0),user.mptr);
}

SHELL	void	mptr_last()
{
/* dbmsgout("mptr_last"); */
	mptr_set(getdatime());
/*	mptr_set(stages->serial+1); */
/*dbprintf("mptr_last %d",mptr_get());*/
	mptr_write();
}

SHELL	void	mptr_clr()
{
	uint	i;

	for(i=0;i<note.num;++i)
		note.s[i]->num=0;
}

/* 読みだし関連 */

SHELL	void	news_prompt()
{
	char	buf[LN_buf+1];

	if (art_chk(board.number)==NT_error)
		{
		 _ma_prompt(IC_ok "総ての書き込みを読みました"
		 				"                                       ");
		 return ;
		}
	if (art.isdel)
		{
		 sprt(buf,lpu(getdisps(board.number),-4),DC_per ,lpu(notes->number,-4),
		 	 "★削除されています。                "
		 	 ,nps(ulist_gethandle(art.owner),24),NULL);
		} else
		{
		 sprt(buf,lpu(getdisps(board.number),-4),DC_per 
			,lpu(notes->number,-4),FC_str2_s 
			,nps(art.title,32)
			,FC_str2_e ,nps(ulist_gethandle(art.owner),24),NULL);
		}
	_ma_prompt(buf);
}

void	art_pageout()
{
/*	if (sion_sr(stages->code,lpu(getserial(getmap(board.number)),0))==0)*/
	if (sion_srn(art.srn)==0)
		 error("アーティクルが存在しません。");
/* dbprt("srn:",lpu(art.srn,0),"\n",NULL); */
	sion_pageout();
}

void	art_look()
{
	char	buf[LN_buf+1];
	num_n	i;
	num_m	j;

	if (art_chk(board.number)==NT_error)
		error("");

	if (art.isdel)
		{
/*		 msgout(IC_ng "このアーティクルは削除されています。");*/
		 return;
		}

	sprt(buf,lpu(notes->no+1,-2),DC_norm
		,fps(notes->name,0),FC_num_s 
		,lpu(getdisps(board.number),-4),DC_per 
		,lpu(notes->number,-4),FC_num_e ,NULL);
	lo_line(buf);

	prt(FC_str2_s ,nps(art.title,32),FC_str2_e
		FC_code_s ,nps(art.kind,23) ,FC_code_e 
		,stredatime(art.datime),"\n",NULL);

	prt("  From  ",delspc(ulist_gethandle(art.owner))
					," #",lpu(art.owner,0),"\n",NULL);

	j=getmserial(art.res);
	i=getnmap(j);
	if (i!=NT_error)
		{
		 art_read(j);
		 prt(FC_comp2_s ,lpu(getdisps(i),0)
			,"番の" FC_str2_s ,nps(art.title,32),FC_str2_e
			"へのレスです。" FC_comp2_e "\n" ,NULL);
		 art_read(getmap(board.number));
		}

	if (art.file!=ERROR)
		{
		 pool_read(art.file);
		 pool_pageout();
		}

	lo_line2();
	art_pageout();
	msgout("");

}

SHELL	void	readquit()
{
	msgout(IC_act "ボード検索を終了しますか？");
	if (!ynq())
		return;
	where_setr();
}

SHELL	void	vscan()
{
	if (iscall())
		ch.frug.monitor=0;
	op_news();
	for(;board.number<notes->number;++board.number)
		{
		 art_read(getmap(board.number));
		 art_look();
		}
	sion_close();
	if (iscall())
		ch.frug.monitor=1;
}

SHELL	void	ascan()
{
	board_first();
	do	{
		 number_mptr();
		 vscan();
		} while(board_plus());
	msgout(IC_ok "全ボードをスキャンしました。");
}

SHELL	void	tescan()
{
	time_t	t;
	ulong	n;

	sendcharnum=0;
	ch.frug.monitor=0;
	timer_res(t);
	board_first();
	do	{
		 number_mptr();
		 vscan();
		} while(board_plus());

	n=sendcharnum;
	ch.frug.monitor=1;
	prt("Scan Speed,",lpu(n,0),"chars/",
		lpu(timer_pas(t),0),"秒 =",lpu(n/timer_pas(t),0),"cps\n",NULL);

}


SHELL	void	nscan()
{
	board_first();
	do	{
		 number_mptr();
		 if (board.number<notes->number)
		 	{
		 	 board_in(stages->code);
		 	 return;
		 	}
		} while(board_plus());
	msgout(IC_nok "未読がありません。");
}

void	kind_sel(char *s)
{
/* dbprintf("Kind_sel[%s]\n",s);*/
	msgout(IC_act "話題を選んでください。デフォが良ければそのままリターン");
	msgout("    ポストを中止する場合は[Q]もしくは[N]");
	dsd_mark(s,stages->kind,*s=='\0');
}

void	readend_news()
{
	putz(IC_act "書き込みを読み終わりました。次のボードに移動しますか？");
	if (!ynq())
		{
		 msgout(IC_ok "ボードに戻ります。");
		 return;
		}

	for(;;)
		{
		 if (!board_plus())
		 	{
		 	 msgout(IC_ok "全ボードを検索しました。");
		 	 where_setr();
		 	 return;
		 	}
		 number_mptr();
		 if (board.number<notes->number)
		 	break;
		}
}

void	readend_mail()
{
	msgout(IC_ok "メールを読み終わりました。");
	where_setr();
}

SHELL	void	enter()
{
	if (art_chk(board.number)==NT_error)
		{
		 note.readend();
		 art.file=ERROR;
		 return ;
		}

	op_news();
	art_look();
	sion_close();
	rf_chg(1);
	number_plus();
}

/* num コンバーター */
num_s	getserial(num_m u)
{
	if (u==stages->num)
		return stages->serial+1;
	if (u>stages->num)
		error(lpu(u,0));
	return stages->map[u];
}

num_m	getmserial(num_s l)
{
	uint	i;

	for (i=0;i<stages->num;++i)
		{
		 if (stages->map[i]==l)
		 	return i;
		}
	return NT_errorm;
}

num_n	getnmap(num_m u)
{
	uint	i;

	if (u==NT_errorm)
		return NT_error;
	for (i=0;i<notes->number;++i)
		{
		 if (notes->chain[i]==u)
		 	return i;
		}
	return NT_error;
}

num_m	getmap(num_n u)
{
	return (u>notes->number)?NT_errorm:notes->chain[u];
}

SHELL	void	art_prof()
{
	prof_read(art.owner);
}

