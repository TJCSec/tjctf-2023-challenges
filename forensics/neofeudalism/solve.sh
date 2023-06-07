#!/bin/sh
# the flag is encoded in the least significant bit of the red channel
# zsteg -a also works, but this gives a single output
zsteg b1,r,msb,yx image.png