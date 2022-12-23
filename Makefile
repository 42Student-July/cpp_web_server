NAME = webserv
NAME_BONUS = webserv_bonus
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -std=c++98 -pedantic -MMD -MP
SRCS = $(wildcard srcs/*/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(OBJS:%.o=%.d)
HEADERS = $(wildcard srcs/*/*.hpp)
INCS = $(addprefix -I,$(wildcard srcs/*))
TESTSPATH = ./tests
UNIT_TEST_NAME = unit_test
ifdef DEBUG
	CXXFLAGS += -D DEBUG=true -g -fsanitize=address
endif

$(NAME): $(OBJS) ## Build webserver
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(NAME_BONUS): $(OBJS) ## Build webserver
	$(CXX) $(CXXFLAGS) -o $(NAME_BONUS) $(OBJS)

bonus: $(NAME_BONUS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<

all: $(NAME) ## Build webserver

fclean: clean ## Delete executable webserver
	$(RM) $(NAME)
	$(RM) $(NAME_BONUS)

clean: ## Delete webserver object files
	$(RM) $(OBJS) $(DEPS)

re: fclean all ## Rebuild webserver

debug: fclean ## Build in debug mode
	make DEBUG=true

integration: re
	make integra -C $(TESTSPATH)
unit:
	make unittest -C $(TESTSPATH)
	make fclean

nginx_up:
	make nginx -C $(TESTSPATH)

nginx_down:
	make nginx_down -C $(TESTSPATH)
test_compose_down: fclean
	make down -C ./tests/docker

N = 100
req:
	bash -c 'for i in {1..${N}}; do curl localhost:8088/cgi-bin/env.cgi; echo $${i} ; done'
.PHONY: all fclean clean re bonus integration unit

-include $(DEPS)

# --------- sourcecodes ---------

# --------- formats ---------

.PHONY: format
format: ## Lint webserver source files
	clang-format -i $(HEADERS) $(SRCS) -Werror
	clang-tidy -fix $(HEADERS) $(SRCS) -- $(CXXFLAGS) $(INCS)

.PHONY: lint
lint: ## Lint webserver source files
	cpplint --root . --filter=-legal/copyright,-build/include_subdir,-runtime/int $(HEADERS) $(SRCS)
	clang-format $(HEADERS) $(SRCS) --dry-run -Werror
	clang-tidy $(HEADERS) $(SRCS) -- $(CXXFLAGS) $(INCS)


# ------------------------ Rules For Developer ----------------------------

.PHONY: setup
setup: ## Set up hooks for commit
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit


# ------------------------- Rules For Docker ------------------------------

CONTAINER = webserv
DOCKER_COMPOSE_FILE = ./docker/$(CONTAINER)/docker-compose.yaml

.PHONY: dc-build
dc-build: ## Build docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) build 

.PHONY: dc-up
dc-up: ## Run docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) up -d

.PHONY: dc-down
dc-down: ## Down docker container
	docker compose -f $(DOCKER_COMPOSE_FILE) down --timeout 1

.PHONY: dc-re
dc-re: dc-down dc-build dc-up ## Rebuild docker image and run container

