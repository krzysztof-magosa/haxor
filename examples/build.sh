#! /bin/sh

# Fail on first error
set -e

for program in *.hax ; do
    echo "Compiling example $program ..."
    ../bin/hcc $program
    ../bin/hld -o $program.e $program.u
done
