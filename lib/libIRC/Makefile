NAME		:= libIRC.a
BUILD_DIR	:= .build
INC_DIR		:= inc
SRC_DIR		:= src
SRC			:= $(wildcard $(SRC_DIR)/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -Ofast -std=c++98 -MMD -MP -I $(INC_DIR)
OBJ			:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(NAME): $(OBJ)
	ar -rcs $@ $^

all: $(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)

.PHONY: all clean fclean re
