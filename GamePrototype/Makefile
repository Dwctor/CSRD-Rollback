GCC=g++
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET=game
C_FILES=main.cpp
HEADERS=

$(TARGET): $(C_FILES) $(HEADERS)
	$(GCC) -o $(TARGET) $(C_FILES) $(LIBS)
