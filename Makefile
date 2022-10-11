CXX = g++-12 # note: clang++ gives a lot of warnings for the code bisonc++ and flexc++ generates
CXX_FLAGS = -s -Ofast -march=native -std=c++20
#CXX_FLAGS = -ggdb3 -O0 -Wall -fsanitize=address -std=c++20
CXX_LDFLAGS = -flto -lpthread -ltbb
#CXX_LDFLAGS = -fsanitize=address -ltbb

# Final binary
BIN = Logara

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
	bisonc++ $(LOG_PARSER_DIR)/grammar    --target-directory=$(LOG_PARSER_DIR)/    --filenames=log_parser    --no-lines --class-name LogParser    --token-class=LogTokens
	bisonc++ $(HEADER_PARSER_DIR)/grammar --target-directory=$(HEADER_PARSER_DIR)/ --filenames=header_parser --no-lines --class-name HeaderParser --token-class=HeaderTokens

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
