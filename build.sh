#!/bin/sh

mkdir -p target

gcc src/tail_reader.c src/tail.c -o target/tail_reader.base
gcc src/tail_appender.c -o target/tail_appender
