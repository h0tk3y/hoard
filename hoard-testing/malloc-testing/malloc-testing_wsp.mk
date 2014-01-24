.PHONY: clean All

All:
	@echo ----------Building project:[ hoard_testing - Debug ]----------
	@"$(MAKE)" -f "hoard_testing.mk"
clean:
	@echo ----------Cleaning project:[ hoard_testing - Debug ]----------
	@"$(MAKE)" -f "hoard_testing.mk" clean
