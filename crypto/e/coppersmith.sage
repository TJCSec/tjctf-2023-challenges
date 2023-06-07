from Crypto.Util.number import bytes_to_long, long_to_bytes
import time


#https://www.cryptologie.net/article/222/implementation-of-coppersmith-attack-rsa-attack-using-lattice-reductions/

"""
Coppersmith revisited by Howgrave-Graham

finds a solution if:
* b|modulus, b >= modulus^beta , 0 < beta <= 1
* |x| < XX
"""
def coppersmith_howgrave_univariate(pol, modulus, beta, mm, tt, XX):
    dd = pol.degree()
    nn = dd * mm + tt

    if not 0 < beta <= 1:
        raise ValueError("beta should belong in (0, 1]")

    if not pol.is_monic():
        raise ArithmeticError("Polynomial must be monic.")

    # change ring of pol and x
    polZ = pol.change_ring(ZZ)
    x = polZ.parent().gen()

    # compute polynomials
    gg = []
    for ii in range(mm):
        for jj in range(dd):
            gg.append((x * XX)**jj * modulus**(mm - ii) * polZ(x * XX)**ii)
    for ii in range(tt):
        gg.append((x * XX)**ii * polZ(x * XX)**mm)
    
    # construct lattice B
    BB = Matrix(ZZ, nn)

    for ii in range(nn):
        for jj in range(ii+1):
            BB[ii, jj] = gg[ii][jj]

    # LLL
    BB = BB.LLL()

    # transform shortest vector in polynomial    
    new_pol = 0
    for ii in range(nn):
        new_pol += x**ii * BB[0, ii] / XX**ii

    # factor polynomial
    potential_roots = new_pol.roots()
    print("potential roots:", potential_roots)

    # test roots
    roots = []
    for root in potential_roots:
        if root[0].is_integer():
            result = polZ(ZZ(root[0]))
            if gcd(modulus, result) >= modulus^beta:
                roots.append(ZZ(root[0]))

    return roots



length_N = 1300  # size of the modulus

# brute force K bits
for numbytes in range(5,15):
    print("Trying",numbytes,"bytes...")
    Kbits = numbytes * 8

    known = bytes_to_long(b'the challenges flag is tjctf{')

    with open('bin/output.txt') as f:
        N = Integer(int(f.readline().rsplit(' ', 1)[1]))
        C = Integer(int(f.readline().rsplit(' ', 1)[1]))
        e = Integer(int(f.readline().rsplit(' ', 1)[1]))
    
    ZmodN = Zmod(N)

    # Problem to equation (default)
    P.<x> = PolynomialRing(ZmodN) #, implementation='NTL')
    pol = ((known * 2^Kbits) + x)^e - C
    dd = pol.degree()

    # Tweak those
    beta = 1                                # b = N
    epsilon = beta / 7                      # <= beta / 7
    mm = 4*ceil(beta**2 / (dd * epsilon))     # optimized value
    tt = 4*floor(dd * mm * ((1/beta) - 1))    # optimized value
    XX = ceil(known + (2^(Kbits+1)))  # optimized value

    # Coppersmith
    start_time = time.time()
    roots = coppersmith_howgrave_univariate(pol, N, beta, mm, tt, XX)

    # output
    if len(roots) > 0:
        print(long_to_bytes(roots[0]))
        break

