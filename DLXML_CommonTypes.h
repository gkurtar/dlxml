#ifndef DLXML_COMMONTYPES_H
#define DLXML_COMMONTYPES_H

#include <xercesc/util/XercesDefs.hpp>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

XERCES_CPP_NAMESPACE_BEGIN
class DOMDocument;
XERCES_CPP_NAMESPACE_END

namespace DLXML
{
	typedef enum {
        DLN_LINK_11,
        DLN_LINK_16,
        DLN_IJMS
	} DLXML_DataLinkNetwork;

	typedef enum {
        //ETP_NO_ERROR, // "No Error"
		ETP_XML_RULE_FILE,
		ETP_EXP_GENERAL,
		ETP_DATA_NOT_EXIST,
		ETP_DATA_INVALID,
		ETP_DATA_UNDEFINED,
		ETP_DATA_ILLEGAL
	} DLXML_ErrorType;
	

	typedef enum {
		ELV_SIMPLE, // 1st level Simple Errors
		ELV_WARNING, // 2nd level errors like invalid data
		ELV_ERROR, // 3rd level errors in XML related situations
		ELV_FATAL // 4th level fatal errors 
	} DLXML_ErrorLevel;

	//**************  Operator **********************************
	typedef enum {
		OPTR_LT,
		OPTR_LE,
		OPTR_EQ,
		OPTR_NE,
		OPTR_GE,
		OPTR_GT
	} DLXML_RelationalOperator; //Represents relational operators

	//**************  Vocabulary **********************************
	typedef struct {
		std::string strName;
		std::map<char, char*> mapVocabulary;
		DLXML_ErrorType eErrorToReport;
	} DLXML_Vocabulary;

	typedef std::vector<DLXML_Vocabulary> DLXML_VocabularyVector;

	//**************  Error **********************************
	typedef struct {
		DLXML_ErrorType enmErrorType;
		DLXML_ErrorLevel enmErrorLevel;
		std::string strErrorMessage;
	} DLXML_ErrorStructure;
	
	typedef std::vector<DLXML_ErrorStructure> DLXML_ErrorVector;

	//*******************************************************************
	//RuleGeneric structure
	struct InclusiveRangeStructure{
        long lStart;
        long lEnd;
		InclusiveRangeStructure(long alStart, long alEnd) : lStart(alStart), lEnd(alEnd) { };
    };
	
	typedef struct {
		std::vector<long> vecInputData;
		std::vector<InclusiveRangeStructure> vecInclusiveRange;
	} DLXML_InvalidStructure;

	typedef struct {
		DLXML_InvalidStructure stIllegal;
		DLXML_InvalidStructure stUndefined;
		std::string strNoStatement;
	} DLXML_RuleGenericStructure;

	//*******************************************************************
	//RuleString structure
	//<Substring StartPos="0" SubstringLength="1" EncodingStartPos="0" EncodedCharLength="1" Vocabulary="Dual" />
	typedef struct {
		short sStartPos; // specifies the Input Position of substring
		short sSubstringLength; // specifies the Input Length of substring
		short sEncodingStartPos;//specifies the start of the encoding position in encoding area of substring
		short sEncodedCharLength;//specifies each of the substring character's encoding length
		std::string strVocabulary; // specifies the vocabulary of substring
	} SubstringStructure;
	
	typedef struct {
		std::vector<SubstringStructure> vecSubstringNodes;
		unsigned int unStringLength;
	} DLXML_RuleStringStructure;

	//*******************************************************************
	//RuleInputMapping structure
	typedef struct {
		double dStart;
		double dEnd;
		long lBitCodeStart;
		long lBitCodeEnd;
		double dIncrement;
	} UniformDistributionStructure;

	typedef struct {
		double dStart;
		double dEnd;
		long lBitCode;
	} BoundedIntervalStructure;
	
	typedef struct {
		long lBitCode;
		double dBoundary;
		DLXML_RelationalOperator enmRelationalOperator;
	} UnboundedIntervalStructure;

	typedef struct {
		std::vector<UniformDistributionStructure> vecUniformDistributions;
		std::vector<BoundedIntervalStructure> vecBoundedIntervals;
		std::vector<UnboundedIntervalStructure> vecUnboundedIntervals;
		std::vector<long> vecExactValues;
	} DLXML_RuleInputMappingStructure;
	
	typedef struct {
		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pddocMessageRule;
		DLXML_DataLinkNetwork eMessageNetwork;
		std::string strMessageName;
		std::string strMessageTemplate;
		//strMessageTemplate consists of " '*', '0', '1' " and it serves as a template for identifying a message
	} DLXML_MessageIdentifierStructure;

	typedef std::vector<DLXML_MessageIdentifierStructure> DLXML_MessageIdentifierVector;

	//****************************************************************************************************
		
	/**
	 * <p>DLXML_Exception class is used for error handling problem in DLXML
	 * library. This class extends std::logic_error and defines methods to query
	 * file and line which are provided during construction of this object.
	 * 
	 * Previous versions of this class was deriving from std::exception class, but
	 * a std::string argument constructor instead of const char* argument was needed
	 * while constructing an object of this kind since DLXML Lib throws exceptions
	 * with 'dynamic' exception messages. The exception std::logic_error derives from
	 * std::exception and provides a std::string argument constructor which is supplied
	 * by DLXML_Exception constructor.
	 * </p>
	 **/
	class DLXML_Exception : public std::logic_error {
	public:
		DLXML_Exception(const char* apchFILE, size_t anLINE, const std::string& arstrEX_MESG) :\
			std::logic_error(arstrEX_MESG), m_pchFILE(apchFILE), m_nLINE(anLINE) 
		{
		}
		
		virtual ~DLXML_Exception() throw()
		{
		}

		const char* file() const
		{
			return m_pchFILE;
		}
		
		const size_t line() const
		{
			return m_nLINE;
		}

	private:
		//Not implemented
		DLXML_Exception& operator=(const DLXML_Exception&);
		
		//Members
		const char* m_pchFILE;
		size_t m_nLINE;
	};
	
	#define DLXML_THROW_EX(EX, MSG) throw EX(__FILE__, __LINE__, MSG);
	
	#define DLXML_ADD_ERROR(OBJ, ETP, ELV, MSG) OBJ.addError(ETP, ELV, MSG);
}
#endif

