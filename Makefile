NAME 		=	test_parse_headers
#NAME 		=	test_parse_method
#NAME 		=	test_parse_version
#NAME 		=	test_request_with_config

CPP			=	g++
#CPP			=	c++
#CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g -O0
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g

INCLUDES	=	./

OBJDIR 		= 	objs
SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/Logs.cpp srcs/TestClasses/testUtils.cpp tests/test_parse_headers.cpp srcs/headers.cpp damianServer/Config.cpp damianServer/Route.cpp damianServer/ConfigParser.cpp

#SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/Logs.cpp srcs/TestClasses/testUtils.cpp tests/test_request_with_config.cpp damianServer/Config.cpp damianServer/Route.cpp damianServer/ConfigParser.cpp
#SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/Logs.cpp srcs/TestClasses/testUtils.cpp tests/test_parse_headers.cpp damianServer/Config.cpp damianServer/Route.cpp damianServer/ConfigParser.cpp
#SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/Logs.cpp srcs/TestClasses/testUtils.cpp tests/test_parse_method.cpp damianServer/Config.cpp damianServer/Route.cpp damianServer/ConfigParser.cpp
#SRCS		=	srcs/Request.cpp srcs/Error.cpp srcs/utility_functions.cpp srcs/Logs.cpp srcs/TestClasses/testUtils.cpp tests/test_parse_version.cpp damianServer/Config.cpp damianServer/Route.cpp damianServer/ConfigParser.cpp

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