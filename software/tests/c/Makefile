# SPDX-License-Identifier: MIT
# Host unit tests for the Ping Lite logic modules (no cFS needed).
CC ?= gcc
A := ../../apps
INC := -I$(A)/pl_inc -I$(A)/pl_eclss/fsw/src -I$(A)/pl_pwrth/fsw/src -I$(A)/pl_ops/fsw/src -I$(A)/pl_tlm/fsw/src
CFLAGS ?= -std=c99 -Wall -Wextra -Werror -pedantic -O1 -g --coverage
LOGIC := $(A)/pl_eclss/fsw/src/pl_eclss_logic.c $(A)/pl_pwrth/fsw/src/pl_pwrth_logic.c \
         $(A)/pl_ops/fsw/src/pl_ops_logic.c $(A)/pl_tlm/fsw/src/pl_tlm_logic.c
TESTS := test_eclss test_pwrth test_ops test_tlm
B := build

all: $(addprefix $(B)/,$(TESTS)) $(B)/pl_replay
$(B)/%: %.c $(LOGIC) ut.h
	@mkdir -p $(B)
	$(CC) $(CFLAGS) $(INC) -o $@ $< $(LOGIC) -lm
test: all
	@set -e; for t in $(TESTS); do ./$(B)/$$t; done
clean:
	rm -rf $(B) *.gcda *.gcno
.PHONY: all test clean
