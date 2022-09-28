import http.client
from unittest import result
from urllib import request, response
import config
import re
import socket
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

def send_request(filepath:str, addr:str, port:int)->str:
    lines = read_file(filepath)
    cmd = replace_newline(lines)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((addr, port))
    msg = cmd.encode()
    sock.send(msg)
    data = sock.recv(4096)
    sock.close()
    return data.decode()

def read_file(filename:str)->list:
    f = open(filename, 'r')
    data = f.readlines()
    f.close()
    return data

def replace_newline(file_list:list)->str:
    str = ""
    for line in file_list:
        str += line.replace("\n","\r\n")
    return str
def list_diff(list1:list,list2:list)->list:
    res = list1.copy()
    for val in list2:
        if val in res:
            res.remove(val)
    return res
def run(test_name:str, filepath:str)->str:
    try:
        nginx_data = send_request(filepath, config.SERVER_ADDR, config.NGINX_PORT).split("\r\n")
        #websev_data = send_request(filepath, config.SERVER_ADDR, config.WEBSERV_PORT).split("\r\n")
        #webserv_data = send_request(filepath, config.SERVER_ADDR, config.NGINX_PORT).split("\r\n")
        webserv_data = ["a","b"]
    except:
        print("{}Cannot connect to the server on port {}{}".format(C_B_RED, config.SERVER_PORT, RESET))
        exit(1)
    res = list_diff(nginx_data, webserv_data)
    char = ""
    color = C_GREEN
    if len(res) == 0:
        char = "✅"
    else:
        color = C_RED
        char =  "❌"
    print(r"{}{:40}{} {}{} {}".format(C_B_GRAY,test_name,RESET, color, char , RESET))
    if len(res) != 0:
        print("{}disaccord{}\n".format(C_B_YELLOW,RESET))
        for line in res:
            print(line)
