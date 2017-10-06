
# standard make variables

LIB = /usr/local/lib/libaoaux.a
CFLAGS = -O4 -I/usr/local/include
VPATH = ../include:/usr/local/lib

# library components

$(LIB):	$(LIB)(check_env.o)	\
	$(LIB)(get_line.o)	\
	$(LIB)(getchar_my.o)

# component dependencies

$(LIB)(check_env.o):		/usr/local/include/libaoaux.h
$(LIB)(get_line.o):		/usr/local/include/libaoaux.h
$(LIB)(getchar_my.o):		/usr/local/include/libaoaux.h

