#!/usr/bin/env bash

# usage: 
# ./pandafinshed.sh <samplestring> [resub]

if [ "$2" == "resub" ]
then
    python getJobs.py --user nwarrack --sample $1 --status finished --resub

else

    python getJobs.py --user nwarrack --sample $1 --status finished

fi
