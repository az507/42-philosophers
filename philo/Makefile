# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: achak <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/09 10:59:45 by achak             #+#    #+#              #
#    Updated: 2024/04/29 16:02:23 by achak            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo
SRCS	= $(shell find srcs -type f -name "*.c")
OBJ_DIR	= objs
OBJS	= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
CC		= cc
IFLAGS	= -I.
CFLAGS	= -Wall -Werror -Wextra
VPATH	= srcs

all:	$(NAME)

$(NAME):$(OBJS)
	$(CC) $(IFLAGS) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o:%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(IFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
