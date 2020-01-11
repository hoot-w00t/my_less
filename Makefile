#
#    my_less
#    Copyright (C) 2020  akrocynova
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

GIT_VERSION	=	$(shell git describe --always --tags --abbrev=10 --dirty)

CC	=	cc
LIBS	=	-lncurses
INCLUDE	=	-I./include
CFLAGS	=	-Wall -Wextra -pipe	\
			$(INCLUDE) $(LIBS)	\
			-DGIT_VERSION=\"git-$(GIT_VERSION)\"

SRC	=	src/main.c		\
		src/version.c	\
		src/files.c		\
		src/interface.c

OBJS	=	$(SRC:.c=.o)

all:	my_less

my_less:	$(OBJS)
	$(CC) -o my_less $(OBJS) $(CFLAGS)

clean:
	@rm -f $(OBJS)

fclean:	clean
	@rm -f my_less

re:	fclean	all
