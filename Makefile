CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=ppd
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

# tinyxml
TINYXML_OBJ= tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG 
#	CFLAGS += -DNO_SPLASH
else
	CFLAGS += -O2
endif

# DUMBlib used?
USE_DUMB=yes
ifeq ($(USE_DUMB), yes)
	CFLAGS += -DDUMB_MUSIC
	LDFLAGS += -ldumb
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

.PHONY: clean cleanxml mrproper

clean:
	-rm *.o

cleanxml:
	-rm tinyxml/*.o
	
mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

