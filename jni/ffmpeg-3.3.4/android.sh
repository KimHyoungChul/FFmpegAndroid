#########################################################################
#      ffmpeg build configure
#      author: zd
#      2015-2016 iexin
#########################################################################
export NDK=/home/ring0/develop/android-ndk-r10d

export PREBUILT=
export PLATFORM=
export CC=
export CROSS=
export NM=
export LD=
export ARCH=
export GCC_PREFIX=
export CC_FLAGS=
export PREFIX=
export PREFIX_LIB=

__internal_build_env_armeabi_v5a(){
	PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-arm
	CC=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-
	NM=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-nm
	LD=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-ld
	GCC_PREFIX=arm-linux-androideabi/4.8/libgcc.a
	ARCH=arm
	PREFIX=android/armeabi
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -marm -march=arm -fvisibility=hidden
}

__internal_build_env_armeabi_v7a(){
	PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.8/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-arm
	CC=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-
	NM=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-nm
	LD=$PREBUILT/linux-x86_64/bin/arm-linux-androideabi-ld
	GCC_PREFIX=arm-linux-androideabi/4.8/libgcc.a
	ARCH=arm
	PREFIX=android/armeabi-v7a
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -mfpu=neon -marm -march=arm -fvisibility=hidden
}

__internal_build_env_aarch64_v8a(){
	PREBUILT=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-arm64
	CC=$PREBUILT/linux-x86_64/bin/aarch64-linux-android-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/aarch64-linux-android-
	NM=$PREBUILT/linux-x86_64/bin/aarch64-linux-android-nm
	LD=$PREBUILT/linux-x86_64/bin/aarch64-linux-android-ld
	GCC_PREFIX=aarch64-linux-android/4.9/libgcc.a
	ARCH=arm64
	PREFIX=android/arm64-v8a
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mfloat-abi=softfp -mfpu=neon -marm -march=arm64 -fvisibility=hidden
}

__internal_build_env_intel_x86(){
	PREBUILT=$NDK/toolchains/x86-4.8/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-x86
	CC=$PREBUILT/linux-x86_64/bin/i686-linux-android-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/i686-linux-android-
	NM=$PREBUILT/linux-x86_64/bin/i686-linux-android-nm
	LD=$PREBUILT/linux-x86_64/bin/i686-linux-android-ld
	GCC_PREFIX=i686-linux-android/4.8/libgcc.a
	ARCH=x86
	PREFIX=android/x86
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
}

__internal_build_env_intel_x86_64(){
	PREBUILT=$NDK/toolchains/x86_64-4.9/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-x86_64
	CC=$PREBUILT/linux-x86_64/bin/x86_64-linux-android-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/x86_64-linux-android-
	NM=$PREBUILT/linux-x86_64/bin/x86_64-linux-android-nm
	LD=$PREBUILT/linux-x86_64/bin/x86_64-linux-android-ld
	GCC_PREFIX=x86_64-linux-android/4.9/libgcc.a
	ARCH=x86_64
	PREFIX=android/x86_64
    PREFIX_LIB=lib64
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mssse3 -mavx -fvisibility=hidden
}

__internal_build_env_mips32(){
	PREBUILT=$NDK/toolchains/mipsel-linux-android-4.8/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-mips
	CC=$PREBUILT/linux-x86_64/bin/mipsel-linux-android-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/mipsel-linux-android-
	NM=$PREBUILT/linux-x86_64/bin/mipsel-linux-android-nm
	LD=$PREBUILT/linux-x86_64/bin/mipsel-linux-android-ld
	GCC_PREFIX=mipsel-linux-android/4.8/libgcc.a
	ARCH=mips
	PREFIX=android/mips
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated
}

__internal_build_env_mips64(){
	PREBUILT=$NDK/toolchains/mips64el-linux-android-4.9/prebuilt
	PLATFORM=$NDK/platforms/android-21/arch-mips64
	CC=$PREBUILT/linux-x86_64/bin/mips64el-linux-android-gcc
	CROSS=$PREBUILT/linux-x86_64/bin/mips64el-linux-android-
	NM=$PREBUILT/linux-x86_64/bin/mips64el-linux-android-nm
	LD=$PREBUILT/linux-x86_64/bin/mips64el-linux-android-ld
	GCC_PREFIX=mips64el-linux-android/4.9/libgcc.a
	ARCH=mips64
	PREFIX=android/mips64
    PREFIX_LIB=lib
	CC_FLAGS=-fPIC -DANDROID -D__thumb__ -Wfatal-errors -Wno-deprecated -mhard-float -mmsa -mdsp -mdspr2
}

__internal_build_general(){
	./configure --target-os=linux \
		--prefix=$PREFIX \
		--cc=$CC \
		--cross-prefix=$CROSS \
		--nm=$NM \
		--sysroot=$PLATFORM \
		--arch=$ARCH \
		--enable-cross-compile \
		--enable-runtime-cpudetect \
		--enable-gpl \
		--enable-nonfree \
		--enable-version3 \
		--enable-small \
		--enable-zlib \
		--enable-bsfs \
		--enable-parsers \
		--enable-swscale \
        --enable-avresample \
		--disable-stripping \
		--disable-ffprobe \
		--disable-ffplay \
		--disable-ffmpeg \
		--disable-ffserver \
		--disable-debug \
		--enable-static \
		--disable-shared \
        --extra-cflags=$CC_FLAGS
}
__internal_build_shread(){
	$LD -rpath-link=$PLATFORM/usr/$PREFIX_LIB \
		-L$PLATFORM/usr/$PREFIX_LIB \
		-L$PREFIX/lib \
		-soname libffmpeg.so \
		-shared \
		-nostdlib \
		-Bsymbolic \
		--whole-archive \
		--no-undefined \
		-o $PREFIX/libffmpeg.so \
		$PREFIX/lib/libavcodec.a \
		$PREFIX/lib/libavfilter.a \
		$PREFIX/lib/libswresample.a \
		$PREFIX/lib/libavformat.a \
		$PREFIX/lib/libavutil.a \
		$PREFIX/lib/libswscale.a \
		$PREFIX/lib/libpostproc.a \
		$PREFIX/lib/libavdevice.a \
		$PREFIX/lib/libavresample.a \
		-lc -lm -lz -ldl -llog \
		--dynamic-linker=/system/bin/linker $PREBUILT/linux-x86_64/lib/gcc/$GCC_PREFIX
}

build_armeabi_v5a(){
	make clean
	__internal_build_env_armeabi_v5a
	__internal_build_general
	make install
	__internal_build_shread
}

build_armeabi_v7a(){
	make clean
	__internal_build_env_armeabi_v7a
	__internal_build_general
	make install
	__internal_build_shread
}

build_aarch64_v8a(){
	make clean
	__internal_build_env_aarch64_v8a
	__internal_build_general
	make install
	__internal_build_shread
}

build_intel_x86(){
	make clean
	__internal_build_env_intel_x86
	__internal_build_general
	make install
	__internal_build_shread
}

build_intel_x86_64(){
	make clean
	__internal_build_env_intel_x86_64
	__internal_build_general
	make install
	__internal_build_shread
}

build_mips32(){
	make clean
	__internal_build_env_mips32
	__internal_build_general
	make install
	__internal_build_shread
}

build_mips64(){
	make clean
	__internal_build_env_mips64
	__internal_build_general
	make install
	__internal_build_shread
}

build_armeabi_v5a
build_armeabi_v7a
build_aarch64_v8a
build_intel_x86
build_intel_x86_64
#build_mips32
#build_mips64
