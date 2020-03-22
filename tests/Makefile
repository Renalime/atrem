CFLAGS ?= -pedantic -Wall -O3
LDLIBS = -lm
TEST_SRCS = main.c token_list_tests.c alteration_list_tests.c
TEST_OBJS = $(TEST_SRCS:%.c=%.o)
DEPS = $(TEST_SRCS:%.c=%.d)
SRCS = alteration_list.c bracket_parser.c cc_token_list.c re_translator.c token_list.c atrem.c print_ir.c
OBJS = $(SRCS:%.c=%.o)
DEPS_DIR = dependencies
BIN_DIR ?= ../bin
DEST_DIR ?= $(BIN_DIR)/tests
IRVIZ_DIR ?= irviz
vpath %.o $(BIN_DIR)
vpath %.o $(DEST_DIR)
vpath %.d $(DEPS_DIR)

.PHONY: clean all clean_dep clean_obj move irviz

all: $(DEPS) $(TEST_OBJS) irviz main move
main: $(TEST_OBJS) $(OBJS:%.o=${BIN_DIR}/%.o)

%.d: %.c
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) -MM $< -MF $(DEPS_DIR)/$@

-include $(DEPS_DIR)/*.d

move:
	mkdir -p $(DEST_DIR)
	@-mv *.o $(DEST_DIR) 2>/dev/null | :
	@-mv main $(DEST_DIR) 2>/dev/null | :

irviz:
	$(MAKE) -C $(IRVIZ_DIR)

clean_obj:
	@-rm $(TEST_OBJS:%.o=${DEST_DIR}/%.o)
clean_dep:
	@-rm -r $(DEPS_DIR)
clean: clean_dep clean_obj
