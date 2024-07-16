#!/usr/bin/env bash

tests=$(ls tests | grep "\-in")
for input in $tests; do
	# output=$(echo "$input" | sed 's/-in/-out/')
	output=${input//-in/-out} # This is suggested by style guide of shellcheck
	valgrind ./snake -i "$input" -o "$output" 2>&1 | tail -n 1
done

