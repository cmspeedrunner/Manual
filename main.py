import sys
import os

byteTable = [
    f"{i:07b}" for i in range(128)  
]

def binary_to_char(binary_str):
    try:
        
        return chr(int(binary_str, 2))
    except ValueError:
        return binary_str  

def translate_symbol(symbol):
    miscMap = ["segment", "global", "extern", "call", "bits", "64", "32", "16", "8", "default", "rel"]
    miscSymMap = ["{.}", "@<", ">@", "#<", "_", "<**>", "<*$>", "<*\">", "<*>", "--", "-@-"]

    dataMap = ["db", "dw", "dd", "dq", "dt"]
    dataSymMap = ["$!", "$\"", "$$", "$*", "$!)"]

    branchmap = ["jmp", "ret", "loop", "loopz", "loopnz", "jz", "jnz", "je", "jne", "jg", "jl", "jge", "jle", "cmp"]
    branchSymMap = ["^", "<<", "<>", "<)>", "<')>", "^)", "^')", "!=", "!'=", "^>", "^<", "^>=", "^<=", "?=?"]

    arithMap = ["add", "sub", "adc", "sbb", "inc", "dec", "mul", "div"]
    arithSymMap = ["+", "-", "+&", "-&", "++", "--", "*", "/"]

    logicMap = ["or", "xor", "and", "not"]
    logicSymMap = ["|", "?|", "&", "'"]

    regMap = ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",    "r8", "r9", "r10", "r11", "r12",      "r13", "r14", "r15"]
    regSymMap = ["(+)", "(:)", "(#)", "($)", "[@]", "[&]", "[:]", "[*]", "{*}", "{(}", "{!)}", "{!!}", "{!\"}", "{!£}","{!$}","{!%}"]

    segMap = [".data", ".text", ".bss"]
    segSymMap = [".$.", ".%.", ".#."]
    
    dataMovMap = ["push", "pop", "mov", "lea", ",", " ","   "]
    dataMovSymMap = [">", "^", "~", "<", ",", "::", ":#:"]

    # Check each symbol map
    for i, sym in enumerate(miscSymMap):
        if symbol == sym:
            return miscMap[i]
    for i, sym in enumerate(dataSymMap):
        if symbol == sym:
            return dataMap[i]
    for i, sym in enumerate(branchSymMap):
        if symbol == sym:
            return branchmap[i]
    for i, sym in enumerate(arithSymMap):
        if symbol == sym:
            return arithMap[i]
    for i, sym in enumerate(logicSymMap):
        if symbol == sym:
            return logicMap[i]
    for i, sym in enumerate(regSymMap):
        if symbol == sym:
            return regMap[i]
    for i, sym in enumerate(segSymMap):
        if symbol == sym:
            return segMap[i]
    for i, sym in enumerate(dataMovSymMap):
        if symbol == sym:
            return dataMovMap[i]

    return symbol  
def transpile(line):
    words = line.split()
    result = []
    for word in words:
        # If word is a binary byte (7-bit), convert to char
        if all(c in '01' for c in word) and len(word) == 7:
            result.append(binary_to_char(word))
        else:
            result.append(translate_symbol(word))
    return ''.join(result)  
userFile = sys.argv[1]
with open(userFile, "r") as f:
    content = f.read()
    lines = content.splitlines()
output_lines = [transpile(line) for line in lines]

with open(userFile.split(".")[0] + ".asm", "w") as af:
    af.write('\n'.join(output_lines))
    asmcode = ('\n'.join(output_lines))
    af.close()

os.system(f"nasm -fwin64 {userFile.split(".")[0]+".asm"} -o {userFile.split(".")[0]+".obj"}")
os.system(f"gcc {userFile.split(".")[0]+".obj"} -o {userFile.split(".")[0]+".exe"}")
if "-asm" not in sys.argv:
    
    os.remove(f"{userFile.split(".")[0]+".asm"}")
if "-obj" not in sys.argv:
    os.remove(f"{userFile.split(".")[0]+".obj"}")
if "-code" in sys.argv:
    print(asmcode)