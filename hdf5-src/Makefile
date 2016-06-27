# H5_DEFAULT_PLUGINDIR=...


TARGET=hdf5
TARGET_DIRS=toolslib tools

MYDIR=$(shell sh -c "pwd")

SRC_C=${OBJ_PATH}/H5Tinit.c $(filter-out H5detect.c  H5make_libsettings.c, $(subst ${VPATH},,$(wildcard ${VPATH}*.c)))

SRC_C+=${OBJ_PATH}/H5lib_settings.c


LIBS_Linuxx86_64=-lpthread
LIBS_Linuxi686=-lpthread
LIBS =  $(LIBS_${MAKE_UNAME}) -lm ${DLLIB}

#LIBS+=-lstacktrace

#AllOfHDF5: make-tools

all.dtor=visible 

include $(VPATH)../GNUmakefile.rules

make-toolslib: all visible
make-tools: make-toolslib

SYSCFLAGS=-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_POSIX_SOURCE -D_BSD_SOURCE \
	-UH5_DEBUG_API -D_HDF5DLL_

INCLUDES+=-I$(OBJ_PATH)

#
# Support for dmalloc library
#
LIBS += $(dmalloc_LIB)
SYSCFLAGS+=${dmalloc_FLAGS}


ifdef	zlib123_FLAGS
SYSCFLAGS+=-DH5_HAVE_ZLIB_H  -DH5_HAVE_FILTER_DEFLATE=1 ${zlib123_FLAGS}
LIBS+=${zlib123_LIB}
endif

ifdef	zlib1_FLAGS
SYSCFLAGS+=-DH5_HAVE_ZLIB_H  -DH5_HAVE_FILTER_DEFLATE=1 ${zlib1_FLAGS}
LIBS+=${zlib1_LIB}
endif


TARGET_FLAGS+=-I$(MYDIR) ${zlib123_FLAGS} ${zlib1_FLAGS}
TARGET_LIB+=$(LIBS_${MAKE_UNAME}) ${zlib123_LIB} ${zlib1_LIB} $${DLLIB}

show:
	echo MYDIR=$(MYDIR)
	echo $(TARGET_FLAGS)

svn:
	mkdir tmp
	cd tmp; svn co http://svn.hdfgroup.uiuc.edu/hdf5/branches/hdf5_1_8/

me=$(shell cd ${VPATH}; pwd)


${OBJ_PATH}/H5Tinit.$O: ${OBJ_PATH}/H5Tinit.c
	$(CC) -I${VPATH}. -I${me} $(CFLAGS) -c $(SOURCEFILE_PARAMETER) $(OUTPUT_OBJECT)$@
	$(MAKEDEP)

${OBJ_PATH}/H5Tinit.c: ${OBJ_PATH}/H5detect.exe
	$< > $@

${OBJ_PATH}/H5detect.exe: ${VPATH}H5detect.c
	$(CC) $< -o $@

settings: ${OBJ_PATH}/H5lib_settings.c
	cat $<


${OBJ_PATH}/H5lib_settings.c: ${OBJ_PATH}/H5make_libsettings.exe
	$< > $@

${OBJ_PATH}/H5make_libsettings.exe: ${VPATH}H5make_libsettings.c
	$(CC) $< -o $@

clean::	clean-libsettings

clean-libsettings:
	-${RM} ${OBJ_PATH}/H5make_libsettings.exe ${OBJ_PATH}/H5lib_settings.c


${VPATH}%.h: tmp/src/%.h

${VPATH}%.h: tmp/src/%.h
	cp $< $@

${VPATH}%.c: tmp/src/%.c
	cp $< $@


update: backup-pubconf *.c *.h restore-pubconf

backup-pubconf:
	cp H5pubconf.h H5pubconf.h.bak

restore-pubconf:
	cp H5pubconf.h.bak H5pubconf.h

