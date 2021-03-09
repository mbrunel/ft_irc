# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/07 11:33:08 by asoursou          #+#    #+#              #
#    Updated: 2021/03/09 17:17:35 by asoursou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= ircserv
SRC			:= $(wildcard src/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -std=c++98 -MMD -MP -g
LDFLAGS		:= -lcrypto -lssl
DOC_DIR		:= doc/html
BUILD_DIR	:= .build
OBJ			:= $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)
C_RED		:= \033[31m
C_GREEN		:= \033[32m
C_CYAN		:= \033[36m
C_NONE		:= \033[0m
CROSS		:= ✘
CHECK		:= ✔
BUILD_MSG	:= @echo "$(C_GREEN)$(CHECK)$(C_NONE)"
REMOVE_MSG	:= @echo "$(C_RED)$(CROSS)$(C_NONE)"

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@$(BUILD_MSG) $@

all: $(NAME)

bonus: all

clean:
	@rm -rf $(DOC_DIR) $(BUILD_DIR)
	@$(REMOVE_MSG) $(DOC_DIR) $(BUILD_DIR)

doc:
	@doxygen doxygen.conf
	$(BUILD_MSG) "Doxygen documentation"

fclean: clean
	@rm -f $(NAME)
	@$(REMOVE_MSG) $(NAME)

re: fclean all

run: $(NAME)
	./$<

$(BUILD_DIR):
	@mkdir -p $@
	@$(BUILD_MSG) $(@F)

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(BUILD_MSG) $(@F)

-include $(OBJ:.o=.d)

.PHONY: all bonus clean doc fclean re run
