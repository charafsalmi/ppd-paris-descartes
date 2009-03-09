CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -framework sfml-graphics -framework sfml-window -framework sfml-system -framework sfml-audio
EXEC=bin/ppd
SRC= $(wildcard src/*/*.cpp)
OBJ= $(SRC:.cpp=.o)

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# test gui_system
WINDOW_TEST=no
ifeq ($(WINDOW_TEST), yes)
	CFLAGS += -DWINDOW_TEST
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
NO_SPLASH=no
ifeq ($(NO_SPLASH), yes)
	CFLAGS += -DNO_SPLASH
endif


$(EXEC): $(OBJ) 
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

.PHONY: clean cleanxml mrproper

clean:
	-rm src/core/*.o
	-rm src/entities/*.o
	-rm src/gui/*.o
	-rm src/gui_system/*.o
	-rm src/misc/*.o
	-rm src/net/*.o

cleanxml:
	-rm src/xml/*.o
	
mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

