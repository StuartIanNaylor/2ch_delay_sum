TARGET = ds
SRC = apply-delay-and-sum.cc pa.cpp ds.h tdoa.h utils.h fft.h parse-option.h

INCLUDE = -I ../libraries/portaudio/include 
LIBS = -lportaudio

# on mac/linux: libs are usually installed in /usr/lib which is likely in PATH
LIBS += -L ../libraries/portaudio/build 

CXX = g++
FLAGS = -std=c++11 -march=native -ffast-math

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(INCLUDE) $(FLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)

run:
	@./$(TARGET)
