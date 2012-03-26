/* 
int	atoi(const char *s)
{
	int	val;
	char	sign, c;

	while (*s == ' ' || *s == '\t')
		s++;
	if ((sign = *s) == '-' || sign == '+')
		s++;
	val = 0;
	while (c = *s++, isdigit(c))
		val = val * 10 + (c - '0');
	if (sign == '-')
		val = -val;
	return (val);
}
*/

#include "ma.h"

/* UTC - JST = TZ(-9) */
char	*strptime(time_t t)
{
	char	*p,h,m,s,d;

	p=getring(14+1);
	*p='\0';

	d=t/(60*60*24);

	h=gethour(t);
	m=getmin(t);
	s=getsec(t);

	if (d>0)
		sprt(p			,lpu(d,0),"日間と",NULL);

	if (h>0)
		sprt(p+strlen(p),lpu(h,-2),"時間",NULL);
	if (m>0)
		sprt(p+strlen(p),lpu(m,-2),"分",NULL);

		sprt(p+strlen(p),lpu(s,-2),"秒",NULL);
	return p;
}

/*
平成,H		1988
昭和,S		1925
大正,T		1911
明治,M		1867
西暦,'		0

年,月,日	
時,分,秒	
/-: 
*/

char	year_kanji[4][4+1]={"明治","大正","昭和","平成"};
char	year_alpha[4+1]={"MTSH"};
uint	year_num[4]={1867,1911,1925,1988};
char	derim_kanji[12+1]={"年月日時分秒"};

void	dtotm(char *s,struct tm *tm)
{
	char	*p;
	uint	a,b[6],y;
	uint	i,j,f;

	setdatime(getdate(),tm);

	b[0]=tm->tm_year;
	for(i=1;i<6;++i)
		b[i]=0;

	y=0;
	if (isdigit(*s))
		f=0; else
		{
		 f=1;
		 if (!iskanji(*s))
		 	{
		 	 p=strchr(year_alpha,*s);
		 	 if (p!=NULL)
		 	 	y=year_num[p-s];
		 	}
		 	{
		 	 for(i=0;i<4;++i)
		 	 	{
		 	 	 if (strncmp(s,year_kanji[i],4)==SAME)
		 	 	 	{
		 	 	 	 y=year_num[i];
		 	 	 	 break;
		 	 	 	}
		 	 	}
		 	 s=skip_kanji(s);
		 	}
		}
	for(i=0;i<6;++i)
		{
		 s=strpbrk(s,"0123456789");
		 if (s==NULL)
		 	{
		 	 if (f==0 &&i<3)
		 	 	{
		 	 	 j=3;
			 	 for (;i>0;)
			 	 	{
			 	 	 --i;
			 	 	 --j;
			 	 	 b[j]=b[i];
			 	 	}
			 	 for(;--j>0;)
			 	 	b[j]=0;
				 b[0]=tm->tm_year;
			 	}
		 	 break;
		 	}
		 a=atoi(s);
		 while(isdigit(*s))
		 	++s;
		 if (iskanji(*s))
		 	{
		 	 for(j=0;j<6;++j)
		 	 	{
		 	 	 if (strncmp(s,derim_kanji+j*2,2)==SAME)
		 	 	 	{
		 	 	 	 f=1;
		 	 	 	 i=j;
		 	 	 	 break;
		 	 	 	}
		 	 	}
		 	 s=skip_kanji(s);
		 	}
		 b[i]=a;
		}
/*
0~69 = +2000 -1970	+2000
70~99= +1900 -1970	+1900
1970~= +0    -1970	+-0
*/
	b[0]+=y;
	if (b[0]<100)
		b[0]+=(b[0]<70)?2000:1900;
/* printf("%d/%d/%d %d:%d:%d ",b[0],b[1],b[2],b[3],b[4],b[5]);*/
/* dbprt(lpu(b[0],0),"/",lpu(b[1],0),"/",lpu(b[2],0)," "
			,lpu(b[3],0),":",lpu(b[4],0),":",lpu(b[5],0),"\n",NULL); */
	tm->tm_year=b[0];
	tm->tm_mon =b[1];
	tm->tm_mday=b[2];

	tm->tm_hour=b[3];
	tm->tm_min =b[4];
	tm->tm_sec =b[5];
}

time_t	dtos(char *s)
{
	struct tm	tm;
	dtotm(s,&tm);
	return mkdatime(mkdate(tm),mktm(tm));
}

void	wait_minute(uint u)
{
	time_t	t;

	timer_res(t);
	for(;;)
		{
		 if (timer_pas(t)>=u)
		 	return;
		}
}


/* 
1970/01/01 00:00:00
date_t


356,355,355,355	356,355,355,355	
leapyear=4年おきで、400年毎ではない100年毎を除いた時。
UTCの範囲内においては、4年おきだけしか存在しない!
1	2	3	4	5	6	7	8	9	10	11	12
31	28	31	30	31	30	31	31	30	31	30	31
-------------------------------------------------
1	32	60	91	121	152	182	213	244	274	305	335
-------------------------------------------------
1	32	61	92	122	153	183	214	245	275	306	336
31	60	91	121	152	182	213	244	274	305	335	366
--------------------------------------------------

*/

int	mind[2][13]=
{
	{1,2,0,1,1,2,2,3,4,4,5,5,6},
	{1,2,1,2,2,3,3,4,5,5,6,6,7}
};

int	addm[2][12]=
{
	{30,29,31,30,30,29,29,28,27,27,26,26},
	{30,29,30,29,29,28,28,27,26,26,25,25}
};

char	*dwstr[7]=
{
	{"Sun"},
	{"Mon"},
	{"Tue"},
	{"Wed"},
	{"Thu"},
	{"Fri"},
	{"Sat"},
};

char	*dwjstr[7]=
{
	{"日曜"},
	{"月曜"},
	{"火曜"},
	{"水曜"},
	{"木曜"},
	{"金曜"},
	{"土曜"},
};

void	setdatime(date_t d,struct tm *tm)
{
	uint	y,a,m;
/*	uint	dd;
	dd=d;*/

/* dbprt("setdatime :",lpu(d,0),"\n",NULL); */

	tm->tm_wday=(d+4)%7;
	y=0;
	if (d<DT_2yearday)
		{
		 a=d/DT_1yearday;
		 d%=DT_1yearday;
		}else
		{
		 d+=DT_2yearday+1;
		 y=d/DT_4yearday-1;
		 d%=DT_4yearday;

		 a=0;
		 if (d>0)
			{
			 --d;
			 a=d/DT_1yearday;
			 d%=DT_1yearday;
			 if (a==0)
			 	++d;
			}
		 a+=2;
		}
	tm->tm_year=DT_baseyear+a+y*4;
	tm->tm_yday=d;

	a=(a==2)?1:0;
	++d;
	m=d/30;
	d%=30;

	if (d>=mind[a][m])
		d-=(mind[a][m]-1); else
		{
		 --m;
		 d+=addm[a][m];
		}
	tm->tm_mday=d;
	tm->tm_mon=m+1;
}

int	amday[2][12]=
{
	{0,31,59,90,120,151,181,212,243,273,304,334},
	{0,31,60,91,121,152,182,213,244,274,305,335}
};

int	hmday[2][12]=
{
	{31,28,31,30,31,30,31,31,30,31,30,31},
	{31,29,31,30,31,30,31,31,30,31,30,31}
};

date_t	mkdate(struct tm tm)
{
	uint	a,y,m,d;

	y=tm.tm_year;
	y=(y<1970||y>2015)?0:y-1970;
	a=(y%4==2)?1:0;
	m=(tm.tm_mon==0||tm.tm_mon>12)?0:tm.tm_mon-1;
	d=(tm.tm_mday==0||hmday[a][m]<tm.tm_mday)?0:tm.tm_mday-1;
	d+=amday[a][m];

	if (y<3)
		d+=y*DT_1yearday; else
		{
		 y-=3;
		 d+= ((y/4)*DT_4yearday) +((y%4)*DT_1yearday) +DT_3yearday+1;
		}
/* dbprt("mkdate:",lpu(d,0),"\n",NULL); */
	return d;
}

time_t	mktm(struct tm tm)
{
	return	((tm.tm_sec>60)?0:tm.tm_sec)
			+((tm.tm_min>60)?0:tm.tm_min)*60
			+((tm.tm_hour>24)?0:tm.tm_hour)*60*60;
}

time_t	mkdatime(date_t d,time_t t)
{
	return	t+d*DT_daytime;
}

/*
	BOARD.C  673	:書き込みの日付		time_t
	BOARD.C  789	:未読ポインタ		...
	GENERIC.C  534	:エラー報告			e
	HOST.C  12		:システムログ		e
	WFC.C  167		:ログイン日時		time_t
	WFC.C  337		:ホストの画面表示	e

	HOST.C  150		:cron_day			e
	WFC.C  115		:ログイン日付の報告	d
					:ログイン時間		t
	WFC.C  89		:曜日				Tue
*/


char	*stredatime(time_t t)
{
	char	*p;
	struct tm	tm;

/* dbprt("stredatime :",lpu(t,0),"/",lpu(DT_daytime,0),"\n",NULL); */

	setdatime(t/DT_daytime,&tm);
	p=getring(8+1+8+1);
	t%=DT_daytime;
	sprt(p,lpu(tm.tm_year-1900,2),"/",lpu(tm.tm_mon,2),"/",lpu(tm.tm_mday,2),
		" ",lpu(gethour(t),2),":",lpu(getmin(t),2),":",lpu(getsec(t),2),NULL);
	return p;
}

char	*strhdatime(time_t t)	/* HGt用時間表示 */
{
	char	*p;
	struct tm	tm;

	setdatime(t/DT_daytime,&tm);
	p=getring(64);
	t%=DT_daytime;
	sprt(p,lpu(tm.tm_year,0),"年 ",lpu(tm.tm_mon,0),"月 ",lpu(tm.tm_mday,0),
		"日 ",dwjstr[tm.tm_wday],"日 牛",(gethour(t)>12)?"後 ":"前 "
		,lpu(gethour(t)%12,0),"時 ",lpu(getmin(t),0),"分",NULL);
	return p;
}

char	*strdate(date_t d)
{
	char	*p;
	struct tm	tm;

	setdatime(d,&tm);
	p=getring(32);
	sprt(p,lpu(tm.tm_year,-4),"年",lpu(tm.tm_mon,-2),"月"
						,lpu(tm.tm_mday,-2),"日(",dwjstr[tm.tm_wday],")",NULL);
	return p;
}

char	*strtime(time_t t)
{
	char	*p;

	p=getring(9);
	t%=DT_daytime;
	sprt(p,lpu(gethour(t),-2),"時",lpu(getmin(t),-2),"分",NULL);
	return p;
}

