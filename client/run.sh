#!/bin/bash

readonly scriptdir=$(realpath $(dirname $0))

LD_LIBRARY_PATH=${scriptdir}/../build/fts-c/src/fts-c/ \
    python3 ${scriptdir}/main.py