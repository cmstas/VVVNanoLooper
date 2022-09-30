# Simple makefile

EXE=doVVVAnalysis

SOURCES=$(wildcard src/*.cc) $(wildcard NanoTools/NanoCORE/*.cc) $(wildcard NanoTools/NanoCORE/Tools/*.cc) $(wildcard NanoTools/NanoCORE/Tools/btagsf/*.cc)
OBJECTS=$(SOURCES:.cc=.o)
HEADERS=$(SOURCES:.cc=.h)

CC          = g++
CXX         = g++
CXXFLAGS    = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LD          = g++
LDFLAGS     = -g -O2
SOFLAGS     = -g -shared
CXXFLAGS    = -g -O2 -Wall -fPIC -Wshadow -Woverloaded-virtual
LDFLAGS     = -g -O2
ROOTLIBS    = $(shell root-config --libs)
ROOTCFLAGS  = $(shell root-config --cflags) -INanoTools/NanoCORE/ -DLorentzVectorPtEtaPhiM4D
CXXFLAGS   += $(ROOTCFLAGS)
CFLAGS      = $(ROOTCFLAGS) -Wall -Wno-unused-function -g -O2 -fPIC -fno-var-tracking
EXTRACFLAGS = $(shell rooutil-config)
EXTRAFLAGS  = -fPIC -ITMultiDrawTreePlayer -Wunused-variable -lTMVA -lEG -lGenVector -lXMLIO -lMLP -lTreePlayer -lMinuit

all: rooutil $(EXE)

$(EXE): $(OBJECTS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(ROOTLIBS) $(EXTRAFLAGS) $(EXTRACFLAGS) -o $@

%.o: %.cc
	$(CC) $(CFLAGS) $(EXTRACFLAGS) $< -c -o $@

cleansmall:
	rm -f src/*.o $(EXE)

clean:
	$(MAKE) -C rooutil/ clean
	rm -f $(OBJECTS) $(EXE)

rooutil:
	$(MAKE) -C rooutil/

.PHONY: rooutil
