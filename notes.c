#include "ma.h"
#include "board.h"

nlist_t	nlist[]=
	{
		{
		 "news",
		 readend_news,
		 mkfirst_news,
		 write_news,
		 FALSE
		}
	 ,
		{
		 "mail",
		 readend_mail,
		 mkfirst_mail,
		 write_mail,
		 TRUE
		}
	 ,
		{
		 "",
		 NULL,
		 NULL,
		 NULL,
		 FALSE
		}
	};
