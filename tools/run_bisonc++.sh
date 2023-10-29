#!/bin/bash

cd "../src/" &&\
bisonc++ ./log_parser_grammar    --filenames=log_parser    --no-lines --class-name log_parser    --token-class=log_tokens    --token-path ./log_token_enums.h &&\
bisonc++ ./header_parser_grammar --filenames=header_parser --no-lines --class-name header_parser --token-class=header_tokens --token-path ./header_token_enums.h &&\
echo SUCCESS