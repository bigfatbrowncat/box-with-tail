#!/bin/sh

if [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  export SUFFIX=.exe
else
  export SUFFIX=
fi

mkdir -p target/test

echo "=== APPENDING TAILS ==="
cp target/tail_reader.base target/test/tailed1${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed1${SUFFIX} test/tail1.txt
cp target/tail_reader.base target/test/tailed2${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed2${SUFFIX} test/tail2.txt

echo "=== RUNING TARGETS ==="
target/test/tailed1
target/test/tailed2