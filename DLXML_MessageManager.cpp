
#include "DLXML_MessageManager.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_MessageEncoder.h"
#include "DLXML_MessageDecoder.h"
#include "DLXML_ErrorRepository.h"
#include "DLXML_VocabularyRepository.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_ParserErrorHandler.h"
#include "DLXML_Constants.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/validators/common/Grammar.hpp>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <exception>
#include <memory>
#include <iterator>

XERCES_CPP_NAMESPACE_USE

using std::ostringstream;
using std::vector;
using std::set;
using std::string;
using std::auto_ptr;
using std::map;
using std::copy;
using std::ifstream;
using std::istream_iterator;

namespace DLXML
{
	cMessageManager::cMessageManager(void) : m_pcMessageEncoder(0), m_pcMessageDecoder(0)
	{
		try {
			XMLPlatformUtils::Initialize();  
			m_pcErrorRepository = new cErrorRepository;
			m_pxdpDomParser = new XercesDOMParser;
			parseXMLFiles();
			getMessageIdentifiers();
			checkRuleFilesFurther();
		} catch (const XMLException& toCatch) {
			char* pchExpMessage = XMLString::transcode(toCatch.getMessage());
			string strExceptionMessage(pchExpMessage);
			XMLString::release(&pchExpMessage);
			DLXML_THROW_EX(DLXML_Exception, strExceptionMessage)
		} catch (const DLXML_Exception& rexpDLXML_Exception) {
			throw;
		} catch (const std::exception& rexpStdException) {
			//pack again..
			DLXML_THROW_EX(DLXML_Exception, rexpStdException.what())
		}

		m_pcMessageEncoder = new cMessageEncoder(m_vecMessageIdentifiers);
		m_pcMessageDecoder = new cMessageDecoder(m_vecMessageIdentifiers);
	}

	cMessageManager::~cMessageManager(void)
	{
		delete m_pxdpDomParser;
		delete m_pcMessageEncoder;
		delete m_pcMessageDecoder;
		delete m_pcErrorRepository;
		XMLPlatformUtils::Terminate();
	}
	
	cMessageManager& cMessageManager::getInstance()
	{
		static cMessageManager objInstance;
	    return objInstance;
	}

	inline void cMessageManager::parseXMLFiles() throw (DLXML_Exception)
	{
		auto_ptr<XercesDOMParser> pxdpDomParser(new XercesDOMParser);
		cParserErrorHandler* pehErrorHandler = 0;
		pehErrorHandler = new cParserErrorHandler(*m_pcErrorRepository);
		try {
			pxdpDomParser->setValidationScheme(XercesDOMParser::Val_Always);
			pxdpDomParser->setDoNamespaces(true);
			pxdpDomParser->setValidationSchemaFullChecking(true);
			pxdpDomParser->setDoSchema(true);
			pehErrorHandler->resetErrors();
			pehErrorHandler->resetDocument();
			pxdpDomParser->setErrorHandler(pehErrorHandler);

			//----------------------------------------------------------------------------
			//============ Configuration File Parsing      ===============================
			string strConfigFileUrl = string(Constants::STR_CONFIG_DIR_URL).\
				append(Constants::STR_CONFIG_FILE);
			pxdpDomParser->parse(strConfigFileUrl.c_str());
			vector<string> vecRuleFileUrls;
			char* pchTempStr = 0;
			DOMNode* pdndChild = 0;
			DOMElement* pdelmElement = 0;
			string strVocabularyFileUrl; //"./Config/Vocabulary.xml";
			string strTemp;
			
			DOMElement* pdelmCfgFileDocumentElement = pxdpDomParser->getDocument()->getDocumentElement();
			string strConfigDirUrl = Constants::STR_CONFIG_DIR_URL;

			pdelmElement = cDataLinkUtil::getElementByName(\
					pdelmCfgFileDocumentElement, Constants::XMLSTR_CFG_RULE_FILES);
			Constants::STR_DATA_LINK_GRAMMAR_FILE = cDataLinkUtil::getAttributeValue(\
								pdelmElement, Constants::XMLSTR_CFG_SCHEMA_FILE);
			pdndChild = pdelmElement->getFirstChild();
			while (pdndChild != 0) {
				if (pdndChild->getNodeType() == DOMNode::ELEMENT_NODE) {
					pchTempStr = XMLString::transcode(pdndChild->getNodeName());
					if (!strcmp(pchTempStr, Constants::XMLSTR_CFG_FILE)) {
						vecRuleFileUrls.push_back(strConfigDirUrl +\
							cDataLinkUtil::getAttributeValue(pdndChild, Constants::XMLSTR_CFG_NAME));
					}
					XMLString::release(&pchTempStr);
				}
				pdndChild = pdndChild->getNextSibling();
			} //end of while

			pdelmElement = cDataLinkUtil::getElementByName(\
					pdelmCfgFileDocumentElement, Constants::XMLSTR_CFG_VOCABULARY);
			strVocabularyFileUrl = strConfigDirUrl + cDataLinkUtil::getAttributeValue(\
					pdelmElement, Constants::XMLSTR_CFG_NAME);

			pdelmElement = cDataLinkUtil::getElementByName(\
					pdelmCfgFileDocumentElement, Constants::XMLSTR_CFG_INPUT_STREAM_SCHEMA);			
			Constants::STR_INPUT_STREAM_GRAMMAR_FILE = cDataLinkUtil::getAttributeValue(\
					pdelmElement, Constants::XMLSTR_CFG_NAME);
			
			pdelmElement = cDataLinkUtil::getElementByName(\
					pdelmCfgFileDocumentElement, Constants::XMLSTR_CFG_PARAMETERS);
			pdndChild = pdelmElement->getFirstChild();
			while (pdndChild != 0) {
				if (pdndChild->getNodeType() == DOMNode::ELEMENT_NODE) {
					pchTempStr = XMLString::transcode(pdndChild->getNodeName());
					if (!strcmp(pchTempStr, Constants::XMLSTR_CFG_PARAM)) {
						strTemp = cDataLinkUtil::\
						getAttributeValue(pdndChild, Constants::XMLSTR_CFG_KEY);
						
						if (!strTemp.compare(Constants::XMLSTR_CFG_VALIDATE_INPUT_STREAMS)) {
							strTemp = cDataLinkUtil::getAttributeValue(pdndChild, Constants::XMLSTR_CFG_VALUE);
							Constants::BLN_VALIDATE_INPUT_STREAM = !strTemp.compare("true");
						}
					}
					XMLString::release(&pchTempStr);
				}
				pdndChild = pdndChild->getNextSibling();
			} //end of while
			//----------------------------------------------------------------------------
			//============ Vocabulary File Parsing         ===============================
			pehErrorHandler->resetErrors();
			pehErrorHandler->resetDocument();
			pxdpDomParser->setErrorHandler(pehErrorHandler);
			pxdpDomParser->parse(strVocabularyFileUrl.c_str());
			
			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_VOC_VOCABULARY);
			DOMNodeList* pdnlsVocNodeList = pxdpDomParser->getDocument()->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);
			char chTemp;
			for (unsigned short j = 0; j < pdnlsVocNodeList->getLength(); ++j) {
				DLXML_Vocabulary vocTemp;
				strTemp = cDataLinkUtil::getAttributeValue(\
						pdnlsVocNodeList->item(j), Constants::XMLSTR_VOC_NAME);
				vocTemp.strName.assign(strTemp);
				
				pdndChild = pdnlsVocNodeList->item(j)->getFirstChild();
				while (pdndChild != 0) {
					if (pdndChild->getNodeType() == DOMNode::ELEMENT_NODE) {
						pchTempStr = XMLString::transcode(pdndChild->getNodeName());
						if (!strcmp(pchTempStr, Constants::XMLSTR_VOC_ENTRY)) {
							strTemp = cDataLinkUtil::getAttributeValue(\
									pdndChild, Constants::XMLSTR_VOC_DATAITEM);
							chTemp = strTemp.at(0);
							strTemp = cDataLinkUtil::getAttributeValue(\
									pdndChild, Constants::XMLSTR_VOC_BITCODE);
							char* pchBitCode = new char[strTemp.size()];
							strcpy(pchBitCode, strTemp.c_str());

							vocTemp.mapVocabulary.insert(map<char, char*>::value_type(chTemp, pchBitCode));
						}
						XMLString::release(&pchTempStr);
					}
					pdndChild = pdndChild->getNextSibling();
				} //end of while

				cVocabularyRepository::addVocabulary(vocTemp);
			} //end of for
			//----------------------------------------------------------------------------
			//============ Data Link Rule Files Parsing      =============================
			pehErrorHandler->resetErrors();
			pehErrorHandler->resetDocument();
			m_pxdpDomParser->setErrorHandler(pehErrorHandler);
			m_pxdpDomParser->setValidationScheme(XercesDOMParser::Val_Always);
			m_pxdpDomParser->setDoNamespaces(true);
			m_pxdpDomParser->setValidationSchemaFullChecking(true);
			m_pxdpDomParser->setDoSchema(true);
			m_pxdpDomParser->loadGrammar(string(Constants::STR_CONFIG_DIR_URL +\
				Constants::STR_DATA_LINK_GRAMMAR_FILE).c_str(),\
				Grammar::SchemaGrammarType, true);
			m_pxdpDomParser->cacheGrammarFromParse(true);

			for (size_t j = 0; j < vecRuleFileUrls.size(); ++j) {
				m_pxdpDomParser->parse(vecRuleFileUrls[j].c_str());
				m_vecMessageSpecifications.push_back(m_pxdpDomParser->getDocument());
			}//end of for
			//----------------------------------------------------------------------------
		} catch (const XMLException& toCatch) {
			char* pchExpMessage = XMLString::transcode(toCatch.getMessage());
			string strExceptionMessage(pchExpMessage);
			XMLString::release(&pchExpMessage);
			delete pehErrorHandler;
			DLXML_THROW_EX(DLXML_Exception, strExceptionMessage)
		} catch (const DOMException& toCatch) {
			char* pchExpMessage = XMLString::transcode(toCatch.msg);
			string strExceptionMessage(pchExpMessage);
			XMLString::release(&pchExpMessage);
			delete pehErrorHandler;
			DLXML_THROW_EX(DLXML_Exception, strExceptionMessage)
		}
		delete pehErrorHandler;
		return;
	}
	
	inline void cMessageManager::getMessageIdentifiers()
	{
		string strTemp;
		string strMesMask;
		DLXML_DataLinkNetwork eDataLinkType = DLN_LINK_11;
		static cEnumRepository& objEnumRepository = cEnumRepository::getInstance();
		
		for (size_t i = 0; i < m_vecMessageSpecifications.size(); ++i) {
			DLXML_MessageIdentifierStructure stMsgIdentifier;
			stMsgIdentifier.pddocMessageRule = m_vecMessageSpecifications[i];
			//get Message Type
			DOMElement* pdelmMessageType = cDataLinkUtil::getElementByName(\
				m_vecMessageSpecifications[i]->getDocumentElement(),\
				Constants::XMLSTR_DLR_MESSAGE_NETWORK);

			strTemp = cDataLinkUtil::getElementContent(pdelmMessageType);
			eDataLinkType = (DLXML_DataLinkNetwork) objEnumRepository.StrToEnum(\
				Constants::STR_ENUM_DLXML_DATA_LINK_NET, strTemp);
			if (eDataLinkType == DLN_LINK_11) {
				strMesMask.assign(Constants::INT_LINK_11_MESSAGE_SIZE, '*');
			}
			else if (eDataLinkType == DLN_LINK_16) {
				strMesMask.assign(Constants::INT_LINK_16_MESSAGE_SIZE, '*');
			}
			else if (eDataLinkType == DLN_IJMS) {
				strMesMask.assign(Constants::INT_IJMS_MESSAGE_SIZE, '*');
			}
			else {
				DLXML_THROW_EX(DLXML_Exception, strTemp.insert(0,\
						" Message Network is not implemented in DLXML."))
			}
			stMsgIdentifier.eMessageNetwork = eDataLinkType;
			
			//get Message Name
			strTemp.clear();
			DOMElement* pdelmMessageName = cDataLinkUtil::getElementByName(\
					m_vecMessageSpecifications[i]->getDocumentElement(),\
					Constants::XMLSTR_DLR_MESSAGE_NAME);
			
			stMsgIdentifier.strMessageName = cDataLinkUtil::getElementContent(pdelmMessageName);
			
			//error checking should be added below..
			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_ID);
			const DOMNodeList* pdnlsIdNodeList = m_vecMessageSpecifications[i]->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);

			for (unsigned int j = 0; j < pdnlsIdNodeList->getLength(); ++j) {
				strTemp = cDataLinkUtil::getAttributeValue(pdnlsIdNodeList->item(j),\
						Constants::XMLSTR_DLR_RESOLUTION);
				if (eDataLinkType == DLN_LINK_11 || eDataLinkType == DLN_LINK_16) {
					std::reverse(strTemp.begin(), strTemp.end());
				}
				strMesMask.replace(
						atoi(cDataLinkUtil::getAttributeValue(pdnlsIdNodeList->item(j),\
								Constants::XMLSTR_DLR_BIT_POSITION).c_str()),
						atoi(cDataLinkUtil::getAttributeValue(pdnlsIdNodeList->item(j),\
								Constants::XMLSTR_DLR_BIT_LENGTH).c_str()),
						strTemp);
			}
			stMsgIdentifier.strMessageTemplate = strMesMask;
			m_vecMessageIdentifiers.push_back(stMsgIdentifier);
		} //end of for
		return;
	}
	
	inline void cMessageManager::checkRuleFilesFurther()
	{
		string strMessageName;
		char* pchNodeValue = 0;
		DOMNode* pdndAttribute = 0;
		vector<short> vecOrderNumbers;

		//check whether order attributes of the field elements are
		//increasing by one starting from zero.
		for (size_t i = 0; i < m_vecMessageSpecifications.size(); ++i) {
			const DOMElement* pdelmMessageName = cDataLinkUtil::getElementByName(\
				m_vecMessageSpecifications[i]->getDocumentElement(), Constants::XMLSTR_DLR_MESSAGE_NAME);
			strMessageName = cDataLinkUtil::getElementContent(pdelmMessageName);

			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_FIELD);
			DOMNodeList* pdnlsFieldNodeList = m_vecMessageSpecifications[i]->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);

			vecOrderNumbers.clear();
			pchNodeValue = 0;
			pdndAttribute = 0;

			for (unsigned short j = 0; j < pdnlsFieldNodeList->getLength(); ++j) {
				pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_ORDER);
				pdndAttribute = pdnlsFieldNodeList->item(j)->getAttributes()->getNamedItem(pxchTmpStr);
				XMLString::release(&pxchTmpStr);
				pchNodeValue = XMLString::transcode(pdndAttribute->getNodeValue());
				vecOrderNumbers.push_back(atoi(pchNodeValue));
				XMLString::release(&pchNodeValue);
			}

			//vector'deki rakamlar 0'dan baslayip sirali olarak artiyor mu kontrolu?
			std::sort(vecOrderNumbers.begin(), vecOrderNumbers.end());
			for (size_t k = 0; k < vecOrderNumbers.size(); ++k) {
				if (vecOrderNumbers[k] != (signed)k) {
					DLXML_THROW_EX(DLXML_Exception, strMessageName.insert(0,\
							"Message Field Rules are not in order for message "))
				}
			}
		}// end of for loop
		
		//***********************************************************************************
		//check no stmt values
		string strNoStmt;
		size_t szBitLengtofField;
		for (size_t i = 0; i < m_vecMessageSpecifications.size(); ++i) {
			strMessageName = cDataLinkUtil::getElementContent(cDataLinkUtil::getElementByName(\
					m_vecMessageSpecifications[i]->getDocumentElement(), Constants::XMLSTR_DLR_MESSAGE_NAME));

			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_FIELD);
			DOMNodeList* pdnlsFieldNodeList = m_vecMessageSpecifications[i]->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);
			
			for (unsigned short j = 0; j < pdnlsFieldNodeList->getLength(); ++j) {
				
				szBitLengtofField = atoi(cDataLinkUtil::getAttributeValue(\
						pdnlsFieldNodeList->item(j), Constants::XMLSTR_DLR_BIT_LENGTH).c_str());
				
				pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_NO_STMT);
				DOMNodeList* pdnlsNoStatementNodeList = ((DOMElement*)pdnlsFieldNodeList->item(j))->\
					getElementsByTagName(pxchTmpStr);
				XMLString::release(&pxchTmpStr);				

				for (unsigned short k = 0; k < pdnlsNoStatementNodeList->getLength(); ++k) {
					
					strNoStmt = cDataLinkUtil::getElementContent(pdnlsNoStatementNodeList->item(k));
					if (strNoStmt.size() != szBitLengtofField) {
						DLXML_THROW_EX(DLXML_Exception, strMessageName.insert(0,\
							"No Statement value size of " + strNoStmt +\
							" should be same with the bitlength of the field it is defined for in message "))
					}
				}
			}
		} //end of checking no stmt value sizes
		
		//***********************************************************************************
		// check that whether the relation between operand names and relational expressions 
		// are "surjective". That is supposed to mean for every operand value there exists
		// at least one relational expresssion and each relational expression is used in at
		// least one operand.
		set<string> setOperandNames;//we prefer std::set since it does not allow duplicates
		vector<string> vecRelExpressionNames;
		
		for (size_t i = 0; i < m_vecMessageSpecifications.size(); ++i) {
			setOperandNames.clear();
			vecRelExpressionNames.clear();

			//find strMessageName
			strMessageName = cDataLinkUtil::getElementContent(cDataLinkUtil::getElementByName(\
					m_vecMessageSpecifications[i]->getDocumentElement(),\
					Constants::XMLSTR_DLR_MESSAGE_NAME));

			XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_RELATIONAL_EXPRESSION);
			DOMNodeList* pdnlsRelExpressionList = m_vecMessageSpecifications[i]->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);
			for (unsigned short j = 0; j < pdnlsRelExpressionList->getLength(); ++j) {
				vecRelExpressionNames.push_back(cDataLinkUtil::getAttributeValue(\
						pdnlsRelExpressionList->item(j), Constants::XMLSTR_DLR_NAME));
			}
			pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_OPERAND);
			DOMNodeList* pdnlsOperandsList = m_vecMessageSpecifications[i]->\
				getDocumentElement()->getElementsByTagName(pxchTmpStr);
			XMLString::release(&pxchTmpStr);
			for (unsigned short j = 0; j < pdnlsOperandsList->getLength(); ++j) {
				setOperandNames.insert(cDataLinkUtil::getElementContent(\
						pdnlsOperandsList->item(j)));
			}

			set<string>::const_iterator itrOperandNames = setOperandNames.begin();
			vector<string>::iterator itrRelExpressionNames = vecRelExpressionNames.begin();
			while(itrOperandNames != setOperandNames.end()) {
				itrRelExpressionNames = std::find(vecRelExpressionNames.begin(),\
						vecRelExpressionNames.end(), *itrOperandNames);
				if (itrRelExpressionNames == vecRelExpressionNames.end()) {
					DLXML_THROW_EX(DLXML_Exception, strMessageName.insert(0, *itrOperandNames +\
						" is not defined in Relational Expressions of the message "))
				}
				else {
					vecRelExpressionNames.erase(itrRelExpressionNames);
				}
				itrOperandNames++;
			}

			//if there exist any element in the vecRelExpressionNames after the above loop
			//then throw an exception defining the situation.
			if (!vecRelExpressionNames.empty()) {
				//std::copy(vecRelExpressionNames.begin(),
				//		vecRelExpressionNames.end(),
				//		std::ostream_iterator<string>(cout, "\n"));
				DLXML_THROW_EX(DLXML_Exception, strMessageName.insert(0,\
					" Not all of the Relational Expressions are used in the message "))
			}
		} //end of checking operand values and relational expressions
		return;
	}

	void cMessageManager::addGeneralSymbol(const string& arstrSymbolName, const string& arstrValue)
	{
		m_pcMessageEncoder->getMessageRepository().addGeneralSymbol(arstrSymbolName, arstrValue);
		m_pcMessageDecoder->getMessageRepository().addGeneralSymbol(arstrSymbolName, arstrValue);
		return;
	}

	const cErrorRepository& cMessageManager::getErrorRepository()
	{
		return *m_pcErrorRepository;
	}

	cMessageEncoder* cMessageManager::getMessageEncoder()
	{
		return m_pcMessageEncoder;
	}

	cMessageDecoder* cMessageManager::getMessageDecoder()
	{
		return m_pcMessageDecoder;
	}
} // end of namespace
