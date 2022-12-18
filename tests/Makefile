DOCKERPATH = ./docker
INTEGRATIONPATH = ./integration_test
WEBSERVDIR = ../
UNIT_TEST_PATH = ./unit_test
.PHONY: integra
integra:
	@make nginx_up -C $(DOCKERPATH)
	@make run -C $(INTEGRATIONPATH)
	@make nginx_down -C $(DOCKERPATH)

.PHONY: unittest
unittest:
	@make -C $(UNIT_TEST_PATH)
	@make fclean -C $(UNIT_TEST_PATH)


.PHONY: lint
lint:
	@make lint -C $(WEBSERVDIR)	

.PHONY: dc-unittest
dc-unittest:
	@make unittest-up -C $(DOCKERPATH)


.PHONY: nginx
nginx:
	@make nginx -C $(DOCKERPATH)
.PHONY: nginx_down
nginx_down:
	@make nginx_down -C $(DOCKERPATH)
.PHONY: webserv
	@make webserv -C $(DOCKERPATH)
