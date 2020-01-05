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

# EMBER
EMBER_DIR = ../Ember
EMBER_LIB_NAME = Ember
EMBER = lib$(EMBER_PATH).a
EMBER_FLAGS += -L$(EMBER_DIR) -l$(EMBER_LIB_NAME)

######################################################################
# TARGETS
######################################################################

# PRIMARY BUILD TARGET
ProjectDMD: clean init $(OBJ) $(HDR) $(EMBER)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ProjectDMD $(EMBER_FLAGS)

######################################################################

$(EMBER):
	$(MAKE) -C $(EMBER_DIR)

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
	$(CXX) -c $(CXXFLAGS) $< -o $@

######################################################################
.PHONY: init, clean, run