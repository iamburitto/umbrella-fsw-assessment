# Makefile for the umbrella-fsw-assessment sensor driver
#
# Targets
#   make          – build the driver object
#   make test     – build and run all unit tests
#   make clean    – remove all build artefacts

CC      ?= gcc
CFLAGS  := -std=c99 -Wall -Wextra -Werror -pedantic -g

SRC_DIR  := src
TEST_DIR := test
BUILD    := build

DRIVER_SRC := $(SRC_DIR)/sensor_driver.c
TEST_SRC   := $(TEST_DIR)/test_sensor_driver.c
TEST_BIN   := $(BUILD)/test_sensor_driver

.PHONY: all test clean

all: $(BUILD)/sensor_driver.o

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/sensor_driver.o: $(DRIVER_SRC) $(SRC_DIR)/sensor_driver.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(DRIVER_SRC) $(TEST_SRC) $(SRC_DIR)/sensor_driver.h \
             $(TEST_DIR)/test_framework.h | $(BUILD)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(DRIVER_SRC) $(TEST_SRC) -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD)
