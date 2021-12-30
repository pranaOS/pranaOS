#!/bin/bash

DIR=$(dirname $(readlink -f $0))

TARGET=x86_64
OPTION=toolchain

for i in "$@"
do
    case $i in
        -t=* | --target=*)
            TARGET="${i#*=}"

            shift

            ;;
        -o=* | --option=*)
            OPTION="${i#*=}"

            shift

            ;;
        *)
            echo "error: invalid option '$i'";
            exit 1
            ;;
    esac
done

if [ "$OPTION" = "toolchain" ]; 
then
    echo $DIR/local/bin/:$PATH
else
    echo $DIR/build/gcc/$TARGET/libgcc
fi
