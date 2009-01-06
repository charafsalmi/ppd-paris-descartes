CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=ppd
#SRC= $(wildcard *.cpp)
#OBJ= $(SRC:.cpp=.o)

SRC =  Animated.cpp Animation.cpp Enemy.cpp Entity.cpp Game.cpp Main.cpp MediaManager.cpp 
SRC += Player.cpp StaticItem.cpp Tile.cpp TileManager.cpp Zone.cpp ControlPanel.cpp
	
OBJ = Animated.o Animation.o Enemy.o Entity.o Game.o Main.o MediaManager.o
OBJ +=	Player.o StaticItem.o Tile.o TileManager.o Zone.o ControlPanel.o 

# tinyxml
TINYXML_OBJ= tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# DUMBlib used?
ifeq ($(DUMB), yes)
	CFLAGS  += -DDUMB_MUSIC
	LDFLAGS += -ldumb
	SRC += Music.cpp
	OBJ += Music.o
endif

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
endif


$(EXEC): $(OBJ) $(TINYXML_OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

tinyxml/tinyxml.o: tinyxml/tinyxml.cpp
	$(CC) $< -c -o $@ $(CFLAGS)
	
tinyxml/tinyxmlparser.o: tinyxml/tinyxmlparser.cpp
	$(CC) $< -c -o $@ $(CFLAGS)
	
tinyxml/tinyxmlerror.o: tinyxml/tinyxmlerror.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

.PHONY: clean mrproper

clean:
	-rm *.o
	
mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

