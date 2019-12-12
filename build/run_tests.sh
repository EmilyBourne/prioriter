#!/bin/bash

curdir=`dirname "$0"`/
echo $curdir

available_tests=$(ls ${curdir}tests)

if [ -z "available_tests" ]
then
    cd ..;
    make tests;
    available_tests=$(ls ${curdir}tests)
fi

for test in $available_tests
do
    echo "Running $test"
    ${curdir}tests/$test
done
