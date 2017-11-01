#########################################################################
#      x264 build configure
#      author: zd
#      2015-2016 iexin
#########################################################################
export NDK=/home/ring0/develop/android-ndk-r14b
export HOST=
export CROSS=
export SYSROOT=
export PREFIX=
export CC_FLAGS=
export PREFIX_LIB=

__internal_build_env_armeabi_v5a(){
      PREFIX=android/armeabi
      HOST=arm-linux-androideabi
      CROSS=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-arm/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -marm -march=arm -fvisibility=hidden
}

__internal_build_env_armeabi_v7a(){
      PREFIX=android/armeabi-v7a
      HOST=arm-linux-androideabi
      CROSS=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-arm/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -mfpu=neon -marm -march=arm -fvisibility=hidden
}

__internal_build_env_aarch64_v8a(){
      PREFIX=android/arm64-v8a
      HOST=aarch64-linux-android
      CROSS=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-arm64
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-arm64/usr/lib
      CC_FLAGS=-march=armv8 -D__ANDROID__ -D__ARM_ARCH_8__ -D__ARM_ARCH_8A__
}

__internal_build_env_intel_x86(){
      PREFIX=android/x86
      HOST=i686-linux-android
      CROSS=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-x86
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-x86/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
}

__internal_build_env_intel_x86_64(){
      PREFIX=android/x86_64
      HOST=x86_64-linux-android
      CROSS=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-x86_64
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-x86_64/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
}

__internal_build_env_mips32(){
      PREFIX=android/mips
      HOST=mipsel-linux-android
      CROSS=$NDK/toolchains/mipsel-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-mips
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-mips/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated
}

__internal_build_env_mips64(){
      PREFIX=android/mips64
      HOST=mips64el-linux-android
      CROSS=$NDK/toolchains/mips64el-linux-android-4.9/prebuilt/linux-x86_64/bin/$HOST-
      SYSROOT=$NDK/platforms/android-21/arch-mips64
      PREFIX_LIB=/home/ring0/develop/android-ndk-r14b/platforms/android-21/arch-mips64/usr/lib
      CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mhard-float -mmsa -mdsp -mdspr2
}

__internal_build_general(){
      ./configure \
            --prefix=$PREFIX \
            --host=$HOST \
            --sysroot=$SYSROOT \
            --cross-prefix=$CROSS \
            --extra-cflags=$CC_FLAGS \
            --extra-ldflags="-nostdlib" \
            --enable-static \
            --enable-shared \
            --enable-strip \
            --disable-asm \
            --disable-cli \
            --disable-win32thread \
            --disable-avs \
            --disable-swscale \
            --disable-lavf \
            --disable-ffms \
            --disable-gpac \
            --disable-lsmash
}

build_armeabi_v5a(){
      make clean
      __internal_build_env_armeabi_v5a
      __internal_build_general
      make -j4 install
}

build_armeabi_v7a(){
      make clean
      __internal_build_env_armeabi_v7a
      __internal_build_general
      make -j4 install
}

build_aarch64_v8a(){
      make clean
      __internal_build_env_aarch64_v8a
      __internal_build_general
      make -j4 install
}

build_intel_x86(){
      make clean
      __internal_build_env_intel_x86
      __internal_build_general
      make -j4 install
}

build_intel_x86_64(){
      make clean
      __internal_build_env_intel_x86_64
      __internal_build_general
      make -j4 install
}

build_mips32(){
      make clean
      __internal_build_env_mips32
      __internal_build_general
      make -j4 install
}

build_mips64(){
      make clean
      __internal_build_env_mips64
      __internal_build_general
      make -j4 install
}

build_armeabi_v5a
build_armeabi_v7a
build_aarch64_v8a
build_intel_x86
build_intel_x86_64
build_mips32
build_mips64
