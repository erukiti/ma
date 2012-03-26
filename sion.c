#include "ma.h"
#define	PT_error	(void *)1

	static	var_t	*tmp;

/* SionIndex */

void	si_add()
{
/* dbprt("  si_add:",lpu(si.num,0),"/",lpu(si.fb->max,0),"\n",NULL); */
	writeb(&index,si.num++,si.fb);
}

void	index_make()
{
/*	char	*p;*/
	char	f;
	ulong	lp;
	char	buf[LN_longbuf+1];

	if (sion.fp==NULL)
		error ("");

	putz(IC_dact "唯今、Sion Indexを作成しています。\r");
/* 
dbprt("Index_make\n",NULL); */
	f=0;
	for (;;)
		{
		 lp=ftell(sion.fp);
		 if (fgets(buf,sizeof(buf),sion.fp)==NULL)
		 	break;
		 if(buf[0]=='/'&& buf[1]=='/')
			{
/* dbprt(" >",lpu(lp,0),":",buf,NULL); */
			 if (f==1)
			 	{
			 	 index.end=lp;
			 	 si_add();
			 	}
		     f=1;

			 memset(&index,0,sizeof(index));
		     sion.header = lp;
		     index.header= lp;
		     index.body=NULL;
			 parse(buf+2,index.type,index.name);
/*
		 	 sion_read(tmp);
			 index.datime=atol(getvar("datime",nowvar));
			 index.frug=atol(getvar("frug",nowvar));
*/		    }
		 if(f==1 && *buf=='*')
		 	index.body = lp;
		}
	if (f==1)
		{
		 index.end=lp;
		 si_add();
		}
	putz(IC_dok "Indexを作成しました               \r");
}

uint	si_seek(uint l)
{

/* dbprt("  si_seek ",lpu(l,0),NULL); */

	si.num=l;
	sion.num=l;

	if (!readb(&index,l,si.fb))
		{
/* dbmsgout("  is not found"); */
		 fseek(sion.fp,0,SEEK_END);
		 sion.header=ERROR;
		 sion.body=NULL;
		 return 0;
		}

/* dbprt("[",index.type,":",index.name,"]",lpu(index.header,0),"\n",NULL); */
	fseek(sion.fp,index.header,SEEK_SET);
	sion.header=index.header;
	sion.body=index.body;
	sion.end=index.end;
	strcpy(sion.type,index.type);
	strcpy(sion.name,index.name);
	return index.body!=NULL?2:1;
}

uint	si_next()
{
	return si_seek(si.num+1);
}

void	si_renew()
{
/* dbmsgout("si_renew"); */

	truncb(si.num,si.fb);
	fseek(sion.fp,index.header,SEEK_SET);
	index_make();
}

void	si_clr()
{
/* dbmsgout("si.clr"); */
	truncb(0,si.fb);
	si.num=0;
	fseek(sion.fp,0,SEEK_SET);
}

void	si_mk()
{
	memset(&index,0,sizeof(index));
	index.header=sion.header;
	index.body=sion.body;
	index.end=sion.end;
/*	index.datime=atol(getvar("datime",v));
	index.frug=atoi(getvar("frug",v));*/
	strcpy(index.type,sion.type);
	strcpy(index.name,sion.name);
}

void	si_new()
{
dbmsgout("! si_new");
	si_clr();
	index_make();
}


/* Sion */

FILE	*sion_bk()
{
	FILE	*fpt;
	char	buf[LN_buf+1];
	int 	i;
/*
dbmsgout("| sion_bk");*/
	fpt=tmpfile();
	if (fpt==NULL)
		error("");
	clearerr(sion.fp);
	for(;;)
		{
		 i=fread(buf,sizeof(buf),1,sion.fp);
		 if (i==0)
		 	break;
		 fwrite(buf,sizeof(buf),1,fpt);
		}
	return fpt;
}

char	Er_name[]={""};

void	_ma_write(var_t *t,FILE *fp,FILE *fpt)
{
	int 	i;
	char	f;
	char	buf[LN_buf+1];
	ulong	lp,bp;
/*
dbputz("||_ma_write (var ");*/
	if (t==NULL)
		error("");

	defvar("frug","0",t);

	clearerr(sion.fp);
	sion.header=ftell(sion.fp);
	fprt(sion.fp,"//",sion.type,":",sion.name,"\n",NULL);
	for (i=0;i<varsize(t);++i)
		{
/*		 setnvar(i,delspc(getnvar(i,t)),t);*/
		 fprt(sion.fp,nps(mkkey(i,t),LN_key),":"
		 	,nps(getnvar(i,t),LN_var),"\n",NULL);
		}

	f=0;
	if (fp==NULL)
		sion.body=NULL; else
		{
		 sion.body=ftell(sion.fp);
		 if (fp==PT_error) /* !! */
		 	f=1; else
		 	{
/*
dbputz("body  ");*/
			 fputs("*Body\n",sion.fp);
			 while(fgets(buf,sizeof(buf),fp))
				fputs(buf,sion.fp);
			 sion.end=ftell(sion.fp);
			 fclose(fp);
			}
		}

	if (fpt!=NULL)
		{
/*
dbputz("temp ");*/
		 fseek(fpt,0,SEEK_SET);
		 for(;;)
		 	{
		 	 i=fread(buf,sizeof(buf),1,sion.fp);
		 	 if (i==0)
		 	 	break;
		 	 fwrite(buf,sizeof(buf),1,fpt);
		 	}
		 fclose(fpt);

		 if (f==1)
		 	{
			 bp=ftell(sion.fp);
			 fseek(sion.fp,sion.body,SEEK_SET);
			 for (;;)
				{
				 lp=ftell(sion.fp);
				 if (fgets(buf,sizeof(buf),sion.fp)==NULL)
				 	break;
				 if((buf[0]=='/'&& buf[1]=='/')||*buf=='*')
				 	break;
				}
			 sion.end=lp;
			 fseek(sion.fp,bp,SEEK_SET);
			}
		}
/*
dbmsgout(")");*/
	chsize(fileno(sion.fp),ftell(sion.fp));/* ごみ消し */
	si_mk();
}

void	sion_write(FILE *fp,var_t *t)
{
	char	buf[LN_buf+1],typetmp[LN_buf+1],nametmp[LN_buf+1];
	ulong	bp,np,lnp;
	char	*p,*q;
	FILE	*fpt;
	int		i,j;


	if (sion.fp==NULL)
		error ("");

dbmsgout("| sion_wr");
	clearerr(sion.fp);

	strcpy(typetmp,sion.type);
	strcpy(nametmp,sion.name);
	if (sion_sr(typetmp,nametmp)==0) /* sion_srを完全に行うため。*/
		{
/*
dbmsgout("||New entry");*/
		 _ma_write(t,fp,NULL);
		 si_add();
		 return;
		}
	sion_read(tmp);
/* varview(tmp); */
/*	lp=sion.header;*/
	bp=sion.body;
	i=sion_next();
	lnp=sion.header;

	if (i==0)
		{
/*
dbmsgout("||Last entry ");*/
		 if (bp==NULL||fp!=NULL)
		 	fpt=NULL; else
		 	{
		 	 fseek(sion.fp,bp,SEEK_SET);
		 	 fpt=sion_bk();
		 	 if (fp==NULL)
		 	 	fp=PT_error;
		 	}
		 sion_sr(typetmp,nametmp);
		 _ma_write(t,fp,fpt);
		 si_add();
		 return;
		}

	if (fp!=NULL)
		{
/*
dbmsgout("||with body");*/
		 fpt=sion_bk();
		 sion_sr(typetmp,nametmp);
		 _ma_write(t,fp,fpt);
		 si_renew();
		 return;
		}

/* varview(t);*/

	if (varsize(t)>varsize(tmp))
		{
_1:
/*
dbmsgout("||_1:");*/
		 fseek(sion.fp,(bp!=NULL)?bp:lnp,SEEK_SET);
		 fpt=sion_bk();
		 sion_sr(typetmp,nametmp);
		 _ma_write(t,fp,fpt);
		 si_renew();
		 return;
		}

	sion_sr(typetmp,nametmp);
	fgets(buf,sizeof(buf),sion.fp); /* ヘッダ行を捨てる */
	defvar("frug","0",t);

/*
dbmsgout("||_2");*/
	i=0;
	for (;;)
		{
		 np=ftell(sion.fp);
		 if (!fgets(buf,sizeof(buf),sion.fp) ||*buf=='/' ||*buf=='*')
		 	break;
		 spc(buf);
		 fseek(sion.fp,np,SEEK_SET);

		 if (i>=varsize(t))
		 	{
		 	 buf[strlen(buf)-1]='\n';
		 	 fputs(buf,sion.fp);
		 	 continue;
		 	}

		 p=getnvar(i,t);
		 q=mkkey(i,t);
		 j=strlen(buf)-strlen(p)-LN_key-1-1;
		 if (j<0)
		 	goto _1;
/*dbprt("||",nps(mkkey(i,t),8),":",p,"\n",NULL);*/
		 fprt(sion.fp,nps(q,LN_key),":",nps(p,strlen(p)+j),"\n",NULL);
		 ++i;
		}
	si_mk();
	si_add();
}

void	sion_pageout()
{
	char	buf[LN_buf+1];

	clearerr(sion.fp);
	sion_getp();
	for(;;)
		{
		 if (ftell(sion.fp)>=sion.end ||disp_chk()
		 	|| fgets(buf,sizeof(buf),sion.fp)==0)
		 	return;
		 putz(buf);
		}
}

bool	sion_gets(char *s)
{
	clearerr(sion.fp);
	if (ftell(sion.fp)>=sion.end)
		return 0;
	return fgets(s,LN_buf,sion.fp)!=NULL;
}

ulong	sion_getp()
{
	char	buf[LN_buf+1];

	if (sion.fp==NULL)
		error ("");
	if (sion.header==ERROR)
		return NULL;

	if (sion.body==NULL)
		return NULL;

	clearerr(sion.fp);

	fseek(sion.fp,sion.body,SEEK_SET);
dbprt(" -rb:",lpu(sion.body,0),"\n",NULL);
	fgets(buf,sizeof(buf),sion.fp); /* ヘッダ行を捨てる */
	return ftell(sion.fp);
}

void	sion_read(var_t *t)
{
	uint	i;
	char	buf[LN_buf+1];
	char	*p,*q,*r;

	if (sion.fp==NULL)
		error ("");

	if (sion.header==ERROR)
		return;

	clearerr(sion.fp);
	fseek(sion.fp,sion.header,SEEK_SET);
	fgets(buf,sizeof(buf),sion.fp); /* ヘッダ行を捨てる */
/* dbputz(buf); */
/* monitor("| ----- rd ------"); */
	q=getring(LN_buf);
	r=getring(LN_buf);
	clrvar(t);
	i=0;
	while (fgets(buf,sizeof(buf),sion.fp))
		{
/* dbputz(buf); */
		 if (*buf=='\0' ||*buf=='*' ||*buf=='/')
		 	break;
		 p=strchr(buf,'\n');
		 if (p!=NULL)
		 	*p='\0';
		 if (!parse(buf,q,r))
		 	continue;
		 ++i;
		 setvar(q,r,t);
		}
/* monitor("||----readed----"); */
}

uint	sion_sr(const char *t,const char *n)
{
	uint	i;


dbprt("| sion_sr[",t,":",n,"]",NULL);
	if (sion.fp==NULL)
		error ("");
	i=si_seek(0);
	for(;;)
		{
		 if (i==0)
			{
			 strcpy(sion.type,t);
			 strcpy(sion.name,n);
		 	 return 0;
		 	}
		 if (stricmp(index.type,t)==SAME &&stricmp(index.name,n)==SAME)
		 	break;
		 i=si_next();
		}
	return i;
}

uint	sion_first()
{
	return si_seek(0);
}


uint	sion_next()
{

dbmsgout("| next");
	if (sion.fp==NULL)
		error ("");
	return si_next();
}

void	sion_close()
{

dbmsgout("|close");

	if (sion.fp==NULL)
		error ("");
	fclose(sion.fp);
	fcloseb(si.fb);
	sion.fp=NULL;
}

uint	sion_srnext(const char *k,ulong u)
{
	uint	i,l;

	i=sion_sr(k,lpu(u,0));
	if (i>0)
		return i;

	sion_first();
	while((i=sion_next())>0)
		{
		 l=atol(sion.name);
		 if (l>u)
		 	return i;
		}
	return i;
}

void	sion_init()
{
	sion.fp=NULL;
	sion.frug=FALSE;
	tmp=varinit(LN_str,MAX_var);
}

void	sion_fin()
{
	varfin(tmp);
}

FILE	*sion_fop(const char *s)
{
	char		buf[LN_buf+1];
	struct stat	st;
	time_t		t;
	bool		f;


dbmsgout("  sion_fop");
	if (sion.fp!=NULL)
		error("");

	sion.fp=fop(s);
	setvbuf(sion.fp,NULL,_IOFBF,BUFSIZ);
	strjncpy(sion.fname,s,LN_fname);
	sion.frug=FALSE;

	sprt(buf,s,".idx",NULL);
	f=fchk(buf,"rb");
	si.fb=fopb(buf,sizeof(index_t));
	setvbuf(si.fb->fp,NULL,_IOFBF,BUFSIZ);

	if (!f||si.fb->max==0)
		{
		 si_new();
		 return sion.fp;
		}
	fstat(fileno(sion.fp),&st);
	t=st.st_mtime;
	fstat(fileno(si.fb->fp),&st);
	if (t>st.st_mtime)
		si_new();
	return sion.fp;
}

/* Variable  */

static	char	Er_var[]={"Var構造体のサイズが小さすぎます。"};

void	clrvar(var_t *v)
{
	v->num=0;
}

char	*mkkey(uint n,var_t *v)
{
	if (n>=v->max)
		error("");
	return &v->key[(LN_key+1)*n];
}

char	lerge	*mkvar(uint n,var_t *v)
{
	if (n>=v->max)
		error("");
	return &v->member[(v->vnum+1)*n];
}

var_t	*varinit(uint n,uint m)
{
	var_t	*v;

	v=malloc(sizeof(var_t));
	if (v==NULL)
		error("");
	v->num=0;
	v->vnum=n;
	v->max=m;
	v->key=malloc((LN_key+1)*m);
	v->member=lmalloc((n+1)*m);
	if (v->key==NULL||v->member==NULL)
		error(lpu(farcoreleft(),0));
	return v;
}

void	varfin(var_t *v)
{
	v->max=0;
	v->num=0;
	v->vnum=0;
	free (v->key);
	lfree(v->member);
	free(v);
}

void	varview(var_t *v)
{
	uint	i;

dbmsgout("------");
	for (i=0;i<varsize(v);++i)
		{
dbprt("  ",nps(mkkey(i,v),LN_key),":[",getnvar(i,v),"]\n",NULL);
		}
dbmsgout("------");
}

uint	newvar(var_t *v)
{
	if (v->num >=v->max)
		error(Er_var);

	++v->num;
	*mkkey(v->num-1,v)='\0';
	*mkvar(v->num-1,v)='\0';

	return v->num-1;
}

uint	varnum(const char *s,var_t *v)
{
	uint	i;

/*dbprintf("varnum:%d",v->num);*/
	for (i=0;i<varsize(v);++i)
		{
/*dbprintf(" %d:[%s]\n",i,mkkey(i,v));*/
		 if (stricmp(mkkey(i,v),s)==SAME)
		 	return i;
		}
	return ERROR;
}

char	*getnvar(uint n,var_t *v)
{
	return fps(mkvar(n,v),0);
}

char	*getvar(const char *s,var_t *v)
{
	uint	i;

	i=varnum(s,v);
	return (i==ERROR)?getring(LN_buf):getnvar(i,v);
}

void	setvar(const char *s,const char *t,var_t *v)
{
	uint	i;

	i=varnum(s,v);
	if (i!=ERROR)
		{
		 setnvar(i,t,v);
		 return ;
		}
	i=newvar(v);
	strjncpy(mkkey(i,v),s,LN_key);
	lstrncpy(mkvar(i,v),t,v->vnum);
}

void	 setnvar(uint n,const char *t,var_t *v)
{
	lstrncpy(mkvar(n,v),t,v->vnum);
}

void	defvar(const char *s,const char *t,var_t *v)
{
	uint	i;

	i=varnum(s,v);
	if (i!=ERROR)
		return ;
	i=newvar(v);
	strjncpy(mkkey(i,v),s,LN_key);
	lstrncpy(mkvar(i,v),t,v->vnum);
}

/*void	varcpy(var_t *t,var_t *s)
{
	uint	i;

	clrvar(t);
	for (i=0;i<varsize(t);++i)
		setvar(mkkey(i,t),getnvar(i,t),s);
}*/

	/* Digitalize Selected Data */

char	dsd_div(char *s,char data[LN_key+1][MAX_dsd])
{
	char	*p;
	uint	i;

	p=s;
	i=0;
	while(*p!='\0' && i<MAX_dsd)
		{
		 p=pull(data[i],p);
		 ++i;
		}
	return (i==MAX_dsd &&*p!='\0')?MAX_dsd:i;
/* !!まだ、残りがあるときの戻り値をどうするのか(overflow) */
}

void	dsd_cup(char *s,char *data[],char num)
{
	char	*p;
	uint	i;

	p="";
	*s='\0';

	for (i=0;i<num;++i)
		{
		 if (strlen(s)+strlen(data[i])+1>=LN_dsd)
			break;
		 strcat(s,p);
		 strcat(s,data[i]);
		 p=":";
		}
}

char	dsd_mark(char *s,var_t *v,uint frug)
{
	char	buf[2+1];
	char	f[MAX_var];
	char	i,n;
	char	*p;
	char	data[LN_key+1][MAX_dsd];

	memset(f,0,sizeof(f));
	i=dsd_div(s,data);

	for (;i>0;--i)
		{
/*dbprt("- [",data[i-1],"]\n",NULL);*/
		 n=varnum(data[i-1],v);
		 if (n!=(char)ERROR)
		 	f[n]=1;
		}	/* デフォルトデータ処理 */

	for (;;)
		{
		 lo_line(frug?"選んでください":"デフォルトデータ");
		 for (i=0;i<varsize(v);++i)
		 	{
		 	 if (!frug&&f[i]==0)
		 	 	continue;
			 prt(f[i]>0?"●":"  ",lpu(i+1,-2)
			 		,DC_norm,getnvar(i,v),"\n",NULL);
			}
		 lo_line1();
		 getstr(buf);
		 if (toupper(*buf)=='Q' ||*buf=='\x1b')
		 	{
		 	 *s='\0';
		 	 return 0;
		 	}
		 if (toupper(*buf)=='?')
		 	{
		 	 frug=TRUE;
		 	 continue;
		 	}
		 i=atoi(buf);
		 if (i==0 ||i>varsize(v))
		 	break;
		 f[i-1]=1-f[i-1];
		 continue;
		}

	p="";
	*s='\0';
	n=0;

	for (i=0;i<varsize(v);++i)
		{
		 if (f[i]==0)
		 	continue;
		 ++n;
/*dbprt("- [",lpu(i,2),DC_norm,getnvar(i,v),"]\n",NULL);*/
		 strcat(s,p);
		 strcat(s,mkkey(i,v));
		 p=":";
		}
	return n;
}

void	dsd_select(char *s,var_t *v)
{
	char	buf[2+1];
	char	i;

	for (;;)
		{
		 lo_line("選んでください");
		 for (i=0;i<varsize(v);++i)
			prt(lpu(i+1,-2),DC_norm,getnvar(i,v),"\n",NULL);
		 lo_line1();
		 getstr(buf);
		 i=atoi(buf);
		 if (i==0 ||i>varsize(v))
		 	continue;
/*printf("[%-2d:%s]\n",i,getnvar(i-1,v));*/
		 strcpy(s,mkkey(i-1,v));
		 return;
		}
}

bool	dsd_chk(char *s,char *t)
{
/*	char	data1[LN_key+1][MAX_dsd];*/
	char	data2[LN_key+1][MAX_dsd];
	char	i;

/* dbprt("dsd_chk [",t,"]\n",NULL); */

/*	i=dsd_div(s,data1);*/
	i=dsd_div(t,data2);
	for (;i>0;--i)
		{
		 if (stricmp(data2[i-1],delspc(nps(s,0)))==SAME)
		 	return TRUE;
		}
	return FALSE;
}

/* Binary Archive */

fileb	*fopb(char *s,size_t n)
{
	uint	buf[MAX_fb/2];
	fileb	*fb;
	uint 	i;

	fb=malloc(sizeof(fileb));
	fb->size=n;
dbprt("--FOpenB:[",s,"]","\n",NULL);

	fb->fp=fopen(s,"r+b");
/* dbprt("--fb->fp:",lpu(fb->fp!=NULL,0),"\n",NULL); */
	if (fb->fp!=NULL)
		{
		 rewind(fb->fp);
		 i=fread(buf,MAX_fb,1,fb->fp);
/* dbprt("--i :",lpu(i,0),"\n",NULL); */
		 if (i==1)
		 	{
		 	 fseek(fb->fp,0,SEEK_END);
		 	 fb->max=buf[2];
		 	 if (BA_ver==buf[0] && n==buf[1]
		 	 		&& ftell(fb->fp)==MAX_fb+ (ulong)n*fb->max)
		 	 	{
dbmsgout("OK\n");
		 	 	 return fb;
		 	 	}
dbprt("--",lpu(ftell(fb->fp)-MAX_fb,0),":"
	,lpu((ulong)(n)*fb->max,0),":"
	,lpu(n,0),":",lpu(fb->max,0),"\n",NULL);
dbprt("--",lpu(BA_ver==buf[0],0)," ",lpu(n==buf[1],0)," ",
 lpu(ftell(fb->fp)==MAX_fb+ (ulong)n*fb->max,0),"\n",NULL);
		 	}
		 fclose(fb->fp);
		}

	fb->max=0;
	fb->fp=fopen(s,"w+b");
	if (fb->fp!=NULL)
		{
		 chsize(fileno(fb->fp),MAX_fb);
		 return fb;
		}

	free(fb);
	error("");
	return NULL;
}

void	fcloseb(fileb *fb)
{
	uint	buf[MAX_fb/2];

/* dbmsgout("--FCloseB");
   dbprt(lpu(fb->size,0),":",lpu(fb->max,0),"\n",NULL); */
	buf[0]=BA_ver;
	buf[1]=fb->size;
	buf[2]=fb->max;

	rewind(fb->fp);
	fwrite(buf,1,MAX_fb,fb->fp);
	fclose(fb->fp);
	free(fb);
}

void	writeb(void *b,uint n,fileb *fb)
{
dbprt("--writeB ",lpu(n,0),"/",lpu(fb->max,0),"\n",NULL);

	if (n>=fb->max)
		{
		 fb->max=n+1;
		 if (n>fb->max)
			 chsize(fileno(fb->fp),MAX_fb+fb->size*n);
		}

	clearerr(fb->fp);
	fseek(fb->fp,MAX_fb+(ulong)fb->size*n,SEEK_SET);
dbprt(lpu(MAX_fb+(ulong)fb->size*n,0),". ",lpu(ftell(fb->fp),0),NULL);
	if (fwrite(b,fb->size,1,fb->fp)!=1)
		error("");
dbprt("->",lpu(ftell(fb->fp),0),"\n",NULL);
}

bool	readb(void *b,uint n,fileb *fb)
{
/* dbprt("readB ",lpu(n,0),"/",lpu(fb->max,0),"\n",NULL); */
	if (n>=fb->max)
		{
		 return FALSE;
		}

	clearerr(fb->fp);
	fseek(fb->fp,MAX_fb+fb->size*n,SEEK_SET);
	if (fread(b,fb->size,1,fb->fp)!=1)
		error("");
	return TRUE;
}

void	truncb(uint n,fileb *fb)
{
/* dbprt("trucateB ",lpu(n,0),"\n",NULL); */
	fb->max=n;
	chsize(fileno(fb->fp),MAX_fb+fb->size*n);
}

bool	lreadb(void lerge *b,uint n,fileb *fb)
{
	void	*p;
	size_t	i;
	uint	m;

/* dbprt("lreadB ",lpu(n,0),"/",lpu(fb->max,0),"\n",NULL); */
	if (n>=fb->max)
		{
		 return FALSE;
		}
	clearerr(fb->fp);
	fseek(fb->fp,(ulong)(MAX_fb+fb->size*n),SEEK_SET);

	m=fb->size;
	p=getring(MAX_ringstr);
	while(m>0)
		{
		 i=(m>MAX_ringstr)?MAX_ringstr:m;
		 fread(p,i,1,fb->fp);
		 lmemcpy(b,p,i);
		 b= (char lerge *)b +i;
		 m-=i;
		}
	return TRUE;
}

void	lwriteb(void lerge *b,uint n,fileb *fb)
{
	void	*p;
	size_t	i;
	size_t	m;

dbprt("--lwriteB ",lpu(n,0),"/",lpu(fb->max,0),"\n",NULL);
	if (n>=fb->max)
		{
		 fb->max=n+1;
		 if (n>fb->max)
			 chsize(fileno(fb->fp),MAX_fb+fb->size*n);
		}
	clearerr(fb->fp);
	fseek(fb->fp,MAX_fb+(ulong)fb->size*n,SEEK_SET);
dbprt(lpu(MAX_fb+(ulong)fb->size*n,0),". ",lpu(ftell(fb->fp),0),NULL);

	m=fb->size;
	p=getring(MAX_ringstr);
	while(m>0)
		{
		 i=(m>MAX_ringstr)?MAX_ringstr:m;
		 lmemcpy(p,b,i);
		 if (fwrite(p,i,1,fb->fp)==0)
			error("fwrite error");
		 b= (char lerge *)b +i;
		 m-=i;
		}
dbprt("->",lpu(ftell(fb->fp),0),"\n",NULL);
/* fseek(fb->fp,0,SEEK_END);
   dbprt("(",lpu(ftell(fb->fp),0),")\n",NULL); */
}

