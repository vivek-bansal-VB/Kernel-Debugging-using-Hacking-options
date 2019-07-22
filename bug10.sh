#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG10 - Hardened str/mem checks against overflows"
echo "######################"
echo "######################"
echo "installing bug10 module"
sh ../install_module.sh 10
echo "running user space program to check bug10"
../xhw3 10
dmesg | tail
