from urllib import request
import requests
import config
import sys

# sys.path.append("../")
# sys.path.append("../lib")

def get_base_url()-> str:
    """
    Return the base URL of the server
    """
    return "http://localhost:{}/".format(config.SERVER_PORT)

def test_get()-> str:
    req = requests.get(get_base_url())
    if req.status_code != 200:
        return "Bad status code ."
    if req.text != "hello world":
        return "Bad content."
    if req.headers["Content-Length"] != "11":
        return "Bad Content-Length"
    if req.headers["Content-type"] != "text/html":
        return "Bad Content-Length"
    return ""

def test_get_dir_index()->str:
    req = requests.get(get_base_url() + "a")
    if req.status_code != 200:
        return "Bad status code."
    if req.text != "hellow":
        return "Bad content."
    return ""

def test_get_dir_not_allowed()->str:
    req = requests.get(get_base_url() + "virtual/a/")
    if req.status_code != 404:
        return "Bad status code."
    return ""

def test_wrong_post()-> str:
    req = requests.post(get_base_url())
    if req.status_code != 405:
        return "Bad status code."
    return ""

def test_too_big_request()-> str:
    payload = "a" *1025
    req = requests.post(get_base_url() + "port/test", data=payload)
    if req.status_code != 413:
        return "Bad status code: {}, expected {}".format(req.status_code, 413)
    return ""

def test_custom_404()->str:
    req = requests.get(get_base_url + "b")
    if req.status_code != 404:
        return "Bad status code."
    if req.text != "custom404":
        return "Bad error page"
    return ""

def test_404()->str:
    req = requests.get(get_base_url() +
        "doidjodoeijdosejfoejfseoifjseiofjsejfsejfesjfseofsejiseofj")
    if req.status_code != 404:
        return "Bad status code."
    return ""

def test_multiple_ports()-> str:
    req = requests.get(get_base_url())
    if req.text != "hello world":
        return "Bad content on first port."
    req = requests.get("http://localhost:{}/".format(config.SERVER_PORT2))
    if req.text != "second file":
        return "Bad content on second port."
    return ""

def test_different_index()-> str:
    req = requests.get(get_base_url())
    if req.text != "hello world":
        return "Bad first index."
    req = requests.get(get_base_url() + "2/")
    if req.text != "second file":
        return "Bad second index."
    return ""
def test_multiple_get()-> str:
    for i in range(100):
        req = requests.get(get_base_url())
        if req.status_code != 200 or req.text != "hellow world":
            return "Bad request at {}th iteration.".format(i + 1)
    return ""

def test_delete(port: int) -> str:
    request.post(get_base_url() + "port/test", data="test put and delete")
    if req.status_code != 201:
        return "Bad status on put request. {}".format(req.status_code)
    location = req.headers["Location"]
    if location != "/post/tmp/test":
        return "Bad Location header: {}, expected: {}".format(location, "/post/tmp/test")
    print(location)
    req = requests.get(get_base_url() + "post/tmp/test")
    if req.status_code != 200 and req.text != "test put and delete":
        return "Bad put request. {}".format(req.status_code)
    req = requests.delete(get_base_url() + "post/tmp/test")
    if req.status_code != 202 and req.status_code != 200:
        return "Bad status code for DELETE."
    req = requests.get(get_base_url() + location)
    if req.status_code != 404:
        return "File still exists after DELETE."
    return ""

def test_delete_no_file() -> str:
    req = requests.delete(get_base_url() + "post/gone")
    if req.status_code != 404:
        return "Bad status code for DELETE."
    return ""