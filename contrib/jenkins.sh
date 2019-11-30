#!/bin/sh

if ! [ -x "$(command -v osmo-deps.sh)" ]; then
	echo "Error: We need to have scripts/osmo-deps.sh from http://git.osmocom.org/osmo-ci/ in PATH !"
	exit 2
fi

set -ex

if [ -z "$MAKE" ]; then
	set +x
	echo "Error: you need to set \$MAKE before invoking, e.g. MAKE=make"
	exit 1
fi

TOPDIR=`pwd`

publish="$1"

osmo-clean-workspace.sh

verify_value_string_arrays_are_terminated.py $(find . -name "*.[hc]")

cd gcc
$MAKE mrproper
$MAKE $PARALLEL_MAKE
$MAKE $PARALLEL_MAKE BOARD=SYSMOOCTSIM

if [ "x$publish" = "x--publish" ]; then
	echo
	echo "=============== UPLOAD BUILD  =============="

	cat > "/build/known_hosts" <<EOF
[rita.osmocom.org]:48 ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDDgQ9HntlpWNmh953a2Gc8NysKE4orOatVT1wQkyzhARnfYUerRuwyNr1GqMyBKdSI9amYVBXJIOUFcpV81niA7zQRUs66bpIMkE9/rHxBd81SkorEPOIS84W4vm3SZtuNqa+fADcqe88Hcb0ZdTzjKILuwi19gzrQyME2knHY71EOETe9Yow5RD2hTIpB5ecNxI0LUKDq+Ii8HfBvndPBIr0BWYDugckQ3Bocf+yn/tn2/GZieFEyFpBGF/MnLbAAfUKIdeyFRX7ufaiWWz5yKAfEhtziqdAGZaXNaLG6gkpy3EixOAy6ZXuTAk3b3Y0FUmDjhOHllbPmTOcKMry9
[rita.osmocom.org]:48 ecdsa-sha2-nistp256 AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBPdWn1kEousXuKsZ+qJEZTt/NSeASxCrUfNDW3LWtH+d8Ust7ZuKp/vuyG+5pe5pwpPOgFu7TjN+0lVjYJVXH54=
[rita.osmocom.org]:48 ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIK8iivY70EiR5NiGChV39gRLjNpC8lvu1ZdHtdMw2zuX
EOF
	SSH_COMMAND="ssh -o 'UserKnownHostsFile=/build/known_hosts' -p 48"
	rsync --archive --copy-links --verbose --compress --delete --rsh "$SSH_COMMAND" \
		$TOPDIR/gcc/bootloader-sysmooctsim.{bin,elf} \
			binaries@rita.osmocom.org:web-files/osmo-asf4-dfu/latest/
	rsync --archive --verbose --compress --rsh "$SSH_COMMAND" \
		$TOPDIR/gcc/bootloader-*-*.{bin,elf} \
			binaries@rita.osmocom.org:web-files/osmo-asf4-dfu/all/
fi
		#--exclude $TOPDIR/sysmoOCTSIM/gcc/bootloader-sysmooctsim.bin \
		#--exclude $TOPDIR/sysmoOCTSIM/gcc/bootloader-sysmooctsim.elf \

osmo-clean-workspace.sh
