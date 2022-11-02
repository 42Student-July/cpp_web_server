#include "Lexer.hpp"

#include "Parser.hpp"
Lexer::Lexer(std::string file_name)
    : space_(" \t\n"), separators_(";{}"), file_(file_name) {
  if (file_.Status() == NOT_FOUND)
    throw ConfigErrException::Err("file not found");
  if (file_.Status() == IS_DIR) throw ConfigErrException::Err("file is dir");
  if (file_.Status() == READ_PERMISSION)
    throw ConfigErrException::Err("file cant read");
  // parentheses_cnt_.push(true);
}
Lexer::~Lexer() {}
Lexer::Lexer(const Lexer &l) { *this = l; }
Lexer &Lexer::operator=(const Lexer &l) {
  if (this == &l) return *this;
  file_ = l.file_;
  tokens_ = l.tokens_;
  separators_ = l.separators_;
  space_ = l.space_;
  return *this;
}
void Lexer::Tokenize() {
  std::vector<std::string> lines = file_.StoreFileLinesInVec();
  for (size_t line_num = 0; line_num < lines.size(); line_num++) {
    std::string line = lines[line_num] + "\n";
    DecomposeAndStoreLineWithDelimiters(&line, line_num);
  }
}
void Lexer::DecomposeAndStoreLineWithDelimiters(std::string *line,
                                                size_t line_num) {
  size_t space_pos = 0;
  while ((space_pos = line->find_first_not_of(space_, 0)) !=
         std::string::npos) {
    size_t find_first_pos = line->find_first_of(space_, space_pos);
    std::string piece = line->substr(space_pos, find_first_pos - space_pos);
    SeparateAndStoreDelimiterCharacters(&piece, line_num);
    *line = line->substr(find_first_pos);
  }
}
void Lexer::SeparateAndStoreDelimiterCharacters(std::string *piece,
                                                size_t line_num) {
  size_t sep_pos = 0;
  while ((sep_pos = piece->find_first_of(separators_, 0)) !=
         std::string::npos) {
    StoreIfNotEmpty(piece->substr(0, sep_pos), line_num);
    StoreIfNotEmpty(piece->substr(sep_pos, 1), line_num);
    *piece = piece->substr(sep_pos + 1);
  }
  StoreIfNotEmpty(*piece, line_num);
}
void Lexer::StoreIfNotEmpty(const std::string &str, size_t line_num) {
  if (str.empty()) return;
  tokens_.push_back(Token(str, line_num));
}
std::vector<Token> Lexer::GetTokens() const { return tokens_; }
void Lexer::DebugPrint() {
  std::cout << "debug print queue" << std::endl;
  std::vector<Token>::iterator it = tokens_.begin();
  while (it != tokens_.end()) {
    Token t = *it;
    std::cout << t.GetData() << std::endl;
    it++;
  }
  std::cout << "==========finish===========" << std::endl;
}
