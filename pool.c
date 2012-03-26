#include "ma.h"
#include "pool.h"
#include "str.h"
#include "board.h"
#include "mio.h"

	/* pool �̃R���X�g���N�^�ƃf�X�g���N�^ */

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
		error("");/* �t�@�C���̎��̂����݂��Ă��܂���B*/
	strcpy(pool.rname,p);
	p=getvar("name",pool.var);
	strcpy(pool.name,p==NULL?"noname":p);
	pool.size =atol(getvar("size",pool.var));
	pool.ftime=otol(getvar("ftime",pool.var));
	pool.count=atol(getvar("dlcount",pool.var));

/* pool.time(�_�E�����[�h�\�莞�Ԍv�Z) */

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
	prt("  filename:�w",pool.name,"�x\n",
		"  filesize:",lpu(pool.size,0),"byte ("
					,lpu(pool.size/1000 +1,0),"Kbyte)\n",
		"  down:",lpu(pool.count,0),"��\n",NULL);
}


void	pool_getname(char *s,size_t l)
{
	msgout("�t�@�C���l�[������͂��Ă��������B");
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
	pool.fpt=op_temp("w+"); /* !! �A�b�v���[�h�p�̃e���|������open����B*/
}

void	name_form(char *s,const char *t,size_t n)	/* ���O�̐��K�� */
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
/* �K�������������A�t�@�C���l�[���̐��K���Ƃ͌���Ȃ�....�B*/

	p=strrchr(t,'\\');							/* !!�����R�[�h��� */
	if (p!=NULL)
		t=p+1;									/* �f�B���N�g���p�X������ */
	q=strrchr(t,'/');
	if (q!=NULL&&q>t)
		t=q+1;									/* UNiX�p�X������ */

	p=strrchr(t,'.');							/* �Ō�̊g���q�𔲂��o���B*/
	name_form(ext,p==NULL?"":p+1,3);			/* �g���q�̐��K���B*/
	name_form(name,t,p==NULL ||p-t>8?8:p-t);	/* �t�@�C�����̐��K���B */

prt("name_form,",t,",",name,",",ext,"\n",NULL);

	name_pooldir(dir);
	sprt(s,name,*ext!='\0'?".":"",ext,NULL);	/* dir�͍폜 */
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

bool	pool_upload()	/* �������export */
{
	if (!file_upload(NULL))
		return FALSE;

	fclose(pool.fp);
	pool_write();	/* �����ŏ��߂āAart.file�������o�����B */
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
		  	 	 msgout("�R���\�[���@�\�͗��p�ł��܂���B");
		  	 	 return FALSE;
		  	 	}
		  	 return pool_ucopy();
		  default:
		  	 msgout("�]�������܂���ł����B");
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
		  	 	 msgout("�R���\�[���@�\�͗��p�ł��܂���B");
		  	 	 return FALSE;
		  	 	}
		  	 return pool_dcopy();
		  default:
		  	 msgout("�]�������܂���ł����B");
		  	 return FALSE;
		  	}
		}
}

  /* �R���\�[���]�� */

bool	pool_ucopy()
{
	FILE		*fpr;
	struct stat	sbuf;
/*	time_t	times[2]; */

	pool_getname(pool.name,sizeof(pool.name));

	fpr=op_local(pool.name);		/* upload�p�̃��[�J���f�B���N�g������ */
	if (fpr==NULL ||fsize(fpr)==0)
		{
		 msgout("�t�@�C�������݂��Ă��܂���B");
		 return FALSE;
		}
	fstat(fileno(fpr),&sbuf);

	pool_up();			/* upload�̔ėp�葱�� */
	fclose(pool.fpt);	/* �A�b�v���[�h�p�e���|�����͎g�p���Ȃ��B*/
/* _______________________________________________________________________ */
	fcopy(pool.fp,fpr);

	pool.size=ftell(fpr);
	fstat(fileno(fpr),&sbuf);
 	pool.ftime= sbuf.st_mtime;
/* prt("����:",strhdatime(sbuf.st_mtime),"\n",NULL); */
	fclose(fpr);
/*!! pool�����t�@�C���̓��t��ύX����B */

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
		 msgout(IC_err "���[�J���t�@�C����open�Ɏ��s���܂����B");
		 return FALSE;
		}
	pool.fp=op_pool(pool.rname);
	if (pool.fp==NULL)
		{
		 msgout(IC_err "pool�̃t�@�C����open�Ɏ��s���܂����B");
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

