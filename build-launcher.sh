g++ src/avian-launcher.cpp src/tail.c \
    avian-pack/avian/build/windows-x86_64-android/libavian.a \
    avian-pack/android/external/fdlibm/libfdm.a \
	avian-pack/android/external/expat/.libs/libexpat.a \
	avian-pack/android/external/icu4c/lib/libsicuin.a \
	avian-pack/android/external/icu4c/lib/libsicuuc.a \
	avian-pack/android/external/icu4c/lib/sicudt.a \
	avian-pack/android/openssl-upstream/libssl.a \
	avian-pack/android/openssl-upstream/libcrypto.a \
    -static -lmingw32 -lmingwthrd -lws2_32 -lshlwapi -mwindows -mconsole -static-libgcc -static-libstdc++\
	-lm -lz \
    -o target/avian-launcher.bin -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/win32" -Isrc \
	-I avian-pack/avian/src -I avian-pack/avian/include

strip target/avian-launcher.bin
target/tail_appender.exe target/avian-launcher.bin avian-pack/avian/build/windows-x86_64-android/classpath.jar
