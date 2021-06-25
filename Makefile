NAME		:= ircserv
CFG_DIR		:= config
SSL_DIR		:= ssl
SSL_CERT	:= $(SSL_DIR)/ircserv.crt
SSL_KEY		:= $(SSL_DIR)/ircserv.key
LOG			:= $(CFG_DIR)/ircserv.log
BUILD_DIR	:= .build
LIB_DIR		:= lib
LIBC4S_DIR	:= $(LIB_DIR)/config4cpp
LIBC4S		:= $(LIBC4S_DIR)/lib/libconfig4cpp.a
LIBFT_DIR	:= $(LIB_DIR)/libft
LIBFT		:= $(LIBFT_DIR)/libft.a
LIBIRC_DIR	:= $(LIB_DIR)/libIRC
LIBIRC		:= $(LIBIRC_DIR)/libIRC.a
LIBTCP_DIR	:= $(LIB_DIR)/libtcp
LIBTCP		:= $(LIBTCP_DIR)/libtcp.a
SRC			:= $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
CXX			:= clang++
CXXFLAGS	:= -Wall -Wextra -Werror -Wpedantic -Wvla -Ofast -std=c++98 -MMD -MP -DNDEBUG \
			   -I ./src -I ./src/Commands -I ./$(LIBC4S_DIR)/include -I ./$(LIBFT_DIR)/inc -I ./$(LIBIRC_DIR)/inc -I ./$(LIBTCP_DIR)/inc
LDFLAGS		:= -lcrypto -lssl -L ./$(LIBC4S_DIR)/lib -lconfig4cpp -L ./$(LIBFT_DIR) -lft -L ./$(LIBIRC_DIR) -lIRC -L ./$(LIBTCP_DIR) -ltcp
OBJ			:= $(SRC:src/%.cpp=$(BUILD_DIR)/%.o)
C_RED		:= \033[31m
C_GREEN		:= \033[32m
C_CYAN		:= \033[36m
C_NONE		:= \033[0m
CROSS		:= ✘
CHECK		:= ✔
BUILD_MSG	:= @echo "$(C_GREEN)$(CHECK)$(C_NONE)"
REMOVE_MSG	:= @echo "$(C_RED)$(CROSS)$(C_NONE)"
INSTL_DIR	:=ircserv_1.0-0_all
BIN_DIR		:=$(INSTL_DIR)/usr/bin/
CONF_DIR	:=$(INSTL_DIR)/etc
CONF		:=$(CONF_DIR)/ircserv

$(NAME): $(OBJ)
	@make -C $(LIBC4S_DIR) BUILD_TYPE=release CXX=$(CXX) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBC4S))
	@make -C $(LIBFT_DIR) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBFT))
	@make -C $(LIBIRC_DIR) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBIRC))
	@make -C $(LIBTCP_DIR) > /dev/null 2>&1
	@$(BUILD_MSG) $(notdir $(LIBTCP))
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@$(BUILD_MSG) $@

all: $(NAME) $(CERT)

bonus: all

certs: $(SSL_CERT)

clean:
	@make -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBFT_DIR))
	@make -C $(LIBIRC_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBIRC_DIR))
	@make -C $(LIBTCP_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBTCP_DIR))
	@rm -rf $(BUILD_DIR)
	@$(REMOVE_MSG) $(BUILD_DIR)

doc:
	@doxygen doxygen.conf
	$(BUILD_MSG) "Doxygen documentation"

fclean: clean
	@make -C $(LIBC4S_DIR) clean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBC4S))
	@make -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBFT))
	@make -C $(LIBIRC_DIR) fclean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBIRC))
	@make -C $(LIBTCP_DIR) fclean > /dev/null 2>&1
	@$(REMOVE_MSG) $(notdir $(LIBTCP))
	@rm -rf $(NAME) $(LOG) $(SSL_CERT) $(SSL_KEY) $(BIN_DIR) $(CONF_DIR) $(INSTL_DIR).deb
	@$(REMOVE_MSG) $(NAME) $(notdir $(LOG)) $(notdir $(SSL_CERT)) $(notdir $(SSL_KEY)) $(BIN_DIR) $(CONF_DIR) $(INSTL_DIR).deb

re: fclean all

run: $(NAME) $(SSL_CERT)
	./$< $(CFG_DIR)/ircserv.conf

$(INSTL_DIR).deb : all certs
	@install -D $(NAME) $(BIN_DIR)/$(NAME)
	@mkdir -p $(CONF)
	@cp -r config/* $(CONF)
	@dpkg-deb --build $(INSTL_DIR)

install: $(INSTL_DIR).deb
	@sudo dpkg -i $(INSTL_DIR).deb

uninstall:
	@sudo dpkg --remove $(NAME)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(BUILD_MSG) $(@F)

$(SSL_CERT):
	mkdir -p $(SSL_DIR)
	openssl req -newkey rsa:2048 -nodes -keyout $(SSL_KEY) -x509 -days 365 -out $@ \
	-subj "/C=FR/ST=France/L=Paris/O=42/OU=Archeology/CN=42.fr/emailAddress=mbrunel@student.42.fr"

-include $(OBJ:.o=.d)

.PHONY: all bonus certs clean doc fclean re run install uninstall
