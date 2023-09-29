#ifndef DLXML_CONSTANTS_H
#define DLXML_CONSTANTS_H

#include <string>

namespace DLXML
{
	/**
	 * @class Constants
	 * 
	 * @brief <p>Constants class simply contains constants used throughout DLXML library,
	 * Most of these constants are char pointer constants which are element and attribute
	 * names used in data link rule files and in other xml files. Apart from constant
	 * values it also has some other "non const" members which have default initial
	 * values but are set from configuration files during initialization of DLXML.</p>
	 **/
	class Constants {
	public:
		//General constants used throughout DLXML
		static const unsigned int INT_LINK_11_MESSAGE_SIZE;
		static const unsigned int INT_LINK_16_MESSAGE_SIZE;
		static const unsigned int INT_IJMS_MESSAGE_SIZE;

		static const char* const STR_STARS;
		static const char* const STR_EMPTY;
		static const char* const STR_UNDEFINED;
		static const char* const STR_ALL;
		static const char* const STR_NOT_DECODED;
		static const char* const STR_L11_AMPMSG_1ST_4BITS;
		static const char* const STR_L11_AMPMSG_CHAR;
		
		static const char* const STR_GS;//Stands for General symbol.=>(forza karakartal)
		static const char* const STR_DLRP_LATITUDE;
		static const char* const STR_DLRP_LONGITUDE;
		static const char* const STR_MEM_BUF_ID;
		static const char* const STR_NOT_OPERATIONAL;
		
		//Enum type names used in DLXML lib
		static const char* const STR_ENUM_DLXML_REL_OP;
		static const char* const STR_ENUM_DLXML_DATA_LINK_NET;
		static const char* const STR_ENUM_DLXML_ERROR_TYPE;
		static const char* const STR_ENUM_DLXML_ERROR_LEVEL;
		
		//Configuration File Element and Attribute names
		static const char* const XMLSTR_CFG_VOCABULARY;
		static const char* const XMLSTR_CFG_RULE_FILES;
		static const char* const XMLSTR_CFG_FILES;
		static const char* const XMLSTR_CFG_DIRECTORYURL;
		static const char* const XMLSTR_CFG_NAME;
		static const char* const XMLSTR_CFG_FILE;
		static const char* const XMLSTR_CFG_INPUT_STREAM_SCHEMA;
		static const char* const XMLSTR_CFG_SCHEMA_FILE;
		static const char* const XMLSTR_CFG_PARAMETERS;
		static const char* const XMLSTR_CFG_PARAM;
		static const char* const XMLSTR_CFG_KEY;
		static const char* const XMLSTR_CFG_VALUE;
		static const char* const XMLSTR_CFG_VALIDATE_INPUT_STREAMS;
		
		//Data Link Message Rule File Element and Attribute names
		static const char* const XMLSTR_DLR_FIELD;
		static const char* const XMLSTR_DLR_ORDER;
		static const char* const XMLSTR_DLR_MESSAGE_NAME;
		static const char* const XMLSTR_DLR_MESSAGE_NETWORK;
		static const char* const XMLSTR_DLR_DFI_DUI;
		static const char* const XMLSTR_DLR_ID;
		static const char* const XMLSTR_DLR_RESOLUTION;

		static const char* const XMLSTR_DLR_LOGICAL;
		static const char* const XMLSTR_DLR_CHAR_STREAM;
		static const char* const XMLSTR_DLR_NUMERIC;
		static const char* const XMLSTR_DLR_BINARY;
		static const char* const XMLSTR_DLR_SWITCH;
		static const char* const XMLSTR_DLR_CASE;
		static const char* const XMLSTR_DLR_SELECTIVE_COND;
		static const char* const XMLSTR_DLR_CASE_COND;
		static const char* const XMLSTR_DLR_AND;
		static const char* const XMLSTR_DLR_OR;
		static const char* const XMLSTR_DLR_BOOLEAN_NOT;
		static const char* const XMLSTR_DLR_OPERAND;
		static const char* const XMLSTR_DLR_NAME;
		static const char* const XMLSTR_DLR_LEFT_OPERAND;
		static const char* const XMLSTR_DLR_OPERATOR;
		static const char* const XMLSTR_DLR_RIGHT_OPERAND;
		static const char* const XMLSTR_DLR_DEFAULT_VALUE;
		static const char* const XMLSTR_DLR_DATA_TYPE;
		static const char* const XMLSTR_DLR_BIT_LENGTH;
		static const char* const XMLSTR_DLR_BIT_POSITION;
		static const char* const XMLSTR_DLR_TRUE;
		static const char* const XMLSTR_DLR_FALSE;
		static const char* const XMLSTR_DLR_RELATIONAL_EXPRESSION;
		static const char* const XMLSTR_DLR_RELATIONAL_EXPRESSIONS;
		
		static const char* const XMLSTR_DLR_TYP_FLOAT;
		static const char* const XMLSTR_DLR_TYP_DOUBLE;
		static const char* const XMLSTR_DLR_TYP_LONG;
		static const char* const XMLSTR_DLR_TYP_SHORT;
		static const char* const XMLSTR_DLR_TYP_BOOLEAN;
		static const char* const XMLSTR_DLR_TYP_STRING;
		static const char* const XMLSTR_DLR_GE;
		static const char* const XMLSTR_DLR_GT;
		static const char* const XMLSTR_DLR_EQ;
		static const char* const XMLSTR_DLR_NE;
		static const char* const XMLSTR_DLR_LT;
		static const char* const XMLSTR_DLR_LE;
		
		static const char* const XMLSTR_DLR_GENERIC_VALUES;
		static const char* const XMLSTR_DLR_ILLEGAL;
		static const char* const XMLSTR_DLR_UNDEFINED;
		static const char* const XMLSTR_DLR_INPUT_DATA;
		
		static const char* const XMLSTR_DLR_INC_RANGE;
		static const char* const XMLSTR_DLR_START;
		static const char* const XMLSTR_DLR_END;
		static const char* const XMLSTR_DLR_NO_STMT;
		static const char* const XMLSTR_DLR_STRING;
		static const char* const XMLSTR_DLR_LENGTH;
		static const char* const XMLSTR_DLR_SUBSTRING;
		static const char* const XMLSTR_DLR_START_POS;
		static const char* const XMLSTR_DLR_SUBSTRING_LEN;
		static const char* const XMLSTR_DLR_ENCODING_START_POS;
		static const char* const XMLSTR_DLR_ENCODED_CHAR_LEN;
		static const char* const XMLSTR_DLR_VOCABULARY;
		
		static const char* const XMLSTR_DLR_INPUT_MAPPING;
		static const char* const XMLSTR_DLR_BOUNDARY;
		static const char* const XMLSTR_DLR_UNBOUNDED_INTERVAL;
		static const char* const XMLSTR_DLR_BOUNDED_INTERVAL;
		static const char* const XMLSTR_DLR_UNIFORM_DISTRIBUTION;
		static const char* const XMLSTR_DLR_BIT_CODE;
		static const char* const XMLSTR_DLR_BIT_CODE_START;
		static const char* const XMLSTR_DLR_BIT_CODE_END;
		static const char* const XMLSTR_DLR_INCREMENT;
		static const char* const XMLSTR_DLR_EXACT_VALUE;
		static const char* const XMLSTR_DLR_DUAL_VOC;
		static const char* const XMLSTR_DLR_OCTAL_VOC;
		static const char* const XMLSTR_DLR_QUADRUPLE_VOC;
		
		//Vocabulary File Element and Attribute Names
		static const char* const XMLSTR_VOC_VOCABULARY;
		static const char* const XMLSTR_VOC_NAME;
		static const char* const XMLSTR_VOC_ENTRY;
		static const char* const XMLSTR_VOC_DATAITEM;
		static const char* const XMLSTR_VOC_BITCODE;
		
		//Input Data Stream Element and Attribute Names
		static const char* const XMLSTR_IDS_MESSAGE;
		static const char* const XMLSTR_IDS_NETWORK;
		static const char* const XMLSTR_IDS_NAME;
		static const char* const XMLSTR_IDS_DATA;
		static const char* const XMLSTR_IDS_DFI_DUI;
		
		static const char* const STR_CONFIG_FILE;
		static const char* const STR_CONFIG_DIR_URL;
		//non consts used in DLXML is also defined here
		static bool BLN_VALIDATE_INPUT_STREAM;
		static std::string STR_INPUT_STREAM_GRAMMAR_FILE;
		static std::string STR_DATA_LINK_GRAMMAR_FILE;
	};
}
#endif

