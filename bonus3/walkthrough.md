1. `i functions` => only main

2. Through disassembling the main, we get to the `source.c`

3. We cannot really do any test through gdb, because teh `fopen` function call returns NULL, as through gdb we don't have the permissioins of user end! So there might be an easy trick in this exercice.

4. buf basically contains the flag. It must be equal to argv[1] so that we execute a shell. The thing is we don't know the flag! We see that argv[1] goes through atoi, and the resulting nmber is an index at which the buffer is set to `\0` = end of buffer! So if we don't know the buffer at all, we might set the first byte of the buffer to `\0`, and argv[1] should also be a null terminating string of 1 byte : 

```
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
