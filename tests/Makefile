DOCKERPATH = ./docker
INTEGRATIONPATH = ./integration_test
WEBSERVDIR = ../
.PHONY: nginx
nginx:
	@make up -C $(DOCKERPATH)
	python3 $(INTEGRATIONPATH)/main.py
	@make down -C $(DOCKERPATH)

.PHONY: webserv
webserv:
	python3 $(INTEGRATIONPATH)/main.py

.PHONY: unittest
unittest:
