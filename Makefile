CC      = c++
CFLAGS  = -g3 -Wall -Wextra -Werror -std=c++98 -pedantic
CFLAGS  += -MMD -MP

INCPATH  = -I./sources -I./sources/response -I./sources/request
INCPATH += -I./sources/server -I./sources/utils
NAME    = webserv

SRC     = main.cpp \
          WebServ.cpp \
          ResponseBase.cpp \
          Request.cpp \
          RequestParser.cpp \
          Pollfd.cpp \
          Server.cpp \
          ServerLocation.cpp \
          Config.cpp \
          Logger.cpp \
          Response.cpp \
          LoadException.cpp \
          validate_input.cpp \
          signal.cpp \
          String.cpp \
          ConfigHelper.cpp \


INC     = defines.hpp \
          WebServ.hpp \
          ResponseBase.hpp \
          Request.hpp \
          RequestParser.hpp \
          Pollfd.hpp \
          Server.hpp \
          ServerLocation.hpp \
          Config.hpp \
          Logger.hpp \
          Response.hpp \
          LoadException.hpp \
          validate_input.hpp \
          signal.hpp \
          String.hpp \
          ConfigHelper.hpp \

OBJDIR  = objects
OBJ     = $(SRC:%.cpp=$(OBJDIR)/%.o)
DEPS    = $(SRC:%.cpp=$(OBJDIR)/%.d)

vpath %.cpp sources sources/response sources/server sources/request \
sources/utils
vpath %.hpp sources sources/response sources/server sources/request \
sources/utils

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ)
	which php-cgi || sudo apt-get update && sudo apt-get install php-cgi
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

dep:
	which php-cgi || (sudo apt-get update && sudo apt-get install php-cgi)

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

word:
	cd server_root && wget https://br.wordpress.org/latest-pt_BR.zip \
	&& unzip latest-pt_BR.zip && rm -rf latest-pt_BR.zip

test_unit:
	clear && cd test/source && npm test

test_intra:
	clear && cd test/bin && yes | ./ubuntu_tester http://localhost:8888

up:
	cd test/source && npm i
	make && clear && valgrind --leak-check=full --show-leak-kinds=all \
	--track-fds=yes ./webserv test/conf/tester.conf

client: client.cpp
	g++ client.cpp -g -o client

-include $(DEPS)
