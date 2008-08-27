#!/bin/bash

convert $1.jpg $1.bmp
./robocr $1.bmp
