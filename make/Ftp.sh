#!/bin/ksh

wget=$(which wget)

if [ -x $wget ]
then
	$wget $*
	exit $?
fi

if [ ! -r $HOME/.netrc ]
then
	printf 'No ~/.netrc file found ! Create ? [y]';
        read Q;
        if [ "$Q" != "" -a "$Q" != y ]
	then
        	ftp $*;
            	return $?;
        fi
        touch $HOME/.netrc;
        chmod 600 $HOME/.netrc;
fi

if [ $# -lt 1 ]
then
	ftp
	return $?;
fi

typeset FTPFILE;
FTPHOST=$1;
if [ $# -gt 1 ]
then
        FTPFILE=$2;
else
	case $1 in 
		ftp://*)
                	f=${FTPHOST#ftp://};
	                FTPFILE=/${f#*/};
        	        FTPHOST=${f%$FTPFILE};
                	unset f
		;;
		*:*)
                	FTPHOST=`IFS=:;	argument 1 $1`;
	                FTPFILE=`IFS=:;	argument 2 $1`
		;;
	esac;
fi

grep $FTPHOST $HOME/.netrc >/dev/null;

if [ $? = 1 ]
then
        printf "'%s' is not in your ~/.netrc file. Add it ? [y]" $FTPHOST;
        read Q;
        if [ "$Q" = "" -o "$Q" = y ]; then
            echo "machine $FTPHOST login ftp password $USER@$HOSTNAME.$DOMAINNAME" >>$HOME/.netrc;
        fi;
fi

if [ $FTPFILE ]
then
        DIR=$(dirname $FTPFILE);
        FILE=$(basename $FTPFILE);
        ftp $FTPHOST << --EOFTP--
cd $DIR
bin
get $FILE
bye
--EOFTP--
        return $?;
else
        ftp $*;
        return $?;
fi

