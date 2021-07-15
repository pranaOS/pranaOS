#!/bin/sh
VERSION=4.3
echo "Grabing FFMPEG version: $VERSION"
git clone --depth 1 --branch release/$VERSION https://git.ffmpeg.org/ffmpeg.git sources