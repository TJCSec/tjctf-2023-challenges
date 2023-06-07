# TODO add solution
enc = """8.'8*{;8m33[o[3[3[%")#*\\}"""

dec = ""

for i, c in enumerate(enc):
    c = ord(c)
    if c >= ord('a') - 60 and c <= ord('z') - 60:
        c += 60
    elif c >= ord('A') + 32 and c <= ord('Z') + 32:
        c -= 32
    elif c >= ord('0') + 43 and c <= ord('4') + 43:
        c -= 43
    elif c >= ord('5') - 21 and c <= ord('9') - 21:
        c += 21

    dec += chr(c)

print(dec)
