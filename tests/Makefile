##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Compiles
##

CC			=	gcc

SRC			:=	shell lib lib2 setup_free parse_tree	\
				arrows main_error str_to_word_array prompt \
				built_in setup_shell alias verif_alias my_strcat_j

SRC			:=	$(addprefix ../src/, $(SRC))
SRC			+=	tester

OBJ			:=	$(addsuffix .o, $(SRC))
SRC			:=	$(addsuffix .c, $(SRC))

NAME		=	test

NAME2		=	$(NAME)

CFLAGS		+=	-W -Wall -Wextra

CPPFLAGS	+=	-iquote ../include

LDLIBS		+=	-l m

RM			?=	rm -f
# count total number of source files
NUM_FILES	=	$(words $(SRC))

CUR_FILE	=	0
BAR_LENGTH	=	27
MOVE_LENGHT	=	\e[$$(($(BAR_LENGTH) + 11))G
MOVE		=	\e[4G
# Define progress bar characters
PROGRESS	=	"≣"
ARROW		=	"⫸"
EMPTY		=	"\ "
SPIN_1		=	"|"
SPIN_2		=	"/"
SPIN_3		=	"—"
SPIN_4		=	"\\"

CLEAR_LINE	=	$(MOVE_LENGHT)\e[0K\r
# define most used colors
CLEAR_C		=	\e[0m
BLUE_C		=	\e[1;34m
# define cursor visibility
CURSOR_VIS	=	\e[?25h
CURSOR_INV	=	\e[?25l

all: 	update_spin $(NAME) stop_update_spin

update_spin:
	@printf "\e[1;31m" > .color
	@while true; do \
		date=$$(gdate +%3N 2> /dev/null || date +%3N); \
		date=$$(printf $$date | sed "s/^0*\([1-9]\)/\1/"); \
		spin=$$(($$date * 8 / 1000 + 1)); \
		spin=$$(printf "|/—\\|/—\\" | cut -c $$spin); \
		color=$$(cat .color); \
		printf "\e7\r$$color $$spin \e[0m\e8"; \
	done & \
	printf $$! > .make;

stop_update_spin:
	@pid=$$(cat .make); \
	kill $$pid;

%.o:	%.c
	@printf "$(CLEAR_LINE)"
	@-$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@printf "$(CURSOR_INV)$(MOVE_LENGHT)$(BLUE_C) Compiling $<\r"
	@$(eval CUR_FILE := $(shell printf $$(($(CUR_FILE) + 1))))
	@$(call progress_bar, $(CUR_FILE), $(NUM_FILES))
	@printf "$(CLEAR_C)\r$(CURSOR_VIS)"

$(NAME):	$(OBJ)
	@printf "\e[1;36m" > .color;
	@printf "$(CLEAR_LINE)"
	@-$(CC) $(OBJ) -o $(NAME) $(LDLIBS)
	@printf "$(CURSOR_INV)\e[1;36m$(CLEAR_LINE)$(MOVE)["
	@printf "%0.s$(PROGRESS)" $$(seq 1 $$(($(BAR_LENGTH))))
	@printf "] 100%% $(BLUE_C)Binary compilation completed!\n\
	$(CLEAR_C)$(CURSOR_VIS)"


clean:
	@$(RM) $(OBJ)
	@printf "$(BLUE_C)All .o successfully removed!\n$(CLEAR_C)"

fclean:
	@$(RM) $(OBJ) $(NAME)
	@printf "$(BLUE_C)All .o and binary successfully removed!\n$(CLEAR_C)"

re:	fclean all

debug:	CFLAGS += -g3
debug:	re

.PHONY: all clean fclean re debug

define progress_bar
	progress=$$(($(BAR_LENGTH) * $(1) / $(2))); \
	percent=$$(($(1) * 100 / $(2))); \
	if [ $$percent -lt 34 ]; then \
		color="\e[1;31m"; \
	elif [ $$percent -lt 67 ]; then \
		color="\e[1;33m"; \
	else \
		color="\e[1;32m"; \
	fi; \
	printf "$(MOVE)$$color["; \
	if [ $$progress -gt 1 ]; then \
		printf "%.s$(PROGRESS)" $$(seq $$(($$progress - 1))); \
	fi; \
	printf "$(ARROW)"; \
	if [ $$progress -lt $(BAR_LENGTH) ]; then \
		if [ $$progress -gt 1 ]; then \
			printf "%.s$(EMPTY)" $$(seq $$(($(BAR_LENGTH) - $$progress))); \
		else \
			printf "%.s$(EMPTY)" $$(seq $$(($(BAR_LENGTH) - 1))); \
		fi; \
	fi; \
	printf "] %3d%%" $$percent; \
	printf "$$color" > .color;
endef
