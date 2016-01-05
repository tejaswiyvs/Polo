#!/bin/sh
set -e

kill -9 $(ps aux | grep bochs | awk '{print $2}')
