#NAME 		=	test_parse_headers.cpp
#NAME 		=	test_parse_method.cpp
#NAME 		=	test_parse_version.cpp
NAME 		=	test_request_with_config.cpp

CPP			=	g++
#CPP			=	c++
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g -O0

INCLUDES	=	./

OBJDIR 		= 	objs
SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/TestClasses/testUtils.cpp tests/test_request_with_config.cpp damienServer/Config.cpp damienServer/Route.cpp damienServer/ConfigParser.cpp 

#SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/TestClasses/testUtils.cpp tests/main.cpp
#DAMIAN CONFIG TEST : SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/TestClasses/testUtils.cpp tests/test_request_with_config.cpp damienServer/Config.cpp damienServer/Route.cpp damienServer/Server.cpp

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -I $(INCLUDES) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re