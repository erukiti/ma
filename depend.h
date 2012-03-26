/* io.c */
extern	bool	rs_init();
extern	void	rs_fin();

extern	void	modem_chk(uint i);
SHELL	void	wait_send();
SHELL	void	modem_dtr(uint f);
SHELL	void	brk_send();
extern	void	flow_on();
extern	void	flow_off();

extern	void	putz(const char *s);
extern	char	get(char *s,char x,char m,const char *retcode);
extern	char	getn();
extern	void	killbuf();

export	ulong	sendcharnum;

/* str.c */
#define	F_normal	0
#define	F_guide		1
#define	F_renew		2
#define	F_mask		4
#define	F_esc		8
#define	F_seqedit	16

extern	char	getocom(char *s,size_t l);
extern	uint	getnumber();
extern	uint	getrnumber(uint u);
extern	uint	getunumber(const char *s);
extern	uint	getdnumber(const char *s,uint u);

extern	char	_getrstr(char *s,size_t l);
extern	char	_getstr(char *s,size_t l);

#define	getstr(s)		_getstr(s,sizeof(s)-1)
#define	getstri(s,l)	_getstr(s,l)
#define	getrstr(s)		_getrstr(s,sizeof(s)-1)
#define	getrstri(s,l)	_getrstr(s,l)
extern	char	_ma_getstr(char *s,size_t l,char frug,char *retcode);

extern	void	guide_clr();
extern	void	guide_up(char *s);
extern	void	guide_out(size_t l);

extern	bool	ynq();

/* Str.c */

extern	void	where_init();
SHELL	void	where_seta();
SHELL	void	where_setr();
SHELL	void	where_sett(const char *t);
SHELL	void	where_set(const char *t);

extern	recursive	void	where_exec(char c);
extern	char	where_comnum(const char *s);

#define	shell()		where_exec(where_comnum("\\shell"))
#define	prompt()	where_exec(where_comnum("\\prompt"))
#define	start()		where_exec(where_comnum("\\start"))
SHELL	void	cs_norm(uint i);
SHELL	void	pr_norm(const char *s);
void	_ma_prompt(const char *s);

extern	void	stack_init();
extern	void	stack_del();

SHELL	void	escape();
extern	void	atescape(void (*func)());
SHELL	void	autoescape(uint i);

SHELL	void	forth_chkpsp();

/* Binary Archive */

typedef	struct
{
	FILE	*fp;	/* そのバイナリアーカイブのファイルポインタ */
	uint	size;	/* アーカイブ1メンバーあたりのサイズ */
	uint	max;	/* メンバーの数 */
}	fileb;

#define	BA_ver	0	/* BinaryArchive verion */

extern	fileb	*fopb(char *s,uint n);
extern	bool	readb(void *b,uint n,fileb *fb);
extern	bool	lreadb(void lerge *b,uint n,fileb *fb);
extern	void	writeb(void *b,uint n,fileb *fb);
extern	void	lwriteb(void lerge *b,uint n,fileb *fb);
extern	void	truncb(uint n,fileb *fb);
extern	void	fcloseb(fileb *fb);


/* generic::sion */
typedef struct
{
	uint			num;		/* 項目の数 */
	uint			max;		/* 実際、メモリに確保している量。*/
	uint			vnum;		/* varの長さ */
	char			*key;		/* key */
	char	lerge	*member;	/* var */
}	var_t;

typedef	struct
{
	ulong	header;
	ulong	body;
	ulong	end;
	char	type[LN_key+1];
	char	name[LN_str+1];
	uint	frug;
	time_t	datime;

}	index_t;
export	index_t	index;

typedef	struct
{
	 uint	num;
	 uint	max;
	 fileb	*fb;
}	si_t;
export	si_t	si;

typedef	struct
{
	uint	num;				/* 何番めの項目か */
	FILE	*fp;				/* 現在オープンしているfp */
	char	fname[LN_fname+1];	/* 現在オープンしているファイル */
	ulong	header;				/* 現在srしているファイルへのfile ptr */
	ulong	body;				/* body から endまで読み込み表示する。 */
	ulong	end;
	char	type[LN_key+1];		/* 現在srしている名前 */
	char	name[LN_str+1];
	uint	frug;				/* srをしているかどうか。 */
}	sion_t;
export	sion_t	sion;

#define	varsize(v)		(v->num)

extern	void	 clrvar (var_t *v);
extern	uint	 newvar (var_t *v);
extern	char			*mkkey(uint n,var_t *v);
extern	char	lerge	*mkvar(uint n,var_t *v);

extern	void	 varfin (var_t *v);
extern	var_t	*varinit(uint n,uint m);
extern	void	 varview(var_t *v);
extern	uint	 varnum (const char *s,var_t *v);
extern	char	*getvar (const char *s,var_t *v);
extern	char	*getnvar(uint n       ,var_t *v);
extern	void	 setvar (const char *s,const char *t,var_t *v);
extern	void	 setnvar(uint n       ,const char *t,var_t *v);
extern	void	 defvar (const char *s,const char *t,var_t *v);

extern	bool	parse(const char *s,char *key,char *var);
extern	char	*pull(char *s,const char *p);
extern	FILE	*fop(const char *s);
extern	FILE	*sion_fop(const char *s);
extern	bool	fchk(const char *s,const char *mode);

extern	void	sion_write(FILE *fp,var_t *t);
extern	void	sion_pageout();
extern	void	sion_read(var_t *t);
extern	bool	sion_gets(char *s);
extern	ulong	sion_getp();
#define	sion_chk(lp)	(lp>sion.end)
extern	uint	sion_sr(const char *t,const char *n);
#define	sion_max()	(si.fb->max)
extern	uint	si_seek(uint l);
#define	sion_srn(n)	si_seek(n)
extern	uint	sion_first();
extern	uint	sion_next();
extern	void	sion_close();
extern	uint	sion_srnext(const char *k,ulong u);
extern	void	sion_init();
extern	void	sion_fin();

extern	void	dsd_select(char *s,var_t *v);
extern	char	dsd_mark(char *s,var_t *v,uint frug);
extern	void	dsd_cup(char *s,char *data[],char num);
extern	char	dsd_div(char *s,char data[LN_key+1][MAX_dsd]);
extern	bool	dsd_chk(char *s,char *t);

export	var_t	*nowvar;

SHELL	int		system(const char *);


