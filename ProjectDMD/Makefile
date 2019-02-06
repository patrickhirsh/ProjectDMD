CXXFLAGS=-Wall -O3 -g
OBJECTS= ProjectDMD.o PanelManager.o Clock.o

RGB_LIB_DISTRIBUTION=lib/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: $(RGB_LIBRARY) ProjectDMD
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o ProjectDMD $(LDFLAGS)

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

ProjectDMD: src/ProjectDMD.cpp PanelManager
	$(CXX) -c $(CXXFLAGS) src/ProjectDMD.cpp

PanelManager: src/PanelManager.cpp include/PanelManager.h Clock
	$(CXX) -c $(CXXFLAGS) src/PanelManager.cpp

Clock: src/sources/Clock.cpp include/PanelSource.h include/libraries.h
	$(CXX) -c $(CXXFLAGS) src/sources/Clock.cpp

clean:
	rm -f $(OBJECTS) ProjectDMD