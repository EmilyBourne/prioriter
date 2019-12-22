
all: $(addprefix $(TEST_DIR)/, $(TESTS))

$(TEST_DIR)/test_jobs: test_jobs.cpp $(addprefix $(OBJ_DIR)/, job_interface.o jobs.o priority.o compare_tools.o job_comparisons.o)
	@echo "Making     "$<
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

clean:
	rm -f $(TEST_DIR)/test_jobs
	rm -f $(TEST_DIR)/test_compare_tools
