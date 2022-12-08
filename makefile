CFLAGS+=-Iinc

.PHONY: all clean
all: urlencode.o libthe.o libthe.a findthe
clean:
	rm *.o *.a findthe

urlencode.o: lib/urlencode.c
	${CC} ${CFLAGS} -c -o $@ $?

libthe.o: lib/libthe.c
	${CC} ${CFLAGS} -c -o $@ $?

libthe.a: libthe.o urlencode.o
	${AR} -crs $@ $?

findthe: libthe.o urlencode.o src/cli.c
	${CC} ${CFLAGS} -o $@ $? -lcurl
