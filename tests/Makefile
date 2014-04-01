
NAME_C		=	client
NAME_S		=	server

CXX		=	g++
RM		=	rm -f

CXXFLAGS	+=	-W -Wall -Wextra -ansi -pedantic
CXXFLAGS	+=	-I../
#CXXFLAGS	+=	-ggdb
BINFLAGS	+=	-L../ -lVriskaCore -lVriskaThreads -lVriskaNetwork
BINFLAGS	+=	-Wl,-rpath,`pwd`/..

SRC_C		=	src/client.cpp
OBJ_C		=	$(SRC_C:.cpp=.o)

SRC_S		=	src/server.cpp
OBJ_S		=	$(SRC_S:.cpp=.o)


all:		Vriska $(NAME_C) $(NAME_S)

$(NAME_C):	Vriska $(OBJ_C)
		$(CXX) -o $@ $(OBJ_C) $(BINFLAGS)

$(NAME_S):	Vriska $(OBJ_S)
		$(CXX) -o $@ $(OBJ_S) $(BINFLAGS)

Vriska:
		make -C ..

clean:
		$(RM) $(OBJ_C) $(OBJ_S)

fclean:		clean
		$(RM) $(NAME_C) $(NAME_S) $(LIB)

re:		fclean all

.PHONY:		all clean fclean re Vriska
