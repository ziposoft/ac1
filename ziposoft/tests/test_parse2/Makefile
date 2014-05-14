#
#         test_c
#
#

CODEROOT= ../../..
SRC_PATH= ./
INC_PATH= .

TGT_NAME= test_parse
TGT= $(BIN_PATH)$(TGT_NAME)


include ../../common.mk 
#order of lib linking is important??!!
$(TGT):   $(OBJS)  
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(TGT) -L$(LIB_PATH) -lzipo_parse -lzipo $(LINK_OPTS)


	


