#include "ErrorPage.hpp"

#include "File.hpp"
#include "Utils.hpp"

namespace {
std::string GetStatusMessage(const int error_code) {
  switch (error_code) {
    case 200:
      return "OK";
    case 400:
      return "Bad Request";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 413:
      return "Payload Too Large";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 505:
      return "HTTP Version Not Supported";
    default:
      return "";
  }
}
}  // namespace

ErrorPage::ErrorPage() {}

ErrorPage::ErrorPage(ErrorPage const &other) { *this = other; }

ErrorPage &ErrorPage::operator=(ErrorPage const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

ErrorPage::~ErrorPage() {}

// return error page content if error page is found
// else return default error page
HttpResponseTmp ErrorPage::GetErrorPage(const ResponseCode error_code,
                                        ErrorPageMap error_pages) {
  std::string path = GetErrorPagePath(error_code, error_pages);
  if (path.empty()) {
    return DefaultErrorPage(error_code);
  }
  return CustomErrorPage(error_code, path);
}

std::string ErrorPage::GetErrorPagePath(const ResponseCode error_code,
                                        ErrorPageMap error_pages) {
  std::map<long, std::string>::const_iterator it = error_pages.find(error_code);
  if (it != error_pages.end()) {
    return it->second;
  }
  return "";
}

HttpResponseTmp ErrorPage::CustomErrorPage(const ResponseCode error_code,
                                           const std::string &path) {
  File file("./www/" + path);  // 決め打ち
  if (!file.IsExist()) {
    return DefaultErrorPage(kKk404NotFound);
  }
  if (!file.IsFile()) {
    return DefaultErrorPage(kKk403Forbidden);
  }
  if (!file.CanRead()) {
    return DefaultErrorPage(kKk403Forbidden);
  }

  HttpResponseTmp response;

  response.rescode = error_code;
  response.body = file.ReadFileLines();
  return response;
}

HttpResponseTmp ErrorPage::DefaultErrorPage(const ResponseCode error_code) {
  HttpResponseTmp response;
  std::string content;

  content += "<html><head>";
  content += "<titile>" + utils::UIntToString(error_code) + " " +
             GetStatusMessage(error_code) + "</title>";
  content += "</head>";

  content += "<body><center>";
  content += "<h1> " + utils::UIntToString(error_code) + " " +
             GetStatusMessage(error_code) + "</h1>";
  content += "</center></body></html>";

  response.body = content;
  response.rescode = error_code;
  return response;
}
