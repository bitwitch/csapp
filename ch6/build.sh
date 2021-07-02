#!/bin/bash

getopts p flag

if [ "${flag}" = p ] 
then 
    gcc -Og -pg -Wno-incompatible-pointer-types -o prog problems.c
else
    gcc -Og -Wno-incompatible-pointer-types -o prog problems.c
fi

