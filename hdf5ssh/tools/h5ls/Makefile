TARGET = h5ls

main_target: bin

include $(VPATH)../GNUmakefile.rules

SYSCFLAGS+=$(hdf5_FLAGS) $(hdf5ref_FLAGS)
LIBS+= $(libssh2_LIB) $(SOCKET_LIBS) $(hdf5_LIB) $(hdf5ref_LIB) -lhdf5ssh $(zlib1_LIB) -lssh2

INCLUDES+=$(hdf5_FLAGS) $(hdf5ref_FLAGS) ${libssh2_FLAGS} -I${MAKE_ROOT}/external/libssh2
