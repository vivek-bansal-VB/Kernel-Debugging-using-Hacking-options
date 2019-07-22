#!/bin/sh
set -x

# WARNING: this script doesn't check for errors, so you have to enhance it in case any of the commands below fail.
var="sys_bug"
var+=$1

lsmod
echo $var
rmmod $var
insmod $var.ko
lsmod
