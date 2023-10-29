#!/bin/bash

cd "../src/" &&\
flexc++ log_scanner_lexer    --lex-source=log_scanner_lex.cc    --filenames=log_scanner    --no-lines --class-name log_scanner &&\
flexc++ header_scanner_lexer --lex-source=header_scanner_lex.cc --filenames=header_scanner --no-lines --class-name header_scanner &&\
echo SUCCESS