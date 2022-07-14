CC=g++
CPPFLAGS=-std=c++11 -ggdb -Iinclude -Ilib/tinyxml/include/
LDFLAGS=-Llib/tinyxml/lib/
LDLIBS=-lrdkafka -lrdkafka++ -lz -lpthread -lrt -ldl -lssl -lcrypto -ltinyxml

EXE = program

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONEY: all clean

all : $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

