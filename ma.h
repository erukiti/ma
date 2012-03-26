/*#define	DEBUG*/

#pragma	optimize	space

/* Program Depend Defination */
#define	MAX_fstack		32
#define	MAX_stack		16
#define	MAX_char		256
#define	MAX_func		254
#define	MAX_mem			512
#define	MAX_op			64
#define	MAX_var			32
#define	MAX_kinds		32
#define MAX_cont		16
#define	MAX_dsd			16
#define	MAX_mcdbuf		4096
#define	MAX_edmem		0x4000
#define	MAX_ringtemp	2048
#define	MAX_ringstr		256
#define	MAX_fbuf		256
#define	MAX_user		256			/* 新しいユーザー管理を考えない限り */
#define	MAX_block		1024
#define	MAX_batch		16
#define	MAX_map			8192		/* 書き込み管理マップの最大数 */
#define	MAX_notes		32			/* ノートの数の最大数 */
#define	MAX_chain		1024		/* 一つのノートの中の最大書き込み数 */
#define	MAX_fb			(ulong)6	/* binary arvhie のヘッダサイズ */

  /* 文字列の長さ */

#define	LN_id			8
#define	LN_pass			16
#define	LN_handle		24

#define	LN_key			8
#define	LN_dengon		32
#define	LN_title		32
#define	LN_ctrl			32
#define	LN_str			78 /*78*/
#define	LN_var			64
#define	LN_longdig		13
#define	LN_buf			128
#define	LN_longbuf		512
#define	LN_netid		32
#define	LN_dirs			32
#define	LN_fname		64
#define	LN_owner		LN_id+1+LN_netid
#define	LN_comline		64
#define	LN_datime		19
#define	LN_from			64
#define	LN_memo			64
#define	LN_dsd			64
#define	LN_result		32
#define	LN_op			128
#define	LN_filename		64
#define	LN_rfilename	8+1+3

#define	TMP_editfile	"temp.tmp"	/* !! */

/* include' */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <ctype.h>
#include <dos.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mgeneric.h"
#include "depend.h"
#include "host.h"
#include "revision.h"

/* */
# undef		SHELL
# define	SHELL
