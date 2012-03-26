typedef	struct
{
	ulong	num;					/* 現在注目している番号 */
	ulong	max;					/* ファイルの最大数 */

	FILE	*fp;					/* file */
	FILE	*fpt;					/* Temporary file */
	var_t	*var;					/* Variable */
	char	name[LN_filename+1];	/*"name"	登録時の名前 */
	char	rname[LN_rfilename+1];	/*"rname"	実際に収納されている名前 */
	ulong	size;					/*"size"	ファイルサイズ */
	time_t	ftime;					/*"ftime"	ファイルの時間 */
	uint	count;					/*"dlcount"	ダウンロードカウント */
}	pool_t;

export	pool_t	pool;

#if 0
typedef	struct
{
	size_t	size;					/* バッファに溜まったバイト数 */
	char	last;					/* 最後に送ったキャラクタ */
	char	buf[MAX_block];
}	block_t;

export	block_t	block;

typedef	struct
{
	ulong	data[MAX_batch];		
	ulong	max;					
	ulong	now;
}	batch_t;

export	batch_t;
#endif

extern	void	pool_init();
extern	void	pool_fin();
extern	bool	pool_read(ulong num);
extern	void	pool_write();
extern	void	pool_pageout();
extern	void	pool_getname(char *s,size_t l);
extern	void	pool_up();
extern	void	name_form(char *s,const char *t,size_t n);/* 名前の正規化 */
extern	void	pool_rname(char *s,const char *t);
extern	uint	name_drive(char *s);
extern	ulong	diskfree(uint n);

extern	bool	pool_upload();	/* 他からのexport */
extern	void	pool_dnload();
extern	bool	file_upload(FILE *fp);
extern	bool	pool_ucopy();
extern	bool	pool_dcopy();



extern	void	block_clr();
extern	void	block_putc(char c);
extern	void	block_putu(uint u);	/* little endian */
extern	void	block_send();

