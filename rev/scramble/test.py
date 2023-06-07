l4 = [84, 125, 102, 38, 102, 52, 114, 112, 25, 113, 103, 21, 73, 64, 233, 38, 94, 66, 98, 69, 72, 93, 117, 79, 92, 17, 101, 106, 107, 40, 124]
flg = "flag{unshuffling_scripts_xdfjk}";
flg2 = "tjctf{unshuffling_scripts_xdfj}";
l5 = [];
for i in range(len(flg)):
    l5.append(l4[i]^ord(flg[i])^ord(flg2[i]));
print(l5);
