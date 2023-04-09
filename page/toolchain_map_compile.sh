#!/bin/bash

current_dir=$(pwd)

for file in $current_dir/src/*
do
    if [[ "${file##*.}" == "c" || "${file##*.}" == "rs" ]]
    then
        if [[ "${file##*.}" == "c" ]]
        then
            echo "compiling $file"
            clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o "${file%.*}.wasm" $file
            rm $file

        elif [[ "${file##*.}" == "rs" ]]
        then
            echo "NOT IMPLEMENTED"
            # rustc $file -o "${file%.*}"
        fi
    fi
done
