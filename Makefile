# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rouali <rouali@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/27 10:49:29 by mamazzal          #+#    #+#              #
#    Updated: 2024/04/23 18:14:13 by rouali           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=webserv

SRC= main.cpp parsing/config/Config.cpp atoi.cpp server/Server.cpp parsing/request/ParsRequest.cpp  cgi/cgi.cpp parsing/ConfigErrors.cpp \
	response/Response.cpp response/Post.cpp response/Delete.cpp #server/response_error.cpp
OBJ=$(SRC:.cpp=.o)

FLAGS=-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

all: $(NAME)

run : all
	@./$(NAME) webserv.conf

$(NAME): $(OBJ)
	c++ $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all