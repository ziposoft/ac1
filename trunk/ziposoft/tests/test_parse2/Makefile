#
#         test_parse
#
#

CODEROOT= ../../..
SRC_PATH= ./
INC_PATH= .

TGT_NAME= test_parse
TGT= $(BIN_PATH)$(TGT_NAME)


include ../../common.mk 

$(TGT):   $(OBJS)  
	$(CPP) $(CFLAGS) -g $(OBJS) -o $(TGT) -L$(LIB_PATH) -lzipo $(LINK_OPTS)


	
allcpp:
	rm -f all.cpp
	touch all.cpp
	@for file in $(CFILES); do \
		echo "#include<$$file>" >> all.cpp; \
	done



