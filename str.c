#include "ma.h"
#include "str.h"

char	__retonekey[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						"!#$%*+-?\r\x1b\x1e\x1f\x03\x12\x0a\x0b"};

char	getocom(char *s,size_t l)
{
	char	c;
	char	buf[LN_buf];

	*s='\0';
	c =_ma_getstr(s,l,F_normal+F_guide+F_esc,__retonekey);
	if (!iscntrl(c))
		{
		 sprt(buf,cps(c),NULL);
		 strcat(s,buf);
		 prt(buf,NULL);
		 c=_CR;
		}
	msgout("");
	return c;
}
/*
void	stack_chk()
{
	uint	i;

	msgout("- stack -");
	for(i=0;i<stack.num;++i)
		prt(lpu(i,2),":[",fps(stack.ptr[i],0),"]\n",NULL);
	msgout("---------");
}
*/

void	stack_push(const char *s)
{
	size_t	size;

	size=strlen(s)+1;
/* dbprintf("stack.push(%d.%d)",stack.num,size);*/
	if (stack.num>=MAX_stack)
		error("");
	stack.ptr[stack.num]=lmalloc(size);
	if (stack.ptr[stack.num]==NULL)
		error("");

	lstrcpy(stack.ptr[stack.num],s);
	++stack.num;
}

void	stack_pop(char *s)
{
/* dbprintf("stack.pop(%d)",stack.num);*/

	if (stack.num==0)
		error("");
	--stack.num;
	lstrcpy(s,stack.ptr[stack.num]);
	lfree(stack.ptr[stack.num]);
}

void	stack_init()
{
	stack.num=0;
}

void	stack_del()
{
	while(stack.num>0)
		lfree(stack.ptr[--stack.num]);
}

char	forth_buf[LN_buf+1];
char	*forth_ptr;
uint	forth_line;

char	*skipspc(char *s)
{
	while(isspace(*s))
		++s;
	return s;
}

void	forth_error(char *s)
{
	char	buf[LN_buf+1];

/*	sprt(buf,"4th: ",s,":",lpu(forth_line,0),t,NULL);*/
	sprt(buf,"4th: ",lpu(forth_line,0),":",s,NULL);
	error(buf);
}

char	forth_gets(char *s,char *buf)
{
	char	*p;
	char	c;
/*	char	temp[LN_buf+1];*/

	for(;;)
		{
		 c= *forth_ptr;
/* dbprt("__[",forth_ptr,"]\n",NULL);*/
/* monitor(); */
		 switch(c)
		 	{
		  case '\r':
		  case '\n':
		  case '\0':
		  case '#':
			 forth_ptr=forth_buf;
			 if (!sion_gets(forth_ptr))
			 	return '\0';
/* dbprt("read .[",forth_ptr,"]",NULL);*/

			 ++forth_line;
		 	 forth_ptr=skipspc(forth_ptr);
/* dbprt(" -> [",forth_ptr,"]\n",NULL);*/
		 	 continue;
		  case '"':
		  	 p=strchr(forth_ptr+1,'\"');
		  	 if (p==NULL)
		  	 	forth_error("\"が閉じていません。");
		  	 /* 面倒なのでクォーティング処理は対応しない。 */
		  	 *p='\0';
		  	 strcpy(buf,forth_ptr+1);
		  	 forth_ptr=skipspc(p+1);
		  	 return c;
		  case '[':
		  case '{':
		  case '@':
		  case '>':
		  case '!':
		  case ';':
		  case ':':
		  	 p=strpbrk(forth_ptr+1," \t\n\r");
		  	 if (p!=NULL)
		  	 	*p='\0'; else
		  	 	{
		  	 	 p=forth_buf-1;
		  	 	 *forth_buf='\0';
		  	 	}
		  	 strcpy(buf,forth_ptr+1);
		  	 forth_ptr=skipspc(p+1);

		  	 return c;
		  case '$':
		  	 p=strpbrk(forth_ptr+1," \t\n\r");
		  	 if (p!=NULL)
		  	 	*p='\0'; else
		  	 	{
		  	 	 p=forth_buf-1;
		  	 	 *forth_buf='\0';
		  	 	}
		  	 strcpy(buf,lpu(xtoi(forth_ptr+1),0));
		  	 forth_ptr=skipspc(p+1);

		  	 return c;
		  default:
		  	 p=strpbrk(forth_ptr," \t\r\n");
		  	 if (p!=NULL)
		  	 	*p='\0'; else
		  	 	{
		  	 	 p=forth_buf-1;
		  	 	 *forth_buf='\0';
		  	 	}
		  	 strcpy(buf,forth_ptr);
		  	 forth_ptr=skipspc(p+1);

		  	 if (isalpha(c))
		  	 	return '%';
		  	 if (isdigit(c))
		  	 	return '$';
		  	 forth_error("謎のコードが入っています。");
			}
		}
}


uint	forth_stack[MAX_fstack];
uint	forth_psp=0;

SHELL	void	forth_chkpsp()
{
	if (forth_psp>0)
		{
		 msgout(lpu(forth_psp,0));
		 error("4th_pspが0より上です。");
		}
}

void	forth_push(uint a)
{
/*
dbprt("4th[",lpu(forth_psp,0),"\n",NULL);*/
	if (forth_psp>MAX_fstack)
		forth_error("parameter stackがあふれました。");
	forth_stack[forth_psp++]=a;
}

uint	forth_pop()
{
/*
dbprt("4th]",lpu(forth_psp,0),"\n",NULL);*/
	if (forth_psp==0)
		forth_error("これ以上parameter stackは存在していません。");
	return forth_stack[--forth_psp];
}

recursive	void	where_exec(char c)
{
	char	cpline[LN_buf+1];
	char	*cp,*sp;
	char	a;
	char	buf[LN_buf+1];
	char	strbuf[LN_buf+1];
	uarg_t	uarg,uarg2;


	if (c==0xff)
		{
		 putz("\a");
		 monitor("そのようなコマンドは存在しません。");
		 return ;
		}
	strcpy(cpline,where.list[c]);
/*dbprt("..Where.exec ",where.name[c]," [",cpline,"]\n",NULL);*/

	cp=cpline;
	sp=strbuf;
	*strbuf='\0';

	while(*cp!='\0')
		{
		 cp = pull(buf,cp);
/*		 if (!isxdigit(buf[0])||!isxdigit(buf[1]))
		 	error("");*/

		 switch(*buf)
		 	{
		  case '$':
/*
dbprt(". push [",buf+1,"]\n",NULL);*/
		  	 uarg.num=atoi(buf+1);
		  	 forth_push(uarg.num);
		  	 continue;
		  case '"':
/*
dbprt(". push [",buf+1,"]\n",NULL);*/
			 strcpy(sp,buf+1);
			 uarg.ptr=sp;
			 sp+=strlen(sp)+1;
		  	 forth_push(uarg.num);
		  	 continue;
		  case '%':
		  	 a=xtoi(buf+1);

dbprt(". exec[",flist.name[a],"(",lpu(flist.argc[a],0),"):",buf+1,"]\n",NULL);
			 uarg.num=0;
			 uarg2.num=0;
		  	 switch(flist.argc[a])
		  	 	{
		  	  case 2:
		  	  	 uarg2.num=forth_pop();
		  	  case 1:
		  	  	 uarg.num=forth_pop();
			  default:
			  	 flist.func[a](uarg,uarg2);
			  	}
			 continue;
		  default:
		  	 error("");
			}
		}
}

char	where_comnum(const char *s)
{
	char	i;
	char	*p,*q;

/* dbprt("where_comnum [",s,"]\n",NULL);*/

	for (i=0;i<MAX_op;++i)
		{
		 p=where.name[i];
		 if(*p=='\0')
		 	return 0xff; /* 番人に引っ掛かったのでもう無い */
		 if(isdigit(*s)&&stricmp("\\num",p)==SAME)
		 	return i;

		 q=strpbrk(s,"= 0123456789");/* =やspaceや数字が含まれる場合cutする */
		 if (q!=NULL &&!isdigit(*s)) /* 先頭が数字でなければ */
		 	*q='\0';

		 if(*s=='\0'   &&stricmp("\\ret",p)==SAME)
			return i;
		 if(stricmp(s,p)==SAME)
		 	return i;
		 if(             stricmp("\\all",p)==SAME)
		 	return i;
		}
	return 0xff;
}


recursive	void	where_shell(const char *t)
{
	char	u[LN_comline+1];
	char	buf[LN_comline+1];
	char	*p,*s;

	strcpy(u,t);
	s=u;
	for(;;)
		{
		 p=strchr(s,'|');
		 if (p!=NULL &&chk_kanji(s,p)%2==0)
		 	*p=':';
		 s=pull(buf,s);

		 if (*buf=='\\')
		 	*buf='\0';

		 p=strchr(buf,' ');
		 if (p==NULL)
		 	p=buf; else
		 	*p++='\0';
		 where.num=getunumber(p);

/* dbprintf("Command [%s]\n",p);*/
		 where_exec(where_comnum(p));
		 if (*s=='\0')
		 	break;
		 prompt();
		 msgout(s);
		}
}

SHELL	void	where_seta()
{
	where_set(where.is);
}

SHELL	void	where_setr()
{
	stack_pop(where.is);
	where_set(where.is);
}

SHELL	void	where_sett(const char *t)
{
	stack_push(where.is);
	where_set(t);
}

char	flist_search(const char *s)
{
	char	i;
	char	*p;

/* dbprt("flist_search [",s,"]\n",NULL);*/

	for (i=0;i<MAX_func;++i)
		{
		 p=flist.name[i];
		 if (*p=='\0')
		 	break;
/*dbprintf("[%s]",p);*/
		 if (stricmp(p,s)==SAME)
		 	return i;
		}
	return 0xff;
}


SHELL	void	where_set(const char *t)
{
	char	a,c;
	char	i;
	char	buf[LN_buf+1];

/* dbprt("..where_set(",t,")",NULL); */

	strcpy(where.is,t);

	op_style();
	if (sion_sr("command",t)<2)
		{
		 msgout(t);
		 error("command not found");
		}
	sion_getp();

	*forth_buf='\0';
	forth_ptr=forth_buf;
	forth_line=0;
	i=0;
	strcpy(where.name[i],"start");
	*where.list[i]='\0';
	for(;;)
		{
		 c=forth_gets(where.name[i],buf);
/*dbprt("forth_gets [",buf,"] ",NULL);*/
		 switch(c)
		 	{
		  case ';':
/*dbmsgout("defination.");*/
		  	 ++i;
		  	 strcpy(where.name[i],buf); /* ?? bufが正常かのチェックいるかな*/
			 *where.list[i]='\0';
		     continue;
		  case '$':
		  case '"':
/*dbmsgout("string/digit");*/
		  	 strcat(where.list[i],cps(c));
		  	 strcat(where.list[i],buf);
		  	 strcat(where.list[i],":");
		  	 continue;
		  case '%':
/* dbmsgout("command");*/
			 a = flist_search(buf);
			 if (a==0xff)
			 	{
			 	 prt("\n[",buf,"]",NULL);
				 error("存在しない命令です。");
				}
			 sprt(buf,"%",lpx(a,2),":",NULL);
			 strcat(where.list[i],buf);
			 continue;
		  case '\0':
		  	 break;
		  default:
			 prt("\n[",buf,"]",NULL);
		  	 error("現在、その命令には対応していません。");
			}
		 break;
		}
	*where.name[++i]='\0'; /* whereのリストにターミネータを仕掛ける。*/

	sion_close();
	start();
}


void	_ma_prompt(const char *s)
{
	char	buf[LN_buf+1];

	sprt(buf,s,cps(isonekey()?'%':'>'),NULL);
	guide_up(buf);
	putz(buf);
}

SHELL	void	pr_norm(const char *s)
{
	_ma_prompt(s);
}

SHELL	void	cs_norm(uint i)
{
	char	c;
	char	buf[LN_comline+1];

	if (i==0)
		i=8;

	wait_send();
	killbuf();
	prompt();
	if (!isonekey())
		c=getstri(buf,i); else
		c=getocom(buf,i);
	if (c!='\r'&&iscntrl(c))
		{
/* dbmsgout("! コントロールコードですぜ"); */
		 sprt(buf,"^",cps(c+'@'),NULL);
 		}
	where_shell(buf);
}

void	where_init()
{
	where.max_escape=0;
}

uint	getrnumber(uint u)
{
	if (where.num ==ERROR)
		return u;
	return where.num;
}

void	atescape(void (*func)())
{
/* dbmsgout("  atescape"); */
dbprt("! atescape:",lpu(where.max_escape,0),"\n",NULL);
	if (where.max_escape >=MAX_cont)
		error("これ以上escapeを登録できません。");
	where.escape[where.max_escape++]=func;
}

SHELL	void	escape()
{
/*dbmsgout("  escape");*/
/*
dbprt("! Escape:",lpu(where.max_escape,0),"\n",NULL);*/
	if (where.max_escape==0)
		error("逃げちゃだめだ！逃げちゃ駄目だ！");
	--where.max_escape;

	if (where.escape[where.max_escape]==NULL)
		return;

	where.result=where.esc_result[where.max_escape];
	where.escape[where.max_escape]();
}

SHELL	void	autoescape(uint i)
{
	while(where.max_escape>i)
		where.escape[--where.max_escape]();
}
