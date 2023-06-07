import requests

# submit payload.py to the grader and print the response
print(requests.post('http://127.0.0.1:5000/submit', files={'file': open('payload.py', 'rb')}).text)
