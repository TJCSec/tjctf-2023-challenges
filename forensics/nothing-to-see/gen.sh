#!/bin/sh
cp nothing_original.png nothing.png
exiftool -all= nothing.png
zip -P panda_d02b3ab3 hidden flag.txt
cat hidden.zip >> nothing.png
exiftool -title='panda_d02b3ab3' nothing.png
