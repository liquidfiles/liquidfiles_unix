# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -Wall -I $(CURDIR)/../

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
noinst_LIBRARIES = liblf.a

liblf_a_SOURCES = attachment_responce.cpp \
				  engine.cpp \
				  filelinks_responce.cpp \
				  messages_responce.cpp \
				  message_responce.cpp
