#!/bin/sh

if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  export SUFFIX=.exe
else
  export SUFFIX=
fi

mkdir -p target

gcc src/tail_reader.c -o target/tail_reader.base
gcc src/tail_appender.c -o target/tail_appender
