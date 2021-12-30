#!/bin/bash
set -e

SOURCE_DIRECTORY=
SOURCE_DIRECTORY_PASSED=
PATCH_DIRECTORY=
PATCH_DIRECTORY_PASSED=

REVERSE=false

check_directory()
{
    if [ ! -e "$1" ]
    then
        echo "error: $2: no such file or directory"
        exit 1
    elif [ ! -r "$1" ]
    then
        echo "error: $2: not readable"
        exit 1
    elif [ ! -w "$1" ]
    then
        echo "error: $2: not writable"
        exit 1
    elif [ ! -d "$1" ]
    then
        echo "error: $2: not a directory"
        exit 1
    fi
}

for i in "$@"
do
    case $i in
        -s=* | --source_directory=*)
            SOURCE_DIRECTORY_PASSED="${i#*=}"
            SOURCE_DIRECTORY=$(realpath -s $SOURCE_DIRECTORY_PASSED)
            check_directory $SOURCE_DIRECTORY $SOURCE_DIRECTORY_PASSED

            shift
            ;;
        -p=* | --patch_directory=*)
            PATCH_DIRECTORY_PASSED="${i#*=}"
            PATCH_DIRECTORY=$(realpath -s $PATCH_DIRECTORY_PASSED)
            check_directory $PATCH_DIRECTORY $PATCH_DIRECTORY_PASSED

            shift
            ;;
        -r | --reverse)
            REVERSE=true

            shift
            ;;
        *)
            echo "error: invalid option '$i'";
            exit 1
            ;;
    esac
done

if [ -z "$SOURCE_DIRECTORY" ]
then
    echo "error: source directory not specified"
    exit 1
elif [ -z "$PATCH_DIRECTORY" ]
then
    echo "error: patch directory not specified"
    exit 1
fi

cd "$PATCH_DIRECTORY"
for FILE in $(find . -type f -printf "%P\n");
do
    if [ "$REVERSE" = true ]
    then
        set +e
        patch -uR --dry-run "$SOURCE_DIRECTORY/${FILE%.*}" < $PATCH_DIRECTORY/$FILE > /dev/null 2>&1

        if [ $? -eq 0 ]
        then
            echo "unpatching ${FILE%.*}"
            patch -uR $SOURCE_DIRECTORY/"${FILE%.*}" < $PATCH_DIRECTORY/$FILE > /dev/null 2>&1
        else
            echo "error: could not unpatch ${FILE%.*} because it has already been unpatched"
        fi
        set -e
    else
        set +e
        patch -u --dry-run $SOURCE_DIRECTORY/"${FILE%.*}" < $PATCH_DIRECTORY/$FILE > /dev/null 2>&1

        if [ $? -eq 0 ]
        then
            echo "patching ${FILE%.*}"
            patch -u $SOURCE_DIRECTORY/"${FILE%.*}" < $PATCH_DIRECTORY/$FILE > /dev/null 2>&1
        else
            echo "error: could not patch ${FILE%.*} because it has already been patched"
        fi
        set -e
    fi
done
