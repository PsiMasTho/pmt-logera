CXX = g++ # note: clang++ gives a lot of warnings for the code bisonc++ and flexc++ generates
#CXX_FLAGS = -s -Ofast -march=native -std=c++20
CXX_FLAGS = -ggdb3 -O0 -Wall -std=c++20 -fsanitize=address
#CXX_LDFLAGS = -flto
CXX_LDFLAGS = -fsanitize=address

# Final binary
BIN = Logera

# Executible, object files and .d files go here
BUILD_DIR = ./build

SOURCE_DIR = ./src

LOG_PARSER_DIR = $(SOURCE_DIR)/log_parser
HEADER_PARSER_DIR = $(SOURCE_DIR)/header_parser

SCANNER_DIR = $(SOURCE_DIR)/scanner

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
	flexc++ $(SCANNER_DIR)/lexer --target-directory=$(SCANNER_DIR)/ --filenames=scanner

.PHONY: run_bisonc++
run_bisonc++:
	bisonc++ $(LOG_PARSER_DIR)/grammar --target-directory=$(LOG_PARSER_DIR)/ --filenames=log_parser --no-lines --scanner=$(SCANNER_DIR)/scanner.h --class-name LogParser --token-path=../scanner/token_enums.h
	bisonc++ $(HEADER_PARSER_DIR)/grammar --target-directory=$(HEADER_PARSER_DIR)/ --filenames=header_parser --no-lines --scanner=$(SCANNER_DIR)/scanner.h --class-name HeaderParser --token-path=../scanner/token_enums.h

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)