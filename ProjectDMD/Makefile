## -------------------------- PROJECTDMD -------------------------- ##
include ../Ember/Makefile.BuildOptions

######################################################################
# VARIABLES
######################################################################

# Compile
CXXFLAGS = -Wall -O3 -g

# CPP Project Files
CPP_BASE = $(wildcard src/*.cpp)

# HDR Project Files
HDR_BASE = $(wildcard src/*.h)

# Composition
CPP = $(CPP_BASE)
HDR = $(HDR_BASE)
TMP := $(subst src/,obj/,$(CPP))
OBJ := $(subst .cpp,.o,$(TMP))

# CMD Util
MD = mkdir -p

# SFML
ifeq ($(VIRTUAL),1)
SFML_FLAGS+=-lsfml-graphics -lsfml-window -lsfml-system
endif

# EMBER
EMBER_LIBDIR = ../Ember
EMBER_LIBRARY = $(EMBER_LIBDIR)/libEmber.a
EMBER_DEP_FLAGS += -lrt -lm -lpthread $(SFML_FLAGS)
EMBER_INCLUDE_FLAGS = -I$(EMBER_LIBDIR)/src -I$(EMBER_LIBDIR)/src/Include
EMBER_FLAGS += -L$(EMBER_LIBDIR) -lEmber $(EMBER_DEP_FLAGS) $(EMBER_INCLUDE_FLAGS)

######################################################################
# TARGETS
######################################################################

# PRIMARY BUILD TARGET
ProjectDMD: clean init Ember $(OBJ) $(HDR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ProjectDMD $(EMBER_FLAGS)

######################################################################

Ember:
	cd $(EMBER_LIBDIR) && $(MAKE)

init:
	${MD} obj
	${MD} obj/src

clean:
	rm -f $(OBJ) ProjectDMD

run:
	sudo ./ProjectDMD

######################################################################
# PATTERNS
######################################################################

obj/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(EMBER_INCLUDE_FLAGS)

######################################################################
.PHONY: init, clean, run