# cube root attack
# or just rsactftool lol
from Crypto.Util.number import long_to_bytes

with open('bin/output.txt', 'r') as f:
    n = Integer(int(f.readline().split(' = ')[1]))
    e = Integer(int(f.readline().split(' = ')[1]))
    c = Integer(int(f.readline().split(' = ')[1]))

m = c ** (1/3)
print(long_to_bytes(int(m)).decode())
