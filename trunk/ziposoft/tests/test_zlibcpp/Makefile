#
#         test_c
#
#

CODEROOT= ../../..
SRC_PATH= ./
INC_PATH= .

TGT_NAME= test_zlibcpp
TGT= $(BIN_PATH)$(TGT_NAME)


include ../../common.mk 
#order of lib linking is important??!!
$(TGT):   $(OBJS)  
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(TGT) -L$(LIB_PATH) -lzipo_cpp -lzipo_c  $(LINK_OPTS)


	


