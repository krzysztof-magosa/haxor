#! /bin/sh

# Fail on first error
set -e

for program in *.hax ; do
    echo "Compiling example $program ..."
    hc $program
done
