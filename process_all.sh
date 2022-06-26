#!/bin/bash
# all files
find ./test-levels -type f -iname '*.cgl' | sed "s/.*\///"| sed -e 's/\.[^./]*$//' | parallel ./run.sh {}
# A specific sequence
# parallel ./run.sh Level{} ::: {11..20} 