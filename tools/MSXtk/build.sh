#!/bin/bash

[ -d "bin" ] || mkdir bin

make -C ./src all
make -C ./src install
make -C ./src clean
