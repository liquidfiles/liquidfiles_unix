# what flags you want to pass to the C compiler & linker
SUBDIRS = ui lf cmd io base

AM_CPPFLAGS = -Wall -I . -std=c++11

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = liquidfiles
liquidfiles_SOURCES = main.cpp

liquidfiles_LDADD = ui/libui.a lf/liblf.a cmd/libcmd.a io/libio.a base/libbase.a
