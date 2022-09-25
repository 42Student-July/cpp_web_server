SRCS = main.cpp Server.cpp RioFileDescriptor.cpp ListenFd.cpp Epoll.cpp ParseRequestMessage.cpp HttpResponse.cpp HttpRequset.cpp
UTILSSRCS = Fd.cpp
SRCDIR = ./srcs/
UTILSDIR = ./srcs/utils/
ADDSRCS = $(addprefix $(SRCDIR),$(SRCS)) $(addprefix $(UTILSDIR),$(UTILSSRCS))
HEADERS := $(wildcard $(SRCDIR)*.hpp) $(wildcard $(UTILSDIR)*.hpp)
INCLUDES := $(addprefix -I,$(wildcard srcs/*))
OBJS =$(ADDSRCS:.cpp=.o)
CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror
NAME = a.out
RM = rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY:		all clean fclean re


.PHONY: format
format:
	clang-format -i $(HEADERS) $(ADDSRCS) -Werror
	clang-tidy -fix $(HEADERS) $(ADDSRCS) -- $(CXXFLANGS)


.PHONY: lint
lint: 
	cpplint --root . --filter=-legal/copyright,-build/include_subdir $(HEADERS) $(ADDSRCS)
	clang-format  $(HEADERS) $(ADDSRCS) -Werror --dry-run
	clang-tidy $(HEADERS) $(ADDSRCS) -- $(CXXFLANGS)


# ------------------------ Rules For Developer ----------------------------

.PHONY: setup
setup: ## Set up hooks for commit
	cp ./.githooks/pre-commit ./.git/hooks/pre-commit
	chmod +x ./.git/hooks/pre-commit
	mkdir -p ./srcs
