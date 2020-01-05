DIR_JOB_SRCS=$(wildcard ../jobs/*.cpp)
JOB_SRCS=$(DIR_JOB_SRCS:../jobs/%=%)
JOB_OBJS=$(addprefix $(OBJ_DIR)/, $(JOB_SRCS:.cpp=.o))

all: $(addprefix $(TEST_DIR)/, $(TESTS))

$(TEST_DIR)/test_jobs: test_jobs.cpp $(JOB_OBJS) $(addprefix $(OBJ_DIR)/, compare_tools.o)
ifeq ($(VERBOSE), 1)
	$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
else
	@$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
endif

$(TEST_DIR)/test_compare_tools: test_compare_tools.cpp $(addprefix $(OBJ_DIR)/, compare_tools.o)
	@echo "Making     "$<
ifeq ($(VERBOSE), 1)
	$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
else
	@$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
endif

$(TEST_DIR)/test_deadlined_jobs: test_deadlined_jobs.cpp $(JOB_OBJS) $(addprefix $(OBJ_DIR)/, compare_tools.o)
	@echo "Making     "$<
ifeq ($(VERBOSE), 1)
	$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
else
	@$(CXX) $(CXXFLAGS) $(TINC) $^ -o $@
endif

clean:
	rm -f $(TEST_DIR)/test_jobs
	rm -f $(TEST_DIR)/test_compare_tools
	rm -f $(TEST_DIR)/test_deadlined_jobs
