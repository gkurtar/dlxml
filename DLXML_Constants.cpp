#include "DLXML_Constants.h"

namespace DLXML
{
	const unsigned int Constants::INT_LINK_11_MESSAGE_SIZE = 48;
	const unsigned int Constants::INT_LINK_16_MESSAGE_SIZE = 70;
	const unsigned int Constants::INT_IJMS_MESSAGE_SIZE = 244;
	
	const char* const Constants::STR_STARS = "*******************************************************";
	const char* const Constants::STR_EMPTY = "";
	const char* const Constants::STR_UNDEFINED = "UNDEFINED";
	const char* const Constants::STR_ALL = "ALL";
	const char* const Constants::STR_NOT_DECODED = "NOT_DECODED";
	const char* const Constants::STR_L11_AMPMSG_1ST_4BITS = "0001";
	const char* const Constants::STR_L11_AMPMSG_CHAR = "8";

	const char* const Constants::STR_GS = "GS"; //Stands for General symbol. => (FORZA BESIKTAS)
	const char* const Constants::STR_DLRP_LATITUDE = "DLRP_Latitude";
	const char* const Constants::STR_DLRP_LONGITUDE = "DLRP_Longitude";
	const char* const Constants::STR_MEM_BUF_ID = "LinkMessageData";
	const char* const Constants::STR_NOT_OPERATIONAL = "NOP";
	
	const char* const Constants::STR_ENUM_DLXML_REL_OP = "DLXML_RelationalOperator";
	const char* const Constants::STR_ENUM_DLXML_DATA_LINK_NET = "DLXML_DataLinkNetwork";
	const char* const Constants::STR_ENUM_DLXML_ERROR_TYPE = "DLXML_ErrorType";
	const char* const Constants::STR_ENUM_DLXML_ERROR_LEVEL = "DLXML_ErrorLevel";
	
	const char* const Constants::XMLSTR_CFG_VOCABULARY = "Vocabulary";
	const char* const Constants::XMLSTR_CFG_RULE_FILES = "RuleFiles";
	const char* const Constants::XMLSTR_CFG_FILES = "Files";
	const char* const Constants::XMLSTR_CFG_DIRECTORYURL = "DirectoryURL";
	const char* const Constants::XMLSTR_CFG_NAME = "Name";
	const char* const Constants::XMLSTR_CFG_FILE = "File";
	const char* const Constants::XMLSTR_CFG_SCHEMA_FILE = "SchemaFile";
	const char* const Constants::XMLSTR_CFG_INPUT_STREAM_SCHEMA = "InputStreamSchema";
	const char* const Constants::XMLSTR_CFG_PARAMETERS = "Parameters";
	const char* const Constants::XMLSTR_CFG_PARAM = "Param";
	const char* const Constants::XMLSTR_CFG_KEY = "Key";
	const char* const Constants::XMLSTR_CFG_VALUE = "Value";
	const char* const Constants::XMLSTR_CFG_VALIDATE_INPUT_STREAMS = "ValidateInputStreams";
	
	const char* const Constants::XMLSTR_DLR_FIELD = "Field";
	const char* const Constants::XMLSTR_DLR_ORDER = "Order";
	const char* const Constants::XMLSTR_DLR_MESSAGE_NAME = "MessageName";
	const char* const Constants::XMLSTR_DLR_MESSAGE_NETWORK = "MessageNetwork";
	const char* const Constants::XMLSTR_DLR_DFI_DUI = "DFI_DUI";
	const char* const Constants::XMLSTR_DLR_ID = "Id";
	const char* const Constants::XMLSTR_DLR_RESOLUTION = "Resolution";

	const char* const Constants::XMLSTR_DLR_LOGICAL = "Logical";
	const char* const Constants::XMLSTR_DLR_CHAR_STREAM = "CharacterStream";
	const char* const Constants::XMLSTR_DLR_NUMERIC = "Numeric";
	const char* const Constants::XMLSTR_DLR_BINARY = "Binary";
	const char* const Constants::XMLSTR_DLR_SWITCH = "Switch";
	const char* const Constants::XMLSTR_DLR_CASE = "Case";
	const char* const Constants::XMLSTR_DLR_SELECTIVE_COND = "SelectiveCondition";
	const char* const Constants::XMLSTR_DLR_CASE_COND = "CaseCondition";
	const char* const Constants::XMLSTR_DLR_AND = "And";
	const char* const Constants::XMLSTR_DLR_OR = "Or";
	const char* const Constants::XMLSTR_DLR_BOOLEAN_NOT = "BooleanNot";
	const char* const Constants::XMLSTR_DLR_OPERAND = "Operand";
	const char* const Constants::XMLSTR_DLR_NAME = "Name";
	const char* const Constants::XMLSTR_DLR_LEFT_OPERAND = "LeftOperand";
	const char* const Constants::XMLSTR_DLR_OPERATOR = "Operator";
	const char* const Constants::XMLSTR_DLR_RIGHT_OPERAND = "RightOperand";
	const char* const Constants::XMLSTR_DLR_DEFAULT_VALUE = "DefaultValue";
	const char* const Constants::XMLSTR_DLR_DATA_TYPE = "DataType";
	const char* const Constants::XMLSTR_DLR_BIT_LENGTH = "BitLength";
	const char* const Constants::XMLSTR_DLR_BIT_POSITION = "BitPosition";
	const char* const Constants::XMLSTR_DLR_TRUE = "true";
	const char* const Constants::XMLSTR_DLR_FALSE = "false";
	const char* const Constants::XMLSTR_DLR_RELATIONAL_EXPRESSIONS = "RelationalExpressions";
	const char* const Constants::XMLSTR_DLR_RELATIONAL_EXPRESSION = "RelationalExpression";
	
	const char* const Constants::XMLSTR_DLR_INPUT_MAPPING = "InputMapping";
	const char* const Constants::XMLSTR_DLR_BOUNDARY = "Boundary";
	const char* const Constants::XMLSTR_DLR_UNBOUNDED_INTERVAL = "UnboundedInterval";
	const char* const Constants::XMLSTR_DLR_BOUNDED_INTERVAL = "BoundedInterval";
	const char* const Constants::XMLSTR_DLR_UNIFORM_DISTRIBUTION = "UniformDistribution";
	const char* const Constants::XMLSTR_DLR_BIT_CODE = "BitCode";
	const char* const Constants::XMLSTR_DLR_BIT_CODE_START = "BitCodeStart";
	const char* const Constants::XMLSTR_DLR_BIT_CODE_END = "BitCodeEnd";
	const char* const Constants::XMLSTR_DLR_INCREMENT = "Increment";
	const char* const Constants::XMLSTR_DLR_EXACT_VALUE = "ExactValue";
	const char* const Constants::XMLSTR_DLR_DUAL_VOC = "Dual";
	const char* const Constants::XMLSTR_DLR_OCTAL_VOC = "Octal";
	const char* const Constants::XMLSTR_DLR_QUADRUPLE_VOC = "Quadruple";
	
	const char* const Constants::XMLSTR_VOC_VOCABULARY = "Vocabulary";
	const char* const Constants::XMLSTR_VOC_NAME = "Name";
	const char* const Constants::XMLSTR_VOC_ENTRY = "Entry";
	const char* const Constants::XMLSTR_VOC_DATAITEM = "DataItem";
	const char* const Constants::XMLSTR_VOC_BITCODE = "BitCode";
	
	const char* const Constants::XMLSTR_DLR_TYP_FLOAT = "float";
	const char* const Constants::XMLSTR_DLR_TYP_DOUBLE = "double";
	const char* const Constants::XMLSTR_DLR_TYP_LONG = "long";
	const char* const Constants::XMLSTR_DLR_TYP_SHORT = "short";
	const char* const Constants::XMLSTR_DLR_TYP_BOOLEAN = "boolean";
	const char* const Constants::XMLSTR_DLR_TYP_STRING = "string";
	const char* const Constants::XMLSTR_DLR_GE = "GE";
	const char* const Constants::XMLSTR_DLR_GT = "GT";
	const char* const Constants::XMLSTR_DLR_EQ = "EQ";
	const char* const Constants::XMLSTR_DLR_NE = "NE";
	const char* const Constants::XMLSTR_DLR_LT = "LT";
	const char* const Constants::XMLSTR_DLR_LE = "LE";
	
	const char* const Constants::XMLSTR_DLR_GENERIC_VALUES = "GenericValues";
	const char* const Constants::XMLSTR_DLR_ILLEGAL = "Illegal";
	const char* const Constants::XMLSTR_DLR_UNDEFINED = "Undefined";
	const char* const Constants::XMLSTR_DLR_INPUT_DATA = "InputData";
	
	const char* const Constants::XMLSTR_DLR_INC_RANGE = "InclusiveRange";
	const char* const Constants::XMLSTR_DLR_START = "Start";
	const char* const Constants::XMLSTR_DLR_END = "End";
	const char* const Constants::XMLSTR_DLR_NO_STMT = "NoStatement";
	const char* const Constants::XMLSTR_DLR_STRING = "String";
	const char* const Constants::XMLSTR_DLR_LENGTH = "Length";
	const char* const Constants::XMLSTR_DLR_SUBSTRING = "Substring";
	const char* const Constants::XMLSTR_DLR_START_POS = "StartPos";
	const char* const Constants::XMLSTR_DLR_SUBSTRING_LEN = "SubstringLength";
	const char* const Constants::XMLSTR_DLR_ENCODING_START_POS = "EncodingStartPos";
	const char* const Constants::XMLSTR_DLR_ENCODED_CHAR_LEN = "EncodedCharLength";
	const char* const Constants::XMLSTR_DLR_VOCABULARY = "Vocabulary";

	const char* const Constants::XMLSTR_IDS_MESSAGE = "Message";
	const char* const Constants::XMLSTR_IDS_NETWORK = "Network";
	const char* const Constants::XMLSTR_IDS_NAME = "Name";
	const char* const Constants::XMLSTR_IDS_DATA = "Data";
	const char* const Constants::XMLSTR_IDS_DFI_DUI = "DFI_DUI";

	const char* const Constants::STR_CONFIG_DIR_URL = "./Config/";
	const char* const Constants::STR_CONFIG_FILE = "DLXML_Config.xml";
	
	bool Constants::BLN_VALIDATE_INPUT_STREAM = true;
	std::string Constants::STR_INPUT_STREAM_GRAMMAR_FILE = "InputDataGrammar.xsd";
	std::string Constants::STR_DATA_LINK_GRAMMAR_FILE = "DataLinkGrammar.xsd";
}
