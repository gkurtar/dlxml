#include "DLXML_MessageEncoder.h"
#include "DLXML_EncodeRuleProcessor.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_ParserErrorHandler.h"
#include "DLXML_Constants.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/validators/common/Grammar.hpp>

#include <sstream>
#include <algorithm>
XERCES_CPP_NAMESPACE_USE

using std::sort;
using std::string;
using std::vector;

namespace DLXML
{
	cMessageEncoder::cMessageEncoder(\
			const DLXML_MessageIdentifierVector& arvecMessageIdentifiers) :\
		m_objMessageRepository(arvecMessageIdentifiers)
	{
		m_pehErrorHandler = new cParserErrorHandler(m_objErrorRepository);
		m_pxdpDataDomParser = new XercesDOMParser();
		if (Constants::BLN_VALIDATE_INPUT_STREAM) {
			m_pxdpDataDomParser->setValidationScheme(XercesDOMParser::Val_Always);
			m_pxdpDataDomParser->setDoNamespaces(true);
			m_pxdpDataDomParser->setValidationSchemaFullChecking(true);
			m_pxdpDataDomParser->setDoSchema(true);
			m_pxdpDataDomParser->setErrorHandler(m_pehErrorHandler);
			m_pxdpDataDomParser->loadGrammar(string(Constants::STR_CONFIG_DIR_URL +\
				Constants::STR_INPUT_STREAM_GRAMMAR_FILE).c_str(),\
				Grammar::SchemaGrammarType, true);
			m_pxdpDataDomParser->cacheGrammarFromParse(true);
		}
	}
	
	cMessageEncoder::~cMessageEncoder(void)
	{
		delete m_pxdpDataDomParser;
		delete m_pehErrorHandler;
	}

	vector<string> cMessageEncoder::encodeMessages(\
			const vector<string>& arvecXML_DataStreams) throw (DLXML_Exception)
	{
		vector<string> vecEncodedMessages;
		//before generating new messages we have to clear contents of these..
		m_objMessageRepository.clearMessageFieldValues();
		m_vecInputStreamDomDocuments.clear();
		parseXMLDataStreams(arvecXML_DataStreams);
		
		for(size_t j = 0; j < m_vecInputStreamDomDocuments.size(); ++j) {
			vecEncodedMessages.push_back(this->encodeMessage(m_vecInputStreamDomDocuments[j]));
		}
		return vecEncodedMessages;
	}

	inline string cMessageEncoder::encodeMessage(\
			const DOMDocument* apddocInputStream) throw (DLXML_Exception)
	{
		string strBitMessage;
		static cEnumRepository& objEnumRepository = cEnumRepository::getInstance();
		try {
			//find Message Identifer to be used while encoding XML Data Stream
			const DOMElement* pdelmMessage = cDataLinkUtil::getElementByName(\
					apddocInputStream->getDocumentElement(), Constants::XMLSTR_IDS_MESSAGE);
			string strMessageNameOfDataStream = cDataLinkUtil::getAttributeValue(\
					pdelmMessage, Constants::XMLSTR_IDS_NAME);
			string strMessageNetworkOfDataStream = cDataLinkUtil::getAttributeValue(\
					pdelmMessage, Constants::XMLSTR_IDS_NETWORK);
			DLXML_DataLinkNetwork eMessageNetwork = (DLXML_DataLinkNetwork) objEnumRepository.\
					StrToEnum(Constants::STR_ENUM_DLXML_DATA_LINK_NET, strMessageNetworkOfDataStream);
			// yukaridaki iki 'find' islemi cok masrafli gorunuyor, parse islemi basariya 
			//ulastiktan sonra bu islemlerin yapildigi dusunulurse, bu islemler yerine baska
			//yontemler kullanilabilir,
			const DLXML_MessageIdentifierStructure* pstMessageIdentifier = m_objMessageRepository.\
				getIdentifierOfMessageName(eMessageNetwork, strMessageNameOfDataStream);
			
			if (pstMessageIdentifier == 0) {
				DLXML_THROW_EX(DLXML_Exception,\
					"XML Rule file is not defined for the input data stream.")
			}

			bool bMsbAtHighestIndex = true;
			if (pstMessageIdentifier->eMessageNetwork == DLN_LINK_11) {
				strBitMessage.assign(Constants::INT_LINK_11_MESSAGE_SIZE, '0');
			}
			else {
				DLXML_THROW_EX(DLXML_Exception,	"Encoding is not ready except for Link11 Messages.")
			}

			string strMessageName = pstMessageIdentifier->strMessageName;
			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_FIELD);
			DOMNodeList* pdnlsFieldNodeList = \
				pstMessageIdentifier->pddocMessageRule->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);

			string strInputData;
			string strCheckedInputData;
			string strMessageField;
			string strDFI_DUI;
			unsigned int unFieldIndex = 0;
			bool bIsFieldProcessed = false;

			for (unsigned int i = 0; i < pdnlsFieldNodeList->getLength(); ++i) {

				//find the DOMNode which has order of 'i'
				for (unFieldIndex = 0; unFieldIndex < pdnlsFieldNodeList->getLength(); ++unFieldIndex) {
					if (atoi(cDataLinkUtil::getAttributeValue(pdnlsFieldNodeList->item(\
							unFieldIndex), Constants::XMLSTR_DLR_ORDER).c_str()) == (signed)i) {
						break;
					}
				}//end of inner for loop
				
				strDFI_DUI = cDataLinkUtil::getAttributeValue(\
						pdnlsFieldNodeList->item(unFieldIndex), Constants::XMLSTR_DLR_DFI_DUI);

				strInputData = getMessageFieldData(apddocInputStream, strDFI_DUI);
				//check input data, if it has errors assume "0" is supplied as input
				strCheckedInputData = checkInputData(pdnlsFieldNodeList->item(unFieldIndex), strInputData);

				bIsFieldProcessed = cEncodeRuleProcessor::applyEncodingRule(\
						pdnlsFieldNodeList->item(unFieldIndex),\
						bMsbAtHighestIndex, strBitMessage, strCheckedInputData,\
						m_objMessageRepository, m_objErrorRepository);
				
				if (bIsFieldProcessed) {
					m_objMessageRepository.addMessageFieldValue(\
							strMessageName, strDFI_DUI, strCheckedInputData);	
				}
				//if an error has been added this method details the error
				m_objErrorRepository.addErrorDetail(strMessageName, strDFI_DUI);
			}
		} catch(const std::exception& rExp) {
			DLXML_ADD_ERROR(m_objErrorRepository, ETP_EXP_GENERAL, ELV_ERROR, rExp.what())
			DLXML_THROW_EX(DLXML_Exception,	rExp.what())
		}
		return strBitMessage;
	}

	inline string cMessageEncoder::getMessageFieldData(\
			const DOMDocument* apddocInputStream,\
			const string& arstrDFI_DUI_Name) throw (DLXML_Exception)
	{
		//0.32245, true, "122", gibi xml data file'da yer alan degerler bu metod ile return edilecektir
		string strResult;
		//controls could be done here
		DOMElement* pdelmInpStrmDocElement = apddocInputStream->getDocumentElement();
		XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_IDS_DATA);
		DOMNodeList* pdnlsDataNodeList = pdelmInpStrmDocElement->getElementsByTagName(pxchTmpStr);
		XMLString::release(&pxchTmpStr);

		char* pchNodeValue = 0;
		DOMNode* pdndDFI_DUI_Attribute = 0;
		DOMNode* pdnodDataNode = 0;
		for (unsigned int j = 0; j < pdnlsDataNodeList->getLength(); ++j) {
			pdnodDataNode = pdnlsDataNodeList->item(j);
			
			pxchTmpStr = XMLString::transcode(Constants::XMLSTR_IDS_DFI_DUI);
			pdndDFI_DUI_Attribute = pdnodDataNode->getAttributes()->getNamedItem(pxchTmpStr);
			XMLString::release(&pxchTmpStr);
			if (pdndDFI_DUI_Attribute == 0) {
				continue;
			}

			pchNodeValue = XMLString::transcode(pdndDFI_DUI_Attribute->getNodeValue());

			if (!arstrDFI_DUI_Name.compare(pchNodeValue)) { //found a match
				if (pdnodDataNode->hasChildNodes()) {
					if (pdnodDataNode->getFirstChild()->getNodeType() == DOMNode::TEXT_NODE) {
						XMLString::release(&pchNodeValue);
						pchNodeValue = XMLString::transcode(pdnodDataNode->getFirstChild()->getNodeValue());
						if (strlen(pchNodeValue) != 0) {
							strResult.assign(pchNodeValue);
						}
						XMLString::release(&pchNodeValue);
					}
				}
				break;
			}
			XMLString::release(&pchNodeValue);
		} // end of for loop
		return strResult;
	}
	
	inline string cMessageEncoder::checkInputData(
			const DOMNode* apdndFieldNode,
			const string& arstrInputData)
	{
		string strInputDataType = cDataLinkUtil::getAttributeValue(\
				apdndFieldNode, Constants::XMLSTR_DLR_DATA_TYPE);
		if (!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_STRING)) {
			return arstrInputData;
		}
		
		//hand made trim function
		bool bIsInputValid = true;
		string strInputData = arstrInputData;
		size_t sztBeginIndex = strInputData.find_first_not_of(" \t\n");
		size_t sztEndIndex = strInputData.find_last_not_of(" \t\n");		
		if (sztBeginIndex == string::npos) {
			//this means it is an empty string or it is full of space chars,
			//which is another representation of no stmt value in our context,//do nothing but simply return
			return arstrInputData;
		}
		else if (sztBeginIndex > sztEndIndex/*is it possible?*/) {
			return arstrInputData;
		}
		else {
			strInputData = arstrInputData.substr(sztBeginIndex, sztEndIndex - sztBeginIndex + 1);
		}

		if (!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_SHORT) ||
			!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_LONG)) {
			
			std::istringstream issStream(strInputData);
			long lTemp;
			if (!(issStream >> lTemp)) {
				bIsInputValid = false;
			}
			else {
				string strTemp;
				issStream >> strTemp;
				if (!strTemp.empty()) {
					//if we have still chars in buffer then deduce that this input is erroneous
					bIsInputValid = false;
				}
			}
		}
		else if (!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_FLOAT) ||
				!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_DOUBLE)) {
			std::istringstream issStream(strInputData);
			double dTemp;
			if (!(issStream >> dTemp)) {
				bIsInputValid = false;
			}
			else {
				string strTemp;
				issStream >> strTemp;
				if (!strTemp.empty()) {
					//if we have still chars in buffer then deduce that this input is erroneous
					bIsInputValid = false;
				}
			}
		}
		else if (!strInputDataType.compare(Constants::XMLSTR_DLR_TYP_BOOLEAN)) {
			for (size_t i = 0; i < strInputData.size(); ++i) {
				strInputData[i] = tolower(strInputData[i]);
			}

			if (!(!strInputData.compare("true") || !strInputData.compare("false") ||
				!strInputData.compare("1") || !strInputData.compare("0"))) {
				bIsInputValid = false;
			}
		}
		else { // for all other cases, although we checked all of them previously in this method
			//as well as via xsd validation, return as original
			strInputData = arstrInputData;
		}
		
		if (!bIsInputValid) {
			//if input is invalid set it to zero to give a defult value for erroneous inputs
			DLXML_ADD_ERROR(m_objErrorRepository, ETP_DATA_INVALID, ELV_WARNING,\
					"Input is invalid:" + arstrInputData)
			strInputData.clear();
			strInputData = "0";
		}
		return strInputData;
	}

	inline void cMessageEncoder::parseXMLDataStreams(\
			const vector<string>& arvecXML_DataStreams) throw (DLXML_Exception)
	{
		MemBufInputSource* pmbiStringSource = 0;
		m_pxdpDataDomParser->resetDocumentPool();//reset allocated memory of previous parses..

		try {
			for(size_t j = 0; j < arvecXML_DataStreams.size(); ++j) {
				if (pmbiStringSource != 0) {
					delete pmbiStringSource;
				}
				pmbiStringSource = new MemBufInputSource((const XMLByte*)\
					arvecXML_DataStreams[j].c_str(), arvecXML_DataStreams[j].size(),\
					Constants::STR_MEM_BUF_ID, false);
				m_pehErrorHandler->resetErrors();
				m_pehErrorHandler->resetDocument();
				m_pxdpDataDomParser->parse(*pmbiStringSource);
				
				//===================================================================================
				//manually validate input stream, if it is not validated via xsd grammar
				if (!Constants::BLN_VALIDATE_INPUT_STREAM) {
					bool bIsInputValid = false;
					char* pchTempStr = 0;

					DOMElement* pdelmInpStrmDocElement = \
						m_pxdpDataDomParser->getDocument()->getDocumentElement();
					pchTempStr = XMLString::transcode(pdelmInpStrmDocElement->getTagName());
					if (!strcmp(pchTempStr, Constants::XMLSTR_IDS_MESSAGE)) {

						if (cDataLinkUtil::hasAttributeNamed(pdelmInpStrmDocElement, Constants::XMLSTR_IDS_NETWORK) &&\
							cDataLinkUtil::hasAttributeNamed(pdelmInpStrmDocElement, Constants::XMLSTR_IDS_NAME)) {
							//it is valid keep checking
							DOMNodeList* pdnlsChildNodes = pdelmInpStrmDocElement->getChildNodes();
							//only <Data> elements with DFI_DUI attributes are allowed
							for (unsigned int j = 0; j < pdnlsChildNodes->getLength(); ++j) {
								if (pdnlsChildNodes->item(j)->getNodeType() == DOMNode::ELEMENT_NODE) {
									XMLString::release(&pchTempStr);
									pchTempStr = XMLString::transcode(pdnlsChildNodes->item(j)->getNodeName());
									if (!strcmp(pchTempStr, Constants::XMLSTR_IDS_DATA)) {
										if (!cDataLinkUtil::hasAttributeNamed(pdnlsChildNodes->item(j),\
												Constants::XMLSTR_IDS_DFI_DUI)) {
											bIsInputValid = false;
											break;
										}
										else {
											bIsInputValid = true;
										}
									}
									else {
										bIsInputValid = false;
										break;
									}
								}
							} //end of for
						}
					} //end of if (!strcmp(pchTempStr, Constants::XMLSTR_IDS_MESSAGE))
					XMLString::release(&pchTempStr);

					if (!bIsInputValid) {
						DLXML_THROW_EX(DLXML_Exception, "Input Data Stream is invalid")
					}
				}
				
				m_vecInputStreamDomDocuments.push_back(m_pxdpDataDomParser->getDocument());
			} //end of for
		}
		catch (const XMLException& toCatch) {
			delete pmbiStringSource; //free possibly allocated memory before rethrowing exception.
			char* pchExpMessage = XMLString::transcode(toCatch.getMessage());
			string strExceptionMessage(pchExpMessage);
			XMLString::release(&pchExpMessage);
			//pack current exception into DLXML_Exception
			DLXML_THROW_EX(DLXML_Exception, strExceptionMessage)
		}
		catch (const DOMException& toCatch) {
			delete pmbiStringSource;
			char* pchExpMessage = XMLString::transcode(toCatch.msg);
			string strExceptionMessage(pchExpMessage);
			XMLString::release(&pchExpMessage);
			DLXML_THROW_EX(DLXML_Exception, strExceptionMessage)
		}
		delete pmbiStringSource;
		return;
	}

	cMessageRepository& cMessageEncoder::getMessageRepository()
	{
		return m_objMessageRepository;
	}

	cErrorRepository& cMessageEncoder::getErrorRepository()
	{
		return m_objErrorRepository;
	}

} // end of namespace
