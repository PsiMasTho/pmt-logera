#!/bin/bash

cd "../src/" &&\
flexc++ log_scanner_lexer    --no-lines &&\
flexc++ header_scanner_lexer --no-lines &&\
echo SUCCESS