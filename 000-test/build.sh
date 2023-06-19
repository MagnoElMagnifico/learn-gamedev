#!/usr/bin/bash

# Check if SFML is installed
if [ "$(ldconfig -p | grep -c libsfml)" == "0" ]
then
    echo SFML is not installed
    echo Try installing libsfml-dev
    exit 1
fi

mkdir -p ../bin
g++ main.cpp -o ../bin/test -lsfml-graphics -lsfml-window -lsfml-system

