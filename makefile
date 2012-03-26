.c.obj:
	$(CC) $(CFLAGS) -c $<
.a86.obj:
	$(CC) $(AFLAGS) -c $<

CC            = lcc
AFLAGS        =
CFLAGS        =
DEST          =
EXTHDRS       = 
HDRS          = ma.h mgeneric.h depend.h host.h revision.h board.h pool.h str.h mio.h
LDFLAGS       = 
LIBS          =
LINKER        = lcc
MAKEFILE      = makefile
RESPFILE      = respfile
OBJS          = board.obj debug.obj generic.obj host.obj io.obj line.obj notes.obj pool.obj revision.obj shell.obj sion.obj str.obj time.obj user.obj wfc.obj asm.obj
EXTOBJS       =
PRINT         = print
PROGRAM       = temp.exe
SRCS          = board.c debug.c generic.c host.c io.c line.c notes.c pool.c revision.c shell.c sion.c str.c time.c user.c wfc.c asm.a86

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS) $(LIBS)
		lcc -o $(PROGRAM) @$(RESPFILE)

clean:;		@rm $(OBJS)

depend:;	@mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@copy $(PROGRAM) $(DEST)

print:;		$(PRINT) $(HDRS) $(SRCS)

program:	$(PROGRAM)

tags:		$(HDRS) $(SRCS)
		@ctags $(HDRS) $(SRCS) >tags
###
board.obj: ma.h mgeneric.h depend.h host.h revision.h board.h pool.h str.h
debug.obj: ma.h mgeneric.h depend.h host.h revision.h mio.h
generic.obj: ma.h mgeneric.h depend.h host.h revision.h mio.h
host.obj: ma.h mgeneric.h depend.h host.h revision.h board.h str.h mio.h pool.h
io.obj: ma.h mgeneric.h depend.h host.h revision.h mio.h
line.obj: ma.h mgeneric.h depend.h host.h revision.h str.h board.h mio.h pool.h
notes.obj: ma.h mgeneric.h depend.h host.h revision.h board.h
pool.obj: ma.h mgeneric.h depend.h host.h revision.h pool.h str.h board.h mio.h
revision.obj: ma.h mgeneric.h depend.h host.h revision.h
shell.obj: ma.h mgeneric.h depend.h host.h revision.h str.h
sion.obj: ma.h mgeneric.h depend.h host.h revision.h
str.obj: ma.h mgeneric.h depend.h host.h revision.h str.h
time.obj: ma.h mgeneric.h depend.h host.h revision.h
user.obj: ma.h mgeneric.h depend.h host.h revision.h str.h
wfc.obj: ma.h mgeneric.h depend.h host.h revision.h mio.h str.h
asm.obj:
	$(CC) $(AFLAGS) -c asm.a86
