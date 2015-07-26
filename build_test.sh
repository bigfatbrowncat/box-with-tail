#!/bin/sh

mkdir -p target

gcc src/tail_reader.c -o target/tail_reader.base
gcc src/tail_appender.c -o target/tail_appender

mkdir -p target/test

cp target/tail_reader.base target/test/tailed1.exe
target/tail_appender.exe target/test/tailed1.exe test/tail1.txt
cp target/tail_reader.base target/test/tailed2.exe
target/tail_appender.exe target/test/tailed2.exe test/tail2.txt
