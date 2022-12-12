#include "main.hpp"

#include "Lexer.hpp"
#include "Parser.hpp"

void print(std::vector<std::string> vec, int num) {
  std::cout << num << " ========================" << std::endl;
  for (auto v : vec) {
    std::cout << v << std::endl;
  }
}
void split_test() {
  std::string a = "aaa/bbb/cccc";
  std::string b = "/aaa/bbb/cccc/";
  std::string c = "//aaa//bbb///cccc//";
  std::string d = "///aaa/bbb/cccc";
  std::string e = "aaa/bbb/cccc///";
  std::string f;
  std::string aa = "aaa/!bbb/!cccc";
  std::string bb = "/!aaa/!bbb!/cccc!/";
  std::string cc = "//!aaa//!bbb//!/cccc/!/";
  std::string dd = "!//!/aaa!/!bbb!/!cccc";
  std::string ee =
      "////a!	a!a/b!b!b//////	////// //!!!!!!!!!!!!!//////c	 c!cc/!/!!/!!!!";

  print(utils::SplitWithMultipleSpecifier(a, "/"), 1);
  print(utils::SplitWithMultipleSpecifier(b, "/"), 2);
  print(utils::SplitWithMultipleSpecifier(c, "/"), 3);
  print(utils::SplitWithMultipleSpecifier(d, "/"), 4);
  print(utils::SplitWithMultipleSpecifier(e, "/"), 5);
  std::cout << "split with multiple specifier" << std::endl;
  print(utils::SplitWithMultipleSpecifier(aa, "/!"), 1);
  print(utils::SplitWithMultipleSpecifier(bb, "/!"), 2);
  print(utils::SplitWithMultipleSpecifier(cc, "!/"), 3);
  print(utils::SplitWithMultipleSpecifier(dd, "!/"), 4);
  print(utils::SplitWithMultipleSpecifier(ee, "!/	 "), 5);
}

void lex_test() {
  std::cout << "lex_test" << std::endl;
  Lexer l1("./test_case/multipleserverdirective");
  Lexer l2("./test_case/normalcase");
  Lexer l3("./test_case/onelineSetting");
  l1.Tokenize();
  l2.Tokenize();
  l3.Tokenize();
  l1.DebugPrint();
  l2.DebugPrint();
  l3.DebugPrint();
}
template <typename T>
void print_vec(std::vector<T> s, std::string msg) {
  typename std::vector<T>::iterator it = s.begin();
  for (; it != s.end(); it++) {
    std::cout << msg << *it << std::endl;
  }
}
void print_listen(std::pair<std::string, std::string> s) {
  std::cout << "host:listen: " << s.second << ":" << s.first << std::endl;
}
template <typename T1, typename T2>
void print_map(std::map<T1, T2> m, std::string msg) {
  typename std::map<T1, T2>::iterator it = m.begin();
  while (it != m.end()) {
    std::cout << msg << it->first << ":" << it->second << std::endl;
    it++;
  }
}
template <typename T>
void print_set(std::set<T> s, std::string msg) {
  typename std::set<T>::iterator it = s.begin();
  while (it != s.end()) {
    std::cout << msg << *it << std::endl;
    it++;
  }
}
void print_location_context(std::map<std::string, LocationContext> l) {
  size_t i = 0;
  for (std::map<std::string, LocationContext>::iterator it = l.begin();
       it != l.end(); it++, i++) {
    std::cout << "======location num: " << i + 1
              << " ================" << std::endl;
    std::cout << it->first << " location" << std::endl;
    std::cout << "==============limit_except==============" << std::endl;
    print_set(it->second.limit_except, "limit_except :");
    std::cout << "==============redirect==============" << std::endl;
    std::cout << "redirect num:file :" << it->second.redirect.first << ":"
              << "`" << it->second.redirect.second << "`" << std::endl;
    std::cout << "=============root==============" << std::endl;
    std::cout << "root: " << it->second.root << std::endl;
    std::cout << "==============auto_index==============" << std::endl;
    std::cout << "auto index :" << it->second.auto_index << std::endl;
    std::cout << "==============index==============" << std::endl;
    print_vec(it->second.index, "index: ");
    std::cout << "==============cgi_extension==============" << std::endl;
    print_vec(it->second.cgi_extension, "cgi_extension :");
    std::cout << "==============upload_path==============" << std::endl;
    std::cout << "upload_path second:" << it->second.upload_path << std::endl;
    std::cout << "======location num: " << i + 1
              << " end============" << std::endl;
  }
}
void print_serv_context(const std::vector<ServerContext> &c) {
  for (size_t i = 0; i < c.size(); i++) {
    std::cout << "============= num: " << i + 1
              << " ================" << std::endl;
    std::cout << "==============listen ==============" << std::endl;
    print_listen(c[i].listen);
    std::cout << "==============client body size==============" << std::endl;
    std::cout << "client_body_size: " << c[i].client_body_size << std::endl;
    std::cout << "==============server name==============" << std::endl;
    std::cout << "server name :" << c[i].server_name << std::endl;
    std::cout << "==============erro page==============" << std::endl;
    print_map(c[i].error_page, "[error page num:error_page]");
    std::cout << "==============locations==============" << std::endl;
    print_location_context(c[i].locations);
    std::cout << "====================================" << std::endl;
  }
}

void parser_run(std::string filename, std::string msg, bool flg) {
  std::cout << "===============" << msg << "===================" << std::endl;
  if (flg) {
    try {
      Lexer l1(filename);
      l1.Tokenize();
      // l1.DebugPrint();
      Parser p1(l1);
      p1.Parse();
      std::cout << "✗" << std::endl;
    } catch (ConfigErrException &e) {
      // std::cout << e.msg() << std::endl;
      std::cout << "OK" << std::endl;
    } catch (std::string str) {
      std::cout << str << std::endl;
      std::cout << "OK" << std::endl;
    }
  } else {
    try {
      Lexer l1(filename);
      l1.Tokenize();
      // l1.DebugPrint();
      Parser p1(l1);
      p1.Parse();
      print_serv_context(p1.GetConfig());
      std::cout << "OK" << std::endl;
    } catch (ConfigErrException &str) {
      // std::cout << str.msg() << std::endl;
      std::cout << "✗" << std::endl;
    }
  }
}

void parser_err_case() {
  std::cout << "parser err case" << std::endl;
  std::string path = "./test_case/err_case/";
  std::cout << "===========normal err========" << std::endl;
  parser_run(path + "empty", "empty", 1);
  parser_run(path + "no_semicolon", "no_semicolon", 1);
  parser_run(path + "isdir", "isdir", 1);
  std::cout << "===========kakko err===========" << std::endl;
  parser_run(path + "kakko_saigo_ooi", "kakko_saigo_ooi", 1);
  parser_run(path + "kakko_saisyo_ooi", "kakko_saisyo_ooi", 1);
  parser_run(path + "kakko_saigo_nai", "kakko_saigo_nai", 1);
  parser_run(path + "kakko_saisyo_nai", "kakko_saisyo_nai", 1);
  parser_run(path + "kakkohutatu", "kakkohutatu", 1);
  std::cout << "===========body err=============" << std::endl;
  parser_run(path + "body_size_duplicate", "body_size_duplicate", 1);
  parser_run(path + "body_size_lower_over", "body_size_lower_over", 1);
  parser_run(path + "body_size_over", "body_size_over", 1);
  parser_run(path + "body_size_no_val", "body_size_no_val", 1);
  std::cout << "===========listen err=============" << std::endl;
  parser_run(path + "listen_duplicate", "listen_duplicate", 1);
  parser_run(path + "listen_no_val", "listen_no_val", 1);
  std::cout << "===========server_name err=============" << std::endl;
  parser_run(path + "server_name_no_val", "server_name_no_val", 1);
  parser_run(path + "server_name_kakko", "server_name_kakko", 1);
  parser_run(path + "server_name_finish", "server_name_finish", 1);
  std::cout << "===========error_page err=============" << std::endl;
  parser_run(path + "error_page_over", "error_page_over", 1);
  parser_run(path + "error_page_lower_over", " error_page_lower_over", 1);
  parser_run(path + "error_page_no_file", "error_page_no_file", 1);
  parser_run(path + "error_page_kakko", "error_page_kakko", 1);
  std::cout << "===========location context err=============" << std::endl;
  parser_run(path + "location_kakkonai", "location_kakkonai", 1);
  parser_run(path + "location_kakkooi", "location_kakkooi", 1);
  parser_run(path + "location_kakko_sukunai", "location_kakko_sukunai", 1);
  parser_run(path + "location_no_val", "location_no_val", 1);
  parser_run(path + "location_duplicate", "location_duplicate", 1);
  std::cout << "===========limit except err=============" << std::endl;
  parser_run(path + "limit_except_duplicate", "limit_except_duplicate", 1);
  parser_run(path + "limit_except_not_method", "limit_except_not_method", 1);
  std::cout << "===========redirect err=============" << std::endl;
  parser_run(path + "redirect_many_argment", "redirect_many_argment", 1);
  parser_run(path + "redirect_not_digit", "redirect_not_digit", 1);
  std::cout << "===========root err=============" << std::endl;
  parser_run(path + "root_many_argment", "root_many_argment", 1);
  parser_run(path + "root_duplicate", "root_duplicate", 1);
  std::cout << "===========auto index err=============" << std::endl;
  parser_run(path + "auto_index_many_argment", "auto_index_many_argment", 1);
  parser_run(path + "auto_index_invalid_argment", "auto_index_invalid_argment",
             1);
  std::cout << "===========index err=============" << std::endl;
  parser_run(path + "index_no_val", "index_no_val", 1);
  std::cout << "===========cgi_extension err=============" << std::endl;
  parser_run(path + "cgi_extension_no_extension", "cgi_extension_no_extension",
             1);
  parser_run(path + "cgi_extension_no_comma", "cgi_extension_no_comma", 1);
  parser_run(path + "cgi_extension_middle_comma", "cgi_extension_middle_comma",
             1);
  parser_run(path + "cgi_extension_end_comma", "cgi_extension_end_comma", 1);
  std::cout << "===========upload_path err=============" << std::endl;
  parser_run(path + "upload_pass_duplicate", "upload_pass_duplicate", 1);
  parser_run(path + "upload_pass_two_argment", "upload_pass_two_argment", 1);
  std::cout << "===========================================" << std::endl;
}
void parser_ok_case() {
  std::cout << "parser ok case" << std::endl;
  std::string path = "./test_case/ok_case/";
  parser_run(path + "all", "all", 0);
  parser_run(path + "multipleserverdirective", "multipleserverdirective", 0);
  parser_run(path + "multipleserverdirective", "multipleserverdirective", 0);
  parser_run(path + "nonewlinenospace", "nonewlinenospace", 0);

  parser_run(path + "normalcase", "normalcase", 0);
}
void parser_test() {
  std::cout << "parsr" << std::endl;
  parser_err_case();
  parser_ok_case();
}

int main() {
  // split_test();
  // pos_test();
  // range_test();
  // parser_test();
  // file_test();
  // lex_test();
  parser_test();
}
