CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=ppd
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
endif


$(EXEC): $(OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

.PHONY: clean mrproper

clean:
	-rm *.o

mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

