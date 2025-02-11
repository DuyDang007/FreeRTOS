#!/bin/bash

if [ $# -lt 3 ]; then
    echo "Usage: $0 <CORE_SELECTION> <INPUT_FILE> <OUTPUT_FILE>"
    echo "CORE_SELECTION must be a number between 0 and 11."
    echo "INPUT_FILE is the source file."
    echo "OUTPUT_FILE is the generated file."
    exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "Error: CORE_SELECTION must be a valid number."
    exit 1
fi

if [ ! -f "$2" ]; then
    echo "Error: Input file '$2' does not exist."
    exit 1
fi

CORE_SELECTION=$1
INPUT_FILE=$2
OUTPUT_FILE=$3


if [ "$CORE_SELECTION" -ge 0 ] && [ "$CORE_SELECTION" -le 11 ]; then
    VRAM_SELECTION="vram_base_addr${CORE_SELECTION}"
else
    echo "Error: CORE_SELECTION must be between 0 and 11."
    exit 1
fi

sed "/REGION_ALIAS/s/, [^)]*)/, ${VRAM_SELECTION})/" "$INPUT_FILE" > "$OUTPUT_FILE"
echo "Generated $OUTPUT_FILE at build time with VRAM_SELECTION=$VRAM_SELECTION"
