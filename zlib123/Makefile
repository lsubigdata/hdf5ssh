OBJS= adler32.$O compress.$O crc32.$O deflate.$O gzio.$O infback.$O inffast.$O inflate.$O inftrees.$O trees.$O uncompr.$O zutil.$O

TARGET = zlib123

all.dtor=visible

include $(VPATH)../GNUmakefile.rules

#TARGET_FLAGS+="-I$${MAKE_ROOT}/external/zlib123"

ifneq (,$(findstring :/,$(SHELL)))
BINPWD:=sh -c "pwd -W"
else
BINPWD:=/bin/pwd
endif

HERE=$(shell $(BINPWD))
TARGET_FLAGS+="-I${HERE}"

SYSCFLAGS=-I.

