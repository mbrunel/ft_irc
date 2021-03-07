# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/07 11:33:08 by asoursou          #+#    #+#              #
#    Updated: 2021/03/07 11:33:09 by asoursou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= ircserv
SRC			:= $(wildcard src/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -std=c++98 -MMD -MP \
			   -I ./inc
LDFLAGS		:= 
BUILD_DIR	:= .build
DEP			:= $(SRC:src/%.cpp=$(BUILD_DIR)/%.d)
OBJ			:= $(DEP:.d=.o)
C_RED		:= \033[31m
C_GREEN		:= \033[32m
C_CYAN		:= \033[36m
C_NONE		:= \033[0m
CROSS		:= ✘
CHECK		:= ✔
BUILD_MSG	:= $(C_GREEN)$(CHECK)$(C_NONE)
REMOVE_MSG	:= $(C_RED)$(CROSS)$(C_NONE)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "$(BUILD_MSG) $@"

all: $(NAME)

bonus: all

clean:
	@rm -rf $(BUILD_DIR)
	@echo "$(REMOVE_MSG) $(BUILD_DIR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(REMOVE_MSG) $(NAME)"

re: fclean all

run: $(NAME)
	./$<

$(BUILD_DIR):
	@mkdir $@
	@echo "$(BUILD_MSG) $@"

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "$(BUILD_MSG) $(<F)"

-include $(DEP)

.PHONY: all bonus clean fclean re run
