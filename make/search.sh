#!/bin/sh

export CFLAGS 
export LDFLAGS

#################################

inc_dir()
{
	if [ -d $1 ]
	then
		INCLUDE=$1
		return 0;
	fi
	return 1;
}

lib_dir()
{
	if [ -d $1 ]
	then
		LIBS=$1
		return 0;
	fi
	return 1;
}

#################################

inc_file()
{
	if [ ! -z "$verbose" ]
	then
		echo "Searching for $FILE in $1..." 1>&2
	fi

	if [ -f $1/$FILE ]
	then
		INCLUDE=$1
		return 0;
	fi
	return 1;
}

lib_file()
{
	if [ -f $1/lib$LIB.so ]
	then
		LIBS=$1
		if [ ! -z "$2" ]
		then
			if [ -d $2 ]
			then
				ln -fs $1/lib$LIB.so $2 
				echo "Found and linked lib$LIB.so from $1!" 1>&2
				LIBS=/lib
				return 0;
			fi
		fi
		echo "Found shared library in $1!" 1>&2
		return 0;
	fi
	if [ -f $1/lib$LIB.a ]
	then
		LIBS=$1
		echo "Found static library in $1!" 1>&2
		return 0;
	fi
	return 1;
}

check_vars()
{
    CFLAGS=''
    LDFLAGS=''
    if [ -z "$INCLUDE" ]
    then
       if [ -z "$LIBS" ]
       then
	  # No packages and no libraries found.
          echo Could not find package $1. 1>&2
	  return 0;
       fi
    fi
    echo $1 includes in $INCLUDE, $1 Libs in $LIBS 1>&2

    # If INCLUDE points to non-standard path (!=/usr/include)
    if [ ! -z "$INCLUDE" ]
    then
       if [ "$INCLUDE" != "/usr/include" ]
       then
	 CFLAGS=-I$INCLUDE
       fi
    fi   

    # If LIBS points to non-standard path (!=/lib or /usr/lib)
    if [ ! -z "$LIBS" ]
    then
	LDFLAGS=-l$LIB
       if [ "$LIBS" != "/lib" ]
       then
	if [ "$LIBS" != "/usr/lib" ]
	then
	  if [ "$LIBS" != "/usr/lib32" ] # IRIX hack workaround
	  then
	     LDFLAGS="-L$LIBS -l$LIB"
	 fi
	fi
       fi
    fi   
    return 1;
}


