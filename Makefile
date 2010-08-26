CC?=gcc
AR=ar rcs
CFLAGS+=-Iinclude/ -c -g
LDFLAGS+=-L. -g
DESTDIR?=
PREFIX?=/usr/local
SOURCES=src/dynarray.c src/nscript.c src/nsfuncs.c src/nsnamemaps.c src/nsobj.c src/nsstack.c src/trie.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libnscript.a

all: libnscript ns

clean:
	@echo "RM 	$(OBJECTS)";\
	rm -f $(OBJECTS)
	@echo "RM 	libnscript.a";\
	rm -f libnscript.a
	@echo "RM 	ns.o";\
	rm -f ns.o
	@echo "RM 	ns";\
	rm -f ns

install:
	mkdir -p ${DESTDIR}/${PREFIX}/lib
	cp libnscript.a ${DESTDIR}/${PREFIX}/lib
	rm -rf ${DESTDIR}/${PREFIX}/include/nscript
	mkdir -p ${DESTDIR}/${PREFIX}/include/nscript
	cp -rf include/* ${DESTDIR}/${PREFIX}/include/nscript
	mkdir -p ${DESTDIR}/${PREFIX}/bin
	cp ns ${DESTDIR}/${PREFIX}/bin
	cp nscc/nscc ${DESTDIR}/${PREFIX}/bin

uninstall deinstall:
	rm -f ${DESTDIR}/${PREFIX}/bin/ns
	rm -f ${DESTDIR}/${PREFIX}/bin/nscc
	rm -rf ${DESTDIR}/${PREFIX}/include/nscript
	rm -f ${DESTDIR}/${PREFIX}/lib/libnscript.a

libnscript: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	@echo "AR 	$(OBJECTS)";\
	$(AR) $@ $(OBJECTS)

.c.o:
	@echo "CC 	$<";\
	$(CC) $(CFLAGS) $< -o $@

ns.o: ns.c
	@echo "CC 	ns.c";\
	$(CC) $(CFLAGS) ns.c

ns: libnscript ns.o
	@echo "LD 	ns.o";\
	$(CC) $(LDFLAGS) -o ns ns.o -lnscript -lm

.PHONY: all install uninstall deinstall clean
