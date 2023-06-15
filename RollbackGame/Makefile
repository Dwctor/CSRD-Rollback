GCC=g++
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

GAMEEXEC=game
SERVEREXEC=server

ALL=$(GAMEEXEC) $(SERVEREXEC)

PLAYER_C_FILES=Player.cpp Game.cpp Common.cpp
PLAYER_HEADERS=Game.h Common.h

SERVER_C_FILES= Server.cpp Backend.cpp Common.cpp
SERVER_HEADERS= Backend.h Common.h

NETWORK_C_FILES= ../NetworkLib/network.cpp message.cpp

all: $(ALL)

$(GAMEEXEC): $(PLAYER_C_FILES) $(PLAYER_HEADERS)
	$(GCC) -o $(GAMEEXEC) $(PLAYER_C_FILES) $(NETWORK_C_FILES) $(LIBS) 

$(SERVEREXEC): $(SERVER_C_FILES) $(SERVER_HEADERS)
	$(GCC) -o $(SERVEREXEC) $(SERVER_C_FILES) $(NETWORK_C_FILES) $(LIBS)


clean:
	rm $(GAMEEXEC) $(SERVEREXEC)
