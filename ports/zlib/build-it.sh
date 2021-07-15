#!/bin/sh

cd sources
CROSS_PREFIX=i686-pc-pranaos- ./configure && make libz.a
cd ..