CC      = c++
CFLAGS  = -g3 -Wall -Wextra -Werror -std=c++98 -pedantic
CFLAGS  += -MMD -MP

INCPATH = -I./sources
NAME    = webserv

SRC     = main.cpp \
          WebServ.cpp \
          HttpBase.cpp \
          HttpResponse.cpp \
          HttpRequest.cpp \
          RequestParser.cpp \
          Pollfd.cpp \
          Server.cpp \
          Config.cpp \
          Logger.cpp \
          # init_servers.cpp \

INC     = defines.hpp \
          WebServ.hpp \
          HttpBase.hpp \
          HttpResponse.hpp \
          HttpRequest.hpp \
          RequestParser.hpp \
          Pollfd.hpp \
          Server.hpp \
          Config.hpp \
          Logger.hpp \

OBJDIR  = objects
SRC_DIR =	sources
OBJ     = $(SRC:%.cpp=$(OBJDIR)/%.o)
DEPS    = $(SRC:%.cpp=$(OBJDIR)/%.d)

vpath %.cpp sources
vpath %.hpp sources
vpath %.h   sources

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCPATH)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

$(OBJDIR):
	mkdir -p objects

re: fclean all

run: $(NAME)
	./$(NAME) ./default.conf

.PHONY: all clean fclean re

-include $(DEPS)
