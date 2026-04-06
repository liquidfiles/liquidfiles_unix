# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -Wall -I ../ -std=c++11

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = liblf.a

liblf_a_SOURCES = attachment_response.cpp \
				  engine.cpp \
				  filelinks_response.cpp \
				  messages_response.cpp \
				  message_response.cpp
