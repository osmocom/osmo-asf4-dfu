#!/bin/sh

set -ex

if [ -z "$MAKE" ]; then
	set +x
	echo "Error: you need to set \$MAKE before invoking, e.g. MAKE=make"
	exit 1
fi

osmo-clean-workspace.sh

verify_value_string_arrays_are_terminated.py $(find . -name "*.[hc]")

cd gcc && $MAKE $PARALLEL_MAKE

osmo-clean-workspace.sh
