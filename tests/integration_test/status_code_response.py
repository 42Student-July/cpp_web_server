from unittest import result
import config
import re
import difflib
import requests
RESET = "\033[0m"

C_BLACK = "\033[30m"
C_RED = "\033[31m"
C_GREEN = "\033[32m"
C_YELLOW = "\033[33m"
C_BLUE = "\033[34m"
C_MAGENTA = "\033[35m"
C_CYAN = "\033[36m"
C_WHITE = "\033[37m"
C_GRAY = "\033[90m"
# color letters Gras
C_B_BLACK = "\033[30;01m"
C_B_RED = "\033[31;01m"
C_B_GREEN = "\033[32;01m"
C_B_YELLOW = "\033[33;01m"
C_B_BLUE = "\033[34;01m"
C_B_MAGENTA = "\033[35;01m"
C_B_GRAY = "\033[36;01m"
C_B_WHITE = "\033[37;01m"
C_B_GRAY = "\033[90;01m"
# color background
B_BLACK = "\033[40m"
B_RED = "\033[41m"
B_GREEN = "\033[42m"
B_YELLOW = "\033[43m"
B_BLUE = "\033[44m"
B_MAGENTA = "\033[45m"
B_GRAY = "\033[46m"
B_WHITE = "\033[47m"
B_GRAY = "\033[100m"
def send_request(url:str)->requests.Response:
	return requests.get(url)
	

def response_diff(nginx:requests.Response, webserv:requests.Response)->list:
    l = []
    if nginx.status_code != webserv.status_code:
        l.append(nginx.status_code)
        l.append(webserv.status_code)
    if nginx.headers["content-type"] != webserv.headers["content-type"]:
        l.append(nginx.headers["content-type"])
        l.append(webserv.headers["content-type"])
    return l

def run(url:str)->str:
    try:
        print(config.SERVER_NAME + ":" + config.NGINX_PORT + url)
        print(config.SERVER_NAME + ":" + config.WEBSERV_PORT + url)
        nginx_response = send_request("http://" + config.SERVER_NAME + ":" + config.NGINX_PORT + url)
        webserv_response = send_request("http://" + config.SERVER_NAME + ":" +config.WEBSERV_PORT + url)
    except Exception as err:
        print(err)
        print("{}Cannot connect to the server {}{}".format(C_B_RED, url, RESET))
        exit(1)
    res = response_diff(webserv_response, nginx_response)
    char = ""
    color = C_GREEN
    if len(res) == 0:
        char = "✅"
    else:
        color = C_RED
        char =  "❌"
    print(r"{}{:40}{} {}{} {}".format(C_B_GRAY,url,RESET, color, char , RESET))
    if len(res) != 0:
        print_diff(res)

def print_diff(res:list)->None:
    for line in res:
        print(line)
