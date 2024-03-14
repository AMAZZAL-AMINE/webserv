# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mamazzal <mamazzal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/27 10:49:29 by mamazzal          #+#    #+#              #
#    Updated: 2024/03/13 21:34:06 by mamazzal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=webserv

SRC= main.cpp parsing/Config.cpp atoi.cpp server/Server.cpp parsing/request/ParsRequest.cpp  cgi/cgi.cpp server/response_error.cpp parsing/ConfigErrors.cpp
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