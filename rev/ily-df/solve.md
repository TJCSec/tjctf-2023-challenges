GDB

To find inventory address:
```
b nodelay
r
```

Hit `Y` + `Enter`

```
fin
```

Address is shown at the right:
```
 0x5555555563b4                  lea    rax, [rip+0x103551]        # 0x55555565990c
```

```
c
```

Hit `Ctrl+C`

```
set ({int[256]}0x55555565990c)[70] = 0x44
set ({int[256]}0x55555565990c)[78] = 100
set ({int[256]}0x55555565990c)[86] = 0x59
set ({int[256]}0x55555565990c)[79] = 4
set ({int[256]}0x55555565990c)[68] = -5
c
```

Mine `L`, fall to bottom, mine `L` at the left to reveal flag
