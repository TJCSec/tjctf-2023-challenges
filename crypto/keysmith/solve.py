from Crypto.Util.number import *
from sage.all import *
import random
from pwn import remote
# generate params

io = remote('localhost', 5000)

# generate params
N = 512
# p = 12934640009879975445271052725510835837384386524697861319216711604449640395036110691351696812789034716963233843017691320263399131946118976858469317269977081 #random_prime(2**N,proof=True)
# q = 789692583430622638193161598980060544381778760348291172401350358952093144361257426582640373301711464556349985646948894236188779133810882224870670913265579 #random_prime(2**N,proof=True)
# print("p",p)
# print("q",q)
# n = p * q
# e = 65537

m = int(io.recvline().decode().strip())
print("M:", m)
# d = inverse_mod(e,(p-1)*(q-1))
s = int(io.recvline().decode().strip())


print("S:", s)
# assert m == pow(s,d,n)

# solve


def get_prime(bits, exclude=[]):
    out = random_prime(2 ** (bits+1) - 1, proof=True, lbound=2 ** (bits))
    while out in exclude:
        bits += 1
        out = random_prime(2 ** (bits+1) - 1, proof=True, lbound=2 ** (bits))
    return out

# https://ctftime.org/writeup/32914


def getSmooth(numBits, exclude=[], smoothness=16):
    out = Integer(2)
    outL = [Integer(2)]
    ctr = Integer(4)
    while out.nbits() < numBits:
        r = get_prime(ctr, exclude+outL)
        out *= r
        outL.append(r)
        ctr += 1

    bitcnt = (numBits - out.nbits()) // 2
    # print("done part 1",bitcnt,outL,out)
    i = 0

    prime = 23

    while True:

        while prime in outL or prime in exclude:
            prime = next_prime(prime+1)
        # print(prime)
        if is_prime(1 + (out * prime)):
            outL.append(prime)
            out = 1+(out * prime)
            break
        prime = next_prime(prime+1)
    outL.sort()

    return out, outL


def isPrimitive(g, p, qs):
    # print(type(g),type(p),type(qs))
    return all([pow(int(g), (p-1)//q, p) != 1 for q in qs])

# print(to_str(762408622718930247757588326597223097891551978575999925580833))


# num = 419254694034365909780537506010574261143445959925355484501404896430836477164294906069087905199184505633906071112602724236508576024589244073540639163448071864288444159
# tot = Integer(pow(int(2),int(123123123),int(num)))
# print(discrete_log(2,tot,num))

"""
factors = []
total = 1
primes = []
staged_factors = []

orgbase = 21
new_s = 11721478752747238947534577901795278971298347908127389421908790123
base = 8999
while total < 2**1000:
    p, p_o_fac = getSmooth(N/5,factors)
    while not (isPrimitive(int(new_s),int(p),p_o_fac) and isPrimitive(int(base),int(p),p_o_fac)):
        print("p:",p)
        p, p_o_fac = getSmooth(N, factors)
    print("P:",p)
    factors += p_o_fac
    staged_factors.append(p_o_fac)
    primes.append(int(p))
    total *= p
print(factors,total,primes, staged_factors)


print("Done: ",base)
print(discrete_log(Mod(base,p),Mod(new_s,p),total))
dlps = []
for prime in primes:
    print(prime)
    prime = Integer(prime)
    dlps.append(Integer(discrete_log(Mod(Integer(new_s),prime),Mod(Integer(base),prime), prime-1)))
print(dlps,primes)
moduli = [p-1 for p in primes]
print(moduli)
print("Final:", base, crt(dlps,moduli), new_s, total)
"""

while True:

    try:
        p, p_o_fac = getSmooth(N)
        while not (isPrimitive(m, p, p_o_fac) and isPrimitive(s, p, p_o_fac)):
            p, p_o_fac = getSmooth(N)

        q, q_o_fac = getSmooth(N, p_o_fac)
        while not (isPrimitive(m, q, q_o_fac) and isPrimitive(s, q, q_o_fac)):
            q, q_o_fac = getSmooth(N, p_o_fac)

        # print(p,q)

        # print("Is prime?: ",p.is_prime(),q.is_prime(),p_o_fac,q_o_fac,[a for a in p_o_fac if a in q_o_fac])

        # print(discrete_log(Mod(4,11),Mod(5,11)))

        dp = discrete_log(Mod(m, p), Mod(s, p), p-1)
        # print(dp)
        dq = discrete_log(Mod(m, q), Mod(s, q), q-1)
        # print(dq)
        dprime = crt([dp, dq], [p-1, q-1])
        assert dprime % (p-1) == dp % (p - 1)
        nprime = p*q
        # print("d, n",dprime,nprime,(p-1)*(q-1))
        eprime = inverse_mod(dprime, (p-1)*(q-1))
        for i in range(100):
            r = random.randint(1, 2 ** (2*N))
            testEnc = pow(r, eprime, nprime)
            assert r == pow(testEnc, dprime, nprime)

        sprime = int(pow(m, eprime, nprime))
        mprime = int(pow(int(s), dprime, nprime))
        # print(sprime,s)
        # print(to_str(mprime),to_str(m))
        assert sprime == s
        assert mprime == m
        print("p'", p)
        print("q'", q)
        print("e'", eprime)

        io.sendline(str(p))
        io.sendline(str(q))
        io.sendline(str(eprime))

        io.interactive()
        break
    except ValueError:
        pass
