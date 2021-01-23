# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amalliar <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/09 23:55:29 by amalliar          #+#    #+#              #
#    Updated: 2021/01/23 08:04:07 by amalliar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL      := /bin/sh
CC         := clang
CFLAGS     := -fdiagnostics-color -g -pipe
              #-march=native -O2 -flto
INCLUDE    := -I./include
NAME       := philo_one
SRCDIR     := src
OBJDIR     := .obj
DEPDIR     := .dep

SRCS       := main.c
SRCS       := $(addprefix src/, $(SRCS))
OBJS       := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS       := $(SRCS:$(SRCDIR)/%.c=$(DEPDIR)/%.d)

# Run multiple threads.
MAKEFLAGS  = -j 4 --output-sync=recurse --no-print-directory

# Protect against make incorrectly setting 'last modified' attribute
# when running in parallel (-j flag).
POST_COMPILE = mv -f $(DEPDIR)/$*.tmp $(DEPDIR)/$*.d && touch $@

# Define some colors for echo:
LGREEN     := \033[1;32m
WHITE      := \033[1;37m
NOC        := \033[0m

all: $(NAME)
$(NAME): $(OBJS)
	@echo -e "$(LGREEN)Linking executable $(NAME)$(NOC)"
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@
	@echo -e "Built target $(NAME)"
.PHONY: all

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPDIR)/%.d | $(OBJDIR) $(DEPDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MF $(DEPDIR)/$*.tmp -c $< -o $@
	@$(POST_COMPILE)
$(OBJDIR):
	@mkdir -p $@
$(DEPDIR):
	@mkdir -p $@
$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

clean:
	@echo -e "$(WHITE)Removing C object files...$(NOC)"
	@-rm -rf $(OBJDIR)
	@echo -e "$(WHITE)Removing make dependency files...$(NOC)"
	@-rm -rf $(DEPDIR)
.PHONY: clean

fclean: clean
	@echo -e "$(WHITE)Removing executable $(NAME)$(NOC)"
	@-rm -f $(NAME)
.PHONY: fclean

re:
	$(MAKE) fclean
	$(MAKE) all
.PHONY: re

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... fclean"
	@echo "... re"
.PHONY: help

# Do not include dependency files if the current goal is
# set to clean/fclean/re.
ifeq ($(findstring $(MAKECMDGOALS), clean fclean re),)
    -include $(DEPS)
endif
