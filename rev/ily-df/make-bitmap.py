from PIL import Image
from io import BytesIO
from PIL import ImageFilter
from PIL import ImageEnhance
  
# open method used to open different extension image file
im = Image.open(r"flag.png")

grayimg = im.convert('L') 
# This method will show image in any image viewer 
#grayimg.show() 
arr = []
grayimg = grayimg.resize((120, 26),Image.BICUBIC)
for x in range(grayimg.width):
    arr.append([])
    for y in range(grayimg.height):
        arr[-1].append(0)
        if grayimg.getpixel((x,y))< 200:
            arr[x][y] = 1
        else:
            arr[x][y] = 0

print("int flag[120][26] = {")
for x in range(len(arr)):
    print("{",end="")
    for y in range(len(arr[x])):
        print(arr[x][y],end="")
        if y != len(arr[x])-1:
            print(",",end="")
    print("}",end="")
    if x != len(arr)-1:
        print(",")
print("};")
