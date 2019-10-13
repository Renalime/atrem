#CFLAGS = -g
LDLIBS = -lm
OBJECTS = main.o functionality_tests.o unit_tests.o matcher.o
VPATH = ../src

.PHONY: clean all

all: main

main: $(OBJECTS)

main.o: main.c

functionality_tests.o: functionality_tests.c functionality_tests.h matcher.c matcher.h

unit_tests.o: unit_tests.c unit_tests.h matcher.c matcher.h

matcher.o: matcher.c matcher.h

clean:
	-rm *.o
