#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG6 - Incorrect freeing of live spinlocks"
echo "######################"
echo "######################"
echo "installing bug6 module"
sh ../install_module.sh 6
echo "running user space program to check bug6"
../xhw3 6
dmesg | tail
