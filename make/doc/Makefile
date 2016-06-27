
make.tar.gz: *.html ../GNUmakefile GNUmakefile ../*.cfg ../*.rules
	-$(RM) make.tar make.tar.gz
	(cd ../..; tar cvf make/doc/make.tar make/)
	gzip -9 make.tar

