#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/bitjackcoin.png
ICON_DST=../../src/qt/res/icons/bitjackcoin.ico
convert ${ICON_SRC} -resize 16x16 bitjackcoin-16.png
convert ${ICON_SRC} -resize 32x32 bitjackcoin-32.png
convert ${ICON_SRC} -resize 48x48 bitjackcoin-48.png
convert bitjackcoin-16.png bitjackcoin-32.png bitjackcoin-48.png ${ICON_DST}

