#!/bin/bash

if [ -z "$1" ]
then
	echo "usage: bash $0 <benchmark number> [input file] [multithreading]"
	exit 1
fi

macro_name="benchmark$1"
macro_path="$(dirname $0)/$macro_name.C"

if [ $# -lt 2 ]
then
	macro_call="$macro_name()"
elif [ $# -lt 3 ]
then
	macro_call="$macro_name(\"$2\")"
else
	macro_call="$macro_name(\"$2\", $3)"
fi

root -b -l <<EOF
try {
	.L $macro_path
	auto start = std::chrono::steady_clock::now();
	$macro_call;
	auto stop = std::chrono::steady_clock::now();
	std::chrono::duration<double> interval = stop - start;
	std::cout << interval.count() << " s" << std::endl;
} catch (...) {
	exit(1);
}
EOF
