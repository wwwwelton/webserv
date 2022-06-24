NAME =		webserv

SRC_DIR =	sources

OBJ_DIR =	objects

SOURCES =	$(addprefix $(SRC_DIR)/, $(SRC_FIL))

OBJECTS =	$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HEADERS =	$(addprefix $(SRC_DIR)/, $(HDR_FIL))

HDR_FIL =	webserv.h Server.hpp

SRC_FIL =	webserv.cpp Server.cpp

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
				$(CC) $(CFLAGS) -c $< -o $@

CC =		clang++
CFLAGS =	-Wall -Wextra -Werror
CFLAGS +=	-std=c++98 -pedantic-errors

RM =		rm -rf

all:		$(NAME)

bonus:		$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJECTS) $(HEADERS)
			$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

clean:
			$(RM) $(OBJ_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			clean all

.PHONY:		all clean fclean re
