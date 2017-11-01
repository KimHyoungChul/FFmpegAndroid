#########################################################################
#      x264 build configure
#      author: zd
#      2015-2016 iexin
#########################################################################
export NDK=/home/ring0/develop/android-ndk-r10d
export HOST=
export CROSS=
export SYSROOT=
export PREFIX=
export CC_FLAGS=
export PREFIX_LIB=
export GCC_LIB=
export LD=
export DISABLE_ASM=

__internal_build_env_armeabi_v5a(){
      PREFIX=android/armeabi
      HOST=arm-linux-androideabi
      CROSS=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm
      PREFIX_LIB=$NDK/platforms/android-21/arch-arm/usr/lib
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -marm -march=arm -fvisibility=hidden
      DISABLE_ASM="--disable-asm"
}

__internal_build_env_armeabi_v7a(){
      PREFIX=android/armeabi-v7a
      HOST=arm-linux-androideabi
      CROSS=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm
      PREFIX_LIB=$NDK/platforms/android-21/arch-arm/usr/lib
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -mfpu=neon -marm -march=arm -fvisibility=hidden
      DISABLE_ASM="--disable-asm"
}

__internal_build_env_aarch64_v8a(){
      PREFIX=android/arm64-v8a
      HOST=aarch64-linux-android
      CROSS=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm64
      PREFIX_LIB=$NDK/platforms/android-21/arch-arm64/usr/lib
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -mfpu=neon -marm -march=arm64 -fvisibility=hidden
      DISABLE_ASM=""
}

__internal_build_env_intel_x86(){
      PREFIX=android/x86
      HOST=i686-linux-android
      CROSS=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-x86
      PREFIX_LIB=$NDK/platforms/android-21/arch-x86/usr/lib
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
      DISABLE_ASM="--disable-asm"
}

__internal_build_env_intel_x86_64(){
      PREFIX=android/x86_64
      HOST=x86_64-linux-android
      CROSS=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-x86_64
      PREFIX_LIB=$NDK/platforms/android-21/arch-x86_64/usr/lib64
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
      DISABLE_ASM="--disable-asm"
}

__internal_build_env_mips32(){
      PREFIX=android/mips
      HOST=mipsel-linux-android
      CROSS=$NDK/toolchains/mipsel-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-mips
      PREFIX_LIB=$NDK/platforms/android-21/arch-mips/usr/lib
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/mipsel-linux-android-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated
      DISABLE_ASM="--disable-asm"
}

__internal_build_env_mips64(){
      PREFIX=android/mips64
      HOST=mips64el-linux-android
      CROSS=$NDK/toolchains/mips64el-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-mips64
      PREFIX_LIB=$NDK/platforms/android-21/arch-mips64/usr/lib64
      LD=$CROSS"ld"
      GCC_LIB=$NDK/toolchains/mips64el-linux-android-4.9/prebuilt/linux-x86_64/lib/gcc/$HOST/4.9/libgcc.a
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mhard-float -mmsa -mdsp -mdspr2
      DISABLE_ASM="--disable-asm"
}

__internal_build_general(){
      ./configure \
            --prefix=$PREFIX         \
            --host=$HOST             \
            --sysroot=$SYSROOT       \
            --cross-prefix=$CROSS    \
            --extra-cflags=$CC_FLAGS \
            $DISABLE_ASM             \
            --enable-static          \
            --enable-strip           \
            --enable-pic             \
            --disable-cli            \
            --disable-win32thread    \
            --disable-avs            \
            --disable-swscale        \
            --disable-lavf           \
            --disable-ffms           \
            --disable-gpac           \
            --disable-lsmash
}

__internal_build_shread(){
      $LD -rpath-link=-$PREFIX_LIB   \
            -L$PREFIX_LIB            \
            -soname libx264.so       \
            -shared                  \
            -nostdlib                \
            -Bsymbolic               \
            --whole-archive          \
            --no-undefined           \
            -o $PREFIX/libx264.so    \
            $PREFIX/lib/libx264.a    \
            -lc -lm -lz -ldl -llog   \
            --dynamic-linker=/system/bin/linker $GCC_LIB
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

build_mips32(){
      make clean
      __internal_build_env_mips32
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_mips64(){
      make clean
      __internal_build_env_mips64
      __internal_build_general
      make -j4 install
      __internal_build_shread
}

build_armeabi_v5a
build_armeabi_v7a
build_aarch64_v8a
build_intel_x86
build_intel_x86_64
build_mips32
build_mips64
