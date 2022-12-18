import sys
import os
from typing import Callable
import http.client
import config
import test
import status_code_response
#test_path
test_path = "./test_case/"
err_path = test_path + "error_case/"
ok_case = test_path + "ok_case"
# testname testfile
#test.run("TEST", err_path + "a.txt")
#test.run("HTTP VERSION 11 ", test_path + "http_version11.txt")

status_code_response.run("/")
