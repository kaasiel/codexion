NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

TSAN_FLAGS	= -g -O1 -fsanitize=thread
ASAN_FLAGS	= -g -O1 -fsanitize=address,undefined

INC_DIR		= includes
SRC_DIR		= srcs
OBJ_DIR		= obj

SRCS		= $(SRC_DIR)/coder.c \
		  $(SRC_DIR)/strategy.c \
		  $(SRC_DIR)/init.c \
		  $(SRC_DIR)/monitor.c \
		  $(SRC_DIR)/scheduler.c \
		  $(SRC_DIR)/helper.c \
		  $(SRC_DIR)/args.c \
		  $(SRC_DIR)/args_utils.c \
		  $(SRC_DIR)/acquire.c \
		  $(SRC_DIR)/heap_utils.c \
		  $(SRC_DIR)/heap.c \
		  $(SRC_DIR)/window.c \
		  $(SRC_DIR)/additional.c \
		  $(SRC_DIR)/main.c
OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS		= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -I$(INC_DIR) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) obj_tsan obj_asan

fclean: clean
	rm -f $(NAME) $(NAME)_tsan $(NAME)_asan

re: fclean all

tsan:
	$(MAKE) NAME=$(NAME)_tsan OBJ_DIR=obj_tsan CFLAGS="$(CFLAGS) $(TSAN_FLAGS)" all

asan:
	$(MAKE) NAME=$(NAME)_asan OBJ_DIR=obj_asan CFLAGS="$(CFLAGS) $(ASAN_FLAGS)" all

.PHONY: all clean fclean re tsan asan