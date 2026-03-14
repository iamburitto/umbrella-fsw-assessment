CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -O0 -g
CXXFLAGS ?= -std=c++20 -Wall -Wextra -Werror -pedantic -O0 -g
BUILD_DIR ?= build
BUILD_STAMP := $(BUILD_DIR)/.dir

SERVER_C := src/propulsion_server.c
SERVER_CPP := src/propulsion_server.cpp
TEST_C := tests/test_propulsion_server.c
TEST_CPP := tests/test_propulsion_server.cpp

ifeq ($(wildcard $(SERVER_CPP)),$(SERVER_CPP))
LANG := cpp
else
LANG := c
endif

ifeq ($(LANG),cpp)
SERVER_SRC := $(SERVER_CPP)
TEST_SRC := $(TEST_CPP)
COMPILER := $(CXX)
COMPILE_FLAGS := $(CXXFLAGS)
else
SERVER_SRC := $(SERVER_C)
TEST_SRC := $(TEST_C)
COMPILER := $(CC)
COMPILE_FLAGS := $(CFLAGS)
endif

SERVER_BIN := $(BUILD_DIR)/propulsion_server
TEST_BIN := $(BUILD_DIR)/test_propulsion_server

.PHONY: help build run test clean

help:
	@printf '%s\n' \
		'make build        Build the default language target (C++ if present, else C)' \
		'make build-c      Build the C server binary' \
		'make build-cpp    Build the C++ server binary' \
		'make build        Build the server binary' \
		'make run          Build and run the default language target' \
		'make run-c        Build and run the C server' \
		'make run-cpp      Build and run the C++ server' \
		'make test         Build and run the default language tests' \
		'make test-c       Build and run the C tests' \
		'make test-cpp     Build and run the C++ tests' \
		'make clean        Remove build outputs'

$(BUILD_STAMP):
	mkdir -p $(BUILD_DIR)
	touch $@

$(SERVER_BIN): $(SERVER_SRC) | $(BUILD_STAMP)
	$(COMPILER) $(COMPILE_FLAGS) $< -o $@

$(TEST_BIN): $(TEST_SRC) | $(BUILD_STAMP)
	$(COMPILER) $(COMPILE_FLAGS) $< -o $@

$(BUILD_DIR)/propulsion_server_c: $(SERVER_C) | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/test_propulsion_server_c: $(TEST_C) | $(BUILD_STAMP)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/propulsion_server_cpp: $(SERVER_CPP) | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/test_propulsion_server_cpp: $(TEST_CPP) | $(BUILD_STAMP)
	$(CXX) $(CXXFLAGS) $< -o $@

build: $(SERVER_BIN)

build-c: $(BUILD_DIR)/propulsion_server_c

build-cpp: $(BUILD_DIR)/propulsion_server_cpp

run: $(SERVER_BIN)
	./$(SERVER_BIN)

run-c: $(BUILD_DIR)/propulsion_server_c
	./$(BUILD_DIR)/propulsion_server_c

run-cpp: $(BUILD_DIR)/propulsion_server_cpp
	./$(BUILD_DIR)/propulsion_server_cpp

test: $(TEST_BIN)
	./$(TEST_BIN)

test-c: $(BUILD_DIR)/test_propulsion_server_c
	./$(BUILD_DIR)/test_propulsion_server_c

test-cpp: $(BUILD_DIR)/test_propulsion_server_cpp
	./$(BUILD_DIR)/test_propulsion_server_cpp

clean:
	rm -rf $(BUILD_DIR)
