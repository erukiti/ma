typedef	struct
{
	ulong	num;					/* ���ݒ��ڂ��Ă���ԍ� */
	ulong	max;					/* �t�@�C���̍ő吔 */

	FILE	*fp;					/* file */
	FILE	*fpt;					/* Temporary file */
	var_t	*var;					/* Variable */
	char	name[LN_filename+1];	/*"name"	�o�^���̖��O */
	char	rname[LN_rfilename+1];	/*"rname"	���ۂɎ��[����Ă��閼�O */
	ulong	size;					/*"size"	�t�@�C���T�C�Y */
	time_t	ftime;					/*"ftime"	�t�@�C���̎��� */
	uint	count;					/*"dlcount"	�_�E�����[�h�J�E���g */
}	pool_t;

export	pool_t	pool;

#if 0
typedef	struct
{
	size_t	size;					/* �o�b�t�@�ɗ��܂����o�C�g�� */
	char	last;					/* �Ō�ɑ������L�����N�^ */
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
extern	void	name_form(char *s,const char *t,size_t n);/* ���O�̐��K�� */
extern	void	pool_rname(char *s,const char *t);
extern	uint	name_drive(char *s);
extern	ulong	diskfree(uint n);

extern	bool	pool_upload();	/* �������export */
extern	void	pool_dnload();
extern	bool	file_upload(FILE *fp);
extern	bool	pool_ucopy();
extern	bool	pool_dcopy();



extern	void	block_clr();
extern	void	block_putc(char c);
extern	void	block_putu(uint u);	/* little endian */
extern	void	block_send();

