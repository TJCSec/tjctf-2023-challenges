from pwn import remote
from Crypto.Util.number import long_to_bytes, bytes_to_long
from ast import literal_eval

r = remote('localhost', 5000)

n = int(r.recvline().strip())
e = 65537


def sign(a):
    r.sendlineafter(b'Cmd: ', b'new')
    r.sendlineafter(b'Name: ', a)
    l = r.recvline().rsplit(b' ', 1)
    if a != literal_eval(l[0].decode()):
        print('error:', l[0].decode())
        return None
    return int(l[1].decode())


# chosen arbitrarily from all numbers coprime to n
for ff in range(2, 1000):
    hidden_admin = long_to_bytes((bytes_to_long(b"admin") * ff) % n)

    hidden_admin_sig = sign(hidden_admin)

    if hidden_admin_sig is None:
        continue

    recons = long_to_bytes(pow(ff, -1, n))
    recons_sig = sign(recons)

    if recons_sig is None:
        continue

    admin_sig = (hidden_admin_sig * recons_sig) % n

    print('bytes(admin_sig):', long_to_bytes(pow(admin_sig, e, n)))

    if long_to_bytes(pow(admin_sig, e, n)) != b'admin':
        continue

    r.sendlineafter(b'Cmd: ', b'login')
    r.sendlineafter(b'Name: ', b'admin')
    r.sendlineafter(b'Sign: ', str(admin_sig).encode())

    recved = r.recvuntil(b'happen...\n', timeout=1)

    if recved != b'':
        print(r.recvline().decode())
        r.close()
        break
