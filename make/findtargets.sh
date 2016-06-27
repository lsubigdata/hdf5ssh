#!/bin/sh

#
# $Id: findtargets.sh,v 1.1.1.1 2000/02/10 19:50:53 cvs Exp $
#
# $Log: findtargets.sh,v $
# Revision 1.1.1.1  2000/02/10 19:50:53  cvs
# Makefile Configuration System
#
# Revision 1.13  1999/02/08 10:21:23  bzfskoro
# case ${arch} statement shortened.
#
# Revision 1.12  1999/02/08 10:16:55  bzfskoro
# Convert some relative paths to absolute paths before invoking compilation.
#
# Revision 1.11  1999/02/05 09:42:12  bzfbenge
# Change into directory, not into filename...
#
# Revision 1.10  1999/01/29 14:31:26  bzfbenge
# Compile in arch-specific directory to avoid timing problems
# with temporary compiler internal object files
# due to simultaneous compilation of incompatible archtitectures
#
# Revision 1.9  1998/05/18 13:24:33  bzfstall
# HP ready
#
# Revision 1.8  1998/05/08 13:23:14  bzfbenge
# compile with -n32 -mips3 in any IRIX case
#
# Revision 1.7  1998/05/05 16:08:37  bzfbenge
# cvs comments
#
#

#
# Hacky script to execute tool programs written in C on
# arbitrary architectures during the make process.
# The corresponding C code ($0.c instead of $0.sh) from the
# $MAKE_ROOT/make directory is compiled on-the-fly, when this
# script is invoked, if the binary not yet exists.
#  If $MAKE_ROOT has a bin/ subdirectory and it is writable,
# then the binary is put there in the appropiate arch subdir,
# otherwise it is created in the /tmp/${arch} subdirectory.
# For compilation cc is used - with no flags, but if the
# environment variables CC and CFLAGS are set, they will be used.
#

#MAKE_ROOT=${0%/*}/..

MAKE_ROOT=`dirname $0`/..

# basename
#bin=${0##*/}
#bin=${bin%.sh}

bin=`basename $0 .sh`

arch=`${MAKE_ROOT}/make/arch`


if [ -x ${MAKE_ROOT}/bin/$arch/$bin ]
then
    exec ${MAKE_ROOT}/bin/$arch/$bin $*
fi

if [ -x ${HOME}/bin/$arch/$bin ]
then
    exec ${HOME}/bin/$arch/$bin $*
fi

if [ -x /tmp/$arch/$bin ]
then
    exec /tmp/$arch/$bin $*
fi

dest=/tmp/$arch

if [ -d ${MAKE_ROOT}/bin ]
then
    if [ -w ${MAKE_ROOT}/bin ]
    then
	if [ ! -d ${MAKE_ROOT}/bin/$arch ]
	then
	    mkdir ${MAKE_ROOT}/bin/$arch
	fi
	if [ -w ${MAKE_ROOT}/bin/$arch ]
	then
	    dest=${MAKE_ROOT}/bin/$arch
	fi
    fi
fi

if [ ! -d $dest ]
then
    mkdir $dest
fi

case ${arch} in
	arch-IRIX*) CFLAGS="-n32 -mips3" ;;
	arch-HP-UX*) CFLAGS=-Aa ;;
esac

echo 1>&2 Note: Recompiling $bin into $dest/$bin
#
# Compile in arch-specific directory to avoid timing problems
# with temporary compiler internal object files
# due to simultaneous compilation of incompatible archtitectures
#
srcdir=`cd ${MAKE_ROOT}/make/; /bin/pwd`
(cd $dest; ${CC:-cc} ${CFLAGS} ${srcdir}/${bin}.c -o $bin)

exec $dest/$bin $*

