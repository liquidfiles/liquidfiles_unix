# what flags you want to pass to the C compiler & linker
SUBDIRS = io

AM_CPPFLAGS = -Wall

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = liquidfiles
liquidfiles_SOURCES = arguments.cpp \
					  attachment_responce.cpp \
					  command.cpp \
					  command_processor.cpp \
					  credentials.cpp \
					  download_command.cpp \
					  engine.cpp \
					  filelink_command.cpp \
					  file_request_command.cpp \
					  get_api_key_command.cpp \
					  help_command.cpp \
					  main.cpp \
					  messages_responce.cpp \
					  message_responce.cpp \
					  messages_command.cpp \
					  send_command.cpp \
					  utility.cpp 

liquidfiles_LDADD = io/libio.a
