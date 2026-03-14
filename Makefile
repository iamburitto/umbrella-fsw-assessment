CXX ?= c++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -Werror -pedantic -O0 -g
BUILD_DIR ?= build
BUILD_STAMP := $(BUILD_DIR)/.dir

SERVER_SRC := src/propulsion_server.cpp
TEST_SRC := tests/test_propulsion_server.cpp
SERVER_BIN := $(BUILD_DIR)/propulsion_server
TEST_BIN := $(BUILD_DIR)/test_propulsion_server

.DEFAULT_GOAL := all

.PHONY: help all build run test clean

help:
	@printf '%s\n' \
		'make all      Clean, then build server and test binaries' \
		'make build    Build the server binary' \
		'make run      Build and run the server' \
		'make test     Build and run the tests' \
		'make clean    Remove build outputs'

$(BUILD_STAMP):
	mkdir -p $(BUILD_DIR)
	touch $@

$(SERVER_BIN): $(SERVER_SRC) | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $< -o $@

$(TEST_BIN): $(TEST_SRC) | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $< -o $@

all: clean
	$(MAKE) $(SERVER_BIN) $(TEST_BIN)

build: $(SERVER_BIN)

run: $(SERVER_BIN)
	./$(SERVER_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR)