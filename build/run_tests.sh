#!/bin/bash

curdir=`dirname "$0"`/

available_tests=$(ls ${curdir}tests)

if [ -z "$available_tests" ]
then
    cd $curdir/..;
    make tests;
    if [ $? -ne 0 ]
    then
        echo "Could not compile"
        exit 1
    fi
    available_tests=$(ls ${curdir}tests)
fi

result=0

for test in $available_tests
do
    echo "Running $test"
    ${curdir}tests/$test
    if [ $? -ne 0 ]
    then
        result=1
    fi
done

exit $result
