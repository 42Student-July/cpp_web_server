import os
import sys
#from turtle import color
from typing import Callable
from unittest import result
from tests import *

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

def cleanup() ->None:
    """
    Remove file created by the tester to make sure the test is new every run.
    """
    os.system("rm -rf www/tmp/*")
    os.system("rm -rf www/long.txt")

def run_test(test_name :str , test: Callable) -> None:
    """
    Runs a test defined in function test, with the name test_name,
    and prints wether it passed or not.
    """
    try:
        result = test()
    except:
        print("{}Cannot connect to the server on port {}{}".format(C_B_RED, config.SERVER_PORT, RESET))
        exit(1)
    char = ""
    color = C_GREEN
    if len(result) == 0:
        char = "✅"
    else:
        color = C_RED
        char =  "❌"
    print(r"{:40} {}{} {}{}".format(test_name, color, char , result , RESET))

def run()->None:
    """
    Entry tester
    """
    #test tuika
    run_test("GET /", test_get)

if __name__ == "__main__":
    cleanup()
    run()