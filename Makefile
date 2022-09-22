SRCS = main.cpp Server.cpp RioFileDescriptor.cpp ListenFd.cpp Epoll.cpp ParseRequestMessage.cpp HttpResponse.cpp
UTILSSRCS = Fd.cpp
SRCDIR = ./srcs/
UTILSDIR = ./srcs/utils/
ADDSRCS = $(addprefix $(SRCDIR),$(SRCS)) $(addprefix $(UTILSDIR),$(UTILSSRCS))
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