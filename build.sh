#!/bin/bash
#
# Compile script for Waifu kernel
# Copyright (C) 2024 NyaruToru

SECONDS=0 # builtin bash timer
ZIPNAME="Waifu-surya-$(date '+%Y%m%d-%H%M').zip"
TC_DIR="$(pwd)/tc/clang"
AK3_DIR="$(pwd)/android/AnyKernel3"
DEFCONFIG="surya-perf_defconfig"

if test -z "$(git rev-parse --show-cdup 2>/dev/null)" &&
   head=$(git rev-parse --verify HEAD 2>/dev/null); then
	ZIPNAME="${ZIPNAME::-4}-$(echo $head | cut -c1-8).zip"
fi

export ARCH="arm64"
export SUBARCH="arm64"
export PATH="$TC_DIR/bin:$PATH"

if ! [ -d "$TC_DIR" ]; then
	echo "Neutron Clang not found! Downloading to $TC_DIR..."
	mkdir -p "$TC_DIR" && cd "$TC_DIR"
	curl -LO "https://raw.githubusercontent.com/Neutron-Toolchains/antman/main/antman"
	bash ./antman -S --noprogress
	cd ../..
fi

echo -e "\nStarting compilation...\n"
make $DEFCONFIG O=out
make -j$(nproc --all) O=out LLVM=1


kernel="out/arch/arm64/boot/Image.gz-dtb"
dtbo="out/arch/arm64/boot/dtbo.img"

if [ -f "$kernel" ]; then
	echo -e "\nKernel compiled succesfully! Zipping up...\n"
	if [ -d "$AK3_DIR" ]; then
		cp -r $AK3_DIR AnyKernel3
	elif ! git clone -q --depth=1 https://github.com/rd-stuffs/AnyKernel3 -b FSociety; then
		echo -e "\nAnyKernel3 repo not found locally and couldn't clone from GitHub! Aborting..."
		exit 1
	fi
	cp $kernel $dtbo AnyKernel3
	cd AnyKernel3
	git checkout FSociety &> /dev/null
	zip -r9 "../$ZIPNAME" * -x .git modules\* patch\* ramdisk\* README.md *placeholder
	cd ..
	rm -rf AnyKernel3
	echo -e "\nCompleted in $((SECONDS / 60)) minute(s) and $((SECONDS % 60)) second(s) !"
	echo "Zip: $ZIPNAME"
else
	echo -e "\nCompilation failed!"
	exit 1
fi
