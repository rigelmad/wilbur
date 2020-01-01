#!/bin/bash

set -e

cd $1

DIR=$(pwd)


 arduino-cli compile $DIR -b arduino:avr:uno 
 arduino-cli upload $DIR -b arduino:avr:uno -p /dev/ttyUSB0 -t -v

 rm $DIR/*.elf
 rm $DIR/*.hex

