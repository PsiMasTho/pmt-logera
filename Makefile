CXX = g++-12 # note: clang++ gives a lot of warnings for the code bisonc++ and flexc++ generates
CXX_FLAGS = -s -Ofast -march=native -std=c++20
#CXX_FLAGS = -ggdb3 -O0 -Wall -fsanitize=address -std=c++20
CXX_LDFLAGS = -flto

# Final binary
BIN = Logera

# Executible, object files and .d files go here
BUILD_DIR = ./build

SOURCE_DIR = ./src

LOG_PARSER_DIR = $(SOURCE_DIR)/log_parser
HEADER_PARSER_DIR = $(SOURCE_DIR)/header_parser

LOG_SCANNER_DIR = $(SOURCE_DIR)/log_scanner
HEADER_SCANNER_DIR = $(SOURCE_DIR)/header_scanner

# All .cc files
SOURCE := $(shell find $(SOURCE_DIR) -iname '*.cc')

# All .o files go to build dir.
OBJ = $(SOURCE:%.cc=$(BUILD_DIR)/%.o)
# G++/Clang++ will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

# Default target named after the binary.
$(BIN): $(BUILD_DIR)/$(BIN)

# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN): $(OBJ)
	mkdir -p $(@D)
	$(CXX) $^ $(CXX_LDFLAGS) -o $@

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o: %.cc
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY: run_flexc++
run_flexc++:
	flexc++ $(LOG_SCANNER_DIR)/lexer    --target-directory=$(LOG_SCANNER_DIR)/    --filenames=log_scanner    --no-lines --class-name LogScanner
	flexc++ $(HEADER_SCANNER_DIR)/lexer --target-directory=$(HEADER_SCANNER_DIR)/ --filenames=header_scanner --no-lines --class-name HeaderScanner

.PHONY: run_bisonc++
run_bisonc++:
	cd $(LOG_PARSER_DIR)    && bisonc++ grammar --filenames=log_parser    --no-lines --class-name LogParser    --token-class=LogTokens    --token-path ../log_scanner/log_token_enums.h
	cd $(HEADER_PARSER_DIR) && bisonc++ grammar --filenames=header_parser --no-lines --class-name HeaderParser --token-class=HeaderTokens --token-path ../header_scanner/header_token_enums.h

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
