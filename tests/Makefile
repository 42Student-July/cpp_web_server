DOCKERPATH = ./docker
INTEGRATIONPATH = ./integration_test
WEBSERVDIR = ../
.PHONY: integra
integra:
	@make up -C $(DOCKERPATH)
	@make run -C $(INTEGRATIONPATH)
	@make down -C $(DOCKERPATH)

.PHONY: config
config:
.PHONY: unittest
unittest:
