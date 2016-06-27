#
# GNUmakefile for main target
#
OBJS = main.$O
TARGET = main
LIBS=-L${LIB_PATH_STATIC}   -lgui -lstalib

#
# Override default rule to create a binary, not a library.
#
main_target: bin

include $(VPATH)../GNUmakefile.rules
include $(MAKE_ROOT)/make/bin-shared.rules

