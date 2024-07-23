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
CC1	= cc -g -Wall -Werror -Wextra
CC2	= cc -g -Wall -Werror -Wextra -fsanitize=address
CC3	= cc -g -Wall -Werror -Wextra -fsanitize=thread
SRCS	= $(wildcard *.c)
ARGS	= 0

all:	$(NAME)

$(NAME):$(SRCS)
	$(CC3) $(SRCS) -pthread -o $(NAME)

1:
	$(CC1) $(SRCS) -pthread -o $(NAME)
	clear && valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) $(ARGS)

2:
	$(CC1) $(SRCS) -pthread -o $(NAME)
	clear && valgrind --tool=drd --fair-sched=yes ./$(NAME) $(ARGS)
	@#clear && valgrind --tool=drd --check-stack-var=no --segment-merging=yes
	@#--show-confl-seg=yes --show-stack-usage=no --var-info=no --fair-sched=yes

3:
	$(CC1) $(SRCS) -pthread -o $(NAME)
	clear && valgrind --tool=helgrind --fair-sched=yes ./$(NAME) $(ARGS)

3a:
	$(CC1) $(SRCS) -pthread -o $(NAME)
	clear && valgrind --tool=helgrind --fair-sched=yes --history-level=none ./$(NAME) $(ARGS)
	@#clear && valgrind --tool=callgrind ./$(NAME) $(ARGS)

4:
	$(CC2) $(SRCS) -pthread -o $(NAME)
	clear && ./$(NAME) $(ARGS)

5:
	$(CC3) $(SRCS) -pthread -o $(NAME)
	clear && ./$(NAME) $(ARGS)

clean:
ifneq ("$(wildcard $(NAME))","")
	rm $(NAME)
endif

fclean:	clean

re:	fclean all

.PHONY:	all clean fclean re
