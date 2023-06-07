import requests
from Crypto.Cipher import ChaCha20
import hlextend
from Crypto.Hash import SHA256
import time

url = 'http://localhost:5000'


with open('../client/meta.dat', 'r') as f:
    meta = bytes.fromhex(f.read().strip())

with open('../client/hash.dat', 'r') as f:
    hash = f.read().strip()

sha = hlextend.new('sha256')
new_meta = sha.extend(
    f',made:{time.time()},user:daniel-kpdfgo'.encode(), meta, 32, hash)

print(requests.get(
    f'{url}/unlock/{new_meta.hex()}/{sha.hexdigest()}').text)
