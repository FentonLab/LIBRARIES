# macro definitions

LIBRARY = /usr/local/lib/libup.a
OPTIM = -O2

OBJS =\
unixpict.o


# program dependencies

$(LIBRARY):$(OBJS)
	ar r $(LIBRARY) $(OBJS)


# inference rule

.c.o:
	cc -c $(OPTIM) $*.c
	chmod a+rw $*.o


# function dependencies

unixpict.o:				unixpict.h

