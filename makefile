CC=g++
CPPFLAGS=-std=c++11 -Iinclude -Ilib/tinyxml/include/
LDFLAGS=-Llib/tinyxml/lib/
LDLIBS=-lrdkafka -lrdkafka++ -lz -lpthread -lrt -ldl -lssl -lcrypto -ltinyxml

EXE = hello

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONEY: all clean

all : $(EXE)

hello: 
	echo $(SRC)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

