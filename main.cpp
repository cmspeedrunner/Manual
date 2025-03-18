#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <map>
#include <cstdlib>

std::string binary_to_char(const std::string& binary_str) {
    try {
        return std::string(1, static_cast<char>(std::stoi(binary_str, nullptr, 2)));
    } catch (...) {
        return binary_str;
    }
}

std::string translate_symbol(const std::string& symbol) {
    std::map<std::string, std::string> symbol_map = {
        // Miscellaneous symbols
        {"{.}", "segment"}, {"@<", "global"}, {">@", "extern"}, {"#<", "call"}, {"_", "bits"},
        {"<**>", "64"}, {"<*$>", "32"}, {"<*\">", "16"}, {"<*>", "8"}, {"--", "default"}, {"-@-", "rel"},
        
        // Data symbols
        {"$!", "db"}, {"$\"", "dw"}, {"$$", "dd"}, {"$*", "dq"}, {"$!)", "dt"},
        
        // Branch symbols
        {"^", "jmp"}, {"<<", "ret"}, {"<>", "loop"}, {"<)>", "loopz"}, {"<')>", "loopnz"},
        {"^)", "jz"}, {"^')", "jnz"}, {"!=", "je"}, {"!='", "jne"}, {"^>", "jg"}, {"^<", "jl"},
        {"^>=", "jge"}, {"^<=", "jle"}, {"?=?", "cmp"},
        
        // Arithmetic symbols
        {"+", "add"}, {"-", "sub"}, {"+&", "adc"}, {"-&", "sbb"},
        {"++", "inc"}, {"--", "dec"}, {"*", "mul"}, {"/", "div"},
        
        // Logic symbols
        {"|", "or"}, {"?|", "xor"}, {"&", "and"}, {"'", "not"},
        
        // Register symbols
        {"(+)", "rax"}, {"(:)", "rbx"}, {"(#)", "rcx"}, {"($)", "rdx"},
        {"[@]", "rsi"}, {"[&]", "rdi"}, {"[:]", "rbp"}, {"[*]", "rsp"},
        {"{*}", "r8"}, {"{(}", "r9"}, {"{!)}", "r10"}, {"{!!}", "r11"},
        {"{!\"}", "r12"}, {"{!£}", "r13"}, {"{!$}", "r14"}, {"{!%}", "r15"},
        
        // Segment symbols
        {".$.", ".data"}, {".%.", ".text"}, {".#.", ".bss"},
        
        // Data movement symbols
        {">", "push"}, {">^", "pop"}, {"~", "mov"}, {"<", "lea"},
        {",", ","}, {"::", " "}, {":#:", "   "}
    };
    
    if (symbol_map.find(symbol) != symbol_map.end()) {
        return symbol_map[symbol];
    }
    return symbol;
}

std::string transpile(const std::string& line) {
    std::stringstream ss(line);
    std::string word, result;
    while (ss >> word) {
        if (word.size() == 7 && word.find_first_not_of("01") == std::string::npos) {
            result += binary_to_char(word);
        } else {
            result += translate_symbol(word);
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename> [-asm] [-obj] [-code]" << std::endl;
        return 1;
    }

    std::string userFile = argv[1];
    std::ifstream file(userFile);
    if (!file) {
        std::cerr << "Error opening file: " << userFile << std::endl;
        return 1;
    }
    
    std::string line, output_content;
    while (std::getline(file, line)) {
        output_content += transpile(line) + "\n";
    }
    file.close();
    
    std::string asmFile = userFile.substr(0, userFile.find_last_of('.')) + ".asm";
    std::ofstream asmOut(asmFile);
    asmOut << output_content;
    asmOut.close();
    
    std::string objFile = userFile.substr(0, userFile.find_last_of('.')) + ".obj";
    std::string exeFile = userFile.substr(0, userFile.find_last_of('.')) + ".exe";
    
    system(("nasm -fwin64 " + asmFile + " -o " + objFile).c_str());
    system(("gcc " + objFile + " -o " + exeFile).c_str());
    
    bool keep_asm = false, keep_obj = false, print_code = false;
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-asm") keep_asm = true;
        if (arg == "-obj") keep_obj = true;
        if (arg == "-code") print_code = true;
    }
    
    if (!keep_asm) std::remove(asmFile.c_str());
    if (!keep_obj) std::remove(objFile.c_str());
    if (print_code) std::cout << output_content;
    
    return 0;
}
