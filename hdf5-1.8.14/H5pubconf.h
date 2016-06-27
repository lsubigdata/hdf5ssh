#ifdef	_WIN32
#ifdef	__GNUC__
#include "H5pubconf-mingw.h"
#endif
#if (_MSC_VER >= 1900)
#include "H5pubconf-msvc2015.h"
#endif
#endif

#ifdef	__linux__
#include "H5pubconf-linux64.h"
#endif

#if defined(__APPLE__)
#include "H5pubconf-macos.h"
#endif


#ifdef _WIN64
#if (_MSC_VER == 1600)	
#include "H5pubconf-msvc2010.h"
#endif
#endif


#ifndef	H5_DEFAULT_PLUGINDIR

#if defined(_WIN32) || defined(_WIN64)
/* Define the default plugins path to compile */
#define H5_DEFAULT_PLUGINDIR "%USERPROFILE%/hdf5/lib/plugins:%ALLUSERSPROFILE%/hdf5/lib/plugins;%ALLUSERSPROFILE%/hdf5/lib/plugin"
#else
#define	H5_DEFAULT_PLUGINDIR	"/usr/local/hdf5/lib/plugin"
#endif

#endif

