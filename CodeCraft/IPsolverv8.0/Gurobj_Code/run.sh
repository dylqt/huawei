#!/bin/bash
#CopyRight by Feiping Wang

rm -fr bin
mkdir bin
mkdir bin/code
rm -fr build
mkdir build

make clean
make

cp -r future_net/ ./bin/code/
cd bin
tar -zcPf future_net.tar.gz *
cp future_net.tar.gz ../
rm -rf code
rm -f future_net.tar.gz