CC=gcc
AR=ar rcs
CFLAGS=-Iinclude/ -c -g
LDFLAGS=-L. -g
SOURCES=src/dynarray.c src/nscript.c src/nsfuncs.c src/nsnamemaps.c src/nsobj.c src/nsstack.c src/trie.c
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libnscript.a

all: libnscript test

clean:
	@echo "RM 	$(OBJECTS)";\
	rm -f $(OBJECTS)
	@echo "RM 	libnscript.a";\
	rm -f libnscript.a
	@echo "RM 	nstest.o";\
	rm -f nstest.o
	@echo "RM 	nstest";\
	rm -f nstest


libnscript: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	@echo "AR 	$(OBJECTS)";\
	$(AR) $@ $(OBJECTS)

.c.o:
	@echo "CC 	$<";\
	$(CC) $(CFLAGS) $< -o $@


test: libnscript nstest.o nstest

nstest.o:
	@echo "CC 	nstest.c";\
	$(CC) $(CFLAGS) nstest.c

nstest:
	@echo "LD 	nstest.o";\
	$(CC) $(LDFLAGS) -o nstest nstest.o -lnscript -lm
