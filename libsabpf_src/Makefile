
# ReadBPFHeader using the sess_anal SESS structure

# standard make variables

LIB = /usr/local/lib/libsabpf.a
CFLAGS = -O4 -I/usr/local/include
VPATH = ../include:/usr/local/lib

# library components

$(LIB):	$(LIB)(ReadBPFHeader.o) \
	$(LIB)(rec_time_sort.o) \
	$(LIB)(reverse.o)

# component dependencies

$(LIB)(ReadBPFHeader.o):		/usr/local/include/libsize_types.h /usr/local/include/libsabpf.h /usr/local/include/libsess_anal.h
$(LIB)(rec_time_sort.o):		/usr/local/include/libsize_types.h /usr/local/include/libsess_anal.h
$(LIB)(reverse.o):			/usr/local/include/libsize_types.h

