NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

SRC = minishell.c

OBJS = $(SRC:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

LIBS = -lreadline -lncurses

all : $(LIBFT) $(NAME)

$(NAME) : $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean :
	@$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean : clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re : fclean all

.PHONY : all clean fclean re
