#include "Cgi.hpp"
Cgi::Cgi(const ServerContext &context, const ParsedRequest &pr, int conn_fd)
    : Event(-1, context, kCgi),
      timer_(kKTimeOut),
      conn_fd_(conn_fd),
      method_(pr.m) {
  // set up
  (void)pr;
  pass_ = "./www/cgi-bin/env.cgi";
  ParseArgv();
  ArgvToCharPtr();
  SetEnv();
  EnvMapToCharPtr();
  PipeOut();
  if (method_ == kPost) PipeIn();
  Reset(pipe_out_[0]);  // fd hennkou
}
Cgi::~Cgi() {
  int status = 0;
  waitpid(chilid_process_, &status, WNOHANG);
  if (!WIFEXITED(status)) kill(chilid_process_, SIGKILL);
  if (method_ == kPost) close(pipe_in_[1]);
  close(pipe_out_[0]);
}

void Cgi::ParseArgv() {
  if (query_.find('=') != std::string::npos) return;
  size_t pos = 0;
  std::string query = query_;
  while ((pos = query.find_first_of("+")) != std::string::npos) {
    StoreStrIfNotEmpty(query.substr(0, pos));
    query = query.substr(pos + 1);
  }
  StoreStrIfNotEmpty(query);
}

void Cgi::StoreStrIfNotEmpty(const std::string &str) {
  if (str.empty()) return;
  argv_.push_back(str);
}
void Cgi::SetEnv() {
  // if(!length.empty)
  env_map_["CONTENT_LENGTH"] =
      std::string("CONTENT_LENGTH=") +
      std::string("50");  // request headder ni attara tukeru
  env_map_["AUTH_TYPE"] =
      std::string("AUTH_TYPE=") +
      std::string("auth");  // hedderの authenticationの値を使う
  env_map_["CONTENT_TYPE"] =
      std::string("CONTENT_TYPE=") +
      std::string("type");  // yttp request headder ni attara must
  env_map_["PATH_INFO"] = std::string("PATH_INFO=") +
                          std::string("paht");  // cgiファイル名の後ろにつくあれ
  env_map_["QUERY_STRING"] =
      std::string("QUERY_STRING=") +
      std::string("abc=abc&bbb=ccc");  // ?の後に=があったら
  env_map_["REMOTE_ADDR"] =
      std::string("REMOTE_ADDR=") + std::string("172.0.0.1");  // host address
  env_map_["REQUEST_METHOD"] =
      std::string("REQUEST_METHOD=") + std::string("GET");  // method name
  env_map_["SCRIPT_NAME"] = std::string("SCRIPT_NAME=") +
                            std::string("/test.cgi");  // encode してないuri
  env_map_["SERVER_NAME"] =
      std::string("SERVER_NAME=") + std::string("localhost");  // 必須
  env_map_["SERVER_PORT"] =
      std::string("SERVER_PORT=") + std::string("80");  // 必須
  env_map_["SERVER_PROTOCOL"] =
      std::string("SERVER_PROTOCOL=") + std::string("HTTP/1.1");
  env_map_["SERVER_SOFTWARE"] = std::string("SERVER_SOFTWARE=") +
                                std::string("WEBSEEEEEEEEEEEEEEEEERVEEEEEEEEE");

  /// env_map_["path"] = "PATH=/usr/bin/perl";
}

void Cgi::ArgvToCharPtr() {
  if (argv_.size() == 0) {
    argv_ptr_ = NULL;
    return;
  }
  argv_ptr_ = new char *[argv_.size() + 1];
  for (size_t i = 0; i < argv_.size(); i++) {
    argv_ptr_[i] = utils::StrToCharPtr(argv_[i]);
  }
  argv_ptr_[argv_.size()] = NULL;
}
void Cgi::EnvMapToCharPtr() {
  env_ptr_ = new char *[env_map_.size() + 1];
  std::map<std::string, std::string>::iterator it = env_map_.begin();
  for (size_t i = 0; it != env_map_.end(); i++, it++) {
    env_ptr_[i] = utils::StrToCharPtr(it->second);
  }
  env_ptr_[env_map_.size()] = NULL;
}
void Cgi::PipeIn() {
  if (pipe(pipe_in_) == -1) throw std::runtime_error("pipe");
  // close(pipe_in_);
}
void Cgi::PipeOut() {
  if (pipe(pipe_out_) == -1) throw std::runtime_error("pipe");
  // close();
}
void Cgi::Fork() {
  if ((chilid_process_ = fork()) == -1) throw std::runtime_error("fork");
}
// void Cgi::print(){
//   std::cout << query_ <<std::endl;
// for(char **ptr = argv_ptr_ ; *ptr != NULL; ptr++){
// std::cout << *ptr << std::endl;
// }
// std::cout << "===================" << std::endl;
// for(char **ptr = env_ptr_ ; *ptr != NULL; ptr++){
// std::cout << *ptr << std::endl;
// }
// }
void Cgi::Dup2() {
  if (dup2(pipe_out_[1], STDOUT_FILENO) == -1) {
    throw std::runtime_error("dup2 err");
  }
  if (method_ == kPost) {
    if (dup2(pipe_in_[0], STDIN_FILENO) == -1)
      throw std::runtime_error("dup2 err");
  }
}
void Cgi::Run() {
  Fork();
  if (chilid_process_ == 0) {
    if (method_ == kPost) close(pipe_in_[1]);
    close(pipe_out_[0]);
    Dup2();
    execve(pass_.c_str(), argv_ptr_, env_ptr_);
    utils::DelPtr(env_ptr_);
    utils::DelPtr(argv_ptr_);
    std::cout << pass_ << std::endl;
    std::cout << "sippai" << std::endl;
    exit(1);
  } else {
    if (method_ == kPost) close(pipe_in_[0]);
    close(pipe_out_[1]);
    utils::DelPtr(env_ptr_);
    utils::DelPtr(argv_ptr_);
    // wait(NULL);
    while (true) {
      std::string reaad = Read();
      chunked_ += reaad;
      if (reaad.size() == 0) break;
    }
  }
}
void Cgi::ReadFromCgi() {}
std::string Cgi::GetChunked() const { return chunked_; }

bool Cgi::TimeOver() const { return timer_.TimeOver(); }
// search?
// q=cgi+%2B+%26+引数
// &ei=OShvY7TfJ9Tw-QaMpLXYBQ
// &ved=0ahUKEwj036b87af7AhVUeN4KHQxSDVsQ4dUDCA8
// &uact=5
// &oq=cgi+%2B+%26+引数
// &gs_lcp=Cgxnd3M
// &sclient=gws-wiz-s

// =　がないなら　argv
// =　があるなら　querystring

// 大文字小文字　区別しない
// %の後がエンコード文字ではない場合 そのまま扱う
// Content-type: TypeName
// location
// status

static const char kEncodeTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

bool IsSafe(char c) {
  return (std::isalnum(c) != 0) || c == '.' || c == '-' || c == '_' || c == '~';
}
static const char kDecodeTable[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
std::string Encode(const std::string &str) {
  std::string encode;
  for (size_t i = 0; i < str.size(); i++) {
    if (IsSafe(str[i])) {
      encode += str[i];
    } else {
      encode += '%';
      encode += kEncodeTable[(str[i] & 0xf0) >> 4];
      encode += kEncodeTable[str[i] & 0x0f];
    }
  }
  return encode;
}
std::string Decode(const std::string &str) {
  std::string decode;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == '%') {
      if (str.size() <= i + 2 ||
          kDecodeTable[static_cast<unsigned char>(str[i + 1])] == -1 ||
          kDecodeTable[static_cast<unsigned char>(str[i + 2])] == -1)
        continue;
      decode += (kDecodeTable[static_cast<unsigned char>(str[i + 1])] << 4) +
                kDecodeTable[static_cast<unsigned char>(str[i + 2])];
      i += 2;
    } else if (str[i] == '+') {
      decode += ' ';
    } else {
      decode += str[i];
    }
  }
  return decode;
}
