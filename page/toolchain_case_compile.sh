#!/bin/sh

# TODO: remove packages after install?

current_dir=$(pwd)
export PATH="$HOME/.cargo/bin:$PATH"
apk update

for file in $current_dir/src/*
do
    case $file in
        *.c)
            echo -e "\nFound file $file\nInstalling clang toolchain\n"
            apk add clang lld
            clang --target=wasm32 --no-standard-libraries -Wl,--export-all -Wl,--no-entry -o "${file%.*}.wasm" $file
            rm $file
            ;;
        *.rs)
            echo -e "\nFound file $file\nInstalling rust toolchain\n"
            apk add rust rustup
            echo $PATH
            rustup --version && rustup target add wasm32-unknown-unknown && rustc --target wasm32-unknown-unknown $file -o "${file%.*}.wasm"
            rm $file
            ;;
        *)
            echo -e "\nFound file $file\nFile extension not supported\n"
            ;;
    esac
done
