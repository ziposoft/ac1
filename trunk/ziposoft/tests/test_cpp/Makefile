#
#         test_c
#
#

CODEROOT= ../../..
SRC_PATH= ./
INC_PATH= .

TGT_NAME= test_cpp
TGT= $(BIN_PATH)$(TGT_NAME)


include ../../common.mk 

$(TGT):   $(OBJS)  
	$(CPP) $(CPPFLAGS) -g $(OBJS) -o $(TGT) -L$(LIB_PATH) -static $(LINK_OPTS)


	


