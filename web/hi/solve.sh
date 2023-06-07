#!/bin/sh
# the flag is just an SVG file, which is text, so we can download it and parse out the flag
curl http://localhost:80/secret-b888c3f2.svg | python3 -c 'svg = input(); print(svg[svg.index("tjctf"):svg.index("}")+1])'
# python3 -c 'svg = input(); print(svg[svg.index("tjctf"):svg.index("}")+1])' < server/secret-b888c3f2.svg