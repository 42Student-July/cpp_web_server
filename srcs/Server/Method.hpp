#ifndef SRCS_SERVER_METHOD_HPP_
#define SRCS_SERVER_METHOD_HPP_
class Method {
 public:
  virtual ~Method();
  virtual void Run() = 0;
  virtual void ChangeSocketState() = 0;
};

#endif  // SRCS_SERVER_METHOD_HPP_
