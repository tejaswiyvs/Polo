#!/bin/sh
set -e
. ./iso.sh

~/Downloads/bochs-2.6.8/bochs -f bochsrc.txt -q
