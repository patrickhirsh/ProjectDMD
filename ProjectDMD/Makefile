######################################################################
# VARIABLES
######################################################################

# Compile
CXXFLAGS = -Wall -O3 -g

# Project Files
HDR = $(wildcard include/*.h)
SRC_BASE = $(wildcard src/*.cpp)
SRC_RESOURCE = $(wildcard src/resource/*.cpp)
SRC_SOURCES = $(wildcard src/source/*.cpp)
SRC_UTILITY = $(wildcard src/utility/*.cpp)
SRC_OBJECT = $(wildcard src/object/*.cpp)
SRC = $(SRC_BASE) $(SRC_SOURCES) $(SRC_UTILITY) $(SRC_RESOURCE) $(SRC_OBJECT)
TMP := $(subst src/,obj/,$(SRC))
OBJ := $(subst .cpp,.o,$(TMP))

# LIB FILES
RGB_LIB_DISTRIBUTION=lib/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread


######################################################################
# TARGETS
######################################################################

all: $(OBJ) $(HDR) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ProjectDMD $(LDFLAGS)

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

clean:
	rm -f $(OBJ) ProjectDMD


######################################################################
# PATTERNS
######################################################################

obj/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@


######################################################################
.PHONY: clean