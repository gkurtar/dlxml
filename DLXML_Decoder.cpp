#include "DLXML_Decoder.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_ErrorRepository.h"
#include "DLXML_VocabularyRepository.h"
#include "DLXML_Constants.h"

#include <algorithm>
#include <sstream>

using std::vector;
using std::string;
using std::map;
using std::ostringstream;
using std::find;
using std::reverse;

namespace DLXML
{
	string cDecoder::decodeLogical(const string& arstrEncodedBitMessage,
        	const DLXML_RuleGenericStructure& arstGeneric, \
        	cErrorRepository& arcErrorRepository, \
        	bool abMSB_AtHighestIndex)
	{
		long lDecodedValue = cDataLinkUtil::convertBinaryStringToDecimal(\
				arstrEncodedBitMessage, abMSB_AtHighestIndex);
		ostringstream ossDecodedValue;
		ossDecodedValue << lDecodedValue;
		
		if (cDecoder::checkValue(lDecodedValue, arstGeneric.stIllegal)) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_ILLEGAL, ELV_WARNING, \
					" Decoded Value is illegal: " + ossDecodedValue.str())
		}//check whether input is undefined or not
		else if (cDecoder::checkValue(lDecodedValue, arstGeneric.stUndefined)) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_UNDEFINED, ELV_WARNING, \
					" Decoded Value is undefined: " + ossDecodedValue.str())
		}
		else if (!arstGeneric.strNoStatement.compare(arstrEncodedBitMessage)) {
			//we found a match as an encoded no stmt value, set decoded string to empty str
			ossDecodedValue.str(Constants::STR_EMPTY);
		}

		return ossDecodedValue.str();
	}
	//---------------------------------------------------------------------------------------------
	string cDecoder::decodeCharStream(const string& arstrEncodedBitMessage,\
			const DLXML_RuleStringStructure& arstStringSubparts,\
			const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex)
	{
		//(OCTAL, every 3 char denotes a val), abMSB_AtHighestIndex variable denotes MSB position 
		//sample input "010001101"  means 542 if abMSB_AtHighestIndex = true, means 215 if it is false,
		string strEncodedBitMessage = arstrEncodedBitMessage;
		if (abMSB_AtHighestIndex) {//reverse string if abMSB_AtHighestIndex is true
			reverse(strEncodedBitMessage.begin(), strEncodedBitMessage.end());
		}

		//for_each substring decode the substring, and check the resultant substring whether it violates any rules
		string strDecodedString;
		string strEncodedSubstring;
		try {
			for (size_t i = 0; i < arstStringSubparts.vecSubstringNodes.size(); ++i) {
				const SubstringStructure& stSubStruct = arstStringSubparts.vecSubstringNodes[i];

				for (short j = 0; j < stSubStruct.sSubstringLength; ++j) {
					strEncodedSubstring = strEncodedBitMessage.substr(\
						(stSubStruct.sEncodingStartPos + (j * stSubStruct.sEncodedCharLength)),\
						stSubStruct.sEncodedCharLength);

					strDecodedString.append(1, \
							cDecoder::getDecodedForm(strEncodedSubstring, stSubStruct.strVocabulary));
				}
			}//end of for

			//------------------------------------------------------------------------------------------
			//check whether decoded string is illegal or undefined, and report it if such a case happens
			bool bDecodedStringConsistsOfDigits = true;
			//determine whether input consists of digits or not
			for (size_t j = 0; j < arstStringSubparts.vecSubstringNodes.size(); ++j) {
				if (arstStringSubparts.vecSubstringNodes[j].strVocabulary.\
						compare(Constants::XMLSTR_DLR_DUAL_VOC) && \
					arstStringSubparts.vecSubstringNodes[j].strVocabulary.\
						compare(Constants::XMLSTR_DLR_QUADRUPLE_VOC) && \
					arstStringSubparts.vecSubstringNodes[j].strVocabulary.\
						compare(Constants::XMLSTR_DLR_OCTAL_VOC)) {
					bDecodedStringConsistsOfDigits = false;
					break;
				}
			}

			for (size_t j = 0; j < strDecodedString.length(); ++j) {
				if (!isdigit(strDecodedString[j])) {
					bDecodedStringConsistsOfDigits = false;
					break;
				}
			}

			//illegal or undefined control starts here
			if (bDecodedStringConsistsOfDigits) {
				long lTemp = atol(strDecodedString.c_str());
				if (checkValue(lTemp, arstGeneric.stIllegal)) {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_ILLEGAL, ELV_WARNING, \
							" Decoded Value is illegal: " + strDecodedString)
				}
				else if (checkValue(lTemp, arstGeneric.stUndefined)) {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_UNDEFINED, ELV_WARNING, \
							" Decoded Value is undefined: " + strDecodedString)
				}
			}// end of if (bDecodedStringConsistsOfDigits)
			//------------------------------------------------------------------------------------------
			if (!arstGeneric.strNoStatement.compare(arstrEncodedBitMessage)) {
				//we found a match as an encoded no stmt value, set decoded string to empty str
				strDecodedString.clear();
			}
		} catch (const DLXML_Exception& rexpException) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING, rexpException.what())
			strDecodedString.clear();
		}
		return strDecodedString;
	}
	//---------------------------------------------------------------------------------------------
	string cDecoder::decodeNumeric(const string& arstrEncodedBitMessage,\
			const DLXML_RuleInputMappingStructure& arstMapping,\
			const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex)
	{
		//Fonksiyon No Statement Value'lar icin empty string return eder..
		ostringstream ossDecodedValue;
		bool bIsMatchFound = false;
		double dDecodedValue = 0;
		long lDecimalValue = cDataLinkUtil::convertBinaryStringToDecimal(\
				arstrEncodedBitMessage, abMSB_AtHighestIndex);		

		if (cDecoder::checkValue(lDecimalValue, arstGeneric.stIllegal)) {
			ossDecodedValue << lDecimalValue;
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_ILLEGAL, ELV_WARNING, \
				" Decoded Value is illegal: " + arstrEncodedBitMessage + \
				" , " + ossDecodedValue.str())
			//we have written the string form of 'lDecimalValue' as error message, now we should erase that
			ossDecodedValue.str("");
		}
		else if (cDecoder::checkValue(lDecimalValue, arstGeneric.stUndefined)) {
			ossDecodedValue << lDecimalValue;
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_UNDEFINED, ELV_WARNING, \
				" Decoded Value is undefined: " + arstrEncodedBitMessage + \
				" , " + ossDecodedValue.str())
			//we have written the string form of 'lDecimalValue' as error message, now we should erase that
			ossDecodedValue.str("");
		}
		else {
			for (size_t j = 0; j < arstMapping.vecUniformDistributions.size(); ++j) {
				const UniformDistributionStructure& stUniform = arstMapping.vecUniformDistributions[j];
				if (lDecimalValue >= stUniform.lBitCodeStart && lDecimalValue <= stUniform.lBitCodeEnd) {
					dDecodedValue = stUniform.dStart + \
					((lDecimalValue - stUniform.lBitCodeStart) * stUniform.dIncrement);
					bIsMatchFound = true;
					break;
				}
			}

			if (!bIsMatchFound) {
				for (size_t j = 0; j < arstMapping.vecBoundedIntervals.size(); ++j) {
					const BoundedIntervalStructure& stBounded = arstMapping.vecBoundedIntervals[j];
					if (lDecimalValue == stBounded.lBitCode) {
						//if decoded value is same with the map value return upper bound
						dDecodedValue = stBounded.dEnd;
						bIsMatchFound = true;
						break;
					}
				}//end of for
			}
			
			if (!bIsMatchFound) {
				//Although we search UnboundedIntervals through a loop, there should be  
				//at most one UnboundedInterval in a DLXML_RuleInputMappingStructure 
				for (size_t j = 0; j < arstMapping.vecUnboundedIntervals.size(); ++j) {
					if (lDecimalValue == arstMapping.vecUnboundedIntervals[j].lBitCode) {
						dDecodedValue = arstMapping.vecUnboundedIntervals[j].dBoundary;
						bIsMatchFound = true;
						break;
					}
				}// end of for
			}

			if (!bIsMatchFound) {
				vector<long>::const_iterator itrTemp = find(arstMapping.vecExactValues.begin(),\
						arstMapping.vecExactValues.end(), lDecimalValue);
				if (itrTemp != arstMapping.vecExactValues.end()) {
					dDecodedValue = lDecimalValue;//same vale decoded..
					bIsMatchFound = true;
				}
			}
			
			if (!bIsMatchFound) {//check No Statement
				if (!arstGeneric.strNoStatement.compare(arstrEncodedBitMessage)) {
					bIsMatchFound = true;//we found a match as an encoded no stmt value
				}
				if (!bIsMatchFound) {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING,\
						"No matching value found while decoding.")
				}
			}
			else {
				ossDecodedValue << dDecodedValue;
			}
		}
		return ossDecodedValue.str();
	}

	//---------------------------------------------------------------------------------------------
	inline bool cDecoder::checkValue(long alInput, const DLXML_InvalidStructure& arstInvalid)
	{	//check whether input is in any of the ranges below
		bool bIsMatchFound = false;
		
		for (size_t j = 0; j < arstInvalid.vecInclusiveRange.size(); ++j) {
			if (alInput >= arstInvalid.vecInclusiveRange[j].lStart && \
				alInput <= arstInvalid.vecInclusiveRange[j].lEnd) {
				bIsMatchFound = true;
				break;
			}
		}

		if (!bIsMatchFound) {
			//check whether input is equal to any of the values below
			vector<long>::const_iterator itrResult = find(arstInvalid.vecInputData.begin(),
					arstInvalid.vecInputData.end(), alInput);
			bIsMatchFound =	(itrResult != arstInvalid.vecInputData.end());
		}
		return bIsMatchFound;
	}

	//---------------------------------------------------------------------------------------------
	inline char cDecoder::getDecodedForm(const string& arstrEncodedSubstring,\
			const string& arstrVocabularyName) throw (DLXML_Exception)
	{
		if (!arstrEncodedSubstring.size()) {
			DLXML_THROW_EX(DLXML_Exception, "Zero length string encountered while decoding")
		}

		char chResult = 0;
		if (!arstrVocabularyName.compare(Constants::XMLSTR_DLR_DUAL_VOC) || \
			!arstrVocabularyName.compare(Constants::XMLSTR_DLR_OCTAL_VOC) || \
			!arstrVocabularyName.compare(Constants::XMLSTR_DLR_QUADRUPLE_VOC)) {
			
			long lDecodedValue = 0;
			for (size_t i = 0; i < arstrEncodedSubstring.length(); ++i) {
				lDecodedValue <<= 1;
				if (arstrEncodedSubstring[i] == '1') {
					lDecodedValue = lDecodedValue | 0x00000001;
				}
			}// end of for
		
			if (!arstrVocabularyName.compare(Constants::XMLSTR_DLR_DUAL_VOC) && lDecodedValue > 1) {
				DLXML_THROW_EX(DLXML_Exception, "Decoded substring did not fit in the allowed range [0-1].")
			}
			else if (!arstrVocabularyName.compare(Constants::XMLSTR_DLR_QUADRUPLE_VOC) && lDecodedValue > 3) {
				DLXML_THROW_EX(DLXML_Exception, "Decoded substring did not fit in the allowed range [0-3].")
			}
			else if (!arstrVocabularyName.compare(Constants::XMLSTR_DLR_OCTAL_VOC) && lDecodedValue > 7) {
				DLXML_THROW_EX(DLXML_Exception, "Decoded substring did not fit in the allowed range [0-7].")
			}
			chResult = '0' + lDecodedValue;
		}
		else {//other vocabularies are handled here
			chResult = cVocabularyRepository::
				lookupEntryValue(arstrVocabularyName, arstrEncodedSubstring);
		}
		return chResult;
	}// end of getDecodedForm
}
