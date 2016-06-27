done: welcome more

welcome:: all
more: all

-include */target.inc

.PHONY: $(TARGET_DIRS)

all: multiple

welcome::
	@echo
	@echo
	@echo The Fiber Bundle HDF5 Library has been built successfully.
	@echo

ifneq (,$(findstring :/,$(SHELL)))
BINPWD:=sh -c "pwd -W"
else
BINPWD:=/bin/pwd
endif

MAKE_ROOT:=$(shell $(BINPWD))

include $(MAKE_ROOT)/make/multiple.rules

more: ${addons} 

libclean::
	-$(RM) ${LIB_PATH}/*.dll
	-$(RM) ${LIB_PATH}/*.so
	-$(RM) ${LIB_PATH}/*.a
	-$(RM) ${LIB_PATH}/*.cfg
	-$(RM) ${LIB_PATH}/*.lib
