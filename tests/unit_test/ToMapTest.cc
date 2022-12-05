#include <gtest/gtest.h>

#include "Parser.hpp"

const std::string path = "./text/ToMap.text";

TEST(tomap, tomap) {
  ServerContext a1;
  ServerContext a2;
  ServerContext a3;
  ServerContext b1;
  ServerContext b2;
  ServerContext b3;
  ServerContext c1;
  ServerContext c2;
  ServerContext c3;
  a1.listen.second = "8080";
  a2.listen.second = "8080";
  a3.listen.second = "8080";
  a1.server_name = "a";
  a2.server_name = "b";
  a3.server_name = "c";
  b1.listen.second = "8081";
  b2.listen.second = "8081";
  b3.listen.second = "8081";
  b1.server_name = "a";
  b2.server_name = "b";
  b3.server_name = "c";
  c1.listen.second = "8081";
  c2.listen.second = "8081";
  c3.listen.second = "8081";
  c1.listen.first = "1";
  c2.listen.first = "1";
  c3.listen.first = "1";
  c1.server_name = "a";
  c2.server_name = "b";
  c3.server_name = "c";
  ContextMap m;
  std::vector<ServerContext> v1;
  std::vector<ServerContext> v2;
  std::vector<ServerContext> v3;
  v1.push_back(a1);
  v1.push_back(a2);
  v1.push_back(a3);
  v2.push_back(b1);
  v2.push_back(b2);
  v2.push_back(b3);
  v3.push_back(c1);
  v3.push_back(c2);
  v3.push_back(c3);
  m.insert(std::make_pair(a1.listen, v1));
  m.insert(std::make_pair(b1.listen, v2));
  m.insert(std::make_pair(c1.listen, v3));
  Lexer l(path);
  l.Tokenize();
  Parser p(l);
  p.Parse();
	ContextMap m2 = ServerContext::ToMap(p.GetConfig());
	ContextMap::iterator it2 = m2.begin();
	EXPECT_EQ(m.size(), m2.size());
	for(ContextMap::iterator it1 = m.begin(); it1 != m.end();it1++,it2++){
		std::vector<ServerContext> sv1 = it1->second;
		std::vector<ServerContext> sv2 = it2->second;
		EXPECT_EQ(sv1.size(), sv2.size());
		for(size_t i = 0; i < sv1.size(); i++){
			EXPECT_EQ(sv1[i].listen, sv2[i].listen);
			EXPECT_EQ(sv1[i].server_name, sv2[i].server_name);
		}
	}
}
