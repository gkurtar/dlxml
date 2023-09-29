#include "DLXML_DataLinkUtil.h"
#include "DLXML_ErrorRepository.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_Constants.h"

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/util/XMLString.hpp>

using std::string;
XERCES_CPP_NAMESPACE_USE

namespace DLXML
{
	string cDataLinkUtil::convertDecimalToBinaryString(long alValue,\
			short asNumberOfBits, bool abMSB_AtHighestIndex)
	{
		string strBitwiseResult(asNumberOfBits, '0'); //init with zeroes
		if (abMSB_AtHighestIndex) {//ex: 12 is encoded as "0011"
			for (short i = 0; i < asNumberOfBits; ++i) {
				if ((alValue & 0x00000001) == 1) {
					strBitwiseResult[i] = '1';
				}
				alValue >>= 1;
			}//end of for
		}
		else {//ex: 12 is encoded as "1100"
			for (short i = asNumberOfBits - 1; i >= 0; --i) {
				if ((alValue & 0x00000001) == 1) {
					strBitwiseResult[i] = '1';
				}
				alValue >>= 1;
			}//end of for
		}	    
		return strBitwiseResult;
	}
	
	long cDataLinkUtil::convertBinaryStringToDecimal(const string& arstrBinaryString,\
			bool abMSB_AtHighestIndex)  
	{
		long lResult = 0;
		if (abMSB_AtHighestIndex) {//ex: "0011" is decoded as 12
			for (int i = arstrBinaryString.length() - 1; i >= 0; --i) {
				lResult <<= 1;
				if (arstrBinaryString.at(i) == '1') {
					lResult = lResult | 0x00000001;
				}
				else if (arstrBinaryString.at(i) != '0') {
					lResult = 0;
					break;
				}	     
			}// end of for
		}
		else {//ex: "0011" is decoded as 3
			for (size_t i = 0; i < arstrBinaryString.length(); ++i) {
				lResult <<= 1;
				if (arstrBinaryString.at(i) == '1') {
					lResult = lResult | 0x00000001;
				}
				else if (arstrBinaryString.at(i) != '0') {
					lResult = 0;
					break;
				}
			}// end of for
	    }
	    return lResult;  
	}

	bool cDataLinkUtil::evalBoolExpression(DLXML_RelationalOperator aenmExpressionOperator,\
						double adLeftOperand, double adRightOperand) throw (DLXML_Exception)
	{
		bool bExpression = false;
		if (aenmExpressionOperator == OPTR_GE) {
			bExpression = adLeftOperand >= adRightOperand;
		}
		else if (aenmExpressionOperator == OPTR_GT) {
			bExpression = adLeftOperand > adRightOperand;
		}
		else if (aenmExpressionOperator == OPTR_EQ) {
			bExpression = adLeftOperand == adRightOperand;
		}
		else if (aenmExpressionOperator == OPTR_NE) {
			bExpression = !(adLeftOperand == adRightOperand);
		}
		else if (aenmExpressionOperator == OPTR_LT) {
			bExpression = adLeftOperand < adRightOperand;
		}
		else if (aenmExpressionOperator == OPTR_LE) {
			bExpression = adLeftOperand <= adRightOperand;
		}
		else {
			DLXML_THROW_EX(DLXML_Exception, "Expression Operator is invalid.")
		}
		return bExpression;
	}

	bool cDataLinkUtil::hasElementNamed(const DOMNode* apdndSearchNode,\
			const string& arstrElementName) throw (DLXML_Exception)
	{
		bool bResult = false;
		string strErrorMessage;
		if (apdndSearchNode == 0) {
			strErrorMessage.assign("XML Node to search within is not set.");
		}
		else if (apdndSearchNode->getNodeType() != DOMNode::ELEMENT_NODE) {
			strErrorMessage.assign("XML Node to search within is not an element node.");
		}
		else if (apdndSearchNode->hasChildNodes()) {

			XMLCh* pxchTemp = XMLString::transcode(arstrElementName.c_str());
			DOMNodeList* pdnlsNodeList = ((DOMElement*)apdndSearchNode)->getElementsByTagName(pxchTemp);
			XMLString::release(&pxchTemp);

			if(pdnlsNodeList->getLength() > 0) {
				bResult = true;
			}
		}

		if (strErrorMessage.size() > 0) {
			DLXML_THROW_EX(DLXML_Exception, strErrorMessage)
		}
		return bResult;
	}


	DOMElement* cDataLinkUtil::getElementByName(const DOMNode* apdndSearchNode,\
			const string& arstrElementName) throw (DLXML_Exception)
	{
		DOMElement* pdelmResult = 0;
		string strErrorMessage;
		if (apdndSearchNode == 0) {
			strErrorMessage.assign("XML Node to search within is not set.");
		}
		else if (apdndSearchNode->getNodeType() != DOMNode::ELEMENT_NODE) {
			strErrorMessage.assign("XML Node to search within is not an element node.");
		}
		else {
			XMLCh* pxchTemp = XMLString::transcode(arstrElementName.c_str());
			DOMNodeList* pdnlsNodeList = ((DOMElement*)apdndSearchNode)->getElementsByTagName(pxchTemp);
			XMLString::release(&pxchTemp);

			if(pdnlsNodeList->getLength() == 0) {
				//extra control, before setting exception message Maybe we missed the big picture,
				//if current apdndSearchNode has the same name with the searched name return it..
				char* pchTempStr = XMLString::transcode(apdndSearchNode->getNodeName());
				if (!arstrElementName.compare(pchTempStr)) {
					pdelmResult = (DOMElement*)apdndSearchNode;
				}
				else {
					strErrorMessage.assign("Could not find element named " +\
							arstrElementName + " in the searched node");
				}
				XMLString::release(&pchTempStr);
			}
			else if(pdnlsNodeList->getLength() > 1) {
				strErrorMessage.assign("Multiple elements named " +\
						arstrElementName + " exist in the searched node");
			}
			else {
				pdelmResult = ((DOMElement*) pdnlsNodeList->item(0));
			}
		}

		if (strErrorMessage.size() > 0) {
			DLXML_THROW_EX(DLXML_Exception, strErrorMessage)
		}
		return pdelmResult;
	}
	
	string cDataLinkUtil::getElementContent(const DOMNode* apdndSearchNode) throw (DLXML_Exception)
	{
		string strErrorMessage;
		string strElementContent;

		if (apdndSearchNode == 0) {
			strErrorMessage.assign("XML Node is not set.");
		}
		else if (apdndSearchNode->getNodeType() != DOMNode::ELEMENT_NODE) {
			strErrorMessage.assign("XML Node is not an element node.");
		}
		else if (!apdndSearchNode->hasChildNodes()) {
			strErrorMessage.assign("XML Node has not got any Child node.");
		}
		else if (apdndSearchNode->getFirstChild()->getNodeType() != DOMNode::TEXT_NODE) {
			strErrorMessage.assign("XML Node has not got any TEXT_NODE as a first child.");
		}
		else {
			char* pchTempStr = XMLString::transcode(apdndSearchNode->getFirstChild()->getNodeValue());
			if (pchTempStr == 0) {
				strErrorMessage.assign("Could not set string via XMLString::transcode.");
			}
			else {
				strElementContent.assign(pchTempStr);
				XMLString::release(&pchTempStr);
			}
		}
		
		if (strErrorMessage.size() > 0) {
			DLXML_THROW_EX(DLXML_Exception, strErrorMessage)
		}
		return strElementContent;
	}

	DOMNode* cDataLinkUtil::getChildNodeByName(const DOMNode* apdndSearchNode,\
			const string& arstrElementName)
	{
		DOMNode* pdndChild = 0;
		if (apdndSearchNode == 0) {
			return pdndChild;
		}

		if (apdndSearchNode->getNodeType() != DOMNode::ELEMENT_NODE) {
			return pdndChild;
		}

		if (!apdndSearchNode->hasChildNodes()) {
			return pdndChild;
		}

		char* pchTempStr = 0;
		pdndChild = apdndSearchNode->getFirstChild();
		while (pdndChild != 0) {
			if (pdndChild->getNodeType() == DOMNode::ELEMENT_NODE) {
				pchTempStr = XMLString::transcode(pdndChild->getNodeName());
				if (!arstrElementName.compare(pchTempStr)) {
					XMLString::release(&pchTempStr);
					break;
				}
				XMLString::release(&pchTempStr);
			}
			pdndChild = pdndChild->getNextSibling();
		}
		return pdndChild;
	}

	DOMNode* cDataLinkUtil::getFieldChildNode(const DOMNode* apdndSearchNode) throw (DLXML_Exception)
	{
		string strErrorMessage;
		DOMNode* pdndFieldChild = 0;
		if (apdndSearchNode == 0) {
			strErrorMessage.assign("XML Node to search within is not set.");
		}
		else if (apdndSearchNode->getNodeType() != DOMNode::ELEMENT_NODE) {
			strErrorMessage.assign("XML Node to search within is not an element node.");
		}
		else if (!apdndSearchNode->hasChildNodes()) {
			strErrorMessage.assign("XML Node to search within has not got any Child node.");
		}
		else {
			char* pchTempStr = 0;
			pdndFieldChild = apdndSearchNode->getFirstChild();
			while (pdndFieldChild != 0) {
				if (pdndFieldChild->getNodeType() == DOMNode::ELEMENT_NODE) {
					pchTempStr = XMLString::transcode(pdndFieldChild->getNodeName());
					if (!strcmp(pchTempStr, Constants::XMLSTR_DLR_SELECTIVE_COND)) {
						XMLString::release(&pchTempStr);
					}
					else if (!strcmp(pchTempStr, Constants::XMLSTR_DLR_LOGICAL) ||\
						!strcmp(pchTempStr, Constants::XMLSTR_DLR_BINARY) ||\
						!strcmp(pchTempStr, Constants::XMLSTR_DLR_CHAR_STREAM) ||\
						!strcmp(pchTempStr, Constants::XMLSTR_DLR_NUMERIC)) {
						//one of the "Logical, Bool, CharStream, UniformRatio"
						XMLString::release(&pchTempStr);
						break;
					}
				}
				pdndFieldChild = pdndFieldChild->getNextSibling();
			}//end of while
			
			if (!pdndFieldChild) {
				strErrorMessage.assign("Could not find any descriptive element of the '<Field>' node.");
			}
		}

		if (strErrorMessage.size() > 0) {
			DLXML_THROW_EX(DLXML_Exception, strErrorMessage)
		}

		return pdndFieldChild;
	}

	bool cDataLinkUtil::hasAttributeNamed(const DOMNode* apdndSearchNode, const char* apchAttributeName)
	{	
		if (apdndSearchNode == 0 || apchAttributeName == 0) {
			return false;
		}

		bool bIsAttributeContained = false;
		if (apdndSearchNode->hasAttributes()) {
			DOMNamedNodeMap* pdnnmAttributes = apdndSearchNode->getAttributes();
			for(unsigned int i = 0; i < pdnnmAttributes->getLength(); ++i) {
				DOMAttr* pdatrAttributeNode = (DOMAttr*) pdnnmAttributes->item(i);
				char* pchTempStr = XMLString::transcode(pdatrAttributeNode->getName());
				if (!strcmp(apchAttributeName, pchTempStr)) {

					if (pdatrAttributeNode->getSpecified()) {
						XMLString::release(&pchTempStr);
						bIsAttributeContained = true;
						break;
					}
				}//end of if
		        XMLString::release(&pchTempStr);
			}//end of for
		}//end of if
		return bIsAttributeContained;
	}

	string cDataLinkUtil::getAttributeValue(const DOMNode* apdndSearchNode, const char* apchAttributeName) throw (DLXML_Exception)
	{
		string strErrorMessage;
		string strAttributeContent;
		
		if (apdndSearchNode == 0) {
			strErrorMessage.assign("XML Node to search within is not set.");
		}
		else if (apchAttributeName == 0) {
			strErrorMessage.assign("Attribute name is not set.");
		}
		else if (!apdndSearchNode->hasAttributes()) {
			strErrorMessage.assign("XML Node to search within has not any attributes.");
		}
		else {
			XMLCh* pxchTmpStr = XMLString::transcode(apchAttributeName);
			DOMNode* pdndAttribute = apdndSearchNode->getAttributes()->getNamedItem(pxchTmpStr);
			XMLString::release(&pxchTmpStr);

			if (pdndAttribute == 0) {//Could not find the attribute
				strErrorMessage.assign("Attribute named " + string(apchAttributeName) + " is not contained within the XML Node");
			}

			char* pchNodeValue = XMLString::transcode(pdndAttribute->getNodeValue());
			if (pchNodeValue == 0) {
				strErrorMessage.assign("Could not set string via XMLString::transcode.");
			}
			else {
				strAttributeContent.assign(pchNodeValue);
				XMLString::release(&pchNodeValue);
			}
		}

		if (strErrorMessage.size() > 0) {
			DLXML_THROW_EX(DLXML_Exception, strErrorMessage)
		}
		return strAttributeContent;
	}

	DLXML_RuleGenericStructure cDataLinkUtil::getGenericStructure(const DOMNode* apdndSearchNode)
	{
		DLXML_RuleGenericStructure stGeneric;
		
		if (!cDataLinkUtil::hasElementNamed(apdndSearchNode, Constants::XMLSTR_DLR_GENERIC_VALUES)) {
			return stGeneric;
		}
		
		DOMElement* pdelmGeneric = cDataLinkUtil::getElementByName(\
				apdndSearchNode, Constants::XMLSTR_DLR_GENERIC_VALUES);		
		if (!pdelmGeneric->hasChildNodes()) {
			return stGeneric;
		}

		DOMNode* pdndGenericChild = 0;
		char* pchTempStr = 0;
		XMLCh* pxchTmpStr = 0;
		string strGenericChildTagName;
		string strInvalidChildTagName;
		pdndGenericChild = pdelmGeneric->getFirstChild();
		while (pdndGenericChild != 0)
		{
			if (pdndGenericChild->getNodeType() == DOMNode::ELEMENT_NODE) {
				pchTempStr = XMLString::transcode(pdndGenericChild->getNodeName());
				strGenericChildTagName.assign(pchTempStr);
				XMLString::release(&pchTempStr);

				if (!strGenericChildTagName.compare(Constants::XMLSTR_DLR_ILLEGAL) || \
						!strGenericChildTagName.compare(Constants::XMLSTR_DLR_UNDEFINED)) {
					//generic child is an either illegal or undefined child
					bool bNodeNameIsIllegal = !strGenericChildTagName.compare(\
							Constants::XMLSTR_DLR_ILLEGAL) ? true : false;

					DOMNode* pdndInvalidChild = 0;
					pdndInvalidChild = pdndGenericChild->getFirstChild();
					while (pdndInvalidChild != 0)
					{
						if (pdndInvalidChild->getNodeType() == DOMNode::ELEMENT_NODE) {
							pchTempStr = XMLString::transcode(pdndInvalidChild->getNodeName());
							strInvalidChildTagName.assign(pchTempStr);
							XMLString::release(&pchTempStr);
							if (!strInvalidChildTagName.compare(Constants::XMLSTR_DLR_INPUT_DATA)) {
								pchTempStr = XMLString::transcode(pdndInvalidChild->getTextContent());
								if (bNodeNameIsIllegal) {
									stGeneric.stIllegal.vecInputData.push_back(atol(pchTempStr));
								}
								else {
									stGeneric.stUndefined.vecInputData.push_back(atol(pchTempStr));
								}
								XMLString::release(&pchTempStr);
							}
							else if (!strInvalidChildTagName.compare(Constants::XMLSTR_DLR_INC_RANGE)) {
								pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_START);
								pchTempStr = XMLString::transcode(\
									pdndInvalidChild->getAttributes()->getNamedItem(pxchTmpStr)->getTextContent());
								long lStart = atol(pchTempStr);
								XMLString::release(&pxchTmpStr);
								XMLString::release(&pchTempStr);
								pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_END);
								pchTempStr = XMLString::transcode(\
									pdndInvalidChild->getAttributes()->getNamedItem(pxchTmpStr)->getTextContent());
								long lEnd = atol(pchTempStr);
								XMLString::release(&pxchTmpStr);
								XMLString::release(&pchTempStr);
								if (bNodeNameIsIllegal) {
									stGeneric.stIllegal.vecInclusiveRange.push_back(InclusiveRangeStructure(lStart, lEnd));
								}
								else {
									stGeneric.stUndefined.vecInclusiveRange.push_back(InclusiveRangeStructure(lStart, lEnd));
								}
							}
						}//end of if (pdndInvalidChild->getNodeType() == DOMNode::ELEMENT_NODE)
						pdndInvalidChild = pdndInvalidChild->getNextSibling();
					}// end of while (pdndInvalidChild != 0)
				}// end of if strTagName is undefined or strTagName is illegal
				else if (!strGenericChildTagName.compare(Constants::XMLSTR_DLR_NO_STMT)) {
					pchTempStr = XMLString::transcode(pdndGenericChild->getTextContent());
					stGeneric.strNoStatement.assign(pchTempStr);
					XMLString::release(&pchTempStr);
				}
			}
			pdndGenericChild = pdndGenericChild->getNextSibling();
		}// end of while
		return stGeneric;
	}

	DLXML_RuleStringStructure cDataLinkUtil::getStringStructure(const DOMNode* apdndSearchNode)
	{
		DLXML_RuleStringStructure stString;

		DOMElement* pdelmString = cDataLinkUtil::getElementByName(\
				apdndSearchNode, Constants::XMLSTR_DLR_STRING);
		if (!pdelmString->hasChildNodes()) {
			return stString;
		}

		DOMNode* pdndStringChild = pdelmString->getFirstChild();
		char* pchTempStr = 0;
		string strTagName;
		//string strTempValue;
		SubstringStructure stSubstring;

		stString.unStringLength = atoi(cDataLinkUtil::getAttributeValue(\
				pdelmString, Constants::XMLSTR_DLR_LENGTH).c_str());

		while (pdndStringChild != 0) {

			if (pdndStringChild->getNodeType() == DOMNode::ELEMENT_NODE) {
				pchTempStr = XMLString::transcode(pdndStringChild->getNodeName());
				strTagName.assign(pchTempStr);
				XMLString::release(&pchTempStr);

				if (!strTagName.compare(Constants::XMLSTR_DLR_SUBSTRING)) {
					//SubstringStructure stSubstring;
					stSubstring.sStartPos = atoi(cDataLinkUtil::getAttributeValue(\
							pdndStringChild, Constants::XMLSTR_DLR_START_POS).c_str());
					stSubstring.sSubstringLength = atoi(cDataLinkUtil::getAttributeValue(\
							pdndStringChild, Constants::XMLSTR_DLR_SUBSTRING_LEN).c_str());
					stSubstring.sEncodingStartPos = atoi(cDataLinkUtil::getAttributeValue(\
							pdndStringChild, Constants::XMLSTR_DLR_ENCODING_START_POS).c_str());
					stSubstring.sEncodedCharLength = atoi(cDataLinkUtil::getAttributeValue(\
							pdndStringChild, Constants::XMLSTR_DLR_ENCODED_CHAR_LEN).c_str());
					stSubstring.strVocabulary.assign(cDataLinkUtil::getAttributeValue(\
							pdndStringChild, Constants::XMLSTR_DLR_VOCABULARY));
					stString.vecSubstringNodes.push_back(stSubstring);
				}
			}
			pdndStringChild = pdndStringChild->getNextSibling();
		}
		return stString;
	}

	DLXML_RuleInputMappingStructure cDataLinkUtil::getInputMappingStructure(const DOMNode* apdndSearchNode)
	{
		DLXML_RuleInputMappingStructure stInputMapping;
		DOMElement* pdelmInputMapping = cDataLinkUtil::getElementByName(\
				apdndSearchNode, Constants::XMLSTR_DLR_INPUT_MAPPING);

		if (!pdelmInputMapping->hasChildNodes()) {
			return stInputMapping;
		}

		DOMNodeList* pdnlsInputMappingNodes = pdelmInputMapping->getChildNodes();
		DOMNode* pdndInputMappingChild = 0;
		DOMElement* pdelmMapChild = 0;
		string strTempValue;

		for (unsigned int j = 0; j < pdnlsInputMappingNodes->getLength(); ++j) {

			if (pdnlsInputMappingNodes->item(j)->getNodeType() == DOMNode::ELEMENT_NODE) {
				pdndInputMappingChild = pdnlsInputMappingNodes->item(j);

				if (hasElementNamed(pdndInputMappingChild, Constants::XMLSTR_DLR_UNIFORM_DISTRIBUTION)) {
					pdelmMapChild = getElementByName(pdndInputMappingChild, Constants::XMLSTR_DLR_UNIFORM_DISTRIBUTION);
					UniformDistributionStructure stUniformDistribution;
					stUniformDistribution.dStart = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_START).c_str());
					stUniformDistribution.dEnd = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_END).c_str());
					stUniformDistribution.lBitCodeStart = atol(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_BIT_CODE_START).c_str());
					stUniformDistribution.lBitCodeEnd = atol(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_BIT_CODE_END).c_str());
					stUniformDistribution.dIncrement = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_INCREMENT).c_str());
					stInputMapping.vecUniformDistributions.push_back(stUniformDistribution);
				}
				else if (hasElementNamed(pdndInputMappingChild, Constants::XMLSTR_DLR_UNBOUNDED_INTERVAL)) {
					pdelmMapChild = getElementByName(pdndInputMappingChild, Constants::XMLSTR_DLR_UNBOUNDED_INTERVAL);
					UnboundedIntervalStructure stUnboundedInterval;
					stUnboundedInterval.dBoundary = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_BOUNDARY).c_str());
					strTempValue = cDataLinkUtil::getAttributeValue(\
							pdelmMapChild, Constants::XMLSTR_DLR_OPERATOR);
					stUnboundedInterval.enmRelationalOperator = \
						(DLXML_RelationalOperator) cEnumRepository::getInstance(). \
						StrToEnum(Constants::STR_ENUM_DLXML_REL_OP, strTempValue);
					stUnboundedInterval.lBitCode = atol(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_BIT_CODE).c_str());
					
					stInputMapping.vecUnboundedIntervals.push_back(stUnboundedInterval);
				}
				else if (hasElementNamed(pdndInputMappingChild, Constants::XMLSTR_DLR_BOUNDED_INTERVAL)) {
					pdelmMapChild = getElementByName(pdndInputMappingChild, Constants::XMLSTR_DLR_BOUNDED_INTERVAL);
					BoundedIntervalStructure stBoundedInterval;
					stBoundedInterval.dStart = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_START).c_str());
					stBoundedInterval.dEnd = atof(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_END).c_str());
					stBoundedInterval.lBitCode = atol(cDataLinkUtil::\
							getAttributeValue(pdelmMapChild, Constants::XMLSTR_DLR_BIT_CODE).c_str());
					stInputMapping.vecBoundedIntervals.push_back(stBoundedInterval);
				}
				else if (hasElementNamed(pdndInputMappingChild, Constants::XMLSTR_DLR_EXACT_VALUE)) {
					pdelmMapChild = cDataLinkUtil::getElementByName(\
							pdndInputMappingChild, Constants::XMLSTR_DLR_EXACT_VALUE);
					stInputMapping.vecExactValues.push_back(atol(cDataLinkUtil::\
							getElementContent(pdelmMapChild).c_str()));
				}
			}
		}//end of for loop
		return stInputMapping;
	}
}// end of namespace

