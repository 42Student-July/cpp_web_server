TESTSRCS = RioFdTest.cc
SRCS = RioFileDescriptor.cpp
UTILSSRCS = Fd.cpp
SRCDIR = ../../srcs/
UTILSDIR = ../../srcs/utils/
ADDSRCS = $(addprefix $(SRCDIR),$(SRCS)) $(addprefix $(UTILSDIR),$(UTILSSRCS))
OBJS =$(TESTSRCS:.cc=.o) $(ADDSRCS:.cpp=.o)
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