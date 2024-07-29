# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 20:10:13 by achak             #+#    #+#              #
#    Updated: 2024/07/29 13:54:24 by achak            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo_bonus
SRCS	= main.c params.c philo.c philo_utils.c monitor.c utils.c cleanup.c
OBJS	= $(SRCS:.c=.o)
CC	= cc
CFLAGS	= -Wall -Werror -Wextra -g -O1

all:	$(NAME)

asan:	CFLAGS += -g -fsanitize=address
asan:	fclean
asan:	$(NAME)

tsan:	CFLAGS += -g -fsanitize=thread
tsan:	fclean
tsan:	$(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -pthread

%.o:%.c
	$(CC) -c $(CFLAGS) $^ -o $@ -pthread

clean:
	rm $(OBJS)

fclean:	clean
	rm $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
