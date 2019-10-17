SHELL = /bin/bash
ARFLAGS = rcs
VPATH = ./src
OBJECTS = matcher.o
ALIBS = libatrem.a
LIBH = matcher.h
SRCDIR = ./src
FINALDEST = $(DESTDIR)$(PREFIX)

ifeq ($(PREFIX),)
	PREFIX = /usr/local
endif

.PHONY: clean install uninstall

libatrem.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

matcher.o: matcher.c matcher.h

install: $(ALIBS)
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp -t $(DESTDIR)$(PREFIX)/lib/ $^ 
	cp -t $(DESTDIR)$(PREFIX)/include/ $(SRCDIR)/matcher.h

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/libatrem.a
	rm -f $(DESTDIR)$(PREFIX)/include/matcher.h

clean:
	-rm *.o
	-rm *.a

