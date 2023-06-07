from sage.crypto.util import ascii_to_bin
from sage.crypto.util import bin_to_ascii
import random
#generate params

def to_str(i):
    b = bin(int(i))[2:]
    #print(len(b))
    return bin_to_ascii(b.zfill(8+8*(len(b)//8)))

def to_int(f):
    return Integer(int(str(ascii_to_bin(f)),2))


##generate params
N = 512
#p = 12934640009879975445271052725510835837384386524697861319216711604449640395036110691351696812789034716963233843017691320263399131946118976858469317269977081 #random_prime(2^N,proof=True)
#q = 789692583430622638193161598980060544381778760348291172401350358952093144361257426582640373301711464556349985646948894236188779133810882224870670913265579 #random_prime(2^N,proof=True)
#print("p",p)
#print("q",q)
#n = p * q
#e = 65537
msg = "yummy yummy chicken feet!"
m = to_int(msg)
print("M:", m)
#d = inverse_mod(e,(p-1)*(q-1))
s = 7058603077944415431501228273585531037657606183606056542470080140657571772988310725694981664370863968723582338792805917740600784597453152499595421410499694210546896647963325381264213050145581644387764711380282239276288377694797819705922664791803606589626934892393121799096737152539616319838089280947412489242 
print("S:",s)
#assert m == pow(s,d,n)

#solve
def get_prime(bits,exclude = []):
    out = random_prime(2^(bits+1) - 1,proof=True,lbound=2^(bits))
    while out in exclude:
        bits += 1
        out = random_prime(2^(bits+1) - 1,proof=True,lbound=2^(bits))
    return out

#https://ctftime.org/writeup/32914
def getSmooth(numBits,exclude=[],smoothness = 16):
    out = 2
    outL = [2]	
    ctr = 4
    while out.nbits() < numBits:
        r = get_prime(ctr,exclude+outL)
        out *= r
        outL.append(r)
        ctr += 1	

    bitcnt = (numBits - out.nbits()) // 2
    #print("done part 1",bitcnt,outL,out)
    i = 0

    prime = 23
    
    while True:
            
        while prime in outL or prime in exclude:
            prime = next_prime(prime+1)
        #print(prime)
        if is_prime(1 + (out * prime)):
            outL.append(prime)
            out = 1+(out * prime)
            break
        prime = next_prime(prime+1)
    outL.sort()
    
    return out, outL

print("\nSmooth: ",getSmooth(250),"\n")

def isPrimitive(g,p,qs):
    #print(type(g),type(p),type(qs))
    return all([pow(int(g),(p-1)//q,p) != 1 for q in qs]) 

#print(to_str(762408622718930247757588326597223097891551978575999925580833))


#num = 419254694034365909780537506010574261143445959925355484501404896430836477164294906069087905199184505633906071112602724236508576024589244073540639163448071864288444159
#tot = Integer(pow(int(2),int(123123123),int(num)))
#print(discrete_log(2,tot,num))


factors = []
total = 1
primes = []
staged_factors = []
numbits = 25
orgbase = 21
new_s = 11721478752747238947534577901795278971298347908127389421908790123
base = 8999
while total < 2^1000:
    p, p_o_fac = getSmooth(numbits,factors)
    while not (isPrimitive(int(new_s),int(p),p_o_fac) and isPrimitive(int(base),int(p),p_o_fac)):
        print("p:",p)
        p, p_o_fac = getSmooth(numbits, factors)
    print("P:",p)
    factors += p_o_fac
    staged_factors.append(p_o_fac)
    primes.append(int(p))
    total *= p
print(factors,total,primes, staged_factors)

g = 8999 
s = 11721478752747238947534577901795278971298347908127389421908790123 
p = 12297383901740584470151577318651150337988716807049317851420298478128932232846789427512414204247770572072680737351875225891650166807323215624748551744377958007176198392481481171792078565005580006750936049744616851983231170824931892761202881982041842121034608612146861881334101500003915726821683000760611763097

print("Done: ",base)

print(factor(total))

R = Integers(p)
print('tjctf{' + str(R(s).log(R(g))) + '}')

print(discrete_log(Mod(s,p), Mod(g,p), p-1))
dlps = []
for prime in primes:
    print(prime)
    prime = Integer(prime)
    dlps.append(Integer(discrete_log(Mod(Integer(new_s),prime),Mod(Integer(base),prime), prime-1)))
print(dlps,primes)
moduli = [p-1 for p in primes]
print(moduli)
print("Final:", base, crt(dlps,moduli), new_s, total)



