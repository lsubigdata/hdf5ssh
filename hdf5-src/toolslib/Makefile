TARGET=hdf5tools

A=a

include $(VPATH)../GNUmakefile.rules
include ${MAKE_ARCH_INCLUDES}/hdf5.cfg

LIBS+=${hdf5_LIB}
SYSCFLAGS+=${hdf5_FLAGS}

#INCLUDES+=-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_POSIX_SOURCE -D_BSD_SOURCE

ifdef	hdf5ref_FLAGS

do_toolslib: hdf5 lib 

else

do_toolslib:
	@echo
	@echo 'Warning: HDF5 Library is not available!'
	@echo '         Tools Library can not be compiled.'
	@echo
endif

hdf5:	$(HDF5_INSTALL_DIR)/include/hdf5.h

TARGET_FLAGS+=$$(hdf5ref_FLAGS) $$(hdf5_FLAGS) ${F5_FLAGS}
TARGET_LIB+=$$(hdf5ref_LIB) $$(hdf5_LIB)


