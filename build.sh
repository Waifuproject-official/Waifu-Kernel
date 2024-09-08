#!/bin/bash
#
# Compile script for Waifu kernel
# Copyright (C) 2024 NyaruToru

SECONDS=0 # builtin bash timer
ZIPNAME="Waifu-surya-$(date '+%Y%m%d-%H%M').zip"
TC_DIR="$(pwd)/tc/clang"
AK3_DIR="$(pwd)/android/AnyKernel3"
DEFCONFIG="surya_defconfig"

if test -z "$(git rev-parse --show-cdup 2>/dev/null)" &&
   head=$(git rev-parse --verify HEAD 2>/dev/null); then
	ZIPNAME="${ZIPNAME::-4}-$(echo $head | cut -c1-8).zip"
fi

export KBUILD_BUILD_USER=NyaruToru
export KBUILD_BUILD_HOST=Neko-Internet
export ARCH="arm64"
export SUBARCH="arm64"
export PATH="$TC_DIR/bin:$PATH"

if ! [ -d "$TC_DIR" ]; then
	echo "Clang not found! Downloading to $TC_DIR..."
	mkdir -p "$TC_DIR" && cd "$TC_DIR"
	wget -nv -c https://github.com/greenforce-project/greenforce_clang/releases/download/26122023/greenforce-clang-12.0.1-26122023-1149.tar.zst -O - | tar --use-compress-program=unzstd -xf - -C "$TC_DIR/"
	cd ../..
fi

echo "CPU Core: $(grep -c ^processor /proc/cpuinfo), CPU Model: $(grep "model name" /proc/cpuinfo | head -1 | awk -F: '{print $2}' | xargs), RAM: $(free -m | grep Mem | awk '{print $2}') MB, Storage: $(df -h / | grep / | awk '{print $2}') (Used: $(df -h / | grep / | awk '{print $3}'), Available: $(df -h / | grep / | awk '{print $4}'))"

echo -e "\nStarting compilation...\n"
make $DEFCONFIG O=out
make -j$(nproc --all) O=out \
    ARCH=arm64 \
    SUBARCH=arm64 \
    CC=clang \
    AR=llvm-ar \
    LD=ld.lld \
    NM=llvm-nm \
    OBJDUMP=llvm-objdump \
    STRIP=llvm-strip \
    OBJCOPY=llvm-objcopy \
    OBJSIZE=llvm-size \
    READELF=llvm-readelf \
    CROSS_COMPILE=aarch64-linux-gnu- \
    CROSS_COMPILE_ARM32=arm-linux-gnueabi-


kernel="out/arch/arm64/boot/Image.gz-dtb"
dtbo="out/arch/arm64/boot/dtbo.img"

if [ -f "$kernel" ]; then
	echo -e "\nKernel compiled succesfully! Zipping up...\n"
	if [ -d "$AK3_DIR" ]; then
		cp -r $AK3_DIR AnyKernel3
	elif ! git clone -q --depth=1 https://github.com/Waifuproject-official/AnyKernel3 -b Waifu; then
		echo -e "\nAnyKernel3 repo not found locally and couldn't clone from GitHub! Aborting..."
		exit 1
	fi
	cp $kernel $dtbo AnyKernel3
	cd AnyKernel3
	git checkout Waifu &> /dev/null
	zip -r9 "../$ZIPNAME" * -x .git modules\* patch\* ramdisk\* README.md *placeholder
	cd ..
	echo -e "\nCompleted in $((SECONDS / 60)) minute(s) and $((SECONDS % 60)) second(s) !"
	echo "Zip: $ZIPNAME"
else
	echo -e "\nCompilation failed!"
	exit 1
fi
