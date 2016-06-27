** Using QT .MOC files **

The QT MOC compiler is called if the variable MOCFILES is defined
and containes a list of .moc files without full path. It must be
set *before* inclusion of the GNUmakefile.rules file, for instance:

MOCFILES=MyMainWindow.moc MyObjectMenu.moc MyConnectWindow.moc

and in addition define the dependencies *after* including the
GNUmakefile.rules using ${VPATH} as prefix:

${VPATH}MyMainWindow.moc: MyMainWindow.cpp 
${VPATH}MyObjectMenu.moc: MyObjectMenu.cpp 
${VPATH}MyConnectWindow.moc: MyConnectWindow.cpp


** Using multiple versions of QT **

Set the variable Q to a version-specific value, e.g.

Q=qt4

and expand the name of all libraries and make targets with a
trailing Q, e.g.

TARGET=guilib$Q

and

LIBS=-lguilib$Q





