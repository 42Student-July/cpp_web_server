#! /usr/bin/python3
import socket


IPADDR = "127.0.0.1"
PORT = 8087

for i in range(4097):
	sock = socket.socket(socket.AF_INET)
	# サーバーへ接続
	sock.connect((IPADDR, PORT))
	# byte 形式でデータ送信
	#sock.send("hello".encode("utf-8"))
