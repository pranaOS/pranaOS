#!/bin/sh

if ! whereis smlrc | cut -b 6- | grep -q "smlrc";
then
	echo "[NOTE] Installing the smaller-c compiler"
	
	PREVPWD=$(pwd)
	
	cd /opt
	
	git clone https://github.com/alexfru/SmallerC
	cd SmallerC
	mkdir build
	cd build
	../configure
	make
	make install
	
	cd $PREVPWD
else
	echo "[NOTE] You have the smaller-c compiler already installed"
fi
