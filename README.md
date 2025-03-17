# Manual
Manual is an esoteric compiled programming language where data is represented as binary, and you have to manually move bytes, with some help. <br>
Manual targets windows for now, more specifically win64, but this is going to be expanded soon. <br>

## Syntax
Lets start by looking at a simple hello world program in x64 asm: <br>
```asm
bits 64
default rel
segment .data
   msg db "Hello, World!", 10, 0

segment .text
   global main
   extern printf
   extern ExitProcess

main:
   push rbp
   mov rbp, rsp
   sub rsp, 32
   lea rcx, [msg]
   call printf

   xor rax, rax
   call ExitProcess
```
In Manual, it would look like this:
```
_ :: <**>
-- :: -@-
{.} :: .$.
:#: 1101101 1110011 1100111 :: $! :: 0100010 1001000 1100101 1101100 1101100 1101111 , :: 1010111 1101111 1110010 1101100 1100100 0100001 0100010 , :: 0110001 0110000 , :: 0110000

{.} :: .%.
:#: @< :: 1101101 1100001 1101001 1101110
:#: >@ :: 1110000 1110010 1101001 1101110 1110100 1100110
:#: >@ :: 1000101 1111000 1101001 1110100 1010000 1110010 1101111 1100011 1100101 1110011 1110011

1101101 1100001 1101001 1101110 0111010
:#: > :: [:]
:#: ~ :: [:] , :: [*]
:#: - :: [*] , :: 0110011 0110010
:#: < :: (#) , :: 1011011 1101101 1110011 1100111 1011101
:#: #< :: 1110000 1110010 1101001 1101110 1110100 1100110

:#: ?| :: (+) , :: (+)
:#: #< :: 1000101 1111000 1101001 1110100 1010000 1110010 1101111 1100011 1100101 1110011 1110011
```
*Whats going on here?* <br>
Well, let start with some basic structuring <br>
`::` is a whitespace, yes, you need to add spaces in manually. (get it) <br>
`:#:` is a tab, just for syntactic sugar, these are not needed but recommended. <br>
`,` are just commas, directly, writing out the binary for a comma every time is somewhat bothersome. <br>
Looking at the first two lines of the Manual code, and comparing them to the assembly, you can see what certain symbol combinations do: <br>

`_ :: <**>`
`bits 64`

`-- :: -@-`
`default rel`
