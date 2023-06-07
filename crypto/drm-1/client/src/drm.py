import requests
import sys

if len(sys.argv) != 2:
    print("Usage: drm.pyc <url>")
    exit(1)

meta = open("meta.dat", "r").read().strip()
hash = open("hash.dat", "r").read().strip()

url = sys.argv[1]

get_data_url = f"{url}/unlock/{meta}/{hash}"

get_song_url = f"{url}/song/daniel-kpdfgo"

recved = requests.get(get_data_url).text.split(" ")
song = requests.get(get_song_url).text

if len(recved) == 1:
    print(recved[0])
elif len(recved) != 2:
    print(" ".join(recved))
else:
    key = bytes.fromhex(recved[0])
    nonce = bytes.fromhex(recved[1])
    open("drm.key", "wb+").write(key)
    open("drm.nonce", "wb+").write(nonce)
    open("enc_song.dat", "wb+").write(bytes.fromhex(song))
    print("successful")
