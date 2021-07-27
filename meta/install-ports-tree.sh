#!/usr/bin/env bash

PRANAOS_PORTS_DIR="${PRANAOS_SOURCE_DIR}/Build/${PRANAOS_ARCH}/Root/usr/ports"

for file in $(git ls-files "${PRANAOS_SOURCE_DIR}/ports"); do
    if [ "$(basename "$file")" != ".hosted_defs.sh" ]; then
        target=${PRANAOS_PORTS_DIR}/$(realpath --relative-to="${PRANAOS_SOURCE_DIR}/ports" "$file")
        mkdir -p "$(dirname "$target")" && cp "$file" "$target"
    fi
done