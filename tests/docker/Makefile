.PHONY: up
up:
	docker-compose up -d

.PHONY: down
down:
	docker-compose down --rmi all

.PHONY: re
re:	down up


.PHONY: unittest-up
unittest-up:
	docker compose up unittest -d --build

.PHONY: unittest-lint
unittest-lint:
	docker compose run unittest make lint

.PHONY:  unittest-build
unittest-run:
	docker compose run unittest make re

.PHONY:  unittest-run
unittest-run:
	docker compose run unittest make unit

.PHONY: nginx
nginx:
	docker compose up -d nginx
.PHONY: nginx_down
nginx_down:
	docker compose down --rmi all
.PHONY: webserv
webserv:
	docker compose up -d webserv
