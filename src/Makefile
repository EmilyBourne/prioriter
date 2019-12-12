
export EXPORTED_VARS $(EXPORTED_VARS)

tests: OBJS
	@echo "Making   tests/"
	@$(MAKE) -C tests all

$(TESTS): OBJS
	@echo "Making   tests/"
	@$(MAKE) -C tests $(TEST_DIR)/$@

OBJS:
	@echo "Making   tools/"
	@$(MAKE) -C tools all
	@echo "Making   jobs/"
	@$(MAKE) -C jobs all

clean:
	@$(MAKE) -C tests $@
