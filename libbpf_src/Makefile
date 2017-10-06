
# standard make variables

LIB = /usr/local/lib/libbpf.a
CFLAGS = -O4 -I/usr/local/include
VPATH = ../include:/usr/local/lib

# library components

$(LIB):	$(LIB)(ReadBPFHeader.o)\
	$(LIB)(sscanfLine.o)	

# component dependencies

$(LIB)(ReadBPFHeader.o):	/usr/local/include/libbpf.h /usr/local/include/libsize_types.h		
$(LIB)(sscanfLine.o):		/usr/local/include/libsize_types.h		
