VERBOSE=0

CXX := g++
CXXFLAGS := --std=c++11
INCLUDE := include \
	include/jobs \
	include/tools

INC := $(addprefix -I$(CURDIR)/, $(INCLUDE))
TINC := $(INC) $(addprefix -I$(CURDIR)/, include/extern)

EXEC_DIR := $(CURDIR)/build
TEST_DIR := $(CURDIR)/build/tests
OBJ_DIR := $(EXEC_DIR)/intermediate

TESTS =	test_jobs \
		test_string_tools

EXPORTED_VARS = CXX CXXFLAGS OBJ_DIR INC TESTS TINC EXEC_DIR VERBOSE TEST_DIR
export EXPORTED_VARS $(EXPORTED_VARS)

all: tests

tests:
	@echo "Making src/"
ifeq ($(VERBOSE),1)
	$(MAKE) -C src $@
else
	@$(MAKE) -C src $@ --no-print-directory
endif

$(TESTS):
	@echo "Making src/"
ifeq ($(VERBOSE),1)
	$(MAKE) -C src $@
else
	@$(MAKE) -C src $@ --no-print-directory
endif

OBJS:
	@echo "Making src/"
ifeq ($(VERBOSE),1)
	$(MAKE) -C src $@
else
	@$(MAKE) -C src $@ --no-print-directory
endif

help:
	@echo "Available targets: $(TESTS)"

clean:
	rm -f $(OBJ_DIR)/*.o
ifeq ($(VERBOSE),1)
	$(MAKE) -C src $@
else
	@$(MAKE) -C src $@ --no-print-directory
endif