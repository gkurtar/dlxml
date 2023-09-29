#include "DLXML_Encoder.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_ErrorRepository.h"
#include "DLXML_VocabularyRepository.h"
#include "DLXML_Constants.h"

#include <math.h>
#include <algorithm>
#include <iterator>

using std::map;
using std::vector;
using std::string;
using std::iterator;
using std::find;
using std::reverse;

namespace DLXML
{
	string cEncoder::encodeLogical(long alInputValue, short asNumberOfBits,\
			bool abDataExists, const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository, bool abMSB_AtHighestIndex)
	{
		string strEncodedString(asNumberOfBits, '0');
		bool bInputIsIllegal = false;
		bool bInputIsUndefined = false;
		bool bInputIsInvalid = false;
		unsigned long ulTemp = 1;
		ulTemp <<= asNumberOfBits;

		if (!abDataExists) { //data does not exist
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_NOT_EXIST, ELV_WARNING,\
					"Input value does not exist")
		}
		//check whether input is out of range or not
		else if (alInputValue < 0 || alInputValue >= (signed)ulTemp) {
			bInputIsInvalid = true;
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING, "Input is invalid")
		}
		//check whether input is illegal or not
		else if (bInputIsIllegal = checkInputValue(alInputValue, arstGeneric.stIllegal)) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_ILLEGAL, ELV_WARNING, "Input is illegal")
		}
		//check whether input is undefined or not
		else if (bInputIsUndefined = checkInputValue(alInputValue, arstGeneric.stUndefined)) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_UNDEFINED, ELV_WARNING, "Input is undefined")
		}
		
		if (bInputIsUndefined || bInputIsIllegal || bInputIsInvalid || !abDataExists) {
			//had we have a no stmt value its length should be greater than zero
			//and it has been checked already whether it has the same length with the field it is defined in
			if (arstGeneric.strNoStatement.size() == 0) {
				//it seems there does not exist any no stmt value for this field
				DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_SIMPLE,\
					"Neither any valid input nor any no stmt value is found for the field")
			}	
			
			return (arstGeneric.strNoStatement.size() == (unsigned) asNumberOfBits ?\
					arstGeneric.strNoStatement : strEncodedString);
			
		}
		strEncodedString = cDataLinkUtil::convertDecimalToBinaryString(\
				alInputValue, asNumberOfBits, abMSB_AtHighestIndex);
		return strEncodedString;
	}

	//---------------------------------------------------------------------------------------------
	//This method do not check undefined or illegal structure during encoding process, since
	//these type of encodings are either defining a uniform range, a bunch of range mappings
	//or exact values and almost always an input range is not interleaved by illegal or
	//undefined values. Because of this while encoding an input value it is either mapped 
	//by a mapping or not, and this unmapping case is handled by adding a log message
	//detailing the situation and selecting no stmt value as the encoded value if such
	//a value exists.
	string cEncoder::encodeNumeric(double adInputValue, short asNumberOfBits,\
        bool abDataExists, const DLXML_RuleInputMappingStructure& arstInputMapping,\
        const DLXML_RuleGenericStructure& arstGeneric,\
		cErrorRepository& arcErrorRepository, bool abMSB_AtHighestIndex)
	{
		string strEncodedString(asNumberOfBits, '0');
		bool bIsMatchFound = false;
		long lValueToEncode = 0;

		if (!abDataExists) { //data does not exist
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_NOT_EXIST, ELV_WARNING,\
					"Input value does not exist")
		}
		else {
			for (size_t j = 0; j < arstInputMapping.vecUniformDistributions.size(); ++j) {
				const UniformDistributionStructure& stUniform =\
				arstInputMapping.vecUniformDistributions[j];
				if (adInputValue >= stUniform.dStart && adInputValue <= stUniform.dEnd) {
					lValueToEncode = stUniform.lBitCodeStart +\
						static_cast<long>(((adInputValue - stUniform.dStart) / stUniform.dIncrement));
					bIsMatchFound = true;
					break;
				}
			}

			if (!bIsMatchFound) {
				for (size_t j = 0; j < arstInputMapping.vecUnboundedIntervals.size(); ++j) {
					const UnboundedIntervalStructure& stUnbounded =\
						arstInputMapping.vecUnboundedIntervals[j];
					if (cDataLinkUtil::evalBoolExpression(stUnbounded.enmRelationalOperator,\
							adInputValue, stUnbounded.dBoundary)) {
						lValueToEncode = stUnbounded.lBitCode;
						bIsMatchFound = true;
					}
				}// end of for
			}

			if (!bIsMatchFound) {
				for (size_t j = 0; j < arstInputMapping.vecBoundedIntervals.size(); ++j) {
					const BoundedIntervalStructure& stBounded = arstInputMapping.vecBoundedIntervals[j];
					if (adInputValue >= stBounded.dStart && adInputValue <= stBounded.dEnd) {
						lValueToEncode = stBounded.lBitCode;
						bIsMatchFound = true;
					}
				}// end of for
			}
			
			if (!bIsMatchFound) {
				vector<long>::const_iterator itrTemp = find(arstInputMapping.vecExactValues.begin(),\
						arstInputMapping.vecExactValues.end(), static_cast<long>(adInputValue)); 
				if (itrTemp != arstInputMapping.vecExactValues.end()) {
					lValueToEncode = *itrTemp;
					bIsMatchFound = true;
				}
			}

			if (!bIsMatchFound) {
				DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING, "Input is invalid")
			}
		}// end of else

		if (!bIsMatchFound || !abDataExists) {
			//had we have a no stmt value its length should be greater than zero
			//and it has been checked already whether it has the same length with the field it is defined in
			if (arstGeneric.strNoStatement.size() == 0) {
				//it seems there does not exist any no stmt value for this field
				DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_SIMPLE,\
					"Neither any valid input nor any no stmt value is found for the field")
			}
			return (arstGeneric.strNoStatement.size() == (unsigned) asNumberOfBits ?\
					arstGeneric.strNoStatement : strEncodedString);
		}

		strEncodedString = cDataLinkUtil::convertDecimalToBinaryString(\
				lValueToEncode, asNumberOfBits, abMSB_AtHighestIndex);
		return strEncodedString;
	}
	
	//---------------------------------------------------------------------------------------------
	inline bool cEncoder::checkInputValue(long alInput, const DLXML_InvalidStructure& arstInvalid)
	{
		bool bResult = false;
		for (size_t j = 0; j < arstInvalid.vecInclusiveRange.size(); ++j) {
			if (alInput >= arstInvalid.vecInclusiveRange[j].lStart && \
				alInput <= arstInvalid.vecInclusiveRange[j].lEnd) {
				bResult = true;
				break;
			}
		}
		if (!bResult) {
			vector<long>::const_iterator itrResult = std::find(
				arstInvalid.vecInputData.begin(), arstInvalid.vecInputData.end(), alInput);
			bResult = itrResult != arstInvalid.vecInputData.end();
		}
		return bResult;
	}

	//---------------------------------------------------------------------------------------------
	string cEncoder::encodeCharStream(const string& arstrInputValue, short asNumberOfBits,\
        bool abDataExists, const DLXML_RuleStringStructure& arstStringSubparts,\
        const DLXML_RuleGenericStructure& arstGeneric,\
        cErrorRepository& arcErrorRepository, bool abMSB_AtHighestIndex)
	{
		string strEncodedString(asNumberOfBits, '0');
		bool bInputIsInvalid = false;
		bool bInputIsIllegal = false;
		bool bInputIsUndefined = false;
		if (!abDataExists) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_NOT_EXIST, ELV_WARNING,\
					"Input value does not exist")
		}
		else if (arstrInputValue.size() != arstStringSubparts.unStringLength) {
			DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING, \
					"Input string is invalid: " + arstrInputValue)
			bInputIsInvalid = true;
		}
		else {
			bool bInputConsistsOfDigits = true;
			for (size_t j = 0; j < arstStringSubparts.vecSubstringNodes.size(); ++j) {
				if (arstStringSubparts.vecSubstringNodes[j].\
						strVocabulary.compare(Constants::XMLSTR_DLR_DUAL_VOC) && \
					arstStringSubparts.vecSubstringNodes[j].\
						strVocabulary.compare(Constants::XMLSTR_DLR_QUADRUPLE_VOC) && \
					arstStringSubparts.vecSubstringNodes[j].\
						strVocabulary.compare(Constants::XMLSTR_DLR_OCTAL_VOC)) {
					bInputConsistsOfDigits = false;
					break;
				}
			}

			for (size_t j = 0; j < arstrInputValue.length(); ++j) {
				if (!isdigit(arstrInputValue[j])) {
					bInputConsistsOfDigits = false;
					break;
				}
			}

			//illegal or undefined control starts here
			if (bInputConsistsOfDigits) {
				long lTemp = atol(arstrInputValue.c_str());
				if (bInputIsIllegal = checkInputValue(lTemp, arstGeneric.stIllegal)) {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_ILLEGAL, ELV_WARNING, \
							"Input is illegal: " + arstrInputValue)
				}
				else if (bInputIsUndefined = checkInputValue(lTemp, arstGeneric.stUndefined)) {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_UNDEFINED, ELV_WARNING, \
							"Input is undefined: " + arstrInputValue)
				}
			}// end of if (bInputConsistsOfDigits)
		} // end of else
		
		if (bInputIsIllegal || bInputIsInvalid || bInputIsUndefined || !abDataExists) {
			//had we have a no stmt value its length should be greater than zero
			//and it has been checked already whether it has the same length with the field it is defined in
			if (arstGeneric.strNoStatement.size() == 0) {
				//it seems there does not exist any no stmt value for this field
				DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_SIMPLE,\
					"Neither any valid input nor any no stmt value is found for the field")
			}
			return (arstGeneric.strNoStatement.size() == (unsigned) asNumberOfBits ?\
					arstGeneric.strNoStatement : strEncodedString);
		}

		//***************************************************************
		string strInputSubstring;
		string strEncodedSubstring;
		for (size_t j = 0; j < arstStringSubparts.vecSubstringNodes.size(); ++j) {
			const SubstringStructure& stSubStruct = arstStringSubparts.vecSubstringNodes[j];
			strInputSubstring = arstrInputValue.substr(stSubStruct.sStartPos,\
					stSubStruct.sSubstringLength);

			for(size_t k = 0; k < strInputSubstring.size(); ++k) {
				
				if (getEncodedForm(strInputSubstring[k], stSubStruct.strVocabulary,\
						strEncodedSubstring)) {
					strEncodedString.replace(stSubStruct.sEncodingStartPos +\
							(k * stSubStruct.sEncodedCharLength),\
							stSubStruct.sEncodedCharLength, strEncodedSubstring); 
				}
				else {
					DLXML_ADD_ERROR(arcErrorRepository, ETP_DATA_INVALID, ELV_WARNING,\
							"Input string is invalid")
                    return string(asNumberOfBits, '0');
				}
			}// end of for loop
		}// end of for loop

		if (abMSB_AtHighestIndex) {
			reverse(strEncodedString.begin(), strEncodedString.end());
		}
		return strEncodedString;
	}//end of encodeCharacterStream

	//---------------------------------------------------------------------------------------------
	inline bool cEncoder::getEncodedForm(char achInput, const string& arstrVocabulary,\
		string& arstrEncodedSubstring)
	{
		if (!arstrVocabulary.compare(Constants::XMLSTR_DLR_DUAL_VOC)) {
			if (achInput != '0' && achInput != '1') {
				return false;
			}
			arstrEncodedSubstring.insert(arstrEncodedSubstring.begin(), achInput);
		}
		else if (!arstrVocabulary.compare(Constants::XMLSTR_DLR_QUADRUPLE_VOC)) {

			if (achInput >= '0' && achInput <= '3') {
				arstrEncodedSubstring.assign(2, '0');
				register unsigned short usTemp = achInput - '0';
				for (register short j = 1; j >= 0; --j) {
					arstrEncodedSubstring.at(j) = (usTemp & 0x0001) ? '1' : '0';
					usTemp >>= 1;
				}
			}
			else {
				return false;
			}
		}
		else if (!arstrVocabulary.compare(Constants::XMLSTR_DLR_OCTAL_VOC)) {

			if (achInput >= '0' && achInput <= '7') {
				arstrEncodedSubstring.assign(3, '0');
				register unsigned short usTemp = achInput - '0';
				for (register short j = 2; j >= 0; --j) {
					arstrEncodedSubstring.at(j) = (usTemp & 0x0001) ? '1' : '0';
					usTemp >>= 1;
				}
			}
			else {
				return false;
			}
		}
		else {
			try {
				arstrEncodedSubstring =
					cVocabularyRepository::lookupEntryKey(arstrVocabulary, achInput);
			} catch (DLXML_Exception) {
				//we do not have to asssign any value since it is not used
				return false;
			}
		}//end of else
		return true;
	}// end of getEncodedFormOfChar
}
