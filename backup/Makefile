# Alias
CXX			= c++
VERSION		= -std=c++98
CXXFLAGS	= -Wall -Werror -Wextra $(VERSION)
RM			= rm

# standard
NAME		=	webserv
TARGET		=	./$(NAME)
OBJS_DIR	=	./objs/
SRCS_DIR	=	./srcs/
INCLUDE		=	-I./includes -I./srcs
#LIBS_DIR	=	./lib/

# sources
SRCS		=	$(shell find $(SRCS_DIR) -type f -name "*.cpp")

OBJS		=	$(patsubst $(SRCS_DIR)%.cpp, $(OBJS_DIR)%.o, $(SRCS))

# Main Command
all: $(NAME)

$(NAME):	$(OBJS)
	@$(CXX) $(INCLUDE) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "Makefile: $(TARGET) compiled."

$(OBJS_DIR)%.o : $(SRCS_DIR)%.cpp
	@mkdir -p $(@D)
	@#echo "compiling $<"
	@$(CXX) $(INCLUDE) -c $< -o $@

clean:
	@$(RM) -rf $(OBJS_DIR)

fclean:		clean
	@$(RM) -f $(NAME)

re:			fclean all

run: re
	@echo "Makefile: $(TARGET) executed.\n"
	@$(TARGET)

.PHONY:		all clean fclean re run