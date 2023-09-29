CXXFLAGS =	-I"/opt/sfw/xerces-c-3.0.1/src/xercesc" -O2 -g -Wall -fmessage-length=0

OBJS =		DLXML.o \
	DLXML_DataLinkUtil.o \
	DLXML_Encoder.o \
	DLXML_ErrorRepository.o \
	DLXML_MessageEncoder.o \
	DLXML_MessageRepository.o \
	DLXML_DecodeRuleProcessor.o \
	DLXML_EncodeRuleProcessor.o \
	DLXML_MessageDecoder.o \
	DLXML_Decoder.o \
	DLXML_VocabularyRepository.o \
	DLXML_MessageManager.o \
	DLXML_ParserErrorHandler.o \
	DLXML_EnumRepository.o \
	DLXML_Constants.o

LIBS = -lxerces-c.dll
LIBPATHS =	-L"/opt/sfw/xerces-c-3.0.1/src/.libs"

TARGET =	DLXML.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBPATHS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
