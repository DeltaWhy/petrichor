#!/bin/sh
exec astyle --suffix=none --style=kr -m2 --pad-header --align-pointer=name -k3 -xC120 --mode=c -z2 -o "$@"
