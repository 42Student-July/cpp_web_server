SRCS = $(wildcard ../../srcs/Utils/*.cpp) $(wildcard ../../srcs/Server/*.cpp) $(wildcard ../../srcs/Config/*.cpp)
OBJS = $(SRCS:%.cpp=%.o)

HEADERS = $(wildcard ../../srcs/*/*.hpp)
INCS = $(addprefix -I,$(wildcard ../../srcs/*))
# TESTINCS = $(addprefix -I,$(wildcard *))
TESTSRCS = $(wildcard *.cc)
TESTOBJS = $(TESTSRCS:%.cc=%.o)
UNIT_TEST_NAME = unit_test
CXX	= g++
CXXFLAGS = -std=c++11
NAME = unit_test
RM = rm -f

ifdef REQ_ONLY
TESTOBJS = ReceiveHttpRequest_test.o 
endif

all:		$(NAME)

$(NAME):	$(OBJS) $(TESTOBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS) $(TESTOBJS) -L/usr/local/lib -lgtest -lgtest_main -pthread
			./$(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ -c $<
clean:
			$(RM) $(TESTOBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

sample: sample.cc sample_test.cc
		$(CXX) $(CXXFLAGS) -o a.out sample.cc sample_test.cc -L/usr/local/lib -lgtest -lgtest_main -pthread

request: $(OBJS) $(TESTOBJS)
		$(RM) $(NAME)
		make REQ_ONLY=1	

.PHONY:		all clean fclean re
