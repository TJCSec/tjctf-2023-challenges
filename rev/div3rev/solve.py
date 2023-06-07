def op1(b):
    if(b%2==0):
        b-=8;
    return b;
def op2(b):
    b-=12;
    return b;
def op3(b):
    cur = b&(1<<7);
    cur>>=7;
    if(b>=128):
        b -=128;
    b = b<<1;
    b+=cur;
    return b;
def recur(b):
    for i in range(len(b)):
        if((i%27)//9==0):
            b[i] = op1(b[i]);
        if((i%27)//9==1):
            b[i] = op2(b[i]);
        if((i%27)//9==2):
            b[i] = op3(b[i]);
        if((i%9)//3==0):
            b[i] = op1(b[i]);
        if((i%9)//3==1):
            b[i] = op2(b[i]);
        if((i%9)//3==2):
            b[i] = op3(b[i]);
        if(i%3==0):
            b[i] = op1(b[i]);
        elif(i%3==1):
            b[i] = op2(b[i]);
        else:
            b[i] = op3(b[i]);
        print(chr(b[i]), end="");
b = bytearray(b'\x8c\x86\xb1\x90\x86\xc9=\xbe\x9b\x80\x87\xca\x86\x8dKJ\xc4e?\xbc\xdbC\xbe!Y \xaf')
res = recur(b);
