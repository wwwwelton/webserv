CC      = c++
CFLAGS  = -g3 -Wall -Wextra -Werror -Wno-long-long -std=c++98 -pedantic
CFLAGS  += -MMD -MP

INCPATH = -I./sources
NAME    = webserv

SRC     = HttpBase.cpp \
          HttpHandler.cpp \
          HttpRequest.cpp \
          Pollfd.cpp \
          Server.cpp \
          Config.cpp \
          init_servers.cpp \
          webserv.cpp

INC     = HttpBase.hpp \
          HttpHandler.hpp \
          HttpRequest.hpp \
          Pollfd.hpp \
          Server.hpp \
          Config.hpp \
          defines.hpp

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

re: clean all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re

-include $(DEPS)
