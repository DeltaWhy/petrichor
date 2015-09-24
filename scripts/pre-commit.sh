#!/bin/bash
files=$(find . \( '!' -path './cross/*' \) -and \( '!' -path './tools/*' \) -and \( -name '*.c' -or -name '*.h' -or -iname '*.s' \))
okay=0

# Check for trailing spaces
testfiles=$(grep -l -P '\s+$' $files)
if [ -n "$testfiles" ]; then
    echo "Trailing spaces (fixed automatically):"
    for f in $testfiles; do
        sed -i -r 's/\s+$//' $f || echo "Couldn't fix"
        echo -e "\t$f"
    done
    okay=1
fi

# Check for tabs in indentation
testfiles=$(grep -l -P '^\s*\t' $files)
if [ -n "$testfiles" ]; then
    echo "Tabs in indentation:"
    for f in $testfiles; do
        echo -e "\t$f"
    done
    okay=1
fi

exit $okay
