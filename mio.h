/* Depend IO */

	/* portの信号status */
#define MCD_DTR		0x0002		/* DTR					(Get/Set) */
#define MCD_RTS		0x0020		/* RTS					(Get/Set) */
#define MCD_DBK		0x0040		/* Break Signal			(Get) */
#define MCD_DSR		0x0080		/* DSR					(Get) */
#define MCD_PERR	0x0100		/* パリティーエラー     (Get) */
#define MCD_FERR	0x0200		/* フレーミングエラー   (Get) */
#define MCD_OERR	0x0400		/* オーバーランエラー   (Get) */
#define MCD_RXFULL	0x0800		/* MCDバッファフル      (Get) */
#define MCD_DCD		0x2000		/* DCD					(Get) */
#define MCD_CTS		0x4000		/* CTS			 		(Get) */
#define MCD_RI		0x8000		/* Ring Indicator		(Get) */

	/* バッファ制御時の定数 */
#define CLR_TX_BUF	0x0001		/* 送信バッファクリア */
#define CLR_RX_BUF	0x0002		/* 受信バッファクリア */
#define REQ_TXSTOP	0x0100		/* 相手に送信停止をリクエストする */
#define REQ_TXSTART	0x0200		/* 相手に送信再開をリクエスとする */
#define R_XOFF		0x1000		/* XOFF文字を受信したことにする */
#define R_XON		0x2000		/* XON文字を受信したことにする */
#define FLWSIG_OFF	0x0010		/* ハードウェアフロー制御信号OFF */
#define FLWSIG_ON	0x0020		/* ハードウェアフロー制御信号ON */
#define FLWSIG_ON2	0x0040		/* ハードウェアフロー制御信号ON(可能なら) */

#define	MCD_DCGET	0
#define	MCD_DCSET	1
#define	MCD_BUFRST	2

typedef struct
{
	uint	func;
	char	min;			/* マイナーバージョン */
	char	maj;			/* メジャーバージョン */
	char	lerge *id;		/* ID文字列へのポインタ */
} mcd_getid_t;

typedef struct
{
	uint	func;			/* 2 */
	uint	lerge *table;	/* 4 各関数のnearポインタの配列の先頭アドレス */
	uint	cseg;			/* 2 関数のセグメント */
	uint	tblnum;			/* 2 配列の大きさ */
} mcd_c_entry_t;

typedef struct
{
	uint	func;					/* 2 */
	char volatile far *inmcd_addr;	/* 4 */
	uint	tint_times;				/* 2 */
	long volatile far *timer_addr;	/* 4 */
	long volatile far *iocnt_addr;	/* 4 */
	char	reserved[16];			/* 16 */
} mcd_mcdinfo_t;

typedef struct
{
	uint	func;					/* 2 */
	void	(far	*ufunc)();		/* 4 */
	uint	ch;						/* 2 */
	uint	dseg;					/* 2 */
	void far	*stack;				/* 4 割り込みで使用するスタックフレーム */
} mcd_timer_t;


typedef	struct
{
	size_t			tx_buflen;
	void	lerge *	tx_buffer;
	size_t			rx_buflen;
	void	lerge *	rx_buffer;
}	mcd_buffer_t;

typedef	struct
{
	uint	txnum;
	uint	rxnum;
	uint	tx_xoff;
	uint	rx_xoff;
}	mcd_bufstat_t;

typedef struct
{
	uint	baud;
	uint	length;
	uint	stop;
	uint	parity;
	uint	flow;
	uint	limit;
}	mcd_lprm_t;

extern	bool	mcd_id    (FILE *fp);
extern	bool	mcd_entry (FILE *fp);

#define	cts()		(rsstat()&MCD_CTS)
#define	dcd()		(rsstat()&MCD_DCD)
#define	dsr()		(rsstat()&MCD_DSR)
#define	rsbufclr()	rsbufs(CLR_TX_BUF|CLR_RX_BUF)
#define	isdisc()	(!dsr()||(ch.frug.cd&&!dcd()))


#if	defined(LSI_C)
	void	_asm_cc(char *,char);
	char	_asm_c(char *);
#	define	hcputc(c)	_asm_cc("\n\tint\t29h",c)
#	define	int28h()	_asm_c("\n\tint\t28h")
#	define	cld()		_asm_c("\n\tcld")
#	define	push_d()	_asm_c("\n\tPUSH\tDX")
#	define	pop_d()		_asm_c("\n\tPOP\tDX")
#	define	push_c()	_asm_c("\n\tPUSH\tCX")
#	define	pop_c()		_asm_c("\n\tPOP\tCX")
#else
#	define	hcputc(c)	putchar(c)
#	define	cld()
#	define	push_d()
#	define	pop_d()
#	define	push_c()
#	define	pop_c()
#endif



/*
#if defined(__TURBOC__)
#	define	rsgetc()	mcd_getc()
#	define	rsputc(u)	mcd_putc(u)
#	define	rsbufs(w)	mcd_bufstat(MCD_DCSET,w)
#	define	rsloc()		mcd_rbufchars()
#	define	rssloc()	mcd_sbufchars()
#	define	rsstat()	mcd_linestat(MCD_DCGET)
#	define	rsctrl(w)	mcd_linestat(MCD_DCSET,w)
#	define	rssetup(pa)	mcd_linepara(MCD_DCSET,pa)
#	define	rsbufg(pa)	mcd_bufstat(MCD_DCGET,pa)
#	define	rsblk()		mcd_breaksig(MCD_DCSET)
#else
*/
/*	extern	uint		rsgetc();*/
	extern	uint		rsputc(char u);
	extern	uint		rsbufs(uint w);
	extern	uint		rsloc();
	extern	uint		rssloc();
	extern	uint		rsstat();
	extern	uint		rsctrl(uint w);
	extern	void		rssetup(mcd_lprm_t lerge *param);
	extern	void		rsbufg(mcd_bufstat_t lerge *param);
	extern	void		rsbrk();
	extern	uint		rsgets(void *buf,size_t ln);

extern	void	hcputs(const char *s);
extern	void	backup(char code);

extern	void	rsputs(const char *s);
extern	void	rsblkwr(const void lerge *p,uint u);
extern	void	rssetbuf();
extern	void	rsrstbuf();
extern	bool	discchk();
extern	uint	rsgetc();
