#!/bin/bash
for filename in $1/*.txt; do
    #echo $filename
    sed -i '1!d' $filename
done
