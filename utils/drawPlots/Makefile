ifndef ROOTSYS
all:
	@echo "ROOTSYS is not set. Please set ROOT environment properly"; echo
else

all: 	build
help:
	@echo "Available Targets:";\
	cat Makefile | perl -ne 'printf("\t%-15s %s\n",$$1,$$2) if(/^(\S+):[^#]+(#.*)$$/)'

ifndef VERBOSE
  QUIET := @
endif

#ROOFITINCLUDE = 
#ifdef CMSSW_VERSION
#	ROOFITINCLUDE = $(shell scramv1 tool info roofitcore | grep INCLUDE | sed 's/^INCLUDE=/-I/')
#endif

CC = g++
CMSROOT = ./
INCLUDE = $(shell root-config --cflags) -I$(CMSROOT)
CFLAGS = -Wall -Wno-unused-function -g -O2 -fPIC $(INCLUDE) $(EXTRACFLAGS)
ROOTLIBS = $(shell root-config --ldflags --cflags --libs) -lTMVA #-lEG -lGenVector

LINKER = g++
LINKERFLAGS = $(shell root-config --ldflags --libs) -lEG -lGenVector -lTMVA
# ifeq ($(shell root-config --platform),macosx)
# 	LINKERFLAGS = -dynamiclib -undefined dynamic_lookup -Wl,-x -O -Xlinker -bind_at_load -flat_namespace $(shell root-config --libs) -lEG -lGenVector
# endif

DIR = ./

SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
LIB = libmakeReweightHist_noconf.so

LIBS = $(LIB)

EXE = drawPlots

libs:	$(LIBS)

$(LIB): $(OBJECTS)
	$(QUIET) echo "Linking $@"; \
	echo $(SOURCES);
	echo "$(LINKER) -shared -o $@ $(OBJECTS) $(LINKERFLAGS)"; \
	$(LINKER) -shared -o $@ $(OBJECTS)  $(LINKERFLAGS)

$(EXE):  $(LIBS)
	$(QUIET) echo "Building $@"; \
	echo "$(CC) -o $@ $(LIBS) $(ROOTLIBS) -Wl,-rpath,./"; \
	$(CC) -o $@ $(LIBS) $(ROOTLIBS) -Wl,-rpath,./ 


%.o : %.cc
	$(QUIET) echo "Compiling $<"; \
		$(CC) $(CFLAGS) $< -c -o $@
libs:	$(LIBS)

build:  $(EXE)

b: build

clean:
	rm -f \
	LinkDef_out* \
	*.o \
	*.so \
	drawPlots



endif
