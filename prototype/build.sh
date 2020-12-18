#!/bin/sh

LIBS="$HOME/aclib/c_string/c_string.c -I$HOME/aclib/include/c_string/ $HOME/s7lib/s7lib.c $HOME/s7lib/s7lib_parser.c -I$HOME/s7lib/include/ -L$HOME/s7lib -lsnap7"
FILES="main.c"
OUTPUT_FILE_NAME="prototype_real"
OUTPUT_PATH="."

gcc $FILES $LIBS -o $OUTPUT_PATH/$OUTPUT_FILE_NAME
