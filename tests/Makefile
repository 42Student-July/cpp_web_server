DOCKERPATH = ./docker
INTEGRATIONPATH = ./integration_test
WEBSERVDIR = ../
UNIT_TEST_PATH = ./unit_test
.PHONY: integra
integra:
	@make up -C $(DOCKERPATH)
	@make run -C $(INTEGRATIONPATH)
	@make down -C $(DOCKERPATH)

.PHONY: unittest
unittest:
	@make -C $(UNIT_TEST_PATH)
	@make fclean -C $(UNIT_TEST_PATH)
