#!/bin/bash
cd $(dirname $0)
cd ..
files=$(find . \( '!' -path './cross/*' \) -and \( '!' -path './tools/*' \) -and \( -name '*.c' -or -name '*.h' \))
exec $(dirname $0)/indent.sh $files
