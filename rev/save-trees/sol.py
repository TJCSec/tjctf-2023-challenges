import hashlib
from pwn import *
from ast import literal_eval
from Crypto.Util.number import bytes_to_long, long_to_bytes
import random

io = remote('localhost', 5000)

# solution starts

threshold = 128
io.recvuntil(b'trees!!\n')
edge_lst = literal_eval(io.recvuntil(b']').decode())
lv_list = literal_eval(io.recvuntil(b']').decode())

fkey = 0

adj_list = [[] for _ in range(16*threshold)]
for (a, b) in edge_lst:
    adj_list[a].append(b)
    adj_list[b].append(a)


def dfs(nd, anc):
    children = []
    for child in adj_list[nd]:
        if child != anc:
            dfs(child, nd)
            children.append(child)
    children.sort()
    # left child has c[16:]
    # right child has c[:16]
    if children:
        ans[nd] = (((ans[children[0]] ^ fkey) << 16) +
                   (ans[children[1]] ^ fkey))


base = bytes_to_long(bytes('save thr trees!!', 'utf-8')) << 16
for o_s in range(1 << 15, 1 << 16):
    fkey = base + o_s
    ans = [0 for _ in range(16*threshold)]
    for lv, val in lv_list:
        ans[lv] = val
    dfs(0, -1)
    if hex(ans[0]).startswith('0x10000000'):
        io.sendline(str(ans[0]).encode())
        print(io.recvuntil(b'}').decode().split()[-1])
