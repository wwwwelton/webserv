NAME =		webserv

SRC_DIR =	sources

OBJ_DIR =	objects

SOURCES =	$(addprefix $(SRC_DIR)/, $(SRC_FIL))

OBJECTS =	$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HEADERS =	$(addprefix $(SRC_DIR)/, $(HDR_FIL))

HDR_FIL = webserv.h HttpBase.hpp HttpHandler.hpp HttpRequest.hpp Pollfd.hpp
HDR_FIL += Server.hpp ServerConfig.hpp

SRC_FIL = HttpBase.cpp HttpHandler.cpp HttpRequest.cpp Pollfd.cpp
SRC_FIL += Server.cpp ServerConfig.cpp init_servers.cpp webserv.cpp

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
