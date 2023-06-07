from pwn import remote
from z3 import BitVec, Solver

n1 = BitVec('n1', 32)
n2 = BitVec('n2', 32)

s = Solver()

s.add((((n1 * 3) ^ 47342 - 534) % 1313) * 35 % 1515 == 1370)
s.add((((n2 * 5) % 7777 | 1111) * 35 - 5) % 1000 == 80)
s.add(n1 % n2 == 20202020)
s.add(n1 * n2 == 14590347874298541)

print(s.check())

print(s.model())

n1 = s.model()[n1].as_long()
n2 = s.model()[n2].as_long()

ADD_1 = b'ask her about some flags'
ADD_22 = b'ask her about her tour'
SQUARE = b'ask her about her new album'

io = remote('localhost', 5000)
io.sendline(b'sing')
io.sendline(b'maybe I asked for too [many challenges to be written]')
io.sendline(str(n1).encode())
io.sendline(str(n2).encode())

for dir in [ADD_22] * 2 + [ADD_1] * 4 + [SQUARE] + [ADD_22] * 2 + [ADD_1] * 3:
    io.sendline(dir)

io.interactive()
