#########################################################################
#      libvpx build configure
#      author: zd
#      2015-2016 iexin
#########################################################################
export NDK=/home/ring0/develop/android-ndk-r15c
export TARGET=
export PREFIX=
export PLATFORM=
export LIB=
export INCLUDE=
export CFLAGS=
export LDFLAGS=
export LD=
export GCC=

__internal_build_env_armeabi_v5a(){
      TARGET=armv7-android-gcc
      PREFIX=android/armeabi
      PLATFORM=android-19
      LIB=$NDK/platforms/$PLATFORM/arch-arm/usr/lib
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm/usr/include
      CFLAGS="-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -march=armv5te -mtune=xscale -msoft-float -fno-exceptions -mfloat-abi=softfp -mfpu=neon -I$INCLUDE"
      LDFLAGS="-Wl,-Bsymbolic --sysroot=$NDK/platforms/$PLATFORM/arch-arm -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -L$LIB -lm -lgcc"
      LD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld
      GCC=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/libgcc.a
}

__internal_build_env_armeabi_v7a(){
      TARGET=armv7-android-gcc
      PREFIX=android/armeabi-v7a
      PLATFORM=android-19
      LIB=$NDK/platforms/$PLATFORM/arch-arm/usr/lib
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm/usr/include
      CFLAGS="-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -march=armv5te -mtune=xscale -msoft-float -fno-exceptions -mfloat-abi=softfp -mfpu=neon -I$INCLUDE"
      LDFLAGS="-Wl,-Bsymbolic --sysroot=$NDK/platforms/$PLATFORM/arch-arm -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -L$LIB -lm -lgcc"
      LD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld
      GCC=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/libgcc.a
}

__internal_build_env_aarch64_v8a(){
      TARGET=arm64-android-gcc
      PREFIX=android/arm64-v8a
      PLATFORM=android-19
      LIB=$NDK/platforms/$PLATFORM/arch-arm64/usr/lib
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm64/usr/include
      CFLAGS="-fpic -fno-exceptions -I$INCLUDE -mfloat-abi=softfp -mfpu=neon"
      LDFLAGS="-Wl,-Bsymbolic --sysroot=$NDK/platforms/$PLATFORM/arch-arm64  -L$LIB -lm -lgcc"
      LD=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-ld
      GCC=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/lib/gcc/aarch64-linux-android/4.9/libgcc.a
}

__internal_build_env_intel_x86(){
      TARGET=x86-android-gcc
      PREFIX=android/x86
      PLATFORM=android-19
      LIB=$NDK/platforms/$PLATFORM/arch-x86/usr/lib
      INCLUDE=$NDK/platforms/$PLATFORM/arch-x86/usr/include
      CFLAGS="-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -msoft-float -fno-exceptions -I$INCLUDE"
      LDFLAGS="-Wl,-Bsymbolic --sysroot=$NDK/platforms/$PLATFORM/arch-x86 -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -L$LIB -lm -lgcc"
      LD=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/i686-linux-android-ld
      GCC=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/lib/gcc/i686-linux-android/4.9/libgcc.a
}

__internal_build_env_intel_x86_64(){
      TARGET=x86_64-android-gcc
      PREFIX=android/x86_64
      PLATFORM=android-19
      LIB=$NDK/platforms/$PLATFORM/arch-x86_64/usr/lib64
      INCLUDE=$NDK/platforms/$PLATFORM/arch-x86_64/usr/include
      CFLAGS="-fpic -ffunction-sections -funwind-tables -fstack-protector -no-canonical-prefixes -msoft-float -fno-exceptions -I$INCLUDE"
      LDFLAGS="-Wl,-Bsymbolic --sysroot=$NDK/platforms/$PLATFORM/arch-x86_64 -no-canonical-prefixes -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -L$LIB -lm -lgcc"
      LD=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin/x86_64-linux-android-ld
      GCC=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/lib/gcc/x86_64-linux-android/4.9/libgcc.a
}

__internal_build_general(){
      ./configure \
            --prefix=$PREFIX \
            --target=$TARGET \
            --sdk-path=$NDK \
            --enable-runtime-cpu-detect \
            --disable-install-docs \
            --disable-examples \
            --disable-tools \
            --disable-docs \
            --enable-vp8 \
            --enable-vp9
}

__internal_build_shread(){
      $LD -rpath-link=-$LIB         \
            -L$LIB                  \
            -soname libvpx.so       \
            -shared                 \
            -nostdlib               \
            -Bsymbolic              \
            --whole-archive         \
            --no-undefined          \
            -o $PREFIX/libvpx.so    \
            $PREFIX/lib/libvpx.a    \
            -lc -lm -lz -ldl -llog  \
            --dynamic-linker=/system/bin/linker $GCC
}

build_armeabi_v5a(){
      make clean
      __internal_build_env_armeabi_v5a
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_armeabi_v7a(){
      make clean
      __internal_build_env_armeabi_v7a
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_aarch64_v8a(){
      make clean
      __internal_build_env_aarch64_v8a
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_intel_x86(){
      make clean
      __internal_build_env_intel_x86
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_intel_x86_64(){
      make clean
      __internal_build_env_intel_x86_64
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_armeabi_v5a
build_armeabi_v7a
#build_aarch64_v8a
#build_intel_x86
#build_intel_x86_64
