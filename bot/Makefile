NAME		:= nestor
BUILD_DIR	:= .build
LIB_DIR		:= ../lib
LIBFT_DIR	:= $(LIB_DIR)/libft
LIBFT		:= $(LIBFT_DIR)/libft.a
LIBIRC_DIR	:= $(LIB_DIR)/libIRC
LIBIRC		:= $(LIBIRC_DIR)/libIRC.a
LIBTCP_DIR	:= $(LIB_DIR)/libtcp
LIBTCP		:= $(LIBTCP_DIR)/libtcp.a
SRC			:= main.cpp
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -Ofast -g -std=c++98 -MMD -MP \
			   -I $(LIBFT_DIR)/inc -I $(LIBIRC_DIR)/inc -I $(LIBTCP_DIR)/inc
LDFLAGS		:= -L $(LIBIRC_DIR) -lIRC -L $(LIBFT_DIR) -lft -L $(LIBTCP_DIR) -ltcp -lcurl -lssl -lcrypto
OBJ			:= $(SRC:%.cpp=$(BUILD_DIR)/%.o)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR)
	make -C $(LIBIRC_DIR)
	make -C $(LIBTCP_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

all: $(NAME)

clean:
	make -C $(LIBFT_DIR) clean
	make -C $(LIBIRC_DIR) clean
	make -C $(LIBTCP_DIR) clean
	rm -rf $(BUILD_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(LIBIRC_DIR) fclean
	make -C $(LIBTCP_DIR) fclean
	rm -f $(NAME)

re: fclean all

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

.PHONY: all clean fclean re
