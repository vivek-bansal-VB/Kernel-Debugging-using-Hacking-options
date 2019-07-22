#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG9 - Detect WorkQueue Stalls"
echo "######################"
echo "######################"
echo "installing bug9 module"
sh ../install_module.sh 9
echo "running user space program to check bug9"
../xhw3 9
dmesg | tail
