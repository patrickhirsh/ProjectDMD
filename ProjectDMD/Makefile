######################################################################
# VARIABLES
######################################################################
VIRTUAL=0

# Build Options
BUILD_OPTIONS+=-DVIRTUAL=$(VIRTUAL)

# Compile
CXXFLAGS = -Wall -O3 -g

# Project Files
SRC_BASE = $(wildcard src/*.cpp)
SRC_RESOURCE = $(wildcard src/resource/*.cpp)
SRC_SOURCES = $(wildcard src/source/*.cpp)
SRC_UTILITY = $(wildcard src/utility/*.cpp)
SRC_OBJECT = $(wildcard src/object/*.cpp)
SRC_RENDER = $(wildcard src/render/*.cpp)
SRC_MODE = $(wildcard src/mode/*.cpp)
HDR = $(wildcard include/*.h) SYSTEMGLOBALS.h
SRC = $(SRC_BASE) $(SRC_SOURCES) $(SRC_UTILITY) $(SRC_RESOURCE) $(SRC_OBJECT) $(SRC_RENDER) $(SRC_MODE)
TMP := $(subst src/,obj/,$(SRC))
OBJ := $(subst .cpp,.o,$(TMP))

# Directory Management
MD = mkdir -p
CD = cd

# RGB-MATRIX LIB
RGB_LIB_DISTRIBUTION=lib/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# SFML LIB
ifeq ($(VIRTUAL),1)
SFML_FLAGS+=-lsfml-graphics -lsfml-window -lsfml-system
endif

######################################################################
# TARGETS
######################################################################

# PRIMARY BUILD TARGET
ProjectDMD: clean init $(OBJ) $(HDR) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ProjectDMD $(LDFLAGS) $(SFML_FLAGS)

######################################################################

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

install:
	sudo apt-get install libsfml-dev

init:
	${MD} obj
	${MD} obj/resource
	${MD} obj/source
	${MD} obj/utility
	${MD} obj/object
	${MD} obj/render
	${MD} obj/mode

clean:
	rm -f $(OBJ) ProjectDMD

run:
	sudo ./ProjectDMD

######################################################################
# PATTERNS
######################################################################

obj/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $(BUILD_OPTIONS) $< -o $@

######################################################################
.PHONY: install, init, clean, run