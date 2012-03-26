#include "ma.h"
#include "mio.h"

const	char	*stralpha(const char *s)
{
	while(*s!='\0')
		{
		 if (isalpha(*s))
		 	return s;
		 ++s;
		}
	return NULL;
}

void	escchk (char *s,const char *t,bool f)
{
	if (strncmp(t,"<verb>",6)==SAME)
		t+=6;
	for(;;)
		{
		 if (*t=='\0')
		 	break;
		 if (f&& *t=='\x1b')
		 	{
		 	 ++t;
		 	 if (*t=='\0')
		 	 	break;
		 	 if (*t++!='[')
		 	 	continue;
		 	 t=stralpha(t);
		 	 if (t==NULL)
		 	 	break;
		 	 ++t;
		 	 continue;
			}
		 if (*t=='\n')
		 	*s++='\r';
		 *s++=*t++;
		}
	*s='\0';
}

void	spc(char *s)
{
	while(*s!='\0')
		*s++=' ';
}

void	addspc(char *s,uint ln)
{
	char	*p,*q;

	p=s+strlen(s);
	q=s+ln;
	while(p<q)
		*p++=' ';
	*p='\0';
}

char	*delspc(char *s)
{
	char	*p;

	p=s+strlen(s);
	while(s!=p)
		{
		 --p;
		 if (chk_kanji(s,p)%2==1)
		 	{
		 	 if(*p!=0x40 ||s==p)
		 	 	return s;
		 	 --p;
		 	 if (*p!=0x81)
		 	 	return s;
		 	 *p='\0';
		 	 continue;
		 	}
		 if (*p!=' ')
		 	return s;
		 *p='\0';
		}
	return s;
}

void	strdel(char *s,char t)
{
	while(*s!=t)
		{
		 if (*s=='\0')
		 	return; else
		 	++s;
		}
	while(*(s+1)!='\0')
		{
		 *s=*(s+1);
		 ++s;
		}
	*s='\0';
}

char	chk_kanji(const char *s,const char *t)
{
	char	r=0;

	while (iskanji(*t))
		{
		 ++r;
		 if (!(iskanji2(*t)) ||s == t)
		 	break;
		 --t;
		}
	return r;
}

void	strjncat(char *s,const char *t,size_t l)
{
	if (l==0)
		return ;

	if (chk_kanji(t,t+l-1)%2==1)
		--l;
	strncat(s,t,l);
}

void	strjncpy(char *s,const char *t,size_t l)
{
	if (l==0)
		return ;

	if (chk_kanji(t,t+l-1)%2==1)
		--l;
	memcpy(s,t,l);
	s[l]='\0';
}

void	lstrjncpy(char *s,const char lerge *t,size_t l)
{
	char	buf[LN_buf+1];

	if (l>LN_buf)
		error("");

	lstrcpy(buf,t);
	strjncpy(s,buf,l);
}

void	prt(const char *s,...)
{
	va_list	ap;
	char	*p;
	char	buf[LN_longbuf+1];

	va_start(ap,s);
	strcpy(buf,s);
/*dbprintf("[%p]",s);*/
	for(;;)
		{
		 p=va_arg(ap,char *);
		 if (p==NULL)
		 	break;
		 strcat(buf,p);
/*dbprintf("[%p]",p);*/
		}
	putz(buf);
}

void	sprt(char *s,...)
{
	va_list	ap;
	char	*p;

	va_start(ap,s);
	*s='\0';
	for(;;)
		{
		 p=va_arg(ap,char *);
		 if (p==NULL)
		 	return;
		 strcat(s,p);
		}
}

void	fprt(FILE *fp,...)
{
	va_list	ap;
	char	*p;

	va_start(ap,fp);
	for(;;)
		{
		 p=va_arg(ap,char *);
		 if (p==NULL)
		 	return;
		 fputs(p,fp);
		}
}

	char	ring_temp[MAX_ringtemp];
	size_t	ring_ptr=0;

char	*getring(size_t u)
{
	char	*p;

	if (u>MAX_ringstr)
		error(lpu(u,0));
	if (ring_ptr+u>MAX_ringtemp)
		ring_ptr=0;
	p=&ring_temp[ring_ptr];
	*p='\0';
/* dbprt("  Ring:",lpx((uint)p,4),"(",lpu(u,0),")\n",NULL); */
	ring_ptr+=u;
	return p;
}

char	*cps(char c)
{
	char	*p;

	p=getring(2);
	p[0]=c;
	p[1]='\0';
	return p;
}

char	*nps(char *s,size_t l)
{
	char	*p;

	if (l==0)
		l=strlen(s);

	if (s==NULL)
		{
		 p=getring(1);
		 *p='\0';
		} else
		{
/*dbprt("nps[",s,":",lpu(l,0),"]\n",NULL);*/
		 p=getring(l+1);
		 strjncpy(p,s,l);
		 addspc(p,l);
		}
	return p;

}

char	*fps(char lerge *s,size_t l)
{
	char	*p;

	if (l==0)
		l=lstrlen(s);

/*dbprt("fps (x)[",lpu(l,0),"]\n",NULL);*/
	p=getring(l+1);
	lstrjncpy(p,s,l);
/*dbprintf("fps[%s]\n",p);*/
	addspc(p,l);
	return p;
}

char	*digprt(char *s,ulong d,char l,int u)
{
	char	*p;
	char	a;
	long	b;
	char	digcode[]={"0123456789abcdef"};
	char	f;
	char	skip_chr[2]={'0',' '};
	ulong	dev[3][13]=
	{
		{
		 01,010,0100,01000,010000,0100000,
		 01000000,010000000,0100000000,01000000000,010000000000,0100000000000
		 ,0
		},
		{
		 1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,0
		 ,0,0
		},
		{
		 0x1,0x10,0x100,0x1000,0x10000,0x100000,0x1000000,0x10000000,0,0,0,0,0
		}
	};

	if (u==0)
		{
		 f=2;
		 u=12;
		} else
		{
		 if (u>0)
		 	f=0; else
		 	{
		 	 u=0-u;
		 	 f=1;
		 	}
		}

	p=s;
	b=dev[l][u];
	if (b!=0)
		d%=b;
	for(;u>0;)
		{
		 --u;
		 b=dev[l][u];
/*dbprintf("%d:%ld/%ld=%ld\n",u,d,b,b!=0?d/b:0);*/
		 if (b==0)
		 	continue;
		 a=d/b;
		 if (a==0)
		 	{
		 	 if (u==0)
		 	 	f=0;
			 if (f==2)
			 	continue;
			 *s++=skip_chr[f];
			 continue;
			}
		 f=0;
		 *s++=digcode[a];
		 d-=a*b;
		}
	*s='\0';
/*dbprintf("%s]\n",p);*/
	return p;
}

long	xtoi(char *s)
{
	return strtol(s,NULL,16);
}

long	otol(char *s)
{
	return strtol(s,NULL,8);
}

#define	Lad_oct	0
#define	Lad_digit	1
#define	Lad_hex	2

char	*lpo(ulong d,int s)
{
	int 	a;

	if (s==0)
		a=13; else
		a=(s>0?s:0-s)+1;
	return digprt(getring(a),d,Lad_oct,s);
}

char	*lpu(ulong d,int s)
{
	int 	a;

	if (s==0)
		a=11; else
		a=(s>0?s:0-s)+1;
	return digprt(getring(a),d,Lad_digit,s);
}

char	*lpx(ulong d,int s)
{
	int 	a;

	if (s==0)
		a=9; else
		a=(s>0?s:0-s)+1;
	return digprt(getring(a),d,Lad_hex,s);
}

bool	fchk(const char *s,const char *mode)
{
	FILE	*fp;
	fp=fopen(s,mode);
	if (fp==NULL)
		return FALSE;
	fclose(fp);
	return TRUE;
}

FILE	*fop(const char *s)
{
	FILE	*fp;

dbprt("--FOpen:[",s,"]\n",NULL);
	fp = fopen(s,"r+");
	if (fp!=NULL)
		return fp;

	fp = fopen(s,"w+");
	if (fp!=NULL)
		return fp;

	error("");
	return NULL;
}

char	*pull(char *s,const char *p)
{
	char	*q;
	size_t	i;

	q=strchr(p,':');
	if (q!=NULL &&chk_kanji(p,q)%2==0)
		{
		 i=(size_t)q-(size_t)p;
		 strncpy(s,p,i);
		 s[i]='\0';
		 ++q;
		} else
		{
		 strcpy(s,p);
		 q=(char *)(p+strlen(p));
		} 
	return q;
}

bool	parse(const char *s,char *key,char *var)
{
	uint	i;
	char	*p;

	while(isspace(*s))
		if (*s=='\0')
			return FALSE; else
			++s;


	s=pull(key,s);
	p=strchr(key,'\n');
	if(p!=NULL)
		*p='\0';
	i=0;
	while(!isspace(*key)&&i<LN_key)
		{
		 *key++=toupper(*key);
		 ++i;
		}
	*key='\0';


	if (*s=='\0')
		return FALSE;
	p=strchr(s,'\n');
	if(p!=NULL)
		*p='\0';
	strjncpy(var,s,LN_str);
	delspc(var);

/*dbprintf("parse[%s][%s]\n",key,var);*/
	return TRUE;
}

void	lo_line(char *s)
{
	uint	i,j;
/*	char	buf[LN_buf+1];*/
	char	*p;

	p=getring(LN_buf+1);

	escchk(p,s,TRUE);
	j=67-strlen(p);
	i=j%2;

	prt(SC_frame "  =-=-=-= " ESC_norm,p,SC_frame,nps("  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="+i,j-1),ESC_norm "\n",NULL);

#if 0
	sprt(buf,SC_frame "  =-=-=-= " ESC_norm,s,SC_frame,NULL);
	strncat(buf,"  =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="+i,j-1);
	putz(buf);
	msgout(ESC_norm);
#endif
}

void	lo_line2()
{
	msgout(SC_frame2 "  --------------------------------------------------------------------------"ESC_norm );
}

void	lo_line1()
{
	msgout(SC_frame "  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" ESC_norm);
}

SHELL	void	null_op()
{
}

void	sys_error(char *s,uint l,char *t)
{
	char	buf[LN_buf+1];

	sprt(buf,s," ",lpu(l,0),":",stredatime(getdatime())," Error:",t,NULL);
	add_text("errors",buf);
	msgout(buf);
	msgout("エラー発生。システムを一旦終了します\n"
			"再度アクセスを行う場合は五分以上待ってください。\n"
			"システム設計者などに、エラー発生までの様子を報告してください。\n"
			"五秒で切断します。");
	fcloseall();
/* SION保存処理.... */

	wait_minute(5);

	reset();
}

char	kururu[4][4+1]={"＼\b\b","｜\b\b","／\b\b","一\b\b"};

bool	disp_chk()
{
	uint	f;
	uint	a;

	f=0;
	a=0;
	for(;;)
		{
		 switch(getn())
		 	{
		  case 0:
		  	 if (f==0)
		  	 	return FALSE;
		  	 ++a;
			 if (a>200)
			 	{
			  	 a=0;
			  	 putz(kururu[f-1]);
			  	 ++f;
			  	 if (f>4)
			  	 	f=1;
			  	}
		  	 continue;
		  case _PAUSE:
		  	 if (f>0)
		  	 	continue;
		  	 f=1;
		  	 putz(ESC_csroff "[Pause ―]\b\b\b");
		  	 continue;
		  case _ABORT:
		  case _CAN:
			 if (f==0)
			 	msgout("[Break！] "); else
		 	 	msgout("\b\b\b\b\b\b\b[Break！] ");
		  	 return TRUE;
		  default:
		  	 if (f>0)
		 	 	putz("\b\b\b\b\b\b\b          \b\b\b\b\b\b\b\b\b\b");
		  	 return FALSE;
		 	}
		}
}

void	_disp(FILE *fpr)
{
	char	buf[LN_longbuf+1];

	putz(ESC_csroff);
/*	flow_on();*/
	while(fgets(buf,sizeof(buf),fpr)!=NULL)
		{
		 if (disp_chk())
		 	break;
		 putz(buf);
		}
/*	flow_off();*/
	putz(ESC_csron);
}

uint	binsrch(char *s,size_t size,char c)
{
	uint	i,j;

	for(i=0,j=0;j<size;++j)
		{
		 if (s[j]==c)
			{
		 	++i;
			}
		}
	return i;
}

uint	binchr(char *s,size_t size,char c,uint count)
{
	uint	i,j;


	for(i=0,j=0;j<size;++j)
		{
		 if (s[j]!=c)
		 	continue;
		 ++i;
		 if (i>=count)
		 	return j;
		}
	error("");
	return NULL;/* DUMMY */
}

void	disp_last(FILE *fpr,uint off)
{
	char	buf[256];
	uint	i,j,k;
	ulong	l;

	fseek(fpr,0,SEEK_END);
	l=ftell(fpr);

	for(k=0;;)
		{
		 i=l>256?256:l;
		 l-=i;
		 clearerr(fpr);
		 fseek(fpr,l,SEEK_SET);
		 i=read(fileno(fpr),buf,i);
/*
		 if (l>256&&i!=256)
		 	{
		 	 sprt(buf,lpu(l,0)," ",lpu(i,0),NULL);
		 	 error(buf);
		 	}
*/
		 j=binsrch(buf,i,'\n');
		 k+=j;
		 if (k>off)
		 	{
		 	 j=binchr(buf,i,'\n',k-off);
		 	 fseek(fpr,l+j+1,SEEK_SET);
		 	 break;
		 	}
		 if (l==0)
			{
			 fseek(fpr,0,SEEK_SET);
			 break;
			}
		}
	_disp(fpr);
	return ;
}

SHELL	void	disp_text(char *name)
{
	FILE	*fpr;

	fpr=op_rtext(name);
	if (fpr==NULL)
		return;
	rewind(fpr);
	_disp(fpr);
	fclose(fpr);
}

SHELL	void	disp_four(char *name)
{
	FILE	*fpr;

	fpr=op_rtext(name);
	if (fpr!=NULL)
		{
		 disp_last(fpr,4);	/* ラスト4 のみしかみない */
		 fclose(fpr);
		}
}

SHELL	void	disp_sixt(char *name)
{
	FILE	*fpr;

	fpr=op_rtext(name);
	if (fpr!=NULL)
		{
		 disp_last(fpr,16);	/* ラスト16のみしかみない */
		 fclose(fpr);
		}
}

void	add_text(char *name,char *s)
{
	FILE	*fpw;

	fpw = op_text(name);
	fseek(fpw,0,SEEK_END);
	fprt(fpw,s,"\n",NULL);
	fclose(fpw);
}

uint	getunumber(const char *s)
{
	s=strpbrk(s,"0123456789");
	if (s==NULL)
		return ERROR;
	return atoi(s);
}

uint	getnumber()
{
	char	buf[5+1];
	uint	u;

	for(;;)
		{
		 getstr(buf);
		 u=getunumber(buf);
		 if (u!=ERROR)
		 	return u;
		 msgout("数字をきちんと入力してください。");
		}
}

uint	getdnumber(const char *s,uint u)
{
	uint	i;
	char	*p;

	p=strpbrk(s,"0123456789");
	if (p==NULL)
		return ERROR;
	i=atoi(p);
	if (s<=p)
		return i;
	switch (*(p-1))
		{
	 case '-':
	 	 i=-i;
	 case '+':
	 	 return u+i;
	 default:
		 return i;
		}
}

char	*skip_kanji(char *s)
{

	while(iskanji(*s))
		{
		 ++s;
		 if (!iskanji2(*s))
		 	{
		 	 --s;
		 	 break;
		 	}
		 ++s;
		}
	return s;
}

ulong	fsize(FILE *fp)
{
	ulong	lp;

	fseek(fp,0,SEEK_END);
	lp=ftell(fp);
	rewind(fp);
	return lp;
}

FILE	*fopr(const char *s)
{
	FILE	*fp;

dbprt("--FOpenR:[",s,"]\n",NULL);
	fp = fopen(s,"r+b");
	if (fp!=NULL)
		return fp;

	fp = fopen(s,"w+b");
	if (fp!=NULL)
		return fp;

	error("");
	return NULL;
}

void	lfread(void lerge *b,size_t u,FILE *fp)
{
	void	*p;
	size_t	i;

	p=getring(MAX_ringstr);
	while(u>0)
		{
		 i=(u>MAX_ringstr)?MAX_ringstr:u;
		 fread(p,i,1,fp);
		 lmemcpy(b,p,i);
		 b= (char lerge *)b +i;
		 u-=i;
		}
}

void	lfwrite(void lerge *b,size_t u,FILE *fp)
{
	void	*p;
	size_t	i;

	p=getring(MAX_ringstr);
	while(u>0)
		{
		 i=(u>MAX_ringstr)?MAX_ringstr:u;
		 lmemcpy(p,b,i);
		 clearerr(fp);
		 if (fwrite(p,i,1,fp)==0)
dbmsgout("fwrite error");
		 b= (char lerge *)b +i;
		 u-=i;
		}
}

void	fcopy(FILE *fpw,FILE *fpr)
{
	int 	i;
	char	buf[256];

	putz(IC_act  "ファイルをコピーしています。\r");

	rewind (fpr);
	rewind (fpw);

/* dbprt("fcopy :",lpu(ftell(fpw),0),NULL); */
	for(;;)
		{
		 i=fread(buf,1,sizeof(buf),fpr);
		 if (i==0)
		 	break;
		 fwrite(buf,1,i,fpw);
		}
/* dbprt(":",lpu(ftell(fpw),0),"\n",NULL); */

	fflush(fpw);
	chsize(fileno(fpw),ftell(fpw));/* ごみ消し */

	msgout(IC_ok "ファイルコピー転送を終了しました。");
}

#if 0
void	name_ext(char *s,char *dir,char *name,char *ext)
{
	char	buf[LN_buf+1];
	char	*p;
	struct	find_t	buffer;
	uint	n,i;

	sprt(buf,dir,name,".",ext,NULL);
	if (_dos_findfirst(buf,_A_NORMAL||_A_HIDDEN
					||_A_SYSTEM||_A_SUBDIR,&buffer)!=0)
		{
		 strcpy(s,buf);
		 return;
		}

	n=0;
	sprt(buf,dir,name,".*",NULL);
	i=_dos_findfirst(buf,_A_NORMAL||_A_HIDDEN||_A_SYSTEM||_A_SUBDIR,&buffer);
	for(;;)
		{
		 if (i!=0)
		 	break;
		 p=strrchr(buffer.name,'.');	/* 拡張子を抜き出す。*/
		 if (p!=NULL)
		 	{
			 if (n<atoi(p))
			 	n=atoi(p);
		 	}
		 i=_dos_findnext(&buffer);
		}
	if (n>999)
		error("");
	sprt(s,name,".",lpu(n,3),NULL);
}
#endif

void	name_ext(char *s,char *dir,char *name,char *ext)
{
	char	buf[LN_buf+1];
	uint	u;

	u=atoi(ext);
	for (;u<1000;++u)
		{
		 name_pooldir(buf);
		 sprt(buf,dir,s,NULL);
		 if (!fchk(buf,"rb"))
		 	return;
		 sprt(s,name,".",lpu(u,3),NULL);
		}
	error("これ以上一意的な拡張子をつける事は出来ません。");

/*	sprt(s,dir,*dir!='\0'?"\\":"",name,*ext!='\0'?".":"",ext,NULL);*/
		/* ディレクトリ、拡張子ある無しにあわせて名前作成 */
}

	/* Editor */

void	_ma_charout(char code)
{
	if (!iscntrl(code))
		putz(cps(code)); else
		prt(SC_ctrl,cps(code+'@'),ESC_norm,NULL);
}

#define	_ma_bs()	putz(frug&F_guide?SC_guide"\b_\b"ESC_norm:"\b \b")

uint	ed_bs(char *buf,uint x,uint frug)
{
	uint	i;

	if (x==0)
		return x;

	buf[--x]='\0';
	_ma_bs();
	if (x==0)
		return x;

	i=chk_kanji(buf,buf+x-1);
	if (i&1)
		{
		 _ma_bs();
		 buf[--x]='\0';
		}
	return x;
}

static	char	guidebuf[LN_longbuf+1];
static	uint	guidenum=0;

void	guide_clr()
{
	guidenum=0;
	*guidebuf='\0';
}

void	guide_up(char *s)
{
	char	*p;

	p=strrchr(s,'\n');

	if (p!=NULL)
		s=p;
	strcpy(guidebuf,s);
	guidenum=strlen(s);
}

void	guide_out(size_t l)
{
	char	buf[79+1];

	if (l>79)
		l=79;	/*....*/
	memset(buf,'_',l);
	buf[l]='\0';

	prt(ESC_csroff SC_guide ,buf,ESC_norm,"\r",guidebuf,ESC_csron,NULL);
}

char	__retcode[]={"\r\x1b\x1e\x1f\x03\x12\x0a\x0b"};

#define	CF_norm	0
#define	CF_del	1
#define	CF_add	2


char	_ma_getstr(char *s,size_t l,char frug,char *retcode)
{
	char	c,cf;
	uint	x;
	bool	f;
	time_t	t,u;

	if (l>79)
		l=79;	/*....*/
/* '\0'の為の1byte も含む つまりstrlen+1 */

/* prt(lpu(l,0),"\n",NULL);*/

	if (*s!='\0')
		guide_up(s);
	if (frug&F_guide)
		guide_out(l);
	guide_clr();

	f=FALSE;
	cf= frug&F_seqedit&& iskanji(*s)?CF_add:CF_norm;
	u=0;
	x=strlen(s);
	putz(ESC_csron ESC_norm);

	for(;;)
		{
		 c=getn();
		 if (strchr(retcode,toupper(c))&&c>0)
		 	break;
		 if (c!=0)
		 	{
			 if (u!=0)
			 	{
			 	 u=0;
				 putz("\b\b\b\b\b\b\b\b\b\b                "
			 		 		"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
			 	}
		 	 f=FALSE;
		 	 if (cf==CF_del)
		 	 	{
		 	 	 cf=CF_norm;
		 	 	 continue;
		 	 	}
		 	} else
		 	{
#if 0
		 	 if (!f)
		 	 	{
		 	 	 f=TRUE;
		 	 	 timer_res(t);
		 	 	} else
		 	 	{
		 	 	 if (timer_pas(t)<180)
		 	 	 	continue;
		 	 	 if (timer_pas(t)>360)
		 			{
				 	 msgout("\b\b\b\b\b\b\b\b\b\b"
		 	 			"活動停止！危険域に入ります。生命維持に支障が････ｶﾞｰ");
					 longjmp(ch.env,1);
			 	 	}
				 if (u==0)
				 	{
				 	 putz("<活動限界:180秒>\b\b\b\b\b\b");
					 timer_res(u);	/* 1秒syncのため */
					 continue;
				 	}
				 if (timer_pas(u)>0)
				 	{
					 timer_res(u);	/* 1秒syncのため */
				 	 prt(lpu(360-timer_pas(t),3),"\b\b\b",NULL);
				 	}
				 continue;
			 	}
#endif
		 	}

		 if (iscntrl(c))
		 	{
		 	 switch(c)
		 	 	{
		 	  case 0:
		 	  	 continue;
		 	  case _BS:
		 	  	 x=ed_bs(s,x,frug);
		 	  	 continue;
			  case _TAB:
		/* local char c; */
		 	 	 c=min((x/8+1)*8,l);
		 	 	 while(x <c-1)
		 	 	 	{
		 	 	 	 s[x++]=' ';
		 	 	 	 putz(" ");
		 	 	 	}
		 	 	 s[x]='\0';
		 	 	 c=' ';
			  	 break;
		 	  case _ESC:
		 	  case _BEEP:
		 	  	 if (frug&F_esc)
		 	  	 	break;
		 	  default:
		 	  	 putz("\a");
		 	  	 continue;
		 	  	}
		 	}
		 if (cf==CF_add)
		 	 cf=CF_norm; else
		 	 {
			  if (x+(iskanji(c)?1:0)>=l)
			  	{
			  	 if (frug&F_seqedit)
			 		return c;
			 	 putz("\a");
			 	 if (iskanji(c))
			 	 	cf=CF_del;
			 	 continue;
			 	}
		 	  cf=iskanji(c)?CF_add:CF_norm;
		 	 }

/*		 if (x>=l)
			error("");*/
		 s[x]  =c;
		 s[++x]='\0';
		 _ma_charout(frug&F_mask?'X':c);
/*prt("\n", lpu(x,0),NULL);*/
		}
	return c;
}

char	_getstr(char *s,size_t l)
{
	char	i;

	*s='\0';
	i=_ma_getstr(s,l,F_normal+F_guide+F_esc,__retcode);

	msgout("");
	return i;
}

char	_getrstr(char *s,size_t l)
{
	char	i;

	putz(s);
/*	if (l-strlen(s)>0)
		guide_out(l-strlen(s));
*/
	i=_ma_getstr(s,l,F_normal+F_guide,__retcode);
	msgout("");
	return i;
}

char	__retynq[]={"\r\n YN"};

bool	ynq()
{
	char	buf[1];

	putz("([Y]es/No) %");
	for(;;)
		{
		 switch (toupper(_ma_getstr(buf,0,F_normal,__retynq)))
		 	{
		  case 'Y':
		  case _CR:
		  	 msgout("Yes");
			 return TRUE;
		  case ' ':
		  case 'N':
			 msgout("No");
			 return FALSE;
		  	}
		}
}

/* !!エディタ時に、前のラインからの継承時に動作が変。*/
