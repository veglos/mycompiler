#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <string>

enum class TokenType {
  _return,
  int_lit,
  semi
};

struct Token {
  TokenType type;
  std::optional<std::string> value;
};

std::vector<Token> tokenize(const std::string& str){
  std::vector<Token> tokens {};
  std::string buff;

  for(int i = 0; i < str.size(); i++){
    char c = str[i];
    if(std::isalpha(c)){
      buff.push_back(c);
      i++;
      while(std::isalnum(str[i])){
        buff.push_back(str[i]);
        i++;
      }
      i--;
      if(buff == "return"){
        tokens.push_back({.type = TokenType::_return});
        buff.clear();
        continue;
      }
      else {
        std::cerr << "Error" << buff << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    else if(std::isdigit(c)){
      buff.push_back(c);
      i++;
      while(std::isdigit(str[i])){
        buff.push_back(str[i]);
        i++;
      }
      i--;
      tokens.push_back({.type = TokenType::int_lit, .value = buff});
      buff.clear();
    }
    else if(c == ';'){
      tokens.push_back({.type = TokenType::semi});
    }
    else
    if(std::isspace(c)){
      continue;
    }
    else{
      std::cerr << "Error" << c << std::endl;
      exit(EXIT_FAILURE);
    } 
  }

  return tokens;

};

std::string tokens_to_asm(const std::vector<Token>& tokens){
  std::stringstream asm_code;
  asm_code <<  "global _start\n_start:\n";
  for(int i = 0; i < tokens.size(); i++){
    Token token = tokens[i];
    if(i+1 < tokens.size() && tokens[i+1].type == TokenType::int_lit){
      if(i+2 < tokens.size() && tokens[i+2].type == TokenType::semi){
        asm_code << "    mov rax, 60\n";
        asm_code << "    mov rdi, " << tokens[i+1].value.value() << "\n";
        asm_code << "    syscall";
      }
    }
  }
  return asm_code.str();
};

int main(int argc, char* argv[]){
  if(argc != 2){
    std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
    std::cerr << "hydro <input.hy>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string contents;
  {
    std::stringstream contents_stream;
    std::fstream input(argv[1], std::ios::in);
    contents_stream << input.rdbuf();
    contents = contents_stream.str();
  }
  std::cout << contents << std::endl;

  std::vector<Token> tokens = tokenize(contents);
  std::cout << tokens_to_asm(tokens) << std::endl;

  {
    std::fstream output("out.asm", std::ios::out);
    output << tokens_to_asm(tokens);
  }

  system("nasm -felf64 out.asm");
  system("ld out.o -o out");

  return EXIT_SUCCESS;
};
