MK_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

TEST1	=	testPrograms/headerParsing/
TEST2	=	testPrograms/methodParsing/
TEST3	=	testPrograms/versionParsing/
TEST4	=	testPrograms/withConfig/

maketests:
	$(MAKE) -C $(TEST1)
	$(MAKE) -C $(TEST2)
	$(MAKE) -C $(TEST3)
	$(MAKE) -C $(TEST4)
 
headerTest:
	$(MAKE) -C $(TEST1)
	@echo "headerTest"

methodTest:
	$(MAKE) -C $(TEST2)
	@echo "methodTest"

versionTest:
	$(MAKE) -C $(TEST3)
	@echo "versionTest"

configTest:
	$(MAKE) -C $(TEST4)
	@echo "configTest"

fclean: clean
	$(MAKE) fclean -C $(TEST1)
	$(MAKE) fclean -C $(TEST2)
	$(MAKE) fclean -C $(TEST3)
	$(MAKE) fclean -C $(TEST4)

re: fclean all
.PHONY: all clean fclean re