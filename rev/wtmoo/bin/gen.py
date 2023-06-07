flag = "tjctf{wtMoo0O0o0o0a7e8f1}"
# flag = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789{}'

enc = ''

for c in flag:
    n = ord(c)
    if n >= ord('a') and n <= ord('z'):
        n -= 60
    elif n >= ord('A') and n <= ord('Z'):
        n += 32
    elif n >= ord('0') and n <= ord('4'):
        n += 43
    elif n >= ord('5') and n <= ord('9'):
        n -= 21
    enc += chr(n)

print(enc)
# print(bytes(enc, 'utf-8').hex())
