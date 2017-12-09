#########################################################################
#      cpu-features.a
#      author: zd
#      2017-2018 iexin
#########################################################################
export NDK=/home/ring0/develop/android-ndk-r12b
export PREFIX=
export PLATFORM=
export INCLUDE=
export CC=
export AR=

__internal_build_env_armeabi_v5a(){
      PREFIX=android/armeabi
      PLATFORM=android-21
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm/usr/include
      CC=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
      AR=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar
}

__internal_build_env_armeabi_v7a(){
      PREFIX=android/armeabi-v7a
      PLATFORM=android-21
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm/usr/include
      CC=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
      AR=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar
}

__internal_build_env_aarch64_v8a(){
      PREFIX=android/arm64-v8a
      PLATFORM=android-21
      INCLUDE=$NDK/platforms/$PLATFORM/arch-arm64/usr/include
      CC=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-gcc
      AR=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-ar
}

__internal_build_env_intel_x86(){
      PREFIX=android/x86
      PLATFORM=android-21
      INCLUDE=$NDK/platforms/$PLATFORM/arch-x86/usr/include
      CC=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/i686-linux-android-gcc
      AR=$NDK/toolchains/x86-4.9/prebuilt/linux-x86_64/bin/i686-linux-android-ar
}

__internal_build_env_intel_x86_64(){
      PREFIX=android/x86_64
      PLATFORM=android-21
      INCLUDE=$NDK/platforms/$PLATFORM/arch-x86_64/usr/include
      CC=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin/x86_64-linux-android-gcc
      AR=$NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin/x86_64-linux-android-ar
}

__internal_build_general(){
      $CC -I$INCLUDE -c cpu-features.c -o libcpu-features.o
      $AR rcs libcpu-features.a libcpu-features.o
      mkdir -p $PREFIX
      cp libcpu-features.a $PREFIX
      rm -f libcpu-features.o
      rm -f libcpu-features.a
}

build_armeabi_v5a(){
      __internal_build_env_armeabi_v5a
      __internal_build_general
}

build_armeabi_v7a(){
      __internal_build_env_armeabi_v7a
      __internal_build_general
}

build_aarch64_v8a(){
      __internal_build_env_aarch64_v8a
      __internal_build_general
}

build_intel_x86(){
      __internal_build_env_intel_x86
      __internal_build_general
}

build_intel_x86_64(){
      __internal_build_env_intel_x86_64
      __internal_build_general
}

build_armeabi_v5a
build_armeabi_v7a
build_aarch64_v8a
build_intel_x86
build_intel_x86_64
