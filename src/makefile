SRCS = command.cpp command_processor.cpp main.cpp messenger.cpp send_command.cpp utility.cpp

TOP = ..

CPLUSPLUS = g++
LD		  = g++
RM		  = rm -f
MV		  = mv
MKDIR     = mkdir -p

INCPATH		= -I
#CXXFLAGS_X	= -mfpmath=sse -msse3
CXXFLAGS	= -O2 #-fPIC -std=c++0x -msse3
LIB_PATH	= -L
LIBS	    = -l

TARGET_BIN = $(BINDIR)/liquidfiles

#########################################
.SUFFIXES: .c .cpp .mk .o .h
#########################################
OBJDIR := $(TOP)/obj
BINDIR := $(TOP)/bin
LIBDIR := $(TOP)/lib

src-cpp:=$(filter %.cpp,$(SRCS))
SRCDEPENDS:= $(src-cpp:%.cpp=$(OBJDIR)/%.mk)
CXXLIBOBJS = $(src-cpp:%.cpp=$(OBJDIR)/%.o)

.PRECIOUS: %/.dir
%/.dir:
	@$(MKDIR) $(@D)
	@touch $@

$(OBJDIR)/%.mk : %.cpp $(OBJDIR)/.dir
	$(CPLUSPLUS) $(INCPATH) $(CXXFLAGS) -MM -MT'$(OBJDIR)/$*.o' -MT'$(OBJDIR)/$*.lo' $< > $@

#########################################
#  compilation rules
#########################################
$(TARGET_BIN): $(BINDIR)/.dir $(OBJDIR)/.objs
	TMP_EXEC=`mktemp /tmp/lf_XXXXXX`; \
chmod ugo+r $$TMP_EXEC; \
$(LD) $(LIB_PATH) $(LIBS) $$(cat $(OBJDIR)/.objs) -o $$TMP_EXEC; \
$(MV) -f $$TMP_EXEC $@

$(OBJDIR)/%.o : %.cpp $(OBJDIR)/.dir
	cd $(@D) ; $(CPLUSPLUS) $(INCPATH) -I$(CURDIR) $(CXXFLAGS) -o $(@F) -c $(CURDIR)/$<

$(OBJDIR)/.objs: $(CXXLIBOBJS) $(OBJDIR)/.dir
	@echo "$(CXXLIBOBJS)" > $(OBJDIR)/.objs

clean:
	$(RM) -r $(TOP)/obj/ $(TOP)/bin/

-include $(SRCDEPENDS)
