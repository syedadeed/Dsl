#!/bin/bash

mkdir build
cd build
gcc -c ../src/*
ar rcs libdsl.a *
for file in ../test/*; do
    name=$(basename "$file" .c)
    gcc "$file" -L . -l dsl -o "$name"
    ./"$name"
    if [ $? -ne 0 ]; then
        echo "$name failed"
        exit 1
    fi
done
mv libdsl.a ..
cd ..
rm -rf build
