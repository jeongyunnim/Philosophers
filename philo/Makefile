NAME		=	./philo

CC			=	cc
CFLAGS		=	\
 -Wall -Wextra -Werror \


INC			=	./philosopher.h

SRC			=	./main.c\
				./ft_atoi.c\
				./ft_calloc.c\
				./parse_arguments.c\
				./init_structure.c\
				./philo_eat.c\
				./philo_sleep.c\
				./philo_think.c\
				./monitor_philo.c\
				./philo_do.c\
				./time_utils.c\
				./thread_create_join.c\
				./error_handle.c\

OBJ			=	$(SRC:%.c=%.o)

RM			=	rm -f

all			:	$(NAME)

$(NAME)		:	$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)		:	$(SRC) $(INC)
	$(CC) $(CFLAGS) -c $^

clean		:
	$(RM) $(OBJ) $(OBJ_BONUS)

fclean		:	clean
	$(RM) $(NAME) $(NAME_BONUS)

re			:	fclean all

.PHONY		:	all clean fclean re