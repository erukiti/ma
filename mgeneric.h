#if	defined(S_Data)
#	define	export	
/*# undef	S_Data*/
#else
#	define	export	extern
#endif

#if	!defined(LSI_C)
#	define	recursive	
#endif

#define	SHELL	extern

/* Generic Defination */

#define	iskanji(c)	('\x81'<=(c)&&(c)<='\x9f' || '\xe0'<=(c)&&(c)<='\xfc')
#define	iskanji2(c)	('\x40'<=(c)&&(c)<='\x7e' || '\x80'<=(c)&&(c)<='\xfc')

#define	FALSE	0
#define	TRUE	!FALSE
#define	ERROR	0xffff
#define	SAME	0

#define	_CAN	(char)0x18
#define	_ABORT	(char)0x03
#define	_PAUSE	(char)0x13
#define	_RESTT	(char)0x11
#define	_BEEP	(char)0x07
#define	_BS		(char)0x08
#define	_TAB	(char)0x09
#define	_CR		(char)0x0d
#define	_LF		(char)0x0a
#define	_CLS	(char)0x0c
#define	_HOME	(char)0x0b
#define	_ESC	(char)0x1b
#define	_RIGHT	(char)0x1c
#define	_LEFT	(char)0x1d
#define	_UP		(char)0x1e
#define	_DOWN	(char)0x1f
#define	_DEL	(char)0x7f

#define	ESC_cls		"\x1b[2J"
#define	ESC_clrline	"\x1b[2K"
#define	ESC_del		"\x1b[1M"
#define	ESC_nl		"\x1b[1L"
#define	ESC_up		"\x1b[1A"
#define	ESC_left	"\x1b[1D"
#define	ESC_right	"\x1b[1C"
/*#define	ESC_locate	"\x1b[%d;%dH"*/	/* pf (ESC.locate,y,x) */
#define	ESC_locate0	"\x1b[0;0H"
#define	ESC_save	"\x1b[s"
#define	ESC_unsave	"\x1b[u"
#define	ESC_csron	"\x1b[>5l"
#define	ESC_csroff	"\x1b[>5h"
#define	ESC_norm	"\x1b[m"

/* Standard Color defination */


/* •¶šF */
#define	SC_ctrl		"\x1b[36m"				/* [36mƒRƒ“ƒgƒ[ƒ‹ƒR[ƒh[m */
#define	SC_open		"\x1b[32m"				/* [32m‰^‰cŠÔ[m */
#define	SC_msg		"\x1b[32m"	/*basic*/	/* [32mƒVƒXƒeƒ€ƒƒbƒZ[ƒW[m */
#define	SC_msg1		"\x1b[32m"	/*basic*/	/* [32mƒVƒXƒeƒ€‘¤“®“I•¶š[m */
#define	SC_msg2		"\x1b[33m"				/* [32m’†ŠÔ[m */
#define	SC_msg3		"\x1b[36m"				/* [32mƒ†[ƒU[‘¤“®“I•¶š[m */

/* ‹L†F */
#define	SC_norm		SC_msg					/* ’Êí */
#define	SC_warn		"\x1b[33m"				/* [33mŒx/’ˆÓ[m */
#define	SC_stop		"\x1b[34m"				/* [34m’â~[m */
#define	SC_err		"\x1b[31m"				/* [31mƒGƒ‰[[m */
#define	SC_atn		"\x1b[36m"				/* [36m’–Ú[m */

/* ˜g“™ */
#define	SC_frame	"\x1b[32m"				/*[32m=-=[m   ŠO˜g  */
#define	SC_frame2	"\x1b[34m"				/*[34m---[m   ‹æØ‚è*/
#define	SC_frame_s	"\x1b[32m"				/*[32m¢£wx[m•¶š   */
#define	SC_frame_c	"\x1b[36m"				/*[36m[][m    ƒR[ƒh */
#define	SC_frame_n	"\x1b[33m"				/*[33m()[m    ”Ô†   */
#define	SC_frame_e	"\x1b[32m"				/*[32m<>st[m‚»‚Ì‘¼A¬‡ */
#define	SC_guide	"\x1b[32m"				/*[32m___[m   under score */
#define	SC_derim							/*[36m:/[m    ‹æØ‚è */

/* Frame Character */

#define	FC_str2_s	SC_frame_s "w"	ESC_norm/*•¶š*/
#define	FC_str2_e	SC_frame_s "x"	ESC_norm
#define	FC_code_s	SC_frame_c "["	ESC_norm/*ƒR[ƒh*/
#define	FC_code_e	SC_frame_c "]"	ESC_norm
#define	FC_num_s	SC_frame_n "("	ESC_norm/*”š*/
#define	FC_num_e	SC_frame_n ")"	ESC_norm
#define	FC_comp2_s	SC_frame_e "s"	ESC_norm/*¬‡*/
#define	FC_comp2_e	SC_frame_e "t"	ESC_norm
#define	FC_time_s	SC_frame_e "y"	ESC_norm/*‹ó*/
#define	FC_time_e	SC_frame_e "z"	ESC_norm

/* Derim Charcter */

#define	DC_norm		SC_derim ":"	/*ESC_norm*//* ’Êí‚Ì‹æØ‚è */
#define	DC_per		SC_derim "/"	/*ESC_norm*//* per */
#define	DC_list		SC_derim ")"	/*ESC_norm*//* list */
#define	DC_area		SC_derim "`"	/*ESC_norm*//* ”ÍˆÍ */

/* Image Charcter */

#define	IC_act		"  "SC_norm "œ"ESC_norm/*‹““®					*/
#define	IC_dact		"  "SC_warn "œ"ESC_norm/*ŠëŒ¯‹““®				*/
#define	IC_ok		"  "SC_norm "›"ESC_norm/*“®ì‚Ì¬Œ÷,’Êí•ñ	*/
#define	IC_dok		"  "SC_warn "›"ESC_norm/*ŠëŒ¯“®ì‚Ì¬Œ÷		*/
#define	IC_nok		"  "SC_norm "¢"ESC_norm/*”Û’è•ñ				*/
#define	IC_err		"  "SC_err  "~"ESC_norm/*“®ì‚Ì¸”s			*/
#define	IC_stop		"  "SC_stop ""ESC_norm/*’†~					*/
#define	IC_warn		"  "SC_warn "¦"ESC_norm/*Œx					*/
#define	IC_atn		"  "SC_atn  "™"ESC_norm/*’–Ú					*/
#define	IC_iatn		"  "SC_atn  ""ESC_norm/*’–Ú					*/

/* Generic Type Defination */

typedef	unsigned int	uint;
typedef	int				sint;
typedef	unsigned short	ushort;
typedef	unsigned long	ulong;
typedef	unsigned char	uchar;
typedef	unsigned int	bool;
typedef	unsigned int	date_t;


/* far */
#if	defined(LSI_C)
#	include <farstr.h>
#	define	lerge			far
#	define	lfree 			farfree
#	define	lmalloc 		farmalloc
#	define	lstrcpy(x,y)	far_strcpy(x,y)
#	define	lstrncpy(x,y,z)	far_strncpy(x,y,z)
#	define	lstrcat(x,y)	far_strcat(x,y)
#	define	lmemcpy(x,y,z)	far_memcpy(x,y,z)
#	define	lmemset(x,y,z)	far_memset(x,y,z)
#	define	lstrcmp 		far_strcmp
#	define	lstricmp		far_stricmp
#	define	lstrlen			far_strlen
#else
#	include <alloc.h>
#	define	lerge			far
#	define	lmalloc 		farmalloc
#	define	lfree 			farfree
/*
#	define	lmemcpy(x,y,z)	memcpy(x,y,z)
#	define	lmemset(x,y,z)	memset(x,y,z)
#	define	lstrcat(x,y)	strcat(x,y)
#	define	lstrcpy(x,y)	strcpy(x,y)
#	define	lstrncpy(x,y,z)	strncpy(x,y,z)
#	define	lstrcmp 		strcmp
#	define	lstricmp		stricmp
#	define	lstrlen			strlen
*/
extern	void lerge	*lmemcpy(void lerge *d, const void lerge *s, size_t n);
extern	void lerge	*lmemset(void lerge *s, char b, size_t n);
extern	char lerge	*lstrcat(char lerge *s, const char lerge *t);
extern	char lerge	*lstrcpy(char lerge *s, const char lerge *t);
extern	char lerge	*lstrncpy(char lerge *s, const char lerge *t, size_t n);
extern	int	 		lstrcmp(const char lerge *d, const char lerge *s);
extern	size_t		lstrlen(const char lerge *s);
extern	int	 		lstricmp(const char lerge *s, const char lerge *t);
#endif


/* generic.c */

#define	strfcmp(x,y)	stricmp(x,y)
extern	void	escchk (char *s,const char *t,bool f);

SHELL	void	msgout(const char *s);

extern	void	prt(const char *s,...);
extern	void	sprt(char *s,...);
extern	void	fprt(FILE *fp,...);
extern	char	*getring(size_t u);

extern	void	strjncat(char *s,const char *t,size_t l);
extern	void	strjncpy(char *s,const char *t,size_t l);
extern	void	lstrjncpy(char *s,const char lerge *t,size_t l);

extern	long	otol(char *s);
extern	long	xtoi(char *s);
extern	char	chk_kanji(const char *s,const char *t);
extern	char	*skip_kanji(char *s);
extern	void	strdel(char *s,char t);
extern	char	*delspc(char *s);
extern	void	addspc(char *s,uint ln);
extern	void	spc(char *s);

extern	char	*digprt(char *s,ulong d,char l,int u);
extern	char	*cps(char c);
extern	char	*nps(char *s,size_t l);
extern	char	*fps(char lerge *s,size_t l);
extern	char	*lpx(ulong d,int s);
extern	char	*lpu(ulong d,int s);
extern	char	*lpo(ulong d,int s);

extern	void	wait_minute(uint u);

extern	ulong	fsize(FILE *fp);
extern	FILE	*fopr(const char *s);
extern	void	lfwrite(void lerge *b,size_t u,FILE *fp);
extern	void	lfread(void lerge *b,size_t u,FILE *fp);

/* Debug.c */

#define	debug_start()
extern	void	dbprt(const char *s,...);
extern	void	dbmsgout(const char *s);
extern	void	dbputz(const char *s);
extern	void	monitor(const char *s);

extern	bool	disp_chk();
extern	void	add_text(char *name,char *s);
SHELL	void	disp_text(char *name);
SHELL	void	disp_four(char *name);
SHELL	void	disp_sixt(char *name);
extern	void	disp_last(FILE *fpr,uint off);


#define	error(s)	sys_error(__FILE__,__LINE__ ,s)

extern	void	sys_init();
SHELL	void	sys_fin();
extern	void	sys_error(char *s,uint l,char *t);

extern	void	lo_line(char *s);
extern	void	lo_line2();
extern	void	lo_line1();
SHELL	void	copyright();

SHELL	void	end();
SHELL	void	null_op();
extern	void	fcopy(FILE *fpw,FILE *fpr);
extern	void	name_ext(char *s,char *dir,char *name,char *ext);

/* Time */
/* UTC - JST = TZ(-9) */

#define	DT_daytime	(time_t)(60L*60L*24L)

#define	DT_4yearday	(366+365+365+365)
#define	DT_3yearday	(365+365+365)
#define	DT_2yearday	(365+365)
#define	DT_1yearday	(365)
#define	DT_baseyear	(1970)

#define	getdatime()	(time_t)(time(NULL)-timezone)
#define	getdate()	(date_t)(getdatime()/DT_daytime)
#define	gettime()	(time_t)(getdatime()%DT_daytime)
#define	gethour(t)	((t%DT_daytime)/(60*60))
#define	getmin(t)	(((t%DT_daytime)/60)%60)
#define	getsec(t)	((t%DT_daytime)%60)

#define	date_s(s)	((getdate()-s)*DT_daytime)

#define	timer_res(t)	t=getdatime()
#define	timer_pas(t)	(getdatime()-t)

#define	isopentime(t)	((t+sys.starttime)%DT_daytime <= sys.opentime)

extern	char	*strptime(time_t t);
extern	time_t	mkdatime(date_t d,time_t t);
extern	time_t	mktm(struct tm tm);
extern	date_t	mkdate(struct tm tm);
extern	void	setdatime(date_t d,struct tm *tm);
extern	char	*strtime(time_t t);
extern	char	*strdate(date_t d);
extern	char	*stredatime(time_t t);
extern	char	*strhdatime(time_t t);	/* HGt—pŠÔ•\¦ */
extern	time_t	dtos(char *s);
extern	void	dtotm(char *s,struct tm *tm);

extern	char	*dwstr[7];
extern	char	*dwjstr[7];
