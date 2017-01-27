#
# makefile for g++
#
# by exa
#

SHELL = /bin/sh

.SUFFIXES:

PROJECT = superquadrix

srcdir = .
curdir := $(shell pwd)
parentdir := $(shell cd ..; pwd)
dirname := $(subst $(parentdir)/,,$(curdir))

# Project files

EXEC = superquadrics	# Executable

# Object Files

OBJS = Application.o SQ_Window.o SQ_Display.o SQ_GL.o

# Source Files

CXXFILES = Application.cxx SQ_Window.cxx SQ_Display.cxx SQ_GL.cxx
CCFILES = 
CFILES =
AFILES =
INCFILES = General.hxx ANSILibrary.hxx SQ_Window.hxx SQ_Display.hxx SQ_GL.hxx
GRAMMARFILES =
LEXFILES =

# Documentation

TEXFILES =
DVIFILES = $(addsuffix .dvi, $(basename $(TEXFILES)))
DOCFILES = $(DVIFILES)


IMPLEMENTATION = $(CXXFILES) $(CFILES) $(AFILES) $(GRAMMARFILES)
INTERFACE = $(INCFILES)
SRC = $(IMPLEMENTATION) $(INTERFACE)
VERSUFFIX:=,v
#VER = $(addsuffix $(VERSUFFIX), $(SRC)) # BUG: gives rise to wrong dep.

AUX = Makefile
ARCHIVE = $(PROJECT).tgz
ARCPATH = $HOME/arc

# System Resources

LIBS = $(shell gtkmm-config --libs) -lgtkglmm -lMesaGL -lMesaGLU
INCLUDEDIR =
LIBDIR =

# Options

GENERAL_DEBUG = -g
GENERAL_OPTIMISE =
GENERAL_WARN = -Wcomment -Wimplicit -Wmain -Wswitch
CPPFLAGS =
CXXOPTIMISE =
CXXDEBUG =
CXXWARN = -ftemplate-depth-64 #-Wtemplate-debugging -Weffc++
CXXOTHER = -foperator-names $(shell gtkmm-config --cflags)
CXXFLAGS = $(GENERAL_DEBUG) $(GENERAL_OPTIMISE) $(GENERAL_WARN)\
	   $(CXXDEBUG) $(CXXOPTIMISE) $(CXXWARN) $(CXXOTHER)
COPTIMISE =
CDEBUG =
CWARN =
COTHER = 
CFLAGS = $(GENERAL_DEBUG) $(GENERAL_OPTIMISE) $(GENERAL_WARN)\
	 $(CDEBUG) $(COPTIMISE) $(CWARN) $(COTHER)
ASFLAGS =
LDFLAGS = $(GENERAL_DEBUG)

# Programs used

MAKE =	make # For subsequent make's
CC =	gcc  # C compiler
CXX =	g++  # C++ compiler
ASM =	as   # Assembler
LD = 	g++  # Linker
BISON =	bison --defines # lexical analyzer
FLEX =	flex            # syntax analyzer
LS =	ls -l
CP =    cp --interactive
ED =	emacs
LATEX = latex
BIBTEX = bibtex
PAGER = most

# Tool Options

PROG = -o $(EXEC) # Program to build

# Rules

# Pattern Rules

# Automatic dependency rules

# C/C++

%.d: %.cxx
	@echo Generating dependencies for $*.cxx
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                 | sed '\''s/$*\\.o[ :]*/& $@/g'\'' > $@'
%.d: %.cc
	@echo Generating dependencies for $*.cc
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                 | sed '\''s/$*\\.o[ :]*/& $@/g'\'' > $@'
%.d: %.c
	@echo Generating dependencies for $*.c
	@$(SHELL) -ec '$(CXX) -MM $(CPPFLAGS) $< \
                 | sed '\''s/$*\\.o[ :]*/& $@/g'\'' > $@'

# Compilation rules

# C/C++

%.o : %.cxx
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
%.o : %.cc
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@
%.o : %.c
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

# Flex - Bison rules

%.tab.c %.tab.h : %.grammar %.hxx
	$(BISON) $< --output-file=$*.tab.c

%.flex.c : %.flex %.tab.h %.hxx
	$(FLEX) -o$@ $<

# Other compilers

%.dvi : %.tex
	@echo Compiling latex to dvi $<
	@$(LATEX) $<

# Phony Targets

.PHONY: all compileinfo clean realclean list listsrc \
	arc listarc extractarc rmarc backup restore edit

# Default rule

all: defaultmesg compileinfo $(EXEC) #$(DOC)

defaultmesg:
	@echo Default rule invoked, building everything.

compileinfo:
	@echo Project name: $(PROJECT) 
	@echo C preprocessor flags: $(CPPFLAGS)
	@echo C flags: $(CFLAGS)
	@echo C++ flags: $(CXXFLAGS)

$(EXEC): $(OBJS)
	@echo Building executable: $(EXEC)
	@echo LD flags: $(LDFLAGS)
	@echo Libraries used: $(LIBS)
	@$(LD) $(LDFLAGS) $(LIBDIR) $(INCLUDEDIR) $(OBJS) $(LIBS) $(PROG)


# Include automatic dependencies
#ifdef $(CXXFILES)
include $(CXXFILES:.cxx=.d)
#endif

ifdef $(CCFILES)
include $(CCFILES:.cc=.d)
endif

ifdef $(CFILES)
include $(CFILES:.c=.d)
endif

clean:
	@echo Deleting object modules and executables
	@-rm $(OBJS) $(EXEC)

distclean: clean
	@echo Deleting archive
	@-rm $(ARCHIVE)

realclean: distclean
	@echo Deleting miscallenous files
	@-rm *.bak *~

list:
	$(LS) $(SRC) $(AUX)

listsrc:
	$(LS) $(SRC)

# Archive

arc:	$(ARCHIVE)

listarc:
	@echo Listing content of tar archive
	@tar tzvf $(ARCHIVE) | $(PAGER)

$(ARCHIVE): $(SRC) $(VER) $(AUX)
	@cd .. ;\
	echo Creating tar archive ;\
	tar czvf $(dirname)/$(ARCHIVE) $(addprefix $(dirname)/,$?)

extractzip:
	@cd .. ;\
	echo Extracting from tar archive ;\
	tar xzvf $(dirname)/$(ARCHIVE)

updatearc:
	@cd .. ;\
	echo Updating tar archive ;\
	tar uzvf $(dirname)/$(ARCHIVE) $(addprefix $(dirname)/,$?)

rmarc:	$(ARCHIVE)
	@-rm -f $(ARCHIVE)

backup:
	@echo Backing up archive
	@$(CP) $(ARCHIVE) $(ARCPATH)

restore:
	@echo Restoring archive
	@$(CP) $(ARCPATH)/$(ARCHIVE) .

edit:
	@Invoking editor for all source files
	@$(ED) $(SRC)&
 
