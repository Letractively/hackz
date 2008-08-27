#!/bin/bash

convert $1.jpeg  robocr/$1.bmp

rm $1.jpeg

cd robocr

./robocr $1.bmp
rm $1.bmp
