CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=ppd
SRC= src/$(wildcard *.cpp)
OBJ= src/$(SRC:.cpp=.o)

# tinyxml
TINYXML_OBJ= src/tinyxml/tinyxml.o src/tinyxml/tinyxmlparser.o src/tinyxml/tinyxmlerror.o

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG 
else
	CFLAGS += -O2
endif

# DUMBlib used?
USE_DUMB=no
ifeq ($(USE_DUMB), yes)
	CFLAGS += -DDUMB_MUSIC
	LDFLAGS += -ldumb
endif

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
endif

# disable splash screen
NO_SPLASH=yes
ifeq ($(NO_SPLASH), yes)
	CFLAGS += -DNO_SPLASH
endif


$(EXEC): $(OBJ) $(TINYXML_OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c $(CFLAGS)

src/tinyxml/tinyxml.o: src/tinyxml/tinyxml.cpp
	$(CC) $< -c $(CFLAGS)
	
src/tinyxml/tinyxmlparser.o: src/tinyxml/tinyxmlparser.cpp
	$(CC) $< -c $(CFLAGS)
	
src/tinyxml/tinyxmlerror.o: src/tinyxml/tinyxmlerror.cpp
	$(CC) $< -c $(CFLAGS)

.PHONY: clean cleanxml mrproper

clean:
	-rm src/*.o

cleanxml:
	-rm src/tinyxml/*.o
	
mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

