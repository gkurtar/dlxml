#include "DLXML_MessageDecoder.h"
#include "DLXML_DataLinkUtil.h"
#include "DLXML_DecodeRuleProcessor.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_Constants.h"

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/util/XMLString.hpp>
#include <map>
#include <algorithm>

XERCES_CPP_NAMESPACE_USE
using std::reverse;
using std::map;
using std::vector;
using std::string;

namespace DLXML
{
	cMessageDecoder::cMessageDecoder(\
			const DLXML_MessageIdentifierVector& arvecMessageIdentifiers) : \
				m_objMessageRepository(arvecMessageIdentifiers)
	{
	}
	
	cMessageDecoder::~cMessageDecoder(void)
	{
	}
	
	cMessageRepository& cMessageDecoder::getMessageRepository()
	{
		return m_objMessageRepository;
	}

	cErrorRepository& cMessageDecoder::getErrorRepository()
	{
		return m_objErrorRepository;
	}

	vector<string> cMessageDecoder::decodeMessages(\
			const vector<string>& arvecEncodedMessages) throw (DLXML_Exception)
	{
		vector<string> vecDecodedMessages;
		DLXML_DataLinkNetwork eDataLinkType = DLN_LINK_11;
		this->checkMessages(arvecEncodedMessages, eDataLinkType);

		bool bMSB_AtHighestIndex = true;//denote the bit orientation in messages
		if (eDataLinkType == DLN_LINK_11 || eDataLinkType == DLN_LINK_16) {
			bMSB_AtHighestIndex = true;
		}
		else if (eDataLinkType == DLN_IJMS) {
			bMSB_AtHighestIndex = false;
		}

		string strPrevMesName;
		string strEncodedMessage;
		string strAmplifyMessageName;
		const DLXML_MessageIdentifierStructure* pstMessageIdentifier = 0;

		m_objMessageRepository.clearMessageFieldValues();
		
		//loop through encoded messages, for each find the matching XML Rule file
		//and decode the message according to its rules
		for (size_t i = 0; i < arvecEncodedMessages.size(); ++i) {
			strEncodedMessage.assign(arvecEncodedMessages[i]);
			pstMessageIdentifier = 0; //reset Message Identifier pointer
			
			if (eDataLinkType == DLN_LINK_11) {				
				//if it is not amplify then process it
				if (strEncodedMessage.substr(0, 4).compare(Constants::STR_L11_AMPMSG_1ST_4BITS)) {
					pstMessageIdentifier = m_objMessageRepository.\
						getIdentifierOfEncodedMessage(eDataLinkType, strEncodedMessage);
				}
				else {//if it is amplify, handle them
					if (strPrevMesName.find('.') != string::npos) {
						//Amplify messages has an '8' after the dot char as in M.XL M.8XL
						//Modify copy of prev mesg name to obtain amplify msg name
						strAmplifyMessageName = strPrevMesName.insert(\
								strPrevMesName.find('.') + 1, Constants::STR_L11_AMPMSG_CHAR);

						pstMessageIdentifier = m_objMessageRepository.\
							getIdentifierOfMessageName(eDataLinkType, strAmplifyMessageName);
					}
				} // end of else --it is an amplify message--
			} // end of if (eDataLinkType == DLN_LINK_11)
			
			if (pstMessageIdentifier == 0) { //we couldn't find a match..
				DLXML_ADD_ERROR(m_objErrorRepository, ETP_EXP_GENERAL, ELV_ERROR,\
					"Matching XML Rule not found for the input " + strEncodedMessage)
				DLXML_THROW_EX(DLXML_Exception,\
					"Matching XML Rule not found for the input " + strEncodedMessage)
				strPrevMesName = Constants::STR_UNDEFINED;
			}
			else {
				vecDecodedMessages.push_back(\
					decodeMessage(strEncodedMessage, pstMessageIdentifier));
				strPrevMesName = pstMessageIdentifier->strMessageName;
			}
		} //end of encoded message loop
		//*************************************************************************************************************
		return vecDecodedMessages;
	}

	//order dom document fields into a map such that mapFieldOrders<n, k> means -> 
	//DOM element pdnlsFieldNodeList->item(k) has attribute Order="n"
	//for each field element in dom document in order (by order attribute) 
	//decode the encoded bit string and accumulate decoded parts locally to generate an xml stream.
	inline string cMessageDecoder::decodeMessage(const string& arstrEncodedMessage,\
			const DLXML_MessageIdentifierStructure* apstMessageIdentifier)
	{
		static cEnumRepository& objEnumRepository = cEnumRepository::getInstance();
		XMLCh* pxchTmpStr = XMLString::transcode(Constants::XMLSTR_DLR_FIELD);
		const DOMNodeList* pdnlsFieldNodeList = apstMessageIdentifier->\
			pddocMessageRule->getElementsByTagName(pxchTmpStr);
		XMLString::release(&pxchTmpStr);

		map<int, int> mapFieldOrders;
		for (unsigned int i = 0; i < pdnlsFieldNodeList->getLength(); ++i) {
			mapFieldOrders.insert(map<int, int>::value_type(\
				atoi(cDataLinkUtil::getAttributeValue(pdnlsFieldNodeList->item(i),\
						Constants::XMLSTR_DLR_ORDER).c_str()), i));
		} //Now mapFieldOrders<n, k> means -> DOM element pdnlsFieldNodeList->item(k) has attribute Order="n"
		
		bool bMSB_AtHighestIndex = true;//represents bit orientation in tadil messages
		if (apstMessageIdentifier->eMessageNetwork == DLN_LINK_11 ||\
				apstMessageIdentifier->eMessageNetwork == DLN_LINK_16) {
			bMSB_AtHighestIndex = true;
		}
		else if (apstMessageIdentifier->eMessageNetwork == DLN_IJMS) {
			bMSB_AtHighestIndex = false;
		}
		
		string strDecodedMessage;
		strDecodedMessage.reserve(250);
		string strDecodedField;
		string strDFI_DUI;
		
		strDecodedMessage.append("<");
		strDecodedMessage.append(Constants::XMLSTR_IDS_MESSAGE);
		strDecodedMessage.append(" ");
		strDecodedMessage.append(Constants::XMLSTR_IDS_NETWORK);
		strDecodedMessage.append("=\"");
		strDecodedMessage.append(objEnumRepository.EnumToStr(\
			Constants::STR_ENUM_DLXML_DATA_LINK_NET, apstMessageIdentifier->eMessageNetwork));
		strDecodedMessage.append("\" ");
		strDecodedMessage.append(Constants::XMLSTR_IDS_NAME);
		strDecodedMessage.append("=\"");
		strDecodedMessage.append(apstMessageIdentifier->strMessageName);
		strDecodedMessage.append("\">\n");
		
		map<int, int>::const_iterator itrFieldIndex;
		for (unsigned int i = 0; i < pdnlsFieldNodeList->getLength(); ++i) {
			strDecodedField.clear();
			//find element which has order number "i", and call decode function
			itrFieldIndex = mapFieldOrders.find(i);

			strDFI_DUI = cDataLinkUtil::getAttributeValue(\
					pdnlsFieldNodeList->item(itrFieldIndex->second), Constants::XMLSTR_DLR_DFI_DUI);

			cDecodeRuleProcessor::applyDecodingRule(pdnlsFieldNodeList->item(itrFieldIndex->second),\
					m_objMessageRepository, m_objErrorRepository,\
					bMSB_AtHighestIndex, arstrEncodedMessage, strDecodedField);

			if (!strDecodedField.compare(Constants::STR_NOT_OPERATIONAL)) {
				continue;
			}

			strDecodedMessage.append("<");
			strDecodedMessage.append(Constants::XMLSTR_IDS_DATA);
			strDecodedMessage.append(" ");
			strDecodedMessage.append(Constants::XMLSTR_IDS_DFI_DUI);
			strDecodedMessage.append("=\"");
			strDecodedMessage.append(strDFI_DUI);
			strDecodedMessage.append("\">");
			strDecodedMessage.append(strDecodedField);
			strDecodedMessage.append("</");
			strDecodedMessage.append(Constants::XMLSTR_IDS_DATA);
			strDecodedMessage.append(">\n");

			m_objMessageRepository.addMessageFieldValue(apstMessageIdentifier->\
					strMessageName, strDFI_DUI, strDecodedField);
			m_objErrorRepository.addErrorDetail(apstMessageIdentifier->strMessageName, strDFI_DUI);
		}//end of for

		strDecodedMessage.append("</");
		strDecodedMessage.append(Constants::XMLSTR_IDS_MESSAGE);
		strDecodedMessage.append(">\n");
		return strDecodedMessage;
	}

	void cMessageDecoder::checkMessages(const vector<string>& arvecEncodedMessages,\
			DLXML_DataLinkNetwork& areDataLinkType) throw (DLXML_Exception)
	{
		//kontrol, mesaj yapilari
		if (!arvecEncodedMessages.size()) {;
			DLXML_THROW_EX(DLXML_Exception,\
					"Encoded Message Collection does not contain any element.")
		}

		//identify the link network type
		if (arvecEncodedMessages[0].size() == Constants::INT_LINK_11_MESSAGE_SIZE) {
			areDataLinkType = DLN_LINK_11;
		}
		//else if (arvecEncodedMessages[0].size() == Constants::INT_LINK_16_MESSAGE_SIZE) {
		//	areDataLinkType = DLN_LINK_16;
		//}
		//else if (arvecEncodedMessages[0].size() == Constants::INT_IJMS_MESSAGE_SIZE) {
		//	areDataLinkType = DLN_IJMS;
		//}
		else {
			DLXML_THROW_EX(DLXML_Exception,\
				"Decoding is not ready except for Link11 Messages.")
		}
		
		//all messages in the vector should have same size		
		bool bIsSequenceValid = true; 
		for (size_t i = 0; i < arvecEncodedMessages.size() - 1; ++i) {
			if (arvecEncodedMessages[i].size() != arvecEncodedMessages[i + 1].size()) {
				bIsSequenceValid = false;
				break;
			}
		}
		if (!bIsSequenceValid) {
			DLXML_THROW_EX(DLXML_Exception, "Each Encoded Message must have same length.")
		}

		//All encoded messages should consist of 0's and 1's
		size_t nMesLen = (areDataLinkType == DLN_LINK_11 ? Constants::INT_LINK_11_MESSAGE_SIZE :\
			(areDataLinkType == DLN_LINK_16 ? Constants::INT_LINK_16_MESSAGE_SIZE :\
					Constants::INT_IJMS_MESSAGE_SIZE));

		for (size_t i = 0; i < arvecEncodedMessages.size(); ++i) {
			for (size_t j = 0; j < nMesLen; ++j) {
				if (arvecEncodedMessages[i].at(j) != '0' && \
						arvecEncodedMessages[i].at(j) != '1') {
					bIsSequenceValid = false;
					break;
				}
			}
			if (!bIsSequenceValid) {
				break;
			}
		}
		if (!bIsSequenceValid) {
			DLXML_THROW_EX(DLXML_Exception, "Each Encoded Message should consist of [0-1]+.")
		}

		if (areDataLinkType == DLN_LINK_11) {
			string strExpMessage;
		    //kontrol, amplify messages shoulf not follow each other
			//and a sequence should start with an M.1

			//we have omitted M.1 message checking
			//if (arvecEncodedMessages[0].substr(0, 4).compare("1000")) {
			//	strExpMessage.assign(\
			//			"Encoded Message sequence should start with an M.1 message.");
			//	bIsSequenceValid = false;
			//}
			//else {
				bool bExpectingAmplify = false;
				for (size_t i = 0; i < arvecEncodedMessages.size(); ++i) {

					if (!(arvecEncodedMessages[i].substr(0, 4).compare("0001"))) {
						if (bExpectingAmplify) {
							bExpectingAmplify = false;
						}
						else {				
							bIsSequenceValid = false;
							strExpMessage.assign("Encoded Messages are not in correct order.");
							break;
						}
					}
					else {
						bExpectingAmplify = true;
					}
				}
			//}//end of else

		    if (!bIsSequenceValid) {
		    	DLXML_THROW_EX(DLXML_Exception, strExpMessage)
		    }
		}//end of if Link Type = Link11
		return;
	}
}
