$(shell mkdir -p bin)

INC = ./include
LIBS = ./libs
SRC = ./src
CC = g++
DBFLAGS = -g3 -gdwarf-2 -msse -msse2 -msse3 -Wall
OFLAGS = -O3 -msse -msse2 -msse3 -Wall
LIBFLAGS = -lglfw3 -lGL -lX11 -ldl -lpthread -lXi -lXrandr -lm
BIN = ./bin
HEADER_FILES = $(SRC)/*.h
OBJS = main.o common.o
UI_OBJS = ui.o common.o glad.o

FULL_OBJS = $(addprefix $(BIN)/, $(OBJS) )
FULL_UI_OBJS = $(addprefix $(BIN)/, $(UI_OBJS) )

.PHONY: run display

ui: $(FULL_UI_OBJS)
	$(CC) $(FULL_UI_OBJS) $(DBFLAGS) -L $(LIBS) $(LIBFLAGS) -o $(BIN)/ui

ray: $(FULL_OBJS)
	$(CC) $(FULL_OBJS) $(OFLAGS) -L $(LIBS) $(LIBFLAGS) -o $(BIN)/app

debug: $(FULL_OBJS) 
	$(CC) $(FULL_OBJS) $(DBFLAGS) -L $(LIBS) $(LIBFLAGS) -o $(BIN)/app
run: app 
	./bin/app
	feh ./images/out.png
disp:
	feh ./images/out.png
test: ./src/test.cpp
	$(CC) $< $(DBFLAGS) -I $(INC) -o $@

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $< $(DBFLAGS) -I $(INC) -c -o $@

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $< $(DBFLAGS) -I $(INC) -c -o $@
clean:
	rm -rf ./bin/*
