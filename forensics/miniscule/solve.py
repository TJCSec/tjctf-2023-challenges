import zstandard
import zlib
"""
a png chunk is composed of several parts:
[data length- 4 bytes][chunk name- 4b][data][crc of data- 4b]
each image is composed of:
[PNG magic bytes][IHDR chunk][IDAT chunk][IEND chunk]
IEND should be the same everywhere
for IDAT chunks, data is compressed using zlib (compression method 0, all others are disallowed)
in the challenge, the data is compressed using zstandard
we need to change the compression type in the IHDR and replace the IDAT chunk with zlib data
we also need to fix the crc32 values for these chunks
"""

img_data = open('miniscule.png', 'rb').read()
signature, img_data = img_data[:8], img_data[8:]
img_data, iend = img_data[:-12], img_data[-12:]
ihdr, idat = img_data[:25], img_data[25:]

# change compression type to 0 and truncate crc, then add back crc
ihdr = ihdr[:ihdr.index(b'\x01')]+b'\x00'+ihdr[ihdr.index(b'\x01')+1:-4]
ihdr += zlib.crc32(ihdr[4:]).to_bytes(4, 'big')

 # truncate len, chunk name, and crc
idat = idat[8:-4]
# decompress the IDAT data with zstd, then recompress using zlib, and then join every part back together
# I will point out that the new file ends up being a decent amount larger, despite using the best zlib compression available
new_idat = zlib.compress(zstandard.decompress(idat), level=9)
new_idat = len(new_idat).to_bytes(4, "big")+b'IDAT'+new_idat+zlib.crc32(b'IDAT'+new_idat).to_bytes(4, 'big')

# then, join all the chunks in the correct order and write to a file
open('decoded.png', 'wb').write(signature+ihdr+new_idat+iend)
