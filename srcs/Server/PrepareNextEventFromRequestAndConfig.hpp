#ifndef SRCS_SERVER_PREPARENEXTEVENTFROMREQUESTANDCONFIG_HPP_
#define SRCS_SERVER_PREPARENEXTEVENTFROMREQUESTANDCONFIG_HPP_
#include <string>
#include <vector>

#include "Event.hpp"
#include "File.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
class PrepareNextEventFromRequestAndConfig {
 private:
  ServerContext sc_;
  ParsedRequest pr_;
  std::string full_path_;
  LocationPair selected_location_context_;
  File f_;

 public:
  PrepareNextEventFromRequestAndConfig(const ServerContext &context,
                                       const ParsedRequest &pr);
  ~PrepareNextEventFromRequestAndConfig();
  void UpdateData(Socket *sock) const;
  std::string GetFullPath() const;
  LocationContext GetLocation() const;
  bool IsRequestCgi();
  bool RequestMethodAllowed();
};

#endif  // SRCS_SERVER_PREPARENEXTEVENTFROMREQUESTANDCONFIG_HPP_
