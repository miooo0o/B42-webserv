NAME 		=	webserv

CPP			=	g++
#CPP			=	c++
#CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g -O0
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g

INCLUDES	=	./

OBJDIR 		= 	objs

SRCDIR		=	../../..

SRCS		=	srcs/Request.cpp \
				srcs/Error.cpp \
				srcs/utility_functions.cpp \
				srcs/Logs.cpp \
				srcs/TestClasses/testUtils.cpp \
				srcs/headers.cpp \
				damianServer/Config.cpp \
				damianServer/Route.cpp \
				damianServer/ConfigParser.cpp \
				main.cpp

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -I $(INCLUDES) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re