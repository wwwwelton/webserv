NAME =		webserv

SRC_DIR =	sources

OBJ_DIR =	objects

SOURCES =	$(addprefix $(SRC_DIR)/, $(SRC_FIL))

OBJECTS =	$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HEADERS =	$(addprefix $(SRC_DIR)/, $(HDR_FIL))

HDR_FIL =	webserv.h Server.hpp Utils.hpp ServerConfig.hpp Pollfd.hpp
HDR_FIL += HttpRequest.hpp HttpBase.hpp HttpHandler.hpp

SRC_FIL =		webserv.cpp Server.cpp Utils.cpp init_servers.cpp ServerConfig.cpp
SRC_FIL +=	Pollfd.cpp HttpBase.cpp HttpRequest.cpp HttpHandler.cpp

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
				$(CC) $(CFLAGS) -c $< -o $@

CC =		c++
CFLAGS =	-Wall -Wextra -Werror -g
CFLAGS +=	-std=c++98 -pedantic-errors

RM =		rm -rf

all:		$(NAME)

bonus:		$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJECTS) $(HEADERS)
			$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) -I ./sources

$(OBJ_DIR):
			mkdir -p $(OBJ_DIR)

clean:
			$(RM) $(OBJ_DIR)

fclean:		clean
			$(RM) $(NAME)

re:			clean all

.PHONY:		all clean fclean re
