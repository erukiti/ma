#include "ma.h"
#include "mio.h"

void	dbprt(const char *s,...)
{
#if	defined(DEBUG)
	va_list	ap;
	char	*p;

	va_start(ap,s);
	dbputz(s);
	for(;;)
		{
		 p=va_arg(ap,char *);
		 if (p==NULL)
		 	return;
		 dbputz(p);
		}
#endif
}

void	dbputz(const char *s)
{
#if	defined(DEBUG)
	char	buf[LN_buf+1];

	escchk(buf,s,TRUE);
	hcputs(buf);
#endif
}

void	dbmsgout(const char *s)
{
#if	defined(DEBUG)
	dbputz(s);
	dbputz("\n");
#endif
}

void	monitor(const char *s)
{
#if	defined(DEBUG)
	char	c;
	char	buf[1];

	prt("Monitor:",s,"\n",NULL);
	killbuf();
	c=_ma_getstr(buf,0,F_normal+F_seqedit,"");
	if (toupper(c)=='Q')
		end();
#endif
}

