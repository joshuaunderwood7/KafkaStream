CC=g++
CPPFLAGS=-std=c++11 -Iinclude -Ilib/libconfig/include/
LDFLAGS=-Llib/libconfig/lib/
LDLIBS=-lrdkafka -lrdkafka++ -lz -lpthread -lrt -ldl -lssl -lcrypto -lconfig++
MAKEFLAGS := --jobs=$(shell nproc)

EXE = program

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONEY: all clean
all : $(EXE)

opt: CPPFLAGS += -O2 -s
opt: all

debug: CPPFLAGS += -ggdb
debug: all

profile: CPPFLAGS += -pg
profile: all


$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

