# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/07 11:33:08 by asoursou          #+#    #+#              #
#    Updated: 2021/03/22 13:30:32 by mbrunel          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= ircserv
BUILD_DIR	:= .build
SUB_DIR		:= $(dir $(wildcard src/*/.))
SRC			:= $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -std=c++98 -MMD -MP -g \
			   $(foreach i,$(SUB_DIR:src/%=%),-I./src/$i)
LDFLAGS		:= -lcrypto -lssl
DOC_DIR		:= doc/html
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

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(BUILD_MSG) $(@F)

-include $(OBJ:.o=.d)

.PHONY: all bonus clean doc fclean re run
