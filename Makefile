CC=gcc
AR=ar rcs
CFLAGS=-Iinclude/ -c -g
LDFLAGS=-L. -g
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
