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

.PHONY: all fclean clean re bonus integration

-include $(DEPS)

# --------- sourcecodes ---------

# --------- formats ---------

.PHONY: format
format: ## Lint webserver source files
	clang-format -i $(HEADERS) $(SRCS) -Werror
	clang-tidy -fix $(HEADERS) $(SRCS) -- $(CXXFLAGS) $(INCS)

.PHONY: lint
lint: ## Lint webserver source files
	cpplint --root . --filter=-legal/copyright,-build/include_subdir $(HEADERS) $(SRCS)
	clang-format $(HEADERS) $(SRCS) --dry-run -Werror
	clang-tidy $(HEADERS) $(SRCS) -- $(CXXFLAGS) $(INCS)


# ------------------------ Rules For Developer ----------------------------

.PHONY: setup
setup: ## Set up hooks for commit
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit

