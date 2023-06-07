flag = "\022\021\000\025\vH<\022\fD\000\020Q\031.\026\003\034B\021\nJrV\rztO"

real = list("tjct____________________________")

for i in range(4, 32):
    real[i] = chr(ord(flag[i-4]) ^ ord(real[i-4]))

print("".join(real))
