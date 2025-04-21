# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/23 20:10:13 by achak             #+#    #+#              #
#    Updated: 2024/07/31 20:10:46 by achak            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo_bonus
SRCS	= main.c philo.c routines.c utils.c cleanup.c
OBJ_DIR	= objs
OBJS	= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
CC		= cc
CFLAGS	= -Wall -Werror -Wextra
VPATH	= srcs

all:	$(NAME)

asan:	CFLAGS += -g -fsanitize=address
asan:	fclean
asan:	$(NAME)

tsan:	CFLAGS += -g -fsanitize=thread
tsan:	fclean
tsan:	$(NAME)

$(NAME):$(OBJS)
	$(CC) -I. $(CFLAGS) $(OBJS) -o $(NAME) -lpthread -lrt

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -I. $(CFLAGS) -c $< -o $@ -pthread

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
