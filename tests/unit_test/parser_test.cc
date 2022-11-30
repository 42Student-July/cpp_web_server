#include <gtest/gtest.h>
#include "Parser.hpp"
const std::string path = "./config_test_case/err_case/";
void test(const std::string &str){
	Lexer l(str);
	l.Tokenize();
	Parser p(l);
	p.Parse();
}
TEST(parser_err, empty){
	EXPECT_ANY_THROW(test(path + "empty"));
}
TEST(parser_err, no_semicolon){
	EXPECT_ANY_THROW(test(path + "no_semicolon"));
}
TEST(parser_err, isdir){
	EXPECT_ANY_THROW(test(path + "isdir"));
}
TEST(parser_err, kakko_saigo_ooi){
	EXPECT_ANY_THROW(test(path + "kakko_saigo_ooi"));
}
TEST(parser_err, kakko_saisyo_ooi){
	EXPECT_ANY_THROW(test(path + "kakko_saisyo_ooi"));
}
TEST(parser_err, kakko_saigo_nai){
	EXPECT_ANY_THROW(test(path + "kakko_saigo_nai"));
}
TEST(parser_err, kakko_saisyo_nai){
	EXPECT_ANY_THROW(test(path + "kakko_saisyo_nai"));
}
TEST(parser_err, kakkohutatu){
	EXPECT_ANY_THROW(test(path +"kakkohutatu"));
}
TEST(parser_err, body_size_duplicate){
	EXPECT_ANY_THROW(test(path + "body_size_duplicate"));
}
TEST(parser_err, body_size_lower_over){
	EXPECT_ANY_THROW(test(path + "body_size_lower_over"));
}
TEST(parser_err, body_size_over){
	EXPECT_ANY_THROW(test(path + "body_size_over"));
}
TEST(parser_err, body_size_no_val){
	EXPECT_ANY_THROW(test(path + "body_size_no_val"));
}
TEST(parser_err, listen_duplicate){
	EXPECT_ANY_THROW(test(path + "listen_duplicate"));
}
TEST(parser_err, listen_no_val){
	EXPECT_ANY_THROW(test(path + "listen_no_val"));
}
TEST(parser_err, server_name_no_val){
	EXPECT_ANY_THROW(test(path + "server_name_no_val"));
}
TEST(parser_err, server_name_kakko){
	EXPECT_ANY_THROW(test(path + "server_name_kakko"));
}
TEST(parser_err, server_name_finish){
	EXPECT_ANY_THROW(test(path + "server_name_finish"));
}
TEST(parser_err, error_page_over){
	EXPECT_ANY_THROW(test(path + "error_page_over"));
}
TEST(parser_err, error_page_lower_over){
	EXPECT_ANY_THROW(test(path + "error_page_lower_over"));
}
TEST(parser_err, error_page_no_file){
	EXPECT_ANY_THROW(test(path + "error_page_no_file"));
}
TEST(parser_err, error_page_kakko){
	EXPECT_ANY_THROW(test(path + "error_page_kakko"));
}
TEST(parser_err, location_kakkonai){
	EXPECT_ANY_THROW(test(path + "location_kakkonai"));
}
TEST(parser_err, location_kakkooi){
	EXPECT_ANY_THROW(test(path + "location_kakkooi"));
}
TEST(parser_err, location_kakko_sukunai){
	EXPECT_ANY_THROW(test(path + "location_kakko_sukunai"));
}
TEST(parser_err, location_no_val){
	EXPECT_ANY_THROW(test(path + "location_no_val"));
}
TEST(parser_err, location_duplicate){
	EXPECT_ANY_THROW(test(path + "location_duplicate"));
}
TEST(parser_err, limit_except_duplicate){
	EXPECT_ANY_THROW(test(path + "limit_except_duplicate"));
}
TEST(parser_err, limit_except_not_method){
	EXPECT_ANY_THROW(test(path + "limit_except_not_method"));
}
TEST(parser_err, redirect_many_argment){
	EXPECT_ANY_THROW(test(path + "redirect_many_argment"));
}
TEST(parser_err, redirect_not_digit){
	EXPECT_ANY_THROW(test(path + "redirect_not_digit"));
}
TEST(parser_err, root_many_argment){
	EXPECT_ANY_THROW(test(path + "root_many_argment"));
}
TEST(parser_err, root_duplicate){
	EXPECT_ANY_THROW(test(path + "root_duplicatet"));
}
TEST(parser_err, auto_index_many_argment){
	EXPECT_ANY_THROW(test(path + "auto_index_many_argment"));
}
TEST(parser_err, auto_index_invalid_argment){
	EXPECT_ANY_THROW(test(path + "auto_index_invalid_argment"));
}
TEST(parser_err, index_no_val){
	EXPECT_ANY_THROW(test(path + "index_no_val"));
}
TEST(parser_err, cgi_extension_no_extension){
	EXPECT_ANY_THROW(test(path + "cgi_extension_no_extension"));
}
TEST(parser_err, cgi_extension_no_comma){
	EXPECT_ANY_THROW(test(path + "cgi_extension_no_comma"));
}
TEST(parser_err, cgi_extension_middle_comma){
	EXPECT_ANY_THROW(test(path + "cgi_extension_middle_comma"));
}
TEST(parser_err, cgi_extension_end_comma){
	EXPECT_ANY_THROW(test(path + "cgi_extension_end_comma"));
}
TEST(parser_err, upload_pass_duplicate){
	EXPECT_ANY_THROW(test(path + "upload_pass_duplicate"));
}
TEST(parser_err, upload_pass_two_argment){
	EXPECT_ANY_THROW(test(path + "upload_pass_two_argment"));
}
