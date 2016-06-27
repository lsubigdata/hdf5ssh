MAKE_ROOT=..


include $(MAKE_ROOT)/make/autoselect.cfg
include $(MAKE_ROOT)/make/make.cfg

all: arch-info specific

include $(MAKE_ROOT)/make/info.rules

specific::
	@echo
	@echo Host-specific configuration file is $(MAKE_ROOT)/make/hosts/arch-$(MAKE_UNAME)-$(MAKE_CFG)-$(MAKE_HOSTNAME).cfg
	@echo
	@echo 'Do "make edit-specific" to edit the host-specific configuration file.'
	@echo



edit-specific:
	vi  $(MAKE_ROOT)/make/hosts/arch-$(MAKE_UNAME)-$(MAKE_CFG)-$(MAKE_HOSTNAME).cfg

