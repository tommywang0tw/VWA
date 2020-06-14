LIB_DIR=-L/usr/local/systemc-2.3.3/lib-linux64 -Wl,-rpath=/usr/local/systemc-2.3.3/lib-linux64

INC_DIR=-I/usr/local/systemc-2.3.3/include

LIB=-lsystemc -lm

EXE=RESULT

APP_SRC= main.cpp DMA.cpp DRAM.cpp mm.cpp weightSRAM.cpp inputSRAM.cpp

all:
	g++ -Wall -g -o $(EXE) $(APP_SRC) $(LIB_DIR) $(INC_DIR) $(LIB)
	./$(EXE)

clean:
	rm -rf $(APP_SRC:%.cpp=%.o) $(EXE) *.vcd *.fsdb *Log novas*
