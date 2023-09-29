#include "DLXML_DecodeRuleProcessor.h"
#include "DLXML_Encoder.h"
#include "DLXML_Decoder.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_ErrorRepository.h"
#include "DLXML_MessageRepository.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_Constants.h"

#include <vector>
#include <sstream>

#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

using std::map;
using std::set;
using std::string;
using std::vector;
using std::ostringstream;

namespace DLXML
{

    void cDecodeRuleProcessor::applyDecodingRule(const DOMNode* apdndFieldNode,\
			cMessageRepository& arcMessageRepository, cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex,\
			const std::string& arstrEncodedMessage, std::string& arstrDecodedField)
	{
    	try {
	    	DOMNode* pdndChildNode = cDataLinkUtil::getFieldChildNode(apdndFieldNode);
	    	char* pchNodeValue = XMLString::transcode(((DOMElement *)pdndChildNode)->getTagName());
	    	string strNodeValue(pchNodeValue);
	    	XMLString::release(&pchNodeValue);
	
	    	if (!strNodeValue.compare(Constants::XMLSTR_DLR_LOGICAL)) {
	    		processLogicalDecoding(apdndFieldNode, arcMessageRepository,\
	    			arcErrorRepository, abMSB_AtHighestIndex, arstrEncodedMessage, arstrDecodedField);
	    	}
	    	else if (!strNodeValue.compare(Constants::XMLSTR_DLR_BINARY)) {
	    		processBinaryDecoding(apdndFieldNode, arcMessageRepository,\
	    			arcErrorRepository, arstrEncodedMessage, arstrDecodedField);
	    	}
	    	else if (!strNodeValue.compare(Constants::XMLSTR_DLR_CHAR_STREAM)) {
	    		processCharStreamDecoding(apdndFieldNode, arcMessageRepository,\
	    			arcErrorRepository, abMSB_AtHighestIndex, arstrEncodedMessage, arstrDecodedField);
	    	}
	    	else if (!strNodeValue.compare(Constants::XMLSTR_DLR_NUMERIC)) {
	    		processNumericDecoding(apdndFieldNode, arcMessageRepository,\
	    			arcErrorRepository, abMSB_AtHighestIndex, arstrEncodedMessage, arstrDecodedField);
	    	}
	    	else { //assuming that validation via xsd is done until this point,
	    		//execution should not come in this clause, check it anyway
	    		DLXML_THROW_EX(DLXML_Exception,\
	    				strNodeValue.insert(0, "Field Type is not defined:"))
	    	}
    	}
    	catch (const std::exception& rExp) {
    		DLXML_ADD_ERROR(arcErrorRepository, ETP_EXP_GENERAL, ELV_ERROR,\
				string("Caught exception during decoding: ").append(rExp.what()))
    	}
    	return;
	}

	void cDecodeRuleProcessor::processBinaryDecoding(const DOMNode* apdndFieldNode,\
		cMessageRepository& arcMessageRepository, cErrorRepository& arcErrorRepository,\
		const std::string& arstrEncodedMessage, std::string& arstrDecodedField)
	{
		if (!checkField(apdndFieldNode, arcMessageRepository)) {
			arstrDecodedField.assign(Constants::STR_NOT_OPERATIONAL);
			return;
		}
		arstrDecodedField.assign(1, arstrEncodedMessage.at(atoi(cDataLinkUtil::\
				getAttributeValue(apdndFieldNode, Constants::XMLSTR_DLR_BIT_POSITION).c_str())));
		return;
	}

	void cDecodeRuleProcessor::processLogicalDecoding(const DOMNode* apdndFieldNode, \
			cMessageRepository& arcMessageRepository, cErrorRepository& arcErrorRepository, \
			bool abMSB_AtHighestIndex,\
			const std::string& arstrEncodedBitMessage, std::string& arstrDecodedString)
	{
		if (!checkField(apdndFieldNode, arcMessageRepository)) {
			arstrDecodedString.assign(Constants::STR_NOT_OPERATIONAL);
			return;
		}

		DLXML_RuleGenericStructure stGeneric;
		if (cDataLinkUtil::hasElementNamed(apdndFieldNode, Constants::XMLSTR_DLR_SWITCH)) { //Switch element exists
			DOMElement* pdelmSwitch = cDataLinkUtil::getElementByName(\
					apdndFieldNode, Constants::XMLSTR_DLR_SWITCH);
			XMLCh* pxchTemp = XMLString::transcode(Constants::XMLSTR_DLR_CASE);
			DOMNodeList* pdnlsCaseNodes = pdelmSwitch->getElementsByTagName(pxchTemp);
			XMLString::release(&pxchTemp);
			for (unsigned int j = 0; j < pdnlsCaseNodes->getLength(); ++j) {
				if (checkField(pdnlsCaseNodes->item(j), arcMessageRepository, false)) {
					stGeneric = cDataLinkUtil::getGenericStructure(pdnlsCaseNodes->item(j));
					break;
				}//end of if
			}//end of for
		}
		else { //Switch element does not exist
			stGeneric = cDataLinkUtil::getGenericStructure(apdndFieldNode);
		}

		string strEncodedSubstr = arstrEncodedBitMessage.substr(\
				atoi(cDataLinkUtil::getAttributeValue(\
						apdndFieldNode, Constants::XMLSTR_DLR_BIT_POSITION).c_str()),\
				atoi(cDataLinkUtil::getAttributeValue(\
						apdndFieldNode, Constants::XMLSTR_DLR_BIT_LENGTH).c_str()));

		arstrDecodedString = cDecoder::decodeLogical(strEncodedSubstr, stGeneric,\
				arcErrorRepository, abMSB_AtHighestIndex);
		return;
	}

	void cDecodeRuleProcessor::processCharStreamDecoding(const DOMNode* apdndFieldNode,\
		cMessageRepository& arcMessageRepository, cErrorRepository& arcErrorRepository,\
		bool abMSB_AtHighestIndex,\
		const string& arstrEncodedBitMessage, string& arstrDecodedString)
	{
		if (!checkField(apdndFieldNode, arcMessageRepository)) {
			arstrDecodedString.assign(Constants::STR_NOT_OPERATIONAL);
			return;
		}

		DLXML_RuleGenericStructure stGeneric;
		DLXML_RuleStringStructure stString;
		
		if (cDataLinkUtil::hasElementNamed(apdndFieldNode, Constants::XMLSTR_DLR_SWITCH)) {
			//Switch element exists
			DOMElement* pdelmSwitch = cDataLinkUtil::getElementByName(apdndFieldNode, Constants::XMLSTR_DLR_SWITCH);
			XMLCh* pxchTemp = XMLString::transcode(Constants::XMLSTR_DLR_CASE);
			DOMNodeList* pdnlsCaseNodes = pdelmSwitch->getElementsByTagName(pxchTemp);
			XMLString::release(&pxchTemp);
			bool bIsMatchFound = false;
			for (unsigned int j = 0; j < pdnlsCaseNodes->getLength(); ++j) {
				if (checkField(pdnlsCaseNodes->item(j), arcMessageRepository, false)) {
					stString = cDataLinkUtil::getStringStructure(pdnlsCaseNodes->item(j));
					stGeneric = cDataLinkUtil::getGenericStructure(pdnlsCaseNodes->item(j));
					bIsMatchFound = true;
					break;
				}//end of if
			}//end of for
			if (!bIsMatchFound) {
				DLXML_THROW_EX(DLXML_Exception, "Could not find a matching case for decoding.")
			}
		}
		else { //Switch element does not exist
			stString = cDataLinkUtil::getStringStructure(apdndFieldNode);
			stGeneric = cDataLinkUtil::getGenericStructure(apdndFieldNode);
		}
		
		arstrDecodedString = cDecoder::decodeCharStream(\
				arstrEncodedBitMessage.substr(\
					atoi(cDataLinkUtil::getAttributeValue(\
							apdndFieldNode, Constants::XMLSTR_DLR_BIT_POSITION).c_str()),\
					atoi(cDataLinkUtil::getAttributeValue(\
							apdndFieldNode, Constants::XMLSTR_DLR_BIT_LENGTH).c_str())),\
				stString, stGeneric, arcErrorRepository, abMSB_AtHighestIndex);

		return;
	}
	
	void cDecodeRuleProcessor::processNumericDecoding(const DOMNode* apdndFieldNode,\
			cMessageRepository& arcMessageRepository, cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex,\
			const string& arstrEncodedBitMessage, string& arstrDecodedString)
	{
		if (!checkField(apdndFieldNode, arcMessageRepository)) {
			arstrDecodedString.assign(Constants::STR_NOT_OPERATIONAL);
			return;
		}

		DLXML_RuleInputMappingStructure stInputMapping;
		DLXML_RuleGenericStructure stGeneric;// = cDataLinkUtil::getGenericStructure(apdndFieldNode);
		
		if (cDataLinkUtil::hasElementNamed(apdndFieldNode, Constants::XMLSTR_DLR_SWITCH)) { //Switch element exists
			DOMElement* pdelmSwitch = cDataLinkUtil::getElementByName(apdndFieldNode, Constants::XMLSTR_DLR_SWITCH);
			XMLCh* pxchTemp = XMLString::transcode(Constants::XMLSTR_DLR_CASE);
			DOMNodeList* pdnlsCaseNodes = pdelmSwitch->getElementsByTagName(pxchTemp);
			XMLString::release(&pxchTemp);
			bool bIsMatchFound = false;
			for (unsigned int j = 0; j < pdnlsCaseNodes->getLength(); ++j) {
				if (checkField(pdnlsCaseNodes->item(j), arcMessageRepository, false)) {
					stInputMapping = cDataLinkUtil::getInputMappingStructure(pdnlsCaseNodes->item(j));
					stGeneric = cDataLinkUtil::getGenericStructure(pdnlsCaseNodes->item(j));
					bIsMatchFound = true;
					break;
				}//end of if
			}//end of for
			if (!bIsMatchFound) {
				DLXML_THROW_EX(DLXML_Exception, "Could not find a matching case for decoding.")
			}
		}
		else { //Switch element does not exist
			stInputMapping = cDataLinkUtil::getInputMappingStructure(apdndFieldNode);
			stGeneric = cDataLinkUtil::getGenericStructure(apdndFieldNode);
		}

		arstrDecodedString = cDecoder::decodeNumeric(\
					arstrEncodedBitMessage.substr(\
						atoi(cDataLinkUtil::getAttributeValue(\
								apdndFieldNode, Constants::XMLSTR_DLR_BIT_POSITION).c_str()),\
						atoi(cDataLinkUtil::getAttributeValue(\
								apdndFieldNode, Constants::XMLSTR_DLR_BIT_LENGTH).c_str())),\
					stInputMapping,\
					stGeneric,\
					arcErrorRepository,\
					abMSB_AtHighestIndex);
		return;
	}	

	//---------------------------------------------------------------------------------------------------
	inline bool cDecodeRuleProcessor::checkField(const DOMNode* apdndFieldNode,\
			cMessageRepository& arcMessageRepository,\
			bool abIsSelectiveCondition)
	{
		bool bOperationResult = true;
		const char* pchConditionType = abIsSelectiveCondition ?\
				Constants::XMLSTR_DLR_SELECTIVE_COND : Constants::XMLSTR_DLR_CASE_COND;

		DOMNode* pdndCondition = cDataLinkUtil::getChildNodeByName(apdndFieldNode, pchConditionType);
		if (pdndCondition == 0) {// condition node does not exist
			//if this is a selective condition then there is no problem
			//else if this is a case condition then this check should 
			//already be done in the xml file parsing with schema
			return true;
		}
		
		//Expression'lari olmayan bir Rule dosyasi olabilir.
		if (!cDataLinkUtil::hasElementNamed(apdndFieldNode->getOwnerDocument()->\
				getDocumentElement(), Constants::XMLSTR_DLR_RELATIONAL_EXPRESSIONS)) {
			return true;
		}
		
		//find Operand Names that occur in the SelectiveCondition node or in the FieldNode
		//and insert them into a std::set
		string strOperandName;
		std::set<string> setOperands;
		XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_OPERAND);
		const DOMNodeList* pdndlstOperands = 0;
		if (abIsSelectiveCondition) {
			DOMElement* pdelmSelectiveCondition = cDataLinkUtil::getElementByName(\
					apdndFieldNode, Constants::XMLSTR_DLR_SELECTIVE_COND);
			pdndlstOperands = pdelmSelectiveCondition->getElementsByTagName(pxchTmpStr);
		}
		else {
			pdndlstOperands = ((DOMElement*)apdndFieldNode)->getElementsByTagName(pxchTmpStr);
		}
		XMLString::release(&pxchTmpStr);
		for (unsigned int i = 0; i < pdndlstOperands->getLength(); ++i) {
			strOperandName = cDataLinkUtil::getElementContent(pdndlstOperands->item(i));
			setOperands.insert(strOperandName);
		}

		DOMNode* pdndRelationalExpressionsNode = cDataLinkUtil::getElementByName(\
				apdndFieldNode->getOwnerDocument()->getDocumentElement(),\
				Constants::XMLSTR_DLR_RELATIONAL_EXPRESSIONS);
		map<string, bool> mapRelExpValues = evalRelationalExpressions(\
				pdndRelationalExpressionsNode, setOperands, arcMessageRepository);

		DOMNode* pdndBoolNode = cDataLinkUtil::getChildNodeByName(pdndCondition, Constants::XMLSTR_DLR_AND);
		if (!pdndBoolNode) {
			pdndBoolNode = cDataLinkUtil::getChildNodeByName(pdndCondition, Constants::XMLSTR_DLR_OR);
		}

		bOperationResult = performOperation(pdndBoolNode, mapRelExpValues);
		return bOperationResult;
	}

	//------------------------------------------------------------------------------------------
	inline std::map<std::string, bool> cDecodeRuleProcessor::evalRelationalExpressions(\
			const DOMNode* apdndRelationalExpressionsNode,\
			const set<string>& arsetExpressionNames,\
			cMessageRepository& arcMessageRepository)
	{
		string strExpressionName;
		string strTemp;
		string strLeftOperand;
		string strRightOperand;
		bool bExpression = true;
		double dLeftOperand = 0;
		double dRightOperand = 0;
		DOMNodeList* pdndList = apdndRelationalExpressionsNode->getChildNodes();
		map<string, bool> mapEvaluatedRelExps;
		
		static cEnumRepository& objEnumRepository = cEnumRepository::getInstance();

		for (unsigned int i = 0; i < pdndList->getLength(); ++i) {
			if (pdndList->item(i)->getNodeType() != DOMNode::ELEMENT_NODE) {
				continue;
			}
			strExpressionName = cDataLinkUtil::getAttributeValue(pdndList->item(i), Constants::XMLSTR_DLR_NAME);
			//if strExpressionName is not found in the arsetExpressionNames then continue looping
			if (arsetExpressionNames.find(strExpressionName) == arsetExpressionNames.end()) {
				continue;
			}
			strTemp = cDataLinkUtil::getAttributeValue(pdndList->item(i), Constants::XMLSTR_DLR_LEFT_OPERAND);
			dLeftOperand = cDecodeRuleProcessor::getOperand(strTemp, arcMessageRepository);

			strTemp = cDataLinkUtil::getAttributeValue(pdndList->item(i), Constants::XMLSTR_DLR_RIGHT_OPERAND);
			dRightOperand = cDecodeRuleProcessor::getOperand(strTemp, arcMessageRepository);

			strTemp = cDataLinkUtil::getAttributeValue(pdndList->item(i), Constants::XMLSTR_DLR_OPERATOR);
			DLXML_RelationalOperator enmRelOpr = (DLXML_RelationalOperator)\
				objEnumRepository.StrToEnum(Constants::STR_ENUM_DLXML_REL_OP, strTemp);
			bExpression = cDataLinkUtil::evalBoolExpression(enmRelOpr, dLeftOperand, dRightOperand);
			mapEvaluatedRelExps.insert(make_pair(strExpressionName, bExpression));
		}

		if (mapEvaluatedRelExps.empty()) {
			DLXML_THROW_EX(DLXML_Exception, "Could not fill expressions map.")
		}
		return mapEvaluatedRelExps;
	}

	//---------------------------------------------------------------------------------------------------
	inline double cDecodeRuleProcessor::getOperand(const string& arstrOperandName,\
			cMessageRepository& arcMessageRepository)				
	{
		double dResult = 0;
		string strResult;
		string::size_type sztLocOfStr = arstrOperandName.find("::", 0);

		if (sztLocOfStr == string::npos) { //operand is decimal
			strResult = arstrOperandName;
			dResult = atof(strResult.c_str());
		}
		else {
			string strSpecifier = arstrOperandName.substr(0, sztLocOfStr);
			string strSymbol = arstrOperandName.substr(sztLocOfStr + 2);
			if (!strSpecifier.compare(Constants::STR_GS)) {
				strResult = arcMessageRepository.getGeneralSymbol(strSymbol);
			}
			else {
				strResult = arcMessageRepository.getMessageFieldValue(arstrOperandName);
			}
			
			if (!strResult.compare(Constants::STR_UNDEFINED)) {
				//a special case is handled below, it is a subtle detail about the order of evaluation	
				if (arstrOperandName.compare("M.9E::2694_031") != 0) {
					DLXML_THROW_EX(DLXML_Exception,\
							"Relational Expression Operand not found:" + arstrOperandName)
				}
			}
			else {
				dResult = atof(strResult.c_str());
			}
		}//end of else
		return dResult;
	}
	
	//---------------------------------------------------------------------------------------------------
	inline bool cDecodeRuleProcessor::performOperation(\
			const DOMNode* apdndBoolNode, const map<string, bool>& armapEvaluatedRelExps)
	{
        bool bResult = true;
		bool bOperand = true;
		vector<bool> vecOperandValues;
		vector<DOMNode*> vecOperandNodes;
		string strExpressionName;
		char* pchTempStr = 0;

		DOMNodeList* pdndList = apdndBoolNode->getChildNodes();

		for (unsigned int i = 0; i < pdndList->getLength(); ++i) {
			if (pdndList->item(i)->getNodeType() != DOMNode::ELEMENT_NODE) {
				continue;
			}
			pchTempStr = XMLString::transcode(pdndList->item(i)->getNodeName());

			if (!strcmp(pchTempStr, Constants::XMLSTR_DLR_OPERAND)) {
				vecOperandNodes.push_back(pdndList->item(i));
			}
			else {//child Node operand degilse And veya Or'dan biri olmalidir.
				bOperand = performOperation(pdndList->item(i), armapEvaluatedRelExps);
				vecOperandValues.push_back(bOperand);
			}
			XMLString::release(&pchTempStr);
		}
		
		for (size_t j = 0; j < vecOperandNodes.size(); ++j) {
			strExpressionName = cDataLinkUtil::getElementContent(vecOperandNodes.at(j));

			map<string, bool>::const_iterator itrTemp = armapEvaluatedRelExps.find(strExpressionName);
			if (itrTemp == armapEvaluatedRelExps.end()) {
				DLXML_THROW_EX(DLXML_Exception, strExpressionName.insert(0,\
						"Could not find value of expression: "))
			}
			bOperand = itrTemp->second;
			
			if (cDataLinkUtil::hasAttributeNamed(vecOperandNodes.at(j), Constants::XMLSTR_DLR_BOOLEAN_NOT)) {
				bOperand = !bOperand;
			}

			vecOperandValues.push_back(bOperand);
			XMLString::release(&pchTempStr);
		}

		pchTempStr = XMLString::transcode(apdndBoolNode->getNodeName());
		if (!strcmp(pchTempStr, Constants::XMLSTR_DLR_AND)) {
			bResult = true; // Identity Element
			for (size_t j = 0; j < vecOperandValues.size(); ++j)
				bResult = bResult && vecOperandValues[j];
		}
		else if (!strcmp(pchTempStr, Constants::XMLSTR_DLR_OR)) {
			bResult = false; //Identity Element
			for (size_t j = 0; j < vecOperandValues.size(); ++j)
				bResult = bResult || vecOperandValues[j];
		}
		XMLString::release(&pchTempStr);
        return bResult;
    }
}//end of namespace
