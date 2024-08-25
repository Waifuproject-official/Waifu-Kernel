#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# clang-version [-p] clang-command
#
# Prints the compiler version of `clang-command' in a canonical 4-digit form
# such as `0500' for clang-5.0 etc.
#
# With the -p option, prints the patchlevel as well, for example `050001' for
# clang-5.0.1 etc.
#

<<<<<<< HEAD
if [ "$1" = "-p" ] ; then
	with_patchlevel=1;
	shift;
fi

compiler="$*"

if [ ${#compiler} -eq 0 ]; then
	echo "Error: No compiler specified."
	printf "Usage:\n\t$0 <clang-command>\n"
=======
compiler="$*"

if ! ( $compiler --version | grep -q clang) ; then
	echo 0
>>>>>>> v4.19.83
	exit 1
fi

MAJOR=$(echo __clang_major__ | $compiler -E -x c - | tail -n 1)
MINOR=$(echo __clang_minor__ | $compiler -E -x c - | tail -n 1)
<<<<<<< HEAD
if [ "x$with_patchlevel" != "x" ] ; then
	PATCHLEVEL=$(echo __clang_patchlevel__ | $compiler -E -x c - | tail -n 1)
	printf "%02d%02d%02d\\n" $MAJOR $MINOR $PATCHLEVEL
else
	printf "%02d%02d\\n" $MAJOR $MINOR
fi
=======
PATCHLEVEL=$(echo __clang_patchlevel__ | $compiler -E -x c - | tail -n 1)
printf "%d%02d%02d\\n" $MAJOR $MINOR $PATCHLEVEL
>>>>>>> v4.19.83
