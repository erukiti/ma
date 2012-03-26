#include "ma.h"
#include "str.h"
#include "board.h"
#include "mio.h"
#include "pool.h"

char	__retedcode[]={"\r"};
typedef	struct
{
	FILE	*fpe;		/* �G�f�B�^�ŕҏW���̃t�@�C�� */
	char	c;			/* �G�f�B�^�Ŏ��̍s�Ɏ����z������ */
}	ed_t;
ed_t	ed;

#define	getestr(s)		_getestr(s,sizeof(s)-1)
char	_getestr(char *s,size_t l)
{
	char	i;

	s[0]=ed.c;
	s[1]='\0';

	putz(s); /* �K�C�h���C�����Ȃ�����A�����ŕ\������K�v������B */
	i=_ma_getstr(s,l-1,F_normal+F_seqedit+F_esc,__retedcode);
	msgout("");
/*prt("getestr[",s,"]:",lpu(strlen(s),0),"\n",NULL);*/



	return i;
}

void	ed_init()
{
	ed.fpe=NULL;
}

void	ed_fin()
{
}

void	ed_write()
{
/*dbprintf("ED_wrt:[%s]\n",where.param);*/

	strcat(where.param,"\n");
	fwrite(where.param,strlen(where.param),1,ed.fpe);
}

SHELL	uint	editor()
{
	char	buf[80+1];
	uint	retcode;

/* dbmsgout("ed_start"); */
	ed.c='\0';
	ed.fpe=op_temp("w+");
	if (ed.fpe ==NULL)
		error("");
	disp_text("editline");
	retcode=TRUE;

	if (iscall())
		{
		 ch.frug.monitor=0;
		 hcputs("--- �B�� �������ݒ� ---\r\n");
		}

	for(;;)
		{
/* dbprt("ed_main:",lpu(ftell(ed.fpe),0),"\n",NULL); */
/*dbmsgout("ed_main");*/
		 ed.c=getestr(buf);
/* dbprintf("Editor:[%s]\n",buf);*/
		 if (iscntrl(ed.c))
		 	ed.c='\0';
		 if (strchr(buf,'<')!=NULL)
		 	{
		 	 sprt(where.param,"<verb>",buf,NULL);
		 	 ed_write();
		 	 continue;
		 	}
		 switch(*buf)
		 	{
		  case '.':
		  	 if (buf[1]=='\0' ||buf[1]=='.')
		  	 	{
		  	 	 fclose(ed.fpe);
		  	 	 goto ret;
		  	 	}
		  	 continue;
		  case '/':
			 if (strlen(buf)<4)
			 	{
			  	 switch(buf[1])
			  	 	{
				  case 'b':
/* �A�[�e�B�N���Ƀo�C�i����t������B*/
					 msgout(art.file!=ERROR?IC_err 
					 	"��̃A�[�e�B�N���ɕ����̃t�@�C����t���ł��܂���B":
						pool_upload()?
			 				IC_ok  "�o�C�i���t�@�C�����A�b�v���[�h���܂����B" :
			 				IC_stop"�o�C�i���̓Y�t�𒆎~���܂����B");
				 	 continue;
			  	  case 'u':
				 	 if (file_upload(ed.fpe))
				 	 	msgout(IC_ok "�e�L�X�g�A�b�v���[�h���܂����B");
					 fflush(ed.fpe);
			  	  	 continue;
			  	  case '/':
			  	 	 fclose(ed.fpe);
			  	 	 goto ret;
			  	  case 'a':
			  	  case 'q':
/*dbmsgout("ed_abort");*/
					 fclose(ed.fpe);
					 retcode=FALSE;
			  	  	 goto ret;
			  	  case 't':
					 msgout("�ŏ�����ǂ݂����܂��B");

					 rewind(ed.fpe);
					 while(fgets(buf,sizeof(buf),ed.fpe))
					 	putz(buf);
					 clearerr(ed.fpe);
			  	  	 continue;
			  	  case '?':
			  	  	 disp_text("ed.hlp");
			  	  	 continue;
			  	  	}
				}
		  case '\\':
		  case '^':
		  case '*':
		 	 sprt(where.param,"<verb>",buf,NULL);
		 	 ed_write();
		 	 continue;
		  	}
		 strcpy(where.param,buf);
		 ed_write();
		}
ret:
	if (iscall())
		ch.frug.monitor=1;
	return retcode;
}

uint	line_mptr(char *s)
{
	char	buf[LN_buf+1];
	char	*p;

	p=getvar(s,user.mptr);
	if (p==NULL)
		return 0;
	pull(buf,p);
	return atoi(buf);
}

SHELL	void	line_list()
{
	uint	i;

	lo_line("���X�g");
	for (i=0;i<varsize(stages->kind);++i)
		{
		 prt(nps(mkkey(i,stages->kind),8),":",nps(getnvar(i,stages->kind),32)
		 	," [",line_mptr(mkkey(i,stages->kind))>0?"��":"  ","]\n",NULL);
		}
	lo_line1();
}

char	line_handle[LN_handle+1];

SHELL	void	line_fin()
{
	fclose(ed.fpe);
	where_setr();
}

SHELL	void	line_start(char *s)
{
	ed.fpe =op_text(s);
	ed.c='\0';
	strcpy(line_handle,user.handle);
	where_sett("lined");
}

SHELL	void	line_in()
{
	char	buf[LN_buf+1];

	sprt(buf,stages->code,lpu(where.num,3),NULL);
	line_start(buf);
}

SHELL	void	line_main()
{
	char	buf[78-LN_handle+1];

	prt(nps(line_handle,16)," :",NULL);

	ed.c=getestr(buf);
/*dbprt("--Line:[",buf,"]\n",NULL);*/
	if (iscntrl(ed.c))
		ed.c='\0';
	if (*buf=='\0')
		*where.param=*buf; else
		sprt(where.param,nps(line_handle,16)," :",buf,NULL);
	where_exec(where_comnum(buf));
}

SHELL	void	line_name()
{
	char	buf[LN_handle+1];

	prt("���̖��O��",line_handle,"�ł��B���O��ύX���܂��B\n",NULL);
	getstr(buf);
	if (*buf!='\0')
		{
		 msgout("���O��ύX���܂����B");
		 strcpy(line_handle,buf);
		}
}

SHELL	void	line_last(uint i)
{
	if (i==0)
		{
		 msgout("���b�Z�[�W��ǂݒ����܂��B�����C���O����ǂݒ����܂����H");
		 i=getnumber();
		}
	msgout("  ���`����");
	disp_last(ed.fpe,i);
	clearerr(ed.fpe);
}

SHELL	void	BBS_add()
{
	char	buf[LN_buf+1];
	char	b_tel[12+1],b_time[11+1],b_host[6+1],b_name[46+1];

	msgout("�d�b�ԍ�����͂��Ă��������B");
	getstr(b_tel);

	if (*b_tel=='\0')
		{
		 msgout("�o�^���~���܂��B");
		 return;
		}

	msgout("�^�c���Ԃ́H(���݂݂̂���24hr�^�c)");
	getstr(b_time);

	msgout("�z�X�g�v���O�����̎�ނ́H(ex. RT-BBS,WWIV,RGWWIV��)");
	getstr(b_host);

	msgout("���̃l�b�g�̖��O����͂��Ă��������B");
	getstr(b_name);

	sprt(buf,nps(b_tel,12)," ",nps((*b_time=='\0'?"24hr":b_time),11)
		," ",nps(b_host,6)," ",nps(b_name,46),NULL);
	add_text("bbslist",buf);
}

SHELL	void	line_write()
{
/*dbprintf("ED_wrt:[%s]\n",where.param);*/

	strcat(where.param,"\n");
	fseek(ed.fpe,0,SEEK_END);
	fputs(where.param,ed.fpe);
	fflush(ed.fpe);

	*line_handle='\0';	/* �]���ȃn���h����\�����Ȃ��B */
}



/* Chat */

char	*chat_chid(char *s)
{
		return strcmp(s,"SYSOP")==SAME?user_id():"SYSOP";
}

SHELL	void	chat()
{
	char	buf[LN_buf+1],temp[LN_buf+1];
	char	chat_id[LN_id+1];
	time_t	t;

/*
	if (!iscall())
		{
		 msgout("�����ƃ`���b�g�͏o���܂���B");
		 return;
		}*/

	msgout("�`���b�g�����܂��B�v���͉��ł��傤���H");
	getstr(buf);
	if (*buf=='\0')
		{
		 msgout("���~���܂����B");
		 return;
		}

	sprt(temp,"| chat call (",buf,")",NULL);
	sys_log(temp);
	msgout("�ł̓V�X�I�y���Ăяo���܂��B");

	timer_res(t);
/*	rsbufs(FLWSIG_ON2);*/
	for(;;)
		{
		 hcputc(_BEEP);

		 if (kbhit())
		 	{
		 	 getch();
		 	 break;
		 	}
		 if (rsgetc()!=ERROR)
		 	{
		 	 msgout("��߂܂����B");
/*	rsbufs(FLWSIG_OFF);*/
		 	 return;
		 	}

		 if (timer_pas(t)>30)
		 	{
		 	 msgout("���Ȃ��݂����ł��˥����B");
		 	 return;
		 	}
		}

/* dbmsgout("chat_start"); */
	strcpy(chat_id,"SYSOP");
	ed.c='\0';
	disp_text("chat");

	srand(getdatime()&0xffff);
	for(;;)
		{
/*dbmsgout("chat_main");*/
		 prt("[",nps(chat_id,8),"]",NULL);
		 ed.c=getestr(buf);
		 if (iscntrl(ed.c))
		 	ed.c='\0';
		 switch(*buf)
		 	{
		  case '\0':
		  	 strcpy(chat_id,chat_chid(chat_id));
			 continue;
		  case '.':
		  	 if (buf[1]=='\0' ||buf[1]=='.')
		  	 	return ;
		  	 continue;
		  case '!':	/* �o�g���`���b�g�V�X�e��(��)�B */
		  	 prt("�������I�I ",chat_chid(chat_id),"��",lpu(rand()%256+1,0)
		  	 	,"�̃_���[�W���󂯂�\n",NULL);
		  	 continue;
		  case '/':
		  	 switch(buf[1])
		  	 	{
		  	  case '/':
		  	  	 return ;
		  	  case '?':
		  	  	 disp_text("chat.hlp");
		  	  	 continue;
		  	  	}
		  default:
		  	 continue;
		  	}
		}
}

