#!/bin/bash
find ./test-levels -type f -iname '*.cgl' | sed "s/.*\///"| sed -e 's/\.[^./]*$//' | parallel ./run.sh {}