CFLAGS = -pedantic -Wall
LDLIBS = -lm
OBJECTS = main.o functionality_tests.o unit_tests.o matcher.o re_translator.o token_list.o char_class_token.o
VPATH = ../src

.PHONY: clean all

all: main

main: $(OBJECTS)

main.o: main.c

functionality_tests.o: functionality_tests.c functionality_tests.h matcher.c matcher.h

unit_tests.o: unit_tests.c unit_tests.h matcher.c matcher.h test_framework.h

matcher.o: matcher.c matcher.h

re_translator.o: re_translator.c re_translator.h token_list.h

token_list.o: token_list.h token_list.c

char_class_token.o: char_class_token.h char_class_token.c globals.h

clean:
	-rm *.o
