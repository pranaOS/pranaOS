#!/bin/sh

img="disk.img"
path="/tmp/pranaos"

mkdir -p $path
echo "Mounting $img in $path"
python run/fuse.py $img $path