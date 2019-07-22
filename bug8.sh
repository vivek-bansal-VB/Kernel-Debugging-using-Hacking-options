#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG8 - Slub Debugging"
echo "######################"
echo "######################"
echo "installing bug8 module"
sh ../install_module.sh 8
echo "running user space program to check bug8"
../xhw3 8
dmesg | tail
