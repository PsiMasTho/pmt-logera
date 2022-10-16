#!/bin/bash

LOG_PARSER_DIR=../src/log_parser
HEADER_PARSER_DIR=../src/header_parser

TOOLS_DIR=$(pwd)

cd "${LOG_PARSER_DIR}"    && bisonc++ grammar --filenames=log_parser    --no-lines --class-name LogParser    --token-class=LogTokens    --token-path ../log_scanner/log_token_enums.h &&\
cd "${TOOLS_DIR}" &&\
cd "${HEADER_PARSER_DIR}" && bisonc++ grammar --filenames=header_parser --no-lines --class-name HeaderParser --token-class=HeaderTokens --token-path ../header_scanner/header_token_enums.h &&\
echo SUCCESS