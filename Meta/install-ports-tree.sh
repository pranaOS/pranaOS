#!/usr/bin/env bash

PRANAOS_PORTS_DIR="${PRANAOS_SOURCE_DIR}/Build/${PRANAOS_ARCH}/Root/usr/Ports"

for file in $(git ls-files "${PRANAOS_SOURCE_DIR}/Ports"); do
    if [ "$(basename "$file")" != ".hosted_defs.sh" ]; then
        target=${PRANAOS_PORTS_DIR}/$(realpath --relative-to="${PRANAOS_SOURCE_DIR}/Ports" "$file")
        mkdir -p "$(dirname "$target")" && cp "$file" "$target"
    fi
done