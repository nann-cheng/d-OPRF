#!/bin/bash

if [ -z "$1" ]
then
    make clean > /dev/null
    make > /dev/null

    ./main64
    ./main128
    ./main192
    ./main256
    ./main512
else
    if [ "$1" = "FAST" ]
    then
        make clean > /dev/null
        make OPT_LEVEL=FAST > /dev/null

        ./main64
        ./main128
        ./main192
        ./main256
        ./main512
    elif [[ "$1" =~ "^64$|^128$|^192$|^256$|^512$" ]]
    then
        if { [ "$2" = "FAST" ] && [ "$3" = "S" ]; } || { [ "$2" = "S" ] && [ "$3" = "FAST" ]; }        then
            make clean > /dev/null
            make main$1 OPT_LEVEL=FAST GENERATE_ASSEMBLY_CODE=YES > /dev/null

            ./main$1
        elif [ "$2" = "FAST" ]
        then
            make clean > /dev/null
            make main$1 OPT_LEVEL=FAST > /dev/null

            ./main$1
        elif [ "$2" = "S" ]
        then
            make clean > /dev/null
            make main$1 GENERATE_ASSEMBLY_CODE=YES > /dev/null

            ./main$1
        else
            make clean > /dev/null
            make main$1 > /dev/null

            ./main$1
        fi
    else
        echo "Invalid option. Please use FAST, S or a number from the list: 64, 128, 192, 256, 512."
    fi
fi

