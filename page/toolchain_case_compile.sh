#!/bin/sh

# TODO: remove packages after install?

current_dir=$(pwd)
apk update
apk add libgcc curl
apk add -X http://dl-cdn.alpinelinux.org/alpine/edge/testing wabt

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
            curl https://sh.rustup.rs -sSf | sh -s -- -y &> /dev/null
            source ~/.cargo/env
            rustup toolchain install nightly
            rustup target add wasm32-unknown-unknown --toolchain nightly && rustc +nightly --target wasm32-unknown-unknown -O $file --crate-type=cdylib 
            ls -al
            ls -al src
            rm $file
            ;;
        *)
            echo -e "\nFound file $file\nFile extension not supported\n"
            ;;
    esac
done
