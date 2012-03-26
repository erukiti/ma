/* Depend BOARD */

#define	NT_error	(num_n)0xffff
#define	NT_errorm	(num_m)0xffff
#define	NT_errors	(num_s)0

typedef	struct
{
	uint		no;					/* �m�[�g�ԍ� */

	char		name[LN_title+1];	/* �m�[�g�l�[�� */
	char		kind[LN_dsd+1];		/* ���̃m�[�g�̘b�� */
	uint		number;				/* �������ݐ� */
	num_m		chain[MAX_chain];	/* �m�[�g�z�� */

	uint		frug;				/* �����o���t���O */
	uint		num;				/* ���݂̃��[�h�|�C���^ */
}	notes_t;

typedef	struct
{
	notes_t lerge	*s[MAX_notes];	/* notes ptr�z�� */

	void	(*readend)();		/*  �ǂݏI�������̏����B */
	void	(*mkfirst)();		/*  note_mk�ōŏ��ɍs������ */
	void	(*write)(num_m m);	/*  �ǉ����� */
	bool			dynamic;	/*  ���I�m�[�g���̗p���Ă��邩 */

	uint			num;			/* �m�[�g�̐� */
	uint			number;			/* �����Ă���m�[�g�ԍ� (0~) */
}	note_t;

export	note_t	note;
#define	notes	note.s[note.number]

typedef	struct
{
	num_n	num;				/* �`�F�b�N�p */
	uint	srn;				/* sion_srn�Œ��ړǂݏo�����悤�� */

	uint	to;					/* �������ݑ���/!!CUG�֌W�ɂł॥�� */
	char	title[LN_title+1];	/* �������݂̃^�C�g�� */
	uint	file;				/* �������݂Ƀt�@�C�����t�����Ă��邩 */
	char	kind[LN_dsd+1];		/* �������݂̃W������ */

	num_s	res;				/* �������݂̃��X���ԍ� �x�[�X�Ȃ��0 */

	uint	owner;				/* �������݂̃I�[�i�[ */
	time_t	datime;				/* �������݂̓��� */
	bool	isdel;				/* �������݂͍폜����Ă��邩�H*/
}	art_t;
export	art_t	art;

typedef	struct
{
	char			name[LN_key+1];	/*  �R�[�h */
	void	(*readend)();			/*  �ǂݏI�������̏����B */
	void	(*mkfirst)();			/*  note_mk�ōŏ��ɍs������ */
	void	(*write)(num_m l);		/*  �ǉ����� */
	bool			dynamic;		/*  ���I�m�[�g�̗̍p�y�[�W���ǂ��� */
}	nlist_t;

extern	nlist_t	nlist[];

typedef	struct
{
	num_s	lerge	*map;		/*D �}�b�v�z��ւ�ptr */
	time_t	lerge	*datime;	/*D datime�z��ւ�ptr */
	ulong			num;		/*S ���̃X�e�[�W�̒��̏������݂̐�*/
	num_s			serial;		/*S ���̃X�e�[�W�̒��̍ő�V���A���i���o�[ */
	char			code[LN_key+1];
	char			name[LN_str+1];
	var_t			*kind;		/*S ���̃X�e�[�W�̒��̃W������ */
	fileb			*fba;		/* art index��binary archive ptr */
	bool			frug;		/* map�Ȃǂ�|���o���K�v�����邩�ǂ����H */
	time_t			mptr;		/* ���̃X�e�[�W�ɂ����Ă̖��ǃ|�C���^ */
}	stage_t;

typedef	struct
{
	var_t			*var;
	stage_t			*s;
/*	stage_t			*s[MAX_stage]; */
	bool			readfrug;	/* �������݂�ǂ񂾌ォ�ǂ��� */
	num_n			number;		/* �����ڂ��Ă��鏑�����ݔԍ� */
	uint			stage;		/* �����ڂ��Ă���X�e�[�W�ԍ� */
}	board_t;
export	board_t	board;
#define	stages	board.s
/* #define	stages	board.s[board.stage] */

void	art_init();
void	art_fin();
extern	num_s	stage_add(num_s l);		/* stage.map�ɉ����� */
extern	void	stage_mk();				/* stage�ŕK�v�ȍ\����n�� */
extern	void	stage_out();

/* note ��� */
extern	void	mkfirst_news();
extern	void	mkfirst_mail();
extern	void	write_news(num_m m);
extern	void	write_mail(num_m m);

/* num_n �֘A */
extern	void	number_mptr();
extern	num_n	art_chk(num_n n);

/* art Index */
extern	void	art_write(num_m m);
extern	void	art_varmk();

/* �������� */
extern	void	art_postout();
extern	void	post_after();
extern	void	art_post(num_s res,void (*func)());
extern	void	mail_after();

/* ���ǃ|�C���^�֘A */
#define	mptr_set(t)	stages->mptr=t
#define	mptr_get()	(stages->mptr)

/* �ǂ݂����֘A */
extern	void	art_pageout();
extern	void	art_look();
extern	void	kind_sel(char *s);
extern	void	readend_news();
extern	void	readend_mail();

/* num �R���o�[�^�[ */
extern	num_s	getserial(num_m u);
extern	num_m	getmserial(num_s l);
extern	num_n	getnmap(num_m u);
extern	num_m	getmap(num_n u);
#define	getdisps(u)	(u+1)
#define	getnotes(u)	(u-1)

/* Mask ���� */
