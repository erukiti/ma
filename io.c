#include "ma.h"
#include "mio.h"

#pragma	optimize	time

extern	void	remote_init(FILE *fp);

#define MCD_IOCTL_GETID			9	/* ドライバーID取得 */
#define MCD_IOCTL_GETENT		20	/* ダイレクトエントリ */
#define MCD_IOCTL_GETCENT		21	/* Cエントリ */
#define MCD_IOCTL_GETPENT		22	/* パスカルエントリー */
#define MCD_IOCTL_MCDINFO		23	/* MCD情報構造体取得 */
#define MCD_IOCTL_GETTIMER		24	/* タイマー情報取得 */
#define MCD_IOCTL_SETTIMER		25	/* タイマー設定 */

#define	MCD_DCC_LINEPARA	1
#define	MCD_DCC_LINESTAT	2
#define	MCD_DCC_BUFSTAT		3
#define	MCD_DCC_BLOCKREAD	4
#define	MCD_DCC_BLOCKWRITE	5
#define	MCD_DCC_BREAKSIG	6
#define	MCD_DCC_BUFFER		7
#define	MCD_DCC_GETC		8
#define	MCD_DCC_PUTC		9
#define	MCD_DCC_SBUF_CHARS	10
#define	MCD_DCC_RBUF_CHARS	12
#define	MCD_DCC_NDREAD		14

#define SETENT(u,n)	MK_FP(u,packet.table[n])

	uint	(lerge *mcd_linestat)(int, ...);
	uint	(lerge *mcd_linepara)(int, ...);
	uint	(lerge *mcd_rbufchars)(void);
	uint	(lerge *mcd_sbufchars)(void);
	uint	(lerge *mcd_bufstat)(int, ...);
	sint	(lerge *mcd_ndread)(void);
	sint	(lerge *mcd_getc)(void);
	sint	(lerge *mcd_putc)(uint,...);
	sint	(lerge *mcd_breaksig)(int,...);
	uint	(lerge *mcd_blockwrite)(const void lerge *,size_t,...);
	uint	(lerge *mcd_blockread)(const void lerge *,size_t,...);
	uint	(lerge *mcd_buffer)(uint,...);

uint	ioctl_read(FILE *fp,void lerge *ptr,size_t len)
{
	union REGS		reg;
	struct SREGS	seg;

/*dbprintf("--MCD-IOCTL [%d]Packet size[%d]",*(uint lerge *)ptr,len);*/
	reg.x.ax = 0x4402;
	reg.x.bx = fileno(fp);
	reg.x.cx = len;
	reg.x.dx = FP_OFF(ptr);
	seg.ds   = FP_SEG(ptr);
	intdosx(&reg,&reg,&seg);

/*dbprintf("Cy:%04x AX:%04x ",reg.x.cflag,reg.x.ax);*/
	return reg.x.cflag==0;
}

bool	mcd_id(FILE *fp)
{
	mcd_getid_t packet;

	packet.func= MCD_IOCTL_GETID;
	packet.id=NULL;

	ioctl_read(fp,&packet,sizeof(packet));

	if (lstrcmp(packet.id,"MCD") !=SAME)
		{
		 msgout(IC_err "MCD のID取得に失敗しました。");
		 return FALSE;
		}
/*
	sprintf(s,"MCD Ver %d.%d",packet.maj,packet.min);
	dprintf("%sが使用可能です。",s);
*/
	prt(IC_ok "MCD Ver ",lpu(packet.maj,0),".",lpu(packet.min,0)
			,"が使用可能です。\n",NULL);

	return TRUE;
}

bool	mcd_entry(FILE *fp)
{
	mcd_c_entry_t packet;
	uint	u;

	packet.func= MCD_IOCTL_GETCENT;
	if (!ioctl_read(fp,&packet,sizeof(packet)))
		{
		 msgout("error :MCD API -DIRECT C CALL-が利用不可能です。");
		 return FALSE;
		}

	u=packet.cseg;
	mcd_linepara   = SETENT(u,MCD_DCC_LINEPARA);
	mcd_linestat   = SETENT(u,MCD_DCC_LINESTAT);
	mcd_getc       = SETENT(u,MCD_DCC_GETC);
	mcd_putc       = SETENT(u,MCD_DCC_PUTC);
	mcd_rbufchars  = SETENT(u,MCD_DCC_RBUF_CHARS);
	mcd_sbufchars  = SETENT(u,MCD_DCC_SBUF_CHARS);
	mcd_buffer     = SETENT(u,MCD_DCC_BUFFER);
	mcd_ndread     = SETENT(u,MCD_DCC_NDREAD);
	mcd_bufstat    = SETENT(u,MCD_DCC_BUFSTAT);
	mcd_breaksig   = SETENT(u,MCD_DCC_BREAKSIG);
	mcd_blockwrite = SETENT(u,MCD_DCC_BLOCKWRITE);
	mcd_blockread  = SETENT(u,MCD_DCC_BLOCKREAD);
	return TRUE;
}

bool	rs_init()
{
	mcd_lprm_t	param =
	{
		9600,	/* DTE    */
		8,		/* Octet 8bit */
		1,		/* Stop  1bit */
		0,		/* Parity */
		0x0002,	/* Flow (RTS/CTS)*/
		0		/* Limit */
	};


	ch.afp = fopen("AUX","r");
	if (ch.afp==NULL)
		{
		 msgout(IC_err "aux driverが搭載されていません。");
		 return FALSE;
		}
	cld();
	if (!mcd_id(ch.afp))
		{
		 fclose (ch.afp);
		 return FALSE;
		}
	cld();
	if (!mcd_entry(ch.afp))
		{
		 fclose (ch.afp);
		 return FALSE;
		}
	rssetbuf();

	remote_init(ch.afp);

	param.baud= atoi(getvar("baud",sys.var));
	rssetup(&param);

/*	rsbrk();*/

	modem_dtr(0);
	wait_minute(1);
	killbuf();
	modem_dtr(1);
	rsstat();


	return TRUE;
}

void	rs_fin()
{
	if (sys.rs)
		rsrstbuf();
	fclose(ch.afp);
}

void	hcputs(const char *s)
{
	while(*s!='\0')
		hcputc(*s++);
}

void	backup(char code)
{
	if (ungetc(code,stdin)!=code)
		error("");
}

void	rsputs(const char *s)
{
	rsblkwr(s,strlen(s));
}

void	rsblkwr(const void lerge *p,uint u)
{
	uint	c;

	for(;;)
		{
		 if (discchk())
		 	return;
		 push_d();
		 cld();
		 c=mcd_blockwrite(p,u);
		 pop_d();
		 if (u==c)
		 	return;

/*		 wait_send();	*/ /* !! これはいらないのではないだろうか？ */
		 if (c>u)
		 	error("");
		 p=(char lerge *)p+c;
		 u-=c;
		}
}

	static	mcd_buffer_t	mbuf;

void	rssetbuf()
{
	mbuf.tx_buflen=MAX_mcdbuf;
	mbuf.tx_buffer=lmalloc(MAX_mcdbuf);
	mbuf.rx_buflen=MAX_mcdbuf;
	mbuf.rx_buffer=lmalloc(MAX_mcdbuf);

	push_c();
	push_d();
	cld();
	mcd_buffer(MCD_DCSET,(mcd_buffer_t lerge *)&mbuf);
	pop_d();
	pop_c();

	if (mbuf.tx_buffer==NULL ||mbuf.rx_buffer==NULL)
		error("");
}

void	rsrstbuf()
{
	push_c();
	push_d();
	cld();
	mcd_buffer(MCD_BUFRST);
	pop_d();
	pop_c();

	lfree(mbuf.tx_buffer);
	lfree(mbuf.rx_buffer);
}

void	putz(const char *s)
{
	char	buf[LN_longbuf+1];

	escchk(buf,s,isesc());
	sendcharnum+=(ulong)strlen(buf);
	if (iscall())
		rsputs(buf);
	if (isdisp())
		hcputs(buf);
}

char	getn()
{
	uint	i;

	i=0;
	if (iscall())
		{
		 i=rsgetc();
		 if (i==ERROR)
		 	i=0;
		}
	if (i==0&&kbhit()!=0)
		i=getch();
	return (char)(i&0xff);
}

void	modem_chk(uint i)
{
	mcd_bufstat_t	bstat;

	if (i==ERROR)
		i=rsstat();
	rsbufg(&bstat);
	prt(ESC_clrline,"[R",lpu(rsloc(),4),":S",lpu(rssloc(),4),"]Status["
		,lpx(i,4),":"     ,i&MCD_DSR?"T":" ",i&MCD_DBK?"B":" "
		,i&MCD_DCD?"C":" ",i&MCD_RI ?"I":" ","][Flow:"
		,bstat.tx_xoff!=0?"S":" ",bstat.rx_xoff!=0?"R":" ","]",NULL);
}

SHELL	void	wait_send()
{
	if (iscall())
		{
		 if (discchk())
			return;
		 while (rssloc()>0 &&dsr())
			{
			 if (discchk())	 /* 送信バッファに残りがある限り待つ */
			 	return;
			}
		}
}

SHELL	void	brk_send()
{
/*	wait_send();*/
	rsbrk();
}

SHELL	void	modem_dtr(uint f)
{
	rsctrl(f!=0?MCD_DTR|MCD_RTS:0);
}

SHELL	void	msgout(const char *s)
{
	putz(s);
	putz("\n");
}

void	flow_on()
{
/*	if (iscall()) */
		rsbufs(REQ_TXSTART);
/*		rsbufs(FLWSIG_ON2);*/
}

void	flow_off()
{
/*	if (iscall()) */
		rsbufs(REQ_TXSTOP);
/*		rsbufs(FLWSIG_OFF);*/
}

bool	discchk()
{
	if (isdisc())
		{
		 if (isdis_cut())
		 	return TRUE;
		 autoescape(0);
		 longjmp(ch.env,1);
		}
	return FALSE;
}


extern	void	catch_int29h();
extern	void	release_int29h();
extern	void	far remote_int();

void	(far	*wcd_entry)();
mcd_timer_t	mcdtimer;
char volatile far *inmcd;
char	timer_stack[256];
uint	hsbseg;

void	remote_init(FILE *fp)
{
	mcd_mcdinfo_t	ipacket;
	union REGS		reg;

	ipacket.func= MCD_IOCTL_MCDINFO;

	ioctl_read(fp,&ipacket,sizeof(ipacket));
	inmcd=ipacket.inmcd_addr;

/*	prt("inmcd:",lpu(*inmcd,0),"\n",NULL);*/
/*	count_timer =ipacket.timer_addr; */
/*	int_timer   =ipacket.tint_times; */
/*	prt("remote_init :秒間",lpu(ipacket.tint_times/10,0)
		,"回の割り込みが使用できます。\n",NULL);*/

	fp=fopen("$$wcd","r");
	if (fp==NULL)
		error("ＷＣＤがopenできません。");
	if (!ioctl_read(fp,&wcd_entry,sizeof(wcd_entry)))
		error("");
	fclose(fp);

	reg.x.ax = 0x2b01;
	reg.x.cx = 0x8aa0;
	reg.x.dx = 0xc2e2;
	if (intdos(&reg,&reg)!=0x6f6b)
		hsbseg=0; else
		{
		 msgout(IC_ok "HSBが利用可能です。");
		 hsbseg=reg.x.dx;
		}
}

SHELL	void	remote_exec(const char *s)
{
	mcd_timer_t	tpacket;
	struct SREGS	seg;

	mcdtimer.func =MCD_IOCTL_GETTIMER;
	ioctl_read(ch.afp,&mcdtimer,sizeof(mcdtimer));

	tpacket.func  =MCD_IOCTL_SETTIMER;
	tpacket.ufunc =remote_int;
	tpacket.ch    =0;
	segread(&seg);
	tpacket.dseg  =seg.ds;
	tpacket.stack =(char far*)timer_stack
			+sizeof(timer_stack)/sizeof(*timer_stack);
	ioctl_read(ch.afp,&tpacket,sizeof(tpacket));/* catch Timer interrupt */

	catch_int29h();
	system(s);
	release_int29h();

	tpacket.func =MCD_IOCTL_GETTIMER;
	ioctl_read(ch.afp,&tpacket,sizeof(tpacket));
	if (tpacket.ufunc !=remote_int)
		error("MCD_timerが解除できません。");

	mcdtimer.func =MCD_IOCTL_SETTIMER;
	ioctl_read(ch.afp,&mcdtimer,sizeof(mcdtimer));/* Release Timer interrupt */
}

	static	char	cr_buf[LN_buf+1];
	static	uint	cr_size=0;
	static	uint	cr_ptr=0;

uint	rsgetc()
{

	if (cr_size==0 ||cr_ptr>=cr_size)
		{
		 if (iscall()&&discchk())
		 	error("はまり"); else
			cr_size=rsgets(cr_buf,LN_buf);
		 cr_ptr=0;
		}
	if (cr_size==0)
		return ERROR;

	return cr_buf[cr_ptr++];
}

void	killbuf()
{
	while(kbhit()!=0)
		getch();
	cr_size=0;
	if (iscall())
		{
		 rsbufclr();
		}
}

