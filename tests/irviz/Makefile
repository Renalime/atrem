CFLAGS ?= -pedantic -Wall -O3
SRCS = print_ir.c
OBJS = $(SRCS:%.c=%.o)
DEPS = $(SRCS:%.c=%.d)
DEPS_DIR = dependencies
BIN_DIR ?= ../../bin
DEST_DIR ?= $(BIN_DIR)
vpath %.o $(BIN_DIR)
vpath %.o $(DEST_DIR)
vpath %.d $(DEPS_DIR)

.PHONY: clean all clean_dep clean_obj move

all: $(DEPS) $(OBJS) move

%.d: %.c
	mkdir -p $(DEPS_DIR)
	$(CC) $(CFLAGS) -MM $< -MF $(DEPS_DIR)/$@

-include $(DEPS_DIR)/*.d

move:
	mkdir -p $(DEST_DIR)
	@-mv *.o $(DEST_DIR) 2>/dev/null | :

clean_obj:
	@-rm $(OBJS:%.o=${DEST_DIR}%.o)

clean_dep:
	@-rm -r $(DEPS_DIR)

clean: clean_dep clean_obj
