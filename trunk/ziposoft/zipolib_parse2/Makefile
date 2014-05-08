#
#                ZIPO 
#
#             ZIPOLIB  MAKFILE


CODEROOT= ../..
SRC_PATH= src/
INC_DIRS=-I include/


TGT_NAME= libzipo_parse.a
TGT= $(LIB_PATH)/$(TGT_NAME)

include ../common.mk 

$(TGT) : $(OBJS)
	ar cru	$(TGT)  $(OBJS)
	
allcpp:
	rm -f all.cpp
	touch all.cpp
	@for file in $(CFILES); do \
		echo "#include<$$file>" >> all.cpp; \
	done


