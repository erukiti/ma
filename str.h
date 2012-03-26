/* Depend Str */

typedef	union
{
	char	*ptr;
	uint	num;
}	uarg_t;								/* procedure の引数 */

typedef	struct
{
	char	*name[MAX_func];				/* far */
	uint	(*func[MAX_func])(uarg_t,uarg_t);	/* far */
	char	argc[MAX_func];
}	flist_t;
extern	flist_t	flist;

typedef	struct
{
	char	name [MAX_op][LN_key+1];	/* far */
	char	list [MAX_op][LN_str+1];	/* far */

	char	is   [LN_key+1];	/* 現在の居場所 */

	char	param[LN_buf+1];	/* パラメータ全部*/
	uint	num;				/* オプションがもし数字ならばその数字 */

	uint	result;	
	uint	max_escape;
	uint	esc_result[MAX_cont];
	void	(*escape[MAX_cont])();
/*		where.help=sion.body;*/
}	where_t;
export	where_t	where;

#define	where_setres(n)	where.esc_result[where.max_escape-1]=n
#define	where_getres()	(where.result)

extern	recursive	void	where_shell(const char *t);

typedef	struct
{
	uint		num;
	char lerge	*ptr[MAX_stack];
}	stack_t;
export	stack_t	stack;

extern	bool	continual;
extern	char	max_cnt;
extern	void	(*cnt_proc[MAX_cont])();

