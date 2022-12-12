#include "Method.hpp"

#include "Delete.hpp"
#include "Get.hpp"
#include "Post.hpp"

HttpMethod::~HttpMethod() {}
HttpMethod *HttpMethod::Build(const Method &m) {
  if (m == kDelete) return new Delete();
  if (m == kGet) return new Get();
  if (m == kPost) return new Post();
  return NULL;
}
