NAME		:= ircserv
CERT		:= ircserv.ssl.crt
BUILD_DIR	:= .build
LIB_DIR		:= lib
LIBC4S_DIR	:= $(LIB_DIR)/config4cpp
LIBC4S		:= $(LIBC4S_DIR)/lib/libconfig4cpp.a
LIBIRC_DIR	:= $(LIB_DIR)/libIRC
LIBIRC		:= $(LIBIRC_DIR)/libIRC.a
SUB_DIR		:= $(dir $(wildcard src/*/.))
SRC			:= $(wildcard src/*/*/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -Ofast -std=c++98 -MMD -MP -g \
			   -I ./src $(foreach i,$(SUB_DIR:src/%=%),-I ./src/$i) \
			   -I ./$(LIBC4S_DIR)/include -I ./$(LIBIRC_DIR)/inc
LDFLAGS		:= -lcrypto -lssl -L ./$(LIBC4S_DIR)/lib -lconfig4cpp -L ./$(LIBIRC_DIR) -lIRC
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
	@make -C $(LIBC4S_DIR) BUILD_TYPE=release CXX=$(CXX) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBC4S))
	@make -C $(LIBIRC_DIR) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBIRC))
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@$(BUILD_MSG) $@

all: $(NAME) $(CERT)

bonus: all

clean:
	@make -C $(LIBIRC_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBIRC_DIR))
	@rm -rf $(BUILD_DIR)
	@$(REMOVE_MSG) $(BUILD_DIR)

fclean: clean
	@make -C $(LIBC4S_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBC4S))
	@make -C $(LIBIRC_DIR) fclean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBIRC))
	@rm -f $(NAME) $(CERT)
	@$(REMOVE_MSG) $(NAME) $(CERT)

re: fclean all

run: $(NAME) $(CERT)
	./$<

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(BUILD_MSG) $(@F)

$(CERT):
	openssl req -newkey rsa:2048 -nodes -keyout ircserv.ssl.key -x509 -days 365 -out $@ \
	-subj "/C=FR/ST=France/L=Paris/O=42/OU=Archeology/CN=42.fr/emailAddress=mbrunel@student.42.fr"

-include $(OBJ:.o=.d)

.PHONY: all bonus clean fclean re run
