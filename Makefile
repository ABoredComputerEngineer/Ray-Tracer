$(shell mkdir -p bin)

INC = ./include
LIBS = ./libs
SRC = ./src
CC = g++
DBFLAGS = -g3 -gdwarf-2 -msse -msse2 -msse3 -Wall
OFLAGS = -O3 -msse -msse2 -msse3 -Wall
LIBFLAGS = -lglfw3 -lGL -lX11 -ldl -lpthread -lXi -lXrandr -lm -lSDL2
BIN = ./bin
HEADER_FILES = $(SRC)/*.h
OBJS = main.o common.o
FULL_OBJS = $(addprefix $(BIN)/, $(OBJS) )
 
app: $(FULL_OBJS)
	$(CC) $(FULL_OBJS) $(OFLAGS) -L $(LIBS) $(LIBFLAGS) -o $(BIN)/app

debug: $(FULL_OBJS) 
	$(CC) $(FULL_OBJS) $(DBFLAGS) -L $(LIBS) $(LIBFLAGS) -o $(BIN)/app

.PHONY: run display

run: app 
	./bin/app
	feh ./images/out.png
disp:
	feh ./images/out.png
test: ./src/test.cpp
	$(CC) $< $(DBFLAGS) -I $(INC) -o $@

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $< $(OFLAGS) -I $(INC) -c -o $@

clean:
	rm -rf ./bin/*
