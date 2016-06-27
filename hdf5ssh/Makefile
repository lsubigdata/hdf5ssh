TARGET = hdf5ssh

all.dtor=visible

include $(VPATH)../GNUmakefile.rules
-include ${MAKE_ARCH_INCLUDES}/hdf5.cfg


LIBS+=$(hdf5_LIB) -lssl -lm $(SOCKET_LIBS)
LIBS+=$(libssh2_LIB)
LIBS=$(LIBS_${MAKE_UNAME}) $(hdf5ref_LIB) -lm 
SYSCFLAGS+=${hdf5_FLAGS}

INCLUDES+=$(hdf5_FLAGS) $(hdf5ref_FLAGS) ${libssh2_FLAGS} -I${MAKE_ROOT}/external/libssh2

C_FLAGS_msys=-D_NO_OLDNAMES -Doff_t=long -D_HDF5USEDLL_
C_FLAGS+=${F5_FLAGS_$(MAKE_UNAME)}

INCLUDES+=-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_POSIX_SOURCE -D_BSD_SOURCE

THISDIR:=$(abspath $(dir $(firstword $(MAKEFILE_LIST))))

TARGET_FLAGS+=$$(hdf5_FLAGS) -I$(THISDIR)
TARGET_LIB+=$$(hdf5_LIB)

helpme:
	@ echo "${MAKE_UNAME}"
	@ echo "$(LIBS_${MAKE_UNAME})"
