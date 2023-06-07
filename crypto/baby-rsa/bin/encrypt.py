from Crypto.Util.number import bytes_to_long, getPrime

with open('flag.txt', 'rb') as f:
    m = bytes_to_long(f.read().strip())

e = 3

p = getPrime(1024)
q = getPrime(1024)

n = p * q
c = pow(m, e, n)

print('n =', n)
print('e =', e)
print('c =', c)
