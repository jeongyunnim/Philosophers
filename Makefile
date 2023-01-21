NAME		=	./philosopher

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g3 -fsanitize=address

INC			=	./philosopher.h

SRC			=	./philosopher.c\
				./ft_atoi.c\

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