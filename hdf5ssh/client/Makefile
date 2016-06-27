TARGET=HDF5SSH2Client

me: bin

include $(VPATH)../GNUmakefile.rules

SYSCFLAGS+=$(hdf5_FLAGS) $(hdf5ref_FLAGS) $(hdf5ssh_FLAGS)
LIBS+= $(libssh2_LIB) $(SOCKET_LIBS) $(hdf5_LIB) $(hdf5ref_LIB) -lhdf5ssh -lssh2 $(zlib1_LIB)
