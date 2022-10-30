SRCS = $(wildcard ../../srcs/Config/*.cpp) $(wildcard ../../srcs/Utils/*.cpp) *.cpp
OBJS =$(SRCS:.cpp=.o)
CXX	= c++
CXXFLAGS = -Wall -Wextra -Werror
NAME = a.out
RM = rm -f
INCS = $(addprefix -I,$(wildcard ../../srcs/*))
all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<
.PHONY:		all clean fclean re