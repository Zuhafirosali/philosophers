# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zfiros-a <zfiros-a@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 15:38:24 by zfiros-a          #+#    #+#              #
#    Updated: 2024/05/28 10:13:19 by zfiros-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers

CFLAGS = -Wall -Wextra -Werror -pthread

RM = rm -rf

SRC =	main.c \
		utils.c \
		libft_func.c \
		init.c \
		utils2.c \

$(NAME) :
	cc $(CFLAGS) $(SRC) -o $(NAME)

all : $(NAME)

clean :
	$(RM) $(NAME)

fclean : clean
	$(RM) $(NAME)

re : fclean all