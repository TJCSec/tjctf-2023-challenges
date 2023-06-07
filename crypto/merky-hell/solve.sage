from Crypto.Util.number import long_to_bytes
from Crypto.Util.Padding import pad, unpad
from Crypto.Cipher import AES
from ast import literal_eval as eval

with open('bin/output.txt', 'r') as f:
    lines = f.readlines()
    B = eval(lines[0].split('B = ')[1].strip())
    E = int(lines[1].split('msg = ')[1].strip())
    iv = bytearray.fromhex(lines[2].split('iv = ')[1].strip())
    ct = bytearray.fromhex(lines[3].split('ct = ')[1].strip())

l = len(B)

arr = [[1 if i == j else 0 for i in range(l+1)] for j in range(l+1)]

arr[l][l] = -E

for i in range(l):
    arr[i][l] = Integer(B[i])

M = Matrix(QQ, l+1, l+1, arr)

sol = M.LLL()
# print(sol)
ssn = 0
# this is the super secret number from the encryption
for row in sol:
    if any([a != 0 and a != 1 for a in row]):
        continue

    ssn = int("".join([str(a) for a in row])[:-1], 2)
    break

key = pad(long_to_bytes(ssn), 16)
cipher = AES.new(key, AES.MODE_CBC, iv=iv)
print(unpad(cipher.decrypt(ct), 16).decode())
