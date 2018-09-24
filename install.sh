#!/bin/bash
dir=$(pwd)

mv $dir/bin/lib* /usr/local/lib/
cp $dir/src/include/* /usr/local/include/