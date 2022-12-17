#include "main.hpp"
int main(int ac, char **av) {
  if (ac > 2) {
    std::cout << "usage : ./webserv [config file]" << std::endl;
    return 1;
  }
  std::string conf_path = "./conf/webserv/default.conf";
  if (ac == 2) {
    conf_path = av[1];
  }
  try {
    Lexer lexer(conf_path);
    lexer.Tokenize();
    Parser parser(lexer);
    parser.Parse();

    Server server(ServerContext::ToMap(parser.GetConfig()));
    server.Run();
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  } catch (ConfigErrException &e) {
    std::cout << e.Msg() << std::endl;
  }
  return 0;
}
