.PHONY: clean All

All:
	@echo ----------Building project:[ hoard - Release ]----------
	@"$(MAKE)" -f "hoard.mk"
clean:
	@echo ----------Cleaning project:[ hoard - Release ]----------
	@"$(MAKE)" -f "hoard.mk" clean
