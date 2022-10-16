#!/bin/bash

LOG_SCANNER_DIR=../src/log_scanner
HEADER_SCANNER_DIR=../src/header_scanner

flexc++ "${LOG_SCANNER_DIR}"/lexer    --target-directory="${LOG_SCANNER_DIR}"/    --filenames=log_scanner    --no-lines --class-name LogScanner &&\
flexc++ "${HEADER_SCANNER_DIR}"/lexer --target-directory="${HEADER_SCANNER_DIR}"/ --filenames=header_scanner --no-lines --class-name HeaderScanner &&\
echo SUCCESS