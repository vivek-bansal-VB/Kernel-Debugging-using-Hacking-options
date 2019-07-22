#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG4 - Deadlock Detection"
echo "######################"
echo "######################"
echo "installing bug4 module"
sh ../install_module.sh 4
echo "running user space program to check bug4"
../xhw3 4
dmesg | tail
