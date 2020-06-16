SYSTEMC_DIR = /usr/local/systemc-2.3.3
PROJECT     = RESULT
BUILDFLAGS  = -g3
CXX         = g++

INCFLAGS    = -I. -I${SYSTEMC_DIR}/include
LDFLAGS     = -L${SYSTEMC_DIR}/lib-linux64 -lsystemc -lm
SRC_CPPHEAD1= Controller InputSRAM WeightSRAM PE 
SRC_CPPHEAD2= Accumulator_First  TreeAdder Accumulator_Third BoundarySRAM
SRC_CPP     =
HEADERS     =
MAIN        = main.cpp
OBJECTS     = $(SRC_CPPHEAD1:=.o) $(SRC_CPPHEAD2:=.o) $(SRC_CPPHEAD3:=.o) $(SRC_CPPHEAD4:=.o) $(SRC_CPP:cpp=o)

EXE = $(PROJECT)

all: $(EXE)

$(EXE): $(MAIN) $(OBJECTS) $(HEADERS)
	@echo "$@ building..."
	$(CXX) $(INCFLAGS) $(MAIN) $(OBJECTS) $(LDFLAGS) -o $@
	@echo ""
	@echo "$@ build done successfully..."
	@echo ""
%.o:%.cpp %.h
	@echo "Compiling $< ..."
	$(CXX) -c $< $(INCFLAGS)

clean:
	rm -f $(EXE)
	rm -f *.o
