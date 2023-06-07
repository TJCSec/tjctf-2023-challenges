a = "0.000118125 4.75e-05 0.0011025 0.00386812 1.96875e-05 0.00797781 0.00193844 2.34375e-05 0.00533406 0.000460625 0.000490625 6.125e-05 4.65625e-05 0.00174969 0.00192531 0.00154844 0.00453531 0.000188125 0.000248125 0.0254422 0.000185938 0.000659375 0.00623031 0.00168156 0.13425 4.5625e-05 0.000416875 0.00548094 8.1875e-05 0.00103188 0.00178312 0.00127156 0.0194225 0.000398125 0.000583125 0.00068 0.00539969 7.5e-06 0.000165937 0.000131562 0.00222125 1.78125e-05 2.59375e-05 0.000525937 0.000144375 0.00303813 0.0110662 0.00028625 0.000122813 0.000653438 2.625e-05 0.00134563 0.0308972 0.00058625 2.625e-05 0.0119359 2.5625e-05 0.00256469 0.00810437 0.000267188 0.000672187 0.000575938 0.00401656 3.84375e-05 0.000464375 0.00137563 8.34375e-05 0.0103269 0.00380437 0.000265938 0.00185938 0.00020875 0.003015 0 0.0231638 0.00110406 0.000213437 0.000150312 2.03125e-05 0.0009825 3.125e-06 0.00025125 1.875e-06 0.000148125 0.000140313 0.000437187 0.00555406 2e-05 8.4375e-05 0.000184375 0.0171547 8.21875e-05 8.40625e-05 2.59375e-05 0.000572187 0.000575312 0.000883125 3.53125e-05 0.0362706 0.00357094 0.000129687 0.0390141 0.000205 0.000516875 2.09375e-05 0.000145625 4.78125e-05 0.00339531 0.00026375 0.00119313 0.000325937 0.00065375 0.00255688 0.00114188 0.000799375 0.000698438 0.00114 0.000342813 0.000529375 1.03125e-05 0.000106875 0.000135312 5e-06 0.000480938 9.3125e-05 0.0384375 0.000274375 0.00202688 0.00040375 0.0359847 0.00204031 2.875e-05 9e-05 0.000599375 0.000281563 9.40625e-05 0.000314375 2.28125e-05 0.000255625 0.000749687 0.0018525 0.0122888 0.00139281 0.000112813 0.000162187 0.000244062 0.000165625 0.000359687 0.000623438 0.0175162 0.000921875 0.000151875 0.000260625 9.71875e-05 0.0002725 5e-06 0.0429397 0.000925937 7.84375e-05 0.000797812 0.00771313 8e-05 2.25e-05 0.00117031 0.000116875 0 0.000250312 0.00775438 0.00188875 0.000362813 0.00173719 0.00179625 1e-05 0.0105062 0.000305 0.000140625 7.40625e-05 0.000690938 0.000483125 0.00140375 0.000190312 7.78125e-05 0.00176156 9.125e-05 0.00566875 0.00019375 0.000355937 0.00887969 0.000115938 0.000104687 2.25e-05 0.00035125 6.5625e-06 0.00019375 0.000728438 8.15625e-05 0.000256875 0.00278 0.00184031 0.00072875 0.000276875 0.000220937 4.375e-06 5.03125e-05 7.84375e-05 5.25e-05 0.0051925 0.000466875 4.0625e-06 2.34375e-05 0.00345094 0.00650688 0.0109109 0.0189731 0.00826125 0.00362156 1.25e-06 2.71875e-05 4.375e-06 3.96875e-05 0.000914688 0 0.000135938 0.000254062 7.21875e-05 2.875e-05 4.4375e-05 0.0114422 0.000126875 4.625e-05 1.25e-06 7.75e-05 0.000457813 0.00154437 0.00324281 5.875e-05 9.3125e-05 0.0619191 2.46875e-05 3.6875e-05 0.000406562 9.3125e-05 0.00177938 5.78125e-05 0.0275081 0.00021125 0.0234 0.0013025 0.000480313 0.000167188 0.0896347 0.00164937 0.00593 0.000450938 0.000473438 0.000770313"
b = [float(s) for s in a.split(" ")]
c = [a for a in b]
c.sort()
c = c[::-1]
chrs = "abcdefghijklmnopqrstuvwxyz_{}"


def getnth(num, cnt):
    i = 0
    cnt += 1
    while cnt:
        if b[i] == num:
            cnt -= 1
        i += 1
    return i-1


def trysolve(s):
    outl = []
    for byte in s:
        out = ""
        skipfrq = []
        for f in c:
            char = getnth(f, skipfrq.count(f))
            skipfrq.append(f)

            pt = char ^ byte
            if chr(pt) in chrs:
                out += chr(pt)
        outl.append(out)
    return outl


path = "./bin/flag"

ls = []
for i in range(3):
    s = str(i)
    if i == 0:
        s = ""
    ls.append(open(path + s+".txt.enc", "rb").read())


slv = [trysolve(l) for l in ls]
ranks = [[0 for j in range(29)] for i in range(34)]

for l in ls:
    ranked = trysolve(l)
    for pos in range(len(ranked)):
        for letter in ranked[pos]:
            ranks[pos][chrs.index(letter)] += ranked[pos].index(letter)
words = open("10k.txt", "r").read().lower().split("\n")
words = [word for word in words if len(word) > 3]
words.append("tjctf{")
words.append("_")
words.append("of")
words.append("and")
words.append("but")
words.append("if")
print(ranks)

print("".join([chrs[rank.index(min(rank))] for rank in ranks]))


def rank_guess(ranks, guessed_word):
    if len(ranks) < len(guessed_word):
        return -1
    currRank = 0
    for charI in range(len(guessed_word)):
        char = guessed_word[charI]
        currRank += ranks[charI][chrs.index(char)]
    return currRank


nodes = 0


def guess(ranks, parent, depth=0):
    poss = []
    global nodes
    nodes += 1
    if (depth > 8):
        return [""]
    if len(ranks) < 2:
        return [""]

    for guessed_word in words:
        if parent != "_":
            poss.append("_")
            break
        if guessed_word == parent and parent == "_":
            continue
        currRank = rank_guess(ranks, guessed_word)
        if (currRank < 0):
            continue
        if currRank < 13 * len(guessed_word):
            # print(depth,poss)
            poss.append(guessed_word)
    out = []
    for possibility in poss:
        other = guess(ranks[len(possibility):], possibility[-1], depth+1)
        # print(other)
        for oth in other:
            if len(oth) == 0:
                out.append(possibility+oth)
                continue
            # if oth[0] == "_" and possibility[-1] == "_":
            #    continue

            # if oth[0] != "_" and possibility[-1] != "_":
            #    continue
            out.append(possibility+oth)
    return out


print(rank_guess(ranks, "tjctf{"))
print(guess(ranks[len("tjctf{"):], "_"))
print(nodes)
