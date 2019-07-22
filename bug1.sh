#!/bin/sh

echo "######################"
echo "######################"
echo "Running code to trigger BUG1 - Kernel Memory Leak Detector"
echo "######################"
echo "######################"
echo "installing bug1 module"
sh ../install_module.sh 1
echo "running user space program to check bug1"
../xhw3 1
echo scan > /sys/kernel/debug/kmemleak
echo "sleeping for 60s"
sleep 60s
cat /sys/kernel/debug/kmemleak
