SRCS = alteration_list.c bracket_parser.c cc_token_list.c re_translator.c token_list.c
OBJS = $(SRCS:%.c=%.o)
DEPS = $(SRCS:%.c=%.d)
DEPS_DIR = dependencies
BIN_DIR ?= ../bin
vpath %.o $(BIN_DIR)
vpath %.d $(DEPS_DIR)
CFLAGS += -O3 -Wall -pedantic

.PHONY: clean_obj clean_dep clean depend all move

all: $(DEPS) $(OBJS) move

%.d: %.c
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) -MM $< -MF $(DEPS_DIR)/$@

-include $(DEPS_DIR)/*.d

move:
	mkdir -p $(BIN_DIR)
	@-mv *.o $(BIN_DIR) 2>/dev/null | :

clean_obj:
	@-rm $(OBJS:%.o=${BIN_DIR}/%.o)
clean_dep:
	@-rm -r $(DEPS_DIR)
clean: clean_obj clean_dep
