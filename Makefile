
DIR_C	=	Core
NAME_C	=	libVriskaCore.so

DIR_T	=	Threads
NAME_T	=	libVriskaThreads.so

DIR_N	=	Network
NAME_N	=	libVriskaNetwork.so

DIR_TST	=	tests

MV	= 	mv
RM	=	rm -f

all:		$(NAME_C) $(NAME_T) $(NAME_N)

$(NAME_C):
		$(MAKE) -C $(DIR_C) $@
		$(MV) $(DIR_C)/$@ .

$(NAME_T):
		$(MAKE) -C $(DIR_T) $@
		$(MV) $(DIR_T)/$@ .

$(NAME_N):
		$(MAKE) -C $(DIR_N) $@
		$(MV) $(DIR_N)/$@ .

clean:
		$(MAKE) -C $(DIR_C) $@
		$(MAKE) -C $(DIR_T) $@
		$(MAKE) -C $(DIR_N) $@
		$(MAKE) -C $(DIR_TST) $@

fclean:
		$(MAKE) -C $(DIR_C) $@
		$(MAKE) -C $(DIR_T) $@
		$(MAKE) -C $(DIR_N) $@
		$(MAKE) -C $(DIR_TST) $@
		$(RM) $(NAME_C)
		$(RM) $(NAME_T)
		$(RM) $(NAME_N)

re:		fclean all

$(DIR_TST):
		$(MAKE) -C $@ re

.PHONY:		all clean fclean re $(DIR_TST)
