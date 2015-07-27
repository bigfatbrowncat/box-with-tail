#!/bin/sh

export SUFFIX=

if [ "$(uname)" == "Darwin" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  export SUFFIX=.exe
fi

mkdir -p target

gcc src/tail_reader.c src/tail.c -o target/tail_reader.base
gcc src/tail_appender.c -o target/tail_appender
