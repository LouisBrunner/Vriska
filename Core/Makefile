
NAME		=	libVriskaCore.so

DIR_SOURCE	=	src
DIR_INCLUDE	=	..

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX = clang++
else
	CXX		=	g++
endif
RM		=	rm -f

CXXFLAGS	+=	-W -Wall -Wextra -ansi -pedantic
CXXFLAGS	+=	-fvisibility=hidden -fPIC
CXXFLAGS	+=	-I$(DIR_INCLUDE)
#CXXFLAGS	+=	-ggdb
BINFLAGS	+=	-shared -fPIC

SRC		=	$(DIR_SOURCE)/Error.cpp			\
			$(DIR_SOURCE)/Exceptions.cpp	\
			$(DIR_SOURCE)/StringUtils.cpp	\
			$(DIR_SOURCE)/Time.cpp			\
			$(DIR_SOURCE)/Utils.cpp			\
			$(DIR_SOURCE)/OS.cpp

OBJ		=	$(SRC:.cpp=.o)


$(NAME):	$(OBJ)
		$(CXX) -o $@ $(OBJ) $(BINFLAGS)

all:		$(NAME)

clean:
		$(RM) $(OBJ)

distclean:		clean
		$(RM) $(NAME)

re:		distclean all

.PHONY:		all clean distclean re
