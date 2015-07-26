#!/bin/sh

if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  set SUFFIX=.exe
else
  set SUFFIX=
fi

mkdir -p target

gcc src/tail_reader.c -o target/tail_reader.base
gcc src/tail_appender.c -o target/tail_appender

mkdir -p target/test

cp target/tail_reader.base target/test/tailed1${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed1${SUFFIX} test/tail1.txt
cp target/tail_reader.base target/test/tailed2${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed2${SUFFIX} test/tail2.txt
