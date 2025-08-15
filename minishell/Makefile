NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = Libft/Libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS_DIR = src
# Find all .c files recursively in src/ directory
# Using a single wildcard pattern to avoid duplicates
SRCS = $(wildcard $(SRCS_DIR)/*.c) \
       $(wildcard $(SRCS_DIR)/*/*.c)

OBJS_DIR = obj
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

INCLUDES_DIR = include
INCLUDES = -I$(INCLUDES_DIR) -I$(LIBFT_DIR)

all: $(NAME)

$(LIBFT): 
	@make -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)

fclean:
	@rm -rf $(OBJS_DIR)
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re


