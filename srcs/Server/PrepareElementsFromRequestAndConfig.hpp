#ifndef SRCS_SERVER_PREPAREELEMENTSFROMREQUESTANDCONFIG_HPP_
#define SRCS_SERVER_PREPAREELEMENTSFROMREQUESTANDCONFIG_HPP_
#include <string>
#include <vector>

#include "File.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
class PrepareElementsFromRequestAndConfig {
 private:
  ServerContext sc_;
  ParsedRequest pr_;
  std::string full_path_;
  LocationPair selected_location_context_;
  File f_;

 public:
  PrepareElementsFromRequestAndConfig(const ServerContext &context,
                                      const ParsedRequest &pr);
  ~PrepareElementsFromRequestAndConfig();
  void UpdateData(Socket *sock) const;
  std::string GetFullPath() const;
  LocationContext GetLocation() const;
  bool IsRequestCgi();
  bool RequestMethodAllowed();
};

#endif  // SRCS_SERVER_PREPAREELEMENTSFROMREQUESTANDCONFIG_HPP_
