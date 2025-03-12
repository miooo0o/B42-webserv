NAME 		=	webserv

CPP			=	g++
#CPP			=	c++
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g -O0

INCLUDES	=	./

OBJDIR 		= 	objs
SRCS		=	srcs/main.cpp srcs/Request.cpp srcs/utility_functions.cpp srcs/TestClasses/testUtils.cpp 
 
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