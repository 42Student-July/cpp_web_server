SRCS = sample.cc sample_test.cc
UTILSSRCS = Fd.cpp
SRCDIR = ./echo_epoll/
UTILSDIR = ./utils/
ADDSRCS =
OBJS =$(SRCS:.cc=.o) #$(SRCS:.cpp=.o)
CXX	= g++
CXXFLAGS = -std=c++11
NAME = a.out
RM = rm -f

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) -L/usr/local/lib -lgtest -lgtest_main -pthread
			./a.out

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY:		all clean fclean re