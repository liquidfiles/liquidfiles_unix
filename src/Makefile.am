# what flags you want to pass to the C compiler & linker
CPPFLAGS = -Wall
LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = liquidfiles
liquidfiles_SOURCES = command.cpp command_processor.cpp engine.cpp help_command.cpp main.cpp messenger.cpp send_command.cpp utility.cpp command.h command_processor.h engine.h help_command.h messenger.h send_command.h utility.h
liquidfiles_LDADD = -lcurl
