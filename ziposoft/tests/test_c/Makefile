#
#         test_c
#
#

CODEROOT= ../../..
SRC_PATH= ./
INC_PATH= .

TGT_NAME= test_c
TGT= $(BIN_PATH)$(TGT_NAME)


include ../../common.mk 

$(TGT):   $(OBJS)  
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(TGT) -L$(LIB_PATH) -lzipo_c $(LINK_OPTS)


	


