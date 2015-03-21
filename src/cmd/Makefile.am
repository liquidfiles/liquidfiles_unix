# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -Wall -I $(CURDIR)/../

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = libcmd.a

libcmd_a_SOURCES = arguments.cpp \
				   command.cpp \
				   command_processor.cpp \
				   utility.cpp 
