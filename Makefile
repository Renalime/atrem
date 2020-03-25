SRC_DIR = src
TESTS_DIR = tests
BIN_DIR = bin
CFLAGS = -pedantic -Wall -O0 -g

export CFLAGS

.PHONY: clean install uninstall tests tests_clean

tests:
	$(MAKE) -C $(SRC_DIR)
	$(MAKE) -C $(TESTS_DIR)
tests_clean:
	@-rm -r $(BIN_DIR)
