NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

INC_DIR		= includes
SRC_DIR		= srcs
OBJ_DIR		= obj

SRCS		= $(INC_DIR)/coders.c \
		  $(INC_DIR)/strategy.c \
		  $(INC_DIR)/init.c \
		  $(INC_DIR)/monitor.c \
		  $(INC_DIR)/scheduler.c \
		  $(INC_DIR)/helper.c \
		  $(SRC_DIR)/args.c \
		  $(SRC_DIR)/main.c
OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -I$(INC_DIR) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re