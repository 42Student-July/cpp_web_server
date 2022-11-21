#include "main.hpp"
int main(int ac, char **av) {
  if (ac != 2) {
    std::cout << "usage : ./webserv [config file]" << std::endl;
    return 1;
  }
  try {
    Lexer lexer(av[1]);
    lexer.Tokenize();
    Parser parser(lexer);
    parser.Parse();
    Server server(parser.ConfigSetting());
    server.Run();
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
  } catch (ConfigErrException &e) {
    std::cout << e.Msg() << std::endl;
  }
  return 0;
}
