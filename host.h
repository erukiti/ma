/* #define	isdisk()	(ch.fp!=NULL) */
#define	iscall()	(ch.frug.called)
#define	isdisp()	(ch.frug.monitor)
#define	isesc()		(ch.frug.esc)
#define	isonekey()	(ch.frug.onekey)
#define	isdis_cut()	(ch.frug.discut)

typedef	struct
{
	unsigned	called:1;	/* 232c�A�N�Z�X���ǂ��� */
	unsigned	monitor:1;	/* �R���\�[�����j�^�t���O */
	unsigned	esc:1;		/* �G�X�P�[�v�V�[�P���X�J�b�g�t���O */
	unsigned	onekey:1;	/* Onekey mode �t���O */
	unsigned	cd:1;		/* CD�M�����o�t���O */
	unsigned	discut:1;	/* ����ؒf�֎~�t���O */
}	chfrug_t;

typedef	struct
{
	chfrug_t	frug;
	char		result[LN_result+1];
	char		dengon[LN_dengon+1];
	time_t		login;
	time_t		pass;
	time_t		limit;
	FILE		*fp;
	FILE		*afp;	/* AUX port�p FP */

	jmp_buf		env;
}	ch_t;

export	ch_t	ch;

typedef	struct
{
	uint	execute;				/* �N���� */
	uint	logon;					/* ���O�I���� */
	uint	post;					/* �|�X�g�� */

	char	netname[LN_title+1];	/* �z�X�g�̖��O */
	char	netid[LN_netid+1];		/* �z�X�g��ID */

	char	home[LN_dirs+1];		/* ��f�B���N�g�� */
	char	temp[LN_dirs+1];		/* �e���|���� */

	char	modem[LN_str+1];		/* ���f���������R�}���h */
	time_t	opentime;				/* �^�c���Ԕ͈� */
	time_t	starttime;				/* �^�c�J�n���� */
	uint	erfrug;					
	uint	rs;						/* MCD���g�p�\���ǂ����̃t���O */

	uint	t_logon;				/* �����̃��O�C���� */
	uint	t_post;					/* �����̃|�X�g�� */

	var_t	*var;
	var_t	*result;
}	sys_t;

export	sys_t	sys;


#define	sys_getnid()	(sys.netid)

SHELL	void	sys_log(char *s);
SHELL	void	infomation();

SHELL	void	esc_chg();
SHELL	void	one_chg();

extern	void	op_news();
/*extern	void	op_index();*/
extern	void	op_system();
extern	void	op_stage();
extern	fileb	*op_note();
extern	fileb	*op_art();
extern	FILE	*op_datime();
extern	FILE	*op_map();
extern	void	op_style();
extern	void	op_files();
extern	void	op_user();
extern	FILE	*op_text(char *name);
extern	FILE	*op_rtext(char *name);
extern	FILE	*op_temp(char *mode);
extern	void	name_pooldir(char *dir);
extern	FILE	*op_pool(char *name);
extern	void	name_localdir(char *dir);
extern	FILE	*op_local(char *name);

SHELL	void	timepas();


/* WFC */
SHELL	void	wfc_connect(char *s);
extern	void	wfc_set();
SHELL	void	qlogout();
SHELL	void	logout();
SHELL	void	modem_send(const char *s);
SHELL	void	modem_tsend(const char *s);
SHELL	void	cron();
SHELL	void	cron_day();
SHELL	void	cron_month();
SHELL	void	cron_year();
SHELL	void	called_chg(uint u);
SHELL	void	modem_init();
SHELL	void	wfc_modem();
SHELL	void	wfc_status();
SHELL	void	cs_wfc();
SHELL	void	wfc_dengon(char *s);

/* line */
extern	void	ed_init();
extern	void	ed_fin();

SHELL	uint	editor();

SHELL	void	line_write();
SHELL	void	line_list();
SHELL	void	line_in();
SHELL	void	line_start(char *s);
SHELL	void	line_fin();
SHELL	void	line_main();
SHELL	void	line_name();
SHELL	void	line_last(uint i);

SHELL	void	BBS_add();

SHELL	void	chat();

/* board */
typedef	unsigned long	num_s;		/* �V���A���i���o�[ (0:�G���[)*/
typedef	unsigned int	num_m;		/* �}�b�v�ԍ� */
typedef	unsigned int	num_n;		/* �m�[�g���ԍ� */
typedef	unsigned int	num_d;		/* �\���p�ԍ� */

export	var_t	*mptr;
extern	void	art_init();
extern	void	art_fin();

SHELL	void	enter();
SHELL	void	readquit();
SHELL	void	vscan();
SHELL	void	ascan();
SHELL	void	tescan();
SHELL	void	nscan();
SHELL	void	mptr_day(const char *s);
SHELL	void	mptr_chk();
SHELL	void	mptr_write();
SHELL	void	mptr_last();
SHELL	void	mptr_clr();
SHELL	void	mail_rep();
SHELL	void	mail_ent(uint u);
SHELL	void	res();
SHELL	void	post();
SHELL	void	post_list();
SHELL	void	art_del(uint f);
SHELL	void	art_read(num_m m);
SHELL	void	number_plus();
SHELL	void	number_minus();
SHELL	void	number_number();
SHELL	void	rf_chg(uint i);
SHELL	void	number_kill();
SHELL	void	note_mk(char *s);
SHELL	void	note_info();
SHELL	void	note_list();
SHELL	void	note_list2();
SHELL	void	board_first();
SHELL	void	board_number();
SHELL	bool	board_plus();
SHELL	bool	board_minus();
SHELL	uint	mail_chk(uint i);
SHELL	void	board_in(char *s);
SHELL	void	news_prompt();
SHELL	void	stage_in(const char *s);/* stage�J�n */
/* SHELL	void	hgscan();*/
SHELL	void	art_prof();
SHELL	void	mask_set();
SHELL	void	mask_chk();

/*  user.c */
typedef	struct
{
	char	id[LN_id+1];
	char	pass[LN_pass+1];
	char	handle[LN_handle+1];
	char	memo[LN_memo+1];
	char	hoby[LN_memo+1];

	uint	mpost;
	uint	post;
	uint	mlogin;
	uint	login;

	uint	dl;
	uint	ul;

	uint	birthm;
	uint	birthd;

	uint	wp;		/* �p�X���[�h���̃J�E���g */
}	ulist_t;

ulist_t	lerge *_ult(uint n);
#define	ult(n)	_ult(n)

#define	ulist_getmemo(n)	fps(ult(n)->memo,0)
#define	ulist_gethoby(n)	fps(ult(n)->hoby,0)
#define	ulist_gethandle(n)	fps(ult(n)->handle,0)
#define	ulist_getid(n)		fps(ult(n)->id,0)
#define	ulist_getwp(n)		(ult(n)->wp)		/* �p�X���[�h��� */
#define	ulist_setwp(n,w)	(ult(n)->wp=w)
#define	ulist_incwp(n)		(++ult(n)->wp)
#define	ulist_clrwp(n)		(ult(n)->wp=0)
#define	ulist_chkpass(s,n)	(lstrcmp(s,ult(n)->pass)==SAME)
extern	char	*ulist_getnid(uint i);
extern	void	ulist_write();
extern	void	ulist_set(uint i,var_t *v);
extern	void	ulist_init();
extern	uint	ulist_search(char *s);
extern	uint	ulist_fsearch(const char *s);
SHELL	uint	ulist_disp(char *s);

#define	ulist_getpost(n)	(ult(n)->post)
#define	ulist_getmpost(n)	(ult(n)->mpost)
#define	ulist_clrmpost(n)	(ult(n)->mpost=0)
#define	ulist_getlogin(n)	(ult(n)->login)
#define	ulist_getmlogin(n)	(ult(n)->mlogin)
#define	ulist_clrmlogin(n)	(ult(n)->mlogin=0)

#define	ulist_getdl(n)		(ult(n)->dl)
#define	ulist_incdl(n)		(++ult(n)->dl)
#define	ulist_getul(n)		(ult(n)->ul)
#define	ulist_incul(n)		(++ult(n)->ul)

#define	ulist_getbirthm(n)	(ult(n)->birthm)
#define	ulist_getbirthd(n)	(ult(n)->birthd)

typedef	struct
{
	ulist_t	lerge *	list;				/* ���[�U�[���X�g�ւ�far ptr */
	uint		member;					/* ���[�U�[�� */
	var_t		*defs;					/* �J�X�^����� */

	uint		number;					/* �����Ă��郆�[�U�[�ԍ� */
	var_t		*var;					/* ���[�U�[���S�� */
	var_t		*mptr;					/* ���ǃ|�C���^ */
	char		id[LN_id+1];			/* ���[�U�[ID */
	char		style[LN_key+1];
	char		handle[LN_handle+1];	/* �n���h�� */

}	user_t;

export	user_t	user;
#define	user_getnumber()	(user.number)
#define	user_id()			(user.id)
#define	user_handle()		(user.handle)
extern	char	*user_getnid();

SHELL	void	user_set(uint id);
SHELL	void	user_write();
SHELL	void	user_disp();
SHELL	void	user_chk(uint u);
SHELL	void	user_def(char *s);
extern	void	user_guest(char *s);
extern	void	user_init();
extern	void	user_fin();

SHELL	void	sign_up();

#define	isowner(s)	(strcmp(user_getnid(),s)==SAME)

SHELL	void	prof_write();
SHELL	void	prof_myview();
SHELL	void	prof_view();
extern	void	prof_read(uint i);


extern	char	getpass(char *s);

/* pool */
SHELL	bool	file_dnload();

/* io */

SHELL	void	remote_exec(const char *s);
extern	void	reset();
