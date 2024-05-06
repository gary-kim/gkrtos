#!/usr/bin/env bash

set -euf -o pipefail

# This line will only work in scripts and not sourced bash scripts.
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
cd "$SCRIPTPATH/.."

if [ $# -gt 0 ] && [ "$1" == "flash" ]; then
    openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program {tmp/gkrtos.bin} 0x10000000 verify; reset init; resume; shutdown;"
fi

openocd -f interface/cmsis-dap.cfg -c "adapter speed 5000" -f target/rp2040.cfg -s tcl #2>/dev/null > /dev/null

