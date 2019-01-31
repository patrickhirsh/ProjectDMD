CXXFLAGS=-Wall -O3 -g
OBJECTS= ProjectDMD.o PanelManager.o PanelSource.o

RGB_LIB_DISTRIBUTION=lib/rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: ProjectDMD

ProjectDMD: ProjectDMD.o $(RGB_LIBRARY) PanelManager
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

PanelManager: src/PanelManager.cpp include/PanelManager.h PanelSource
	$(CXX) -c $(CXXFLAGS) src/PanelManager.cpp

PanelSource: src/PanelSource.cpp include/PanelSource.h include/libraries.h
	$(CXX) -c $(CXXFLAGS) src/PanelSource.cpp

clean:
	rm -f ProjectDMD.o PanelManager.o PanelSource.o ProjectDMD