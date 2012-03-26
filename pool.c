#include "ma.h"
#include "pool.h"
#include "str.h"
#include "board.h"
#include "mio.h"

	/* pool のコンストラクタとデストラクタ */

void	pool_init()
{
	pool.var=varinit(LN_str,MAX_var);
}

void	pool_fin()
{
	varfin(pool.var);
}

	/* Pool Generic */

bool	pool_read(ulong num)
{
	char	*p;

	op_files();
	if (sion_sr("file",lpu(num,0))==0)
		{
		 sion_close();
		 error("");
		}
	sion_read(pool.var);

	pool.num=num;
	p=getvar("rname",pool.var);
	if (p==NULL)
		error("");/* ファイルの実体が存在していません。*/
	strcpy(pool.rname,p);
	p=getvar("name",pool.var);
	strcpy(pool.name,p==NULL?"noname":p);
	pool.size =atol(getvar("size",pool.var));
	pool.ftime=otol(getvar("ftime",pool.var));
	pool.count=atol(getvar("dlcount",pool.var));

/* pool.time(ダウンロード予定時間計算) */

	sion_close();
	return TRUE;
}

void	pool_write()
{
	setvar("size" ,lpu(pool.size,0),pool.var);
	setvar("ftime",lpo(pool.ftime,0),pool.var);
	setvar("dlcount","0",pool.var);
	setvar("name" ,pool.name,pool.var);
	setvar("rname",pool.rname,pool.var);

	art.file=++pool.max;

	op_files();
	sion_sr("file",lpu(art.file,0));
	sion_write(NULL,pool.var);
	sion_close();
}

void	pool_pageout()
{
	lo_line2();
	prt("  filename:『",pool.name,"』\n",
		"  filesize:",lpu(pool.size,0),"byte ("
					,lpu(pool.size/1000 +1,0),"Kbyte)\n",
		"  down:",lpu(pool.count,0),"回\n",NULL);
}


void	pool_getname(char *s,size_t l)
{
	msgout("ファイルネームを入力してください。");
	getstri(s,l);
}

void	pool_up()
{
	if (pool.fp==NULL)
		{
		 pool_rname(pool.rname,pool.name);
		 pool.fp=op_pool(pool.rname);
		 if (pool.fp==NULL)
			 error(""); /* !! */
		}
	pool.fpt=op_temp("w+"); /* !! アップロード用のテンポラリをopenする。*/
}

void	name_form(char *s,const char *t,size_t n)	/* 名前の正規化 */
{
	for (;n>0 ||*t=='\0' ||*t=='.'  ;--n,++t,++s)
		{
		 if (strchr("\"*+,/:;<=>?[\\]|\xff", *t) ||*t <= ' ')
		 	*s = '_'; else
		 	*s = *t;
		}
	*s = '\0';
}

void	pool_rname(char *s,const char *t)
{
			char	dir[LN_filename+1],name[8+1],ext[3+1];
	const	char	*p,*q;
/*			char	buf[LN_buf+1]; */
/* 必ずしも正しい、ファイルネームの正規化とは限らない....。*/

	p=strrchr(t,'\\');							/* !!漢字コード問題 */
	if (p!=NULL)
		t=p+1;									/* ディレクトリパスを消す */
	q=strrchr(t,'/');
	if (q!=NULL&&q>t)
		t=q+1;									/* UNiXパスを消す */

	p=strrchr(t,'.');							/* 最後の拡張子を抜き出す。*/
	name_form(ext,p==NULL?"":p+1,3);			/* 拡張子の正規化。*/
	name_form(name,t,p==NULL ||p-t>8?8:p-t);	/* ファイル名の正規化。 */

prt("name_form,",t,",",name,",",ext,"\n",NULL);

	name_pooldir(dir);
	sprt(s,name,*ext!='\0'?".":"",ext,NULL);	/* dirは削除 */
/* prt("name_ext(",s,",",dir,",",name,",",ext,");\n"); */
	name_ext(s,dir,name,ext);
}

uint	name_drive(char *s)
{
	char	*p;

	p=strchr(s,':');
	return p==s ||p==NULL?0:*(p-1)-'A'+1;
}

ulong	diskfree(uint n)
{
	struct	diskfree_t	d;

	_dos_getdiskfree(n,&d);
	return d.avail_clusters* d.sectors_per_cluster* d.bytes_per_sector;
}

bool	pool_upload()	/* 他からのexport */
{
	if (!file_upload(NULL))
		return FALSE;

	fclose(pool.fp);
	pool_write();	/* ここで初めて、art.fileが書き出される。 */
	ulist_incul(user.number);
	return TRUE;
}

bool	file_upload(FILE *fp)
{
	char	c;
	char	buf[32+1];

	pool.fp=fp;
	for(;;)
		{
		 wait_send();
		 killbuf();
		 prt("[Up] ([L]ocal,[Q]uit)",cps(isonekey()?'%':'>'),NULL);
		 if (!isonekey())
			c=getstr(buf); else
		 	c=getocom(buf,sizeof(buf)-1);
		 if (c!='\r'&&iscntrl(c))
		 	 sprt(buf,"^",cps(c+'@'),NULL);
		 switch(toupper(*buf))
		 	{
		  case 'L':
		  	 if (iscall())
		  	 	{
		  	 	 msgout("コンソール機能は利用できません。");
		  	 	 return FALSE;
		  	 	}
		  	 return pool_ucopy();
		  default:
		  	 msgout("転送をしませんでした。");
		  	 return FALSE;
		  	}
		}
}

void	pool_dnload()
{
	if (!file_dnload())
		return;

	setvar("dlcount",lpu(++pool.count,0),pool.var);

	op_files();
	sion_sr("file",lpu(pool.num,0));
	sion_write(NULL,pool.var);
	sion_close();
	ulist_incdl(user.number);
}

SHELL	bool	file_dnload()
{
	char	c;
	char	buf[32+1];

	if (art.file==ERROR)
		return FALSE;

	for(;;)
		{
		 wait_send();
		 killbuf();
		 prt("[Down] ([L]ocal,[Q]uit)",cps(isonekey()?'%':'>'),NULL);
		 if (!isonekey())
			c=getstr(buf); else
		 	c=getocom(buf,sizeof(buf)-1);
		 if (c!='\r'&&iscntrl(c))
		 	 sprt(buf,"^",cps(c+'@'),NULL);
		 switch(toupper(*buf))
		 	{
		  case 'L':
		  	 if (iscall())
		  	 	{
		  	 	 msgout("コンソール機能は利用できません。");
		  	 	 return FALSE;
		  	 	}
		  	 return pool_dcopy();
		  default:
		  	 msgout("転送をしませんでした。");
		  	 return FALSE;
		  	}
		}
}

  /* コンソール転送 */

bool	pool_ucopy()
{
	FILE		*fpr;
	struct stat	sbuf;
/*	time_t	times[2]; */

	pool_getname(pool.name,sizeof(pool.name));

	fpr=op_local(pool.name);		/* upload用のローカルディレクトリから */
	if (fpr==NULL ||fsize(fpr)==0)
		{
		 msgout("ファイルが存在していません。");
		 return FALSE;
		}
	fstat(fileno(fpr),&sbuf);

	pool_up();			/* uploadの汎用手続き */
	fclose(pool.fpt);	/* アップロード用テンポラリは使用しない。*/
/* _______________________________________________________________________ */
	fcopy(pool.fp,fpr);

	pool.size=ftell(fpr);
	fstat(fileno(fpr),&sbuf);
 	pool.ftime= sbuf.st_mtime;
/* prt("時間:",strhdatime(sbuf.st_mtime),"\n",NULL); */
	fclose(fpr);
/*!! poolされるファイルの日付を変更する。 */

	return TRUE;
/* dbprt("pool.fp=",lpu(ftell(pool.fp),0),"\n",NULL); */
}

bool	pool_dcopy()
{
	time_t	times[2];
	FILE	*fpw;
	char	buf[LN_buf+1];

	fpw=op_local(pool.rname);
	if (fpw==NULL)
		{
		 msgout(IC_err "ローカルファイルのopenに失敗しました。");
		 return FALSE;
		}
	pool.fp=op_pool(pool.rname);
	if (pool.fp==NULL)
		{
		 msgout(IC_err "poolのファイルのopenに失敗しました。");
		 fclose (fpw);
		 return FALSE;
		}

/* _______________________________________________________________________ */
	fcopy (fpw,pool.fp);
	fclose(fpw);
	fclose(pool.fp);

	times[1]=pool.ftime;
	name_localdir(buf);
	strcat(buf,pool.rname);
msgout(buf);
	utime(buf,times);
	return TRUE;
}

