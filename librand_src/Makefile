
# standard make variables

LIB = /usr/local/lib/librand.a
CFLAGS = -O4 -I/usr/local/include
VPATH = ../include:/usr/local/lib

# library components

$(LIB):	$(LIB)(expdev.o)	\
	$(LIB)(gammln.o)	\
	$(LIB)(gasdev.o)	\
	$(LIB)(my_randoms.o)	\
	$(LIB)(my_seed48.o)	\
	$(LIB)(poidev.o)	\
	$(LIB)(ran1.o)		\
	$(LIB)(rint_my.o)	

# component dependencies

$(LIB)(expdev.o):		
$(LIB)(gammln.o):		
$(LIB)(gasdev.o):		
$(LIB)(my_randoms.o):		/usr/local/include/librand.h
$(LIB)(my_seed48.o):		/usr/local/include/librand.h
$(LIB)(poidev.o):		
$(LIB)(ran1.o):		
$(LIB)(rint_my.o):	

