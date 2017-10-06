
# standard make variables

LIB = /usr/local/lib/libaux.a
CFLAGS = -O4 -I/usr/local/include
VPATH = ../include:/usr/local/lib

# library components

$(LIB):	$(LIB)(ecalloc.o)	\
	$(LIB)(efopen.o)	\
	$(LIB)(get_line.o)	\
	$(LIB)(sscanfLine.o)	

# component dependencies

$(LIB)(ecalloc.o):		
$(LIB)(efopen.o):		
$(LIB)(get_line.o):		
$(LIB)(sscanfLine.o):		/usr/local/include/libsize_types.h

