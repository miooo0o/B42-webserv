#####################################################################################################################
NAME = webserv
#####################################################################################################################

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g3 -fdiagnostics-color=always
DEBUG_MODE	:= -DDEBUG=true
#####################################################################################################################
# Paths to find the files
VPATH =	include/:		\
		src/:			\
		src/Main/:		\
		src/Parse:		\
		src/Server/:	\
		src/HTTPRequestResponse/:	\
		src/Methods/:
OBJ_FOLDER=obj

# NOTE: Headers
HEADERS=	default.hpp		\
			ConfigBase.hpp	\
			Location.hpp	\
			Server.hpp		\
			Client.hpp		\
			HTTPRequest.hpp	\
			HTTPResponse.hpp	\
			methods.hpp	\
			CGIClient.hpp

# NOTE: Source files
MAIN_SRC =	main.cpp		\
			signals.cpp		\
			process_requests.cpp
OBJ = $(MAIN_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

PARSE_SRC =	parse.cpp \
			parseLocation.cpp \
			parseServer.cpp \
			directives.cpp \
			getDirectives.cpp \
			checkDirectives.cpp
OBJ += $(PARSE_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

SERVER_SRC =	ConfigBase.cpp	\
				Location.cpp	\
				Server.cpp		\
				Client.cpp		\
				CGIClient.cpp
OBJ += $(SERVER_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

HTTPREQUESTRESPONSE_SRC =	HTTPRequest.cpp	\
							HTTPResponse.cpp \
							validate_requests.cpp
OBJ += $(HTTPREQUESTRESPONSE_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

METHODS_SRC =	get_method.cpp	\
				post_method.cpp	\
				delete_method.cpp \
				check_mode.cpp \
				error_send.cpp
OBJ += $(METHODS_SRC:%.cpp=$(OBJ_FOLDER)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -s CLEAN_EXTENSION
	@echo "\n$(COLOR_ORANGE)[ EXE ] Compiling $@..."
	@$(CXX) $^ -o $@

$(OBJ_FOLDER)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJ_FOLDER)

	@if [ ! -e $(OBJ_FOLDER)/.cpp ]; then			\
		touch $(OBJ_FOLDER)/.cpp;					\
		echo "[ C++ ] Compiling c++ files ($(CXXFLAGS) $(DEBUG_MODE))";		\
	fi

	@echo -n "\t Compiling $<"
	@RES=$$($(CXX) $(CXXFLAGS) $(DEBUG_MODE) -c $< -o $@ 2> $(OBJ_FOLDER)/.error; echo $$?);	\
	if [ $$RES -ne 0 ]; then													\
		echo "\r\t[ ERROR ]";							\
		tail -n +1 $(OBJ_FOLDER)/.error;										\
		make -s CLEAN_EXTENSION;												\
		exit $$RES;																\
	fi

	@echo "     [OK] "

# Rule to clean the extension flags that are created during compilation. This flags
# are used to print msgs
CLEAN_EXTENSION:
	@rm -rf $(OBJ_FOLDER)/.cpp $(OBJ_FOLDER)/.error

#####################################################################################################################

nd: no_debug
no_debug: DEBUG_MODE=-DDEBUG=false
no_debug: re v

s: siege
siege:
	@siege -t10s -c10 -b http://localhost:8080

#####################################################################################################################

# Standard clean rule
clean:
	@rm -rf $(OBJ_FOLDER)

# Standard fclean rule
fclean: clean
	@rm -rf $(NAME)

# Standard recompilation rule
re: fclean all

#####################################################################################################################

# Variables to execute the programm. It can be modified to add the parameters
# that you want to pass to the program.
EXECUTE_WAY	= ./$(NAME)
EXECUTE_ARGS= config/default.conf

# Debugger information
DEFAULT_SCRUBBER		= valgrind
DEFAULT_SCRUBBER_FLAGS	= -s --leak-check=full --show-leak-kinds=all --track-fds=yes


# Rule to clean the terminal
clear:
	@clear

# Rules to execute the program. Should be modified if the programm recieves
# any arguments or a pattern
t: run
r: run
run: all clear
	@$(SCRUBBER) $(SCRUBBER_FLAGS) $(EXECUTE_WAY) $(EXECUTE_ARGS)

# Rule to execute
v: valgrind
valgrind: all
valgrind: SCRUBBER=$(DEFAULT_SCRUBBER)
valgrind: SCRUBBER_FLAGS=$(DEFAULT_SCRUBBER_FLAGS)
valgrind: run

#####################################################################################################################

# Rules that won't be treated as files
.PHONY: all CLEAN_EXTENSION nd no_debug s siege clean fclean re r run v valgrind
