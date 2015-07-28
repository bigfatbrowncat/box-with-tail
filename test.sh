#!/bin/sh

export SUFFIX=

if [ "$(uname)" == "Darwin" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  export SUFFIX=.exe
fi

mkdir -p target/test

echo ""
echo "=== APPENDING TAILS ==="
cp target/tail_reader.base target/test/tailed0${SUFFIX}
cp target/tail_reader.base target/test/tailed1${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed1${SUFFIX} test/tail1.txt
cp target/tail_reader.base target/test/tailed2${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed2${SUFFIX} test/tail2.txt
cp target/tail_reader.base target/test/tailed3${SUFFIX}
target/tail_appender${SUFFIX} target/test/tailed3${SUFFIX} test/tail3-1.txt
target/tail_appender${SUFFIX} target/test/tailed3${SUFFIX} test/tail3-2.txt

echo ""
echo "=== RUNING TARGETS ==="
target/test/tailed0
target/test/tailed1
target/test/tailed2
target/test/tailed3