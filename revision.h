/* %%revision */
#define	PROGRAM		"ＭAシステム(仮称)"
#define	VERSION		"0.00t"
#define	BUILD		"361"
#define	COPYRIGHT	"えるちゃん"
#define	DATIME		__DATE__ " " __TIME__
/* %%end */

#if	defined(LSI_C)
#	define	RV_compiler	"LSI-C 86"
#elif defined(__TURBOC__)
#	define	RV_compiler	"Turbo-C"
#else
#	define	RV_cimpiler	"????"
#endif
