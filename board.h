/* Depend BOARD */

#define	NT_error	(num_n)0xffff
#define	NT_errorm	(num_m)0xffff
#define	NT_errors	(num_s)0

typedef	struct
{
	uint		no;					/* ノート番号 */

	char		name[LN_title+1];	/* ノートネーム */
	char		kind[LN_dsd+1];		/* そのノートの話題 */
	uint		number;				/* 書き込み数 */
	num_m		chain[MAX_chain];	/* ノート配列 */

	uint		frug;				/* 書き出しフラグ */
	uint		num;				/* 現在のリードポインタ */
}	notes_t;

typedef	struct
{
	notes_t lerge	*s[MAX_notes];	/* notes ptr配列 */

	void	(*readend)();		/*  読み終わった後の処理。 */
	void	(*mkfirst)();		/*  note_mkで最初に行う処理 */
	void	(*write)(num_m m);	/*  追加処理 */
	bool			dynamic;	/*  動的ノートを採用しているか */

	uint			num;			/* ノートの数 */
	uint			number;			/* 今見ているノート番号 (0~) */
}	note_t;

export	note_t	note;
#define	notes	note.s[note.number]

typedef	struct
{
	num_n	num;				/* チェック用 */
	uint	srn;				/* sion_srnで直接読み出せれるように */

	uint	to;					/* 書き込み相手/!!CUG関係にでも････ */
	char	title[LN_title+1];	/* 書き込みのタイトル */
	uint	file;				/* 書き込みにファイルが付属しているか */
	char	kind[LN_dsd+1];		/* 書き込みのジャンル */

	num_s	res;				/* 書き込みのレス元番号 ベースならば0 */

	uint	owner;				/* 書き込みのオーナー */
	time_t	datime;				/* 書き込みの日時 */
	bool	isdel;				/* 書き込みは削除されているか？*/
}	art_t;
export	art_t	art;

typedef	struct
{
	char			name[LN_key+1];	/*  コード */
	void	(*readend)();			/*  読み終わった後の処理。 */
	void	(*mkfirst)();			/*  note_mkで最初に行う処理 */
	void	(*write)(num_m l);		/*  追加処理 */
	bool			dynamic;		/*  動的ノートの採用ページがどうか */
}	nlist_t;

extern	nlist_t	nlist[];

typedef	struct
{
	num_s	lerge	*map;		/*D マップ配列へのptr */
	time_t	lerge	*datime;	/*D datime配列へのptr */
	ulong			num;		/*S そのステージの中の書き込みの数*/
	num_s			serial;		/*S そのステージの中の最大シリアルナンバー */
	char			code[LN_key+1];
	char			name[LN_str+1];
	var_t			*kind;		/*S そのステージの中のジャンル */
	fileb			*fba;		/* art indexのbinary archive ptr */
	bool			frug;		/* mapなどを掃き出す必要があるかどうか？ */
	time_t			mptr;		/* そのステージにおいての未読ポインタ */
}	stage_t;

typedef	struct
{
	var_t			*var;
	stage_t			*s;
/*	stage_t			*s[MAX_stage]; */
	bool			readfrug;	/* 書き込みを読んだ後かどうか */
	num_n			number;		/* 今注目している書き込み番号 */
	uint			stage;		/* 今注目しているステージ番号 */
}	board_t;
export	board_t	board;
#define	stages	board.s
/* #define	stages	board.s[board.stage] */

void	art_init();
void	art_fin();
extern	num_s	stage_add(num_s l);		/* stage.mapに加える */
extern	void	stage_mk();				/* stageで必要な構造を創る */
extern	void	stage_out();

/* note 回り */
extern	void	mkfirst_news();
extern	void	mkfirst_mail();
extern	void	write_news(num_m m);
extern	void	write_mail(num_m m);

/* num_n 関連 */
extern	void	number_mptr();
extern	num_n	art_chk(num_n n);

/* art Index */
extern	void	art_write(num_m m);
extern	void	art_varmk();

/* 書き込み */
extern	void	art_postout();
extern	void	post_after();
extern	void	art_post(num_s res,void (*func)());
extern	void	mail_after();

/* 未読ポインタ関連 */
#define	mptr_set(t)	stages->mptr=t
#define	mptr_get()	(stages->mptr)

/* 読みだし関連 */
extern	void	art_pageout();
extern	void	art_look();
extern	void	kind_sel(char *s);
extern	void	readend_news();
extern	void	readend_mail();

/* num コンバーター */
extern	num_s	getserial(num_m u);
extern	num_m	getmserial(num_s l);
extern	num_n	getnmap(num_m u);
extern	num_m	getmap(num_n u);
#define	getdisps(u)	(u+1)
#define	getnotes(u)	(u-1)

/* Mask 処理 */
