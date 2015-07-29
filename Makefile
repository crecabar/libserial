
# Environment 
MKDIR=mkdir
CP=cp
CC=clang
CXX=clang++
CFLAGS=-I.
CXXFLAGS=-I.

SRC_DIR=.
SOURCES=$(SRC_DIR)/Serial.cpp \
	$(SRC_DIR)/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)


all: main

main: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o main

clean:
	rm -rf main
	rm $(OBJECTS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $< -o $@


# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
