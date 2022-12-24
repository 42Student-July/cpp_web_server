#include "Cgi.hpp"

#include <fcntl.h>

#include "CgiParser.hpp"
#include "File.hpp"
Cgi::Cgi(const LocationContext &lc, const std::string &full_path)
    : file_path_(full_path) {
  File file(file_path_);
  if (file.IsDir()) {
    if (!lc.index.empty()) {
      file_path_ += lc.index;
      file.SetFileName(file_path_);
    }
  }
  if (!file.CanExec()) throw ErrorResponse("cgi permission", kKk403Forbidden);
}
Cgi::Cgi() {}
Cgi::~Cgi() {}

void Cgi::ParseArgv(Socket *socket) {
  std::string query = socket->pr.query_string;
  if (query.find('=') != std::string::npos) return;
  size_t pos = 0;
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
void Cgi::SetEnv(Socket *socket) {
  if (!socket->pr.request_body.empty()) {
    env_map_["CONTENT_LENGTH"] =
        std::string("CONTENT_LENGTH=") +
        utils::UIntToString(socket->pr.request_body.size());  // headder
  }
  if (!CgiParser::FindByKey(socket->pr.request_header, "content-type")
           .first.empty()) {
    env_map_["CONTENT_TYPE"] =
        std::string("CONTENT_TYPE=") +
        CgiParser::FindByKey(socket->pr.request_header, "content-type")
            .second;  //  headder ni attara must
  }
  env_map_["PATH_INFO"] =
      std::string("PATH_INFO= ") +
      file_path_;  // request line cgiファイル名の後ろにつくあれ
  env_map_["QUERY_STRING"] = std::string("QUERY_STRING=") +
                             socket->pr.query_string;  // ?の後に=があったら
  env_map_["REMOTE_ADDR"] = std::string("REMOTE_ADDR=127.0.0.1");  // host addr

  env_map_["REQUEST_METHOD"] = std::string("REQUEST_METHOD=") +
                               utils::ToStr(socket->pr.m);  // method name
  env_map_["SCRIPT_NAME"] = std::string("SCRIPT_NAME=") +
                            socket->pr.request_path;  // encode してないuri
  env_map_["SERVER_NAME"] =
      std::string("SERVER_NAME=") + socket->server_context.server_name;  // 必須
  env_map_["SERVER_PORT"] =
      std::string("SERVER_PORT=") + socket->server_context.GetPort();  // 必須
  env_map_["SERVER_PROTOCOL"] =
      std::string("SERVER_PROTOCOL=") + std::string("HTTP/1.1");
  env_map_["SERVER_SOFTWARE"] = std::string("SERVER_SOFTWARE=") +
                                std::string("WEBSEEEEEEEEEEEEEEEEERVEEEEEEEEE");
}
//   /// env_map_["path"] = "PATH=/usr/bin/perl";
// }
// // void Cgi::print(){
// //   std::cout << query_ <<std::endl;
// // for(char **ptr = argv_ptr_ ; *ptr != NULL; ptr++){
// // std::cout << *ptr << std::endl;
// // }
// // std::cout << "===================" << std::endl;
// // for(char **ptr = ptr ; *ptr != NULL; ptr++){
// // std::cout << *ptr << std::endl;
// // }
// // }
void Cgi::Fork(CgiRes *cgires) {
  if ((cgires->process_id = fork()) == -1) {
    close(fd_[0]);
    close(fd_[1]);
    throw ErrorResponse("fork", kKk500internalServerError);
  }
}
void Cgi::SockPair() {
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd_) == -1)
    throw ErrorResponse("socketpair", kKk500internalServerError);
}
void Cgi::SetSockopt() {
  int flag = 0;
  if ((flag = fcntl(fd_[0], F_GETFL, 0)) == -1 ||
      fcntl(fd_[0], F_SETFL, flag | O_NONBLOCK) == -1) {
    close(fd_[0]);
    close(fd_[1]);
    throw ErrorResponse("fcntl", kKk500internalServerError);
  }
}
void Cgi::Run(Socket *socket, CgiRes *cgires) {
  SockPair();
  cgires->cgi_fd = fd_[0];
  Fork(cgires);
  if (cgires->process_id == 0) {
    ParseArgv(socket);
    SetEnv(socket);
    close(fd_[0]);
    char **argv = utils::VecToCharDoublePtr(argv_);
    char **env = utils::MapToCharDoublePtr(env_map_);
    if (dup2(fd_[1], STDIN_FILENO) == -1 || dup2(fd_[1], STDOUT_FILENO) == -1) {
      exit(1);
    }
    execve(file_path_.c_str(), argv, env);
    // utils::DelPtr(argv);
    // utils::DelPtr(env);
    exit(1);
  }
  close(fd_[1]);
}

// void Cgi::ReadFromCgi() {}
// std::string Cgi::GetChunked() const { return chunked_; }

// bool Cgi::TimeOver() const { return timer_.TimeOver(); }
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
