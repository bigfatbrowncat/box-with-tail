#!/bin/sh

export SUFFIX=

if [ "$(uname)" == "Darwin" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
  export SUFFIX=
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
  export SUFFIX=.exe
fi

mkdir -p target/test/classes

"$JAVA_HOME/bin/javac" -source 1.7 -target 1.7 -bootclasspath avian-pack/avian/build/windows-x86_64-android/classpath.jar test/Main.java -d target/test/classes
(cd target/test/classes && "$JAVA_HOME/bin/jar" -cfe ../test.jar Main .)
cp -f target/avian-launcher.bin target/test/avian-launcher-test${SUFFIX}
target/tail_appender.exe target/test/avian-launcher-test${SUFFIX} target/test/test.jar
target/tail_appender.exe target/test/avian-launcher-test${SUFFIX} test/classname.txt

target/test/avian-launcher-test${SUFFIX}