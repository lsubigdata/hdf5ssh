#!/bin/sh

if [ $# -lt 1 ]
then
	echo Please give the name of you C++ compiler and options.
fi


#http://www.bby.com.au/~gnb/wp/cd2/index.html

echo > /tmp/for-conform.cc '
main()
{
int	i = 0;
int	a[10];

       for (int i = 0; i < 10; i++)
       		a[i] = i;

	return i;
}
'

rm /tmp/for-conform

$* /tmp/for-conform.cc -o /tmp/for-conform

rm /tmp/for-conform.cc

if /tmp/for-conform 
then
	echo The compiler is ANSI C++ conform.
else
	echo The compiler is NOT ANSI C++ conform.
fi

