CXX    = g++
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Weffc++ -Wconversion -Wsign-conversion -std=c++17

BUILD_DIR  = ../bin
BUILD_EXE  = chess
BUILD_TYPE = debug

SRCS     = $(wildcard *.cpp)
INC_DIRS = .

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

INCS = $(shell find $(INC_DIR) -name '*.hpp')
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

ifeq ($(BUILD_TYPE), release)
	CFLAGS += -O3 -DNDEBUG
else
	CFLAGS += -ggdb
endif

.PHONY: build run

build: $(SRCS) $(INCS)
	@echo $(SRCS)
	@echo $(INCS)
	$(CXX) $(CFLAGS) -o $(BUILD_DIR)/$(BUILD_EXE) $(INC_FLAGS) $(SRCS) $(SFML_LIBS)

run: build
	cd $(BUILD_DIR) && ./$(BUILD_EXE)

