#include "DLXML_MessageRepository.h"
#include "DLXML_Constants.h"

#include <iomanip>

using std::map;
using std::ostringstream;
using std::string;
using std::ostream;

namespace DLXML
{
	
    ostream& operator<<(ostream& arostStream, const cMessageRepository& arcMessageRepository)
    {
    	if (arcMessageRepository.m_mapGeneralSymbols.size()) {
    		map<string, string>::const_iterator itrTemp = arcMessageRepository.m_mapGeneralSymbols.begin();
    		map<string, string>::const_iterator itrEnd = arcMessageRepository.m_mapGeneralSymbols.end();
    		arostStream << Constants::STR_STARS << "\nGeneral Symbols:\n";
    		while (itrTemp != itrEnd) {
    			arostStream << "\t'" << itrTemp->first << "' = '" << itrTemp->second << "'\n"; 
    			itrTemp++;
    		}
    		arostStream << Constants::STR_STARS << std::endl;
    	}
    	if (arcMessageRepository.m_mapMessageFieldValues.size()) {
    		map<string, string>::const_iterator itrTemp = arcMessageRepository.m_mapMessageFieldValues.begin();
    		map<string, string>::const_iterator itrEnd = arcMessageRepository.m_mapMessageFieldValues.end();
    		arostStream << Constants::STR_STARS << "\nMessage Fields:\n";
    		while (itrTemp != itrEnd) {
    			arostStream << "\t'" << itrTemp->first << "' = '" << itrTemp->second << "'\n"; 
    			itrTemp++;
    		}
    		arostStream << Constants::STR_STARS << std::endl;
    	}
    	return arostStream;
    }

    cMessageRepository::cMessageRepository( \
    		const DLXML_MessageIdentifierVector& arvecMessageIdentifiers) : \
    				m_rvecMessageIdentifiers(arvecMessageIdentifiers)
	{
	}

	cMessageRepository::~cMessageRepository(void)
	{
	}

	void cMessageRepository::addMessageFieldValue(\
			const string& arstrMessageName,\
			const string& arstrFieldDFI_DUI,\
			const string& arstrFieldValue)
	{
		
		if (arstrMessageName.size() != 0 && arstrFieldDFI_DUI.size() != 0 && arstrFieldValue.size() != 0) {
			/*time_t timTemp = time(NULL);
			string strKey = string(ctime(&timTemp));
			strKey = strKey.substr(0, strKey.size() - 1); 
			strKey.append("::");
			strKey.append(astrFieldDFI_DUI);*/
			m_mapMessageFieldValues.insert(\
					make_pair(arstrMessageName + "::" + arstrFieldDFI_DUI, arstrFieldValue));
		}
		return;
	}

	string cMessageRepository::getMessageFieldValue(const string& arstrDFI_DUI)
	{
		map<string, string>::reverse_iterator ritStart = m_mapMessageFieldValues.rbegin();
		for (; ritStart != m_mapMessageFieldValues.rend(); ritStart++) {
			if (ritStart->first.find(arstrDFI_DUI, 0) != string::npos) {
				return ritStart->second;
			}
		}
		return Constants::STR_UNDEFINED;
	}

	void cMessageRepository::addGeneralSymbol(const string& arstrSymbolName, const string& arstrValue)
	{
		if (arstrSymbolName.size() == 0 || arstrValue.size() == 0) {
			return;
		}

		map<string, string>::iterator itrTemp;
		itrTemp = m_mapGeneralSymbols.find(arstrSymbolName);
		if (itrTemp == m_mapGeneralSymbols.end()) {
			m_mapGeneralSymbols.insert(make_pair(arstrSymbolName, arstrValue));
		}
		else {
			itrTemp->second.assign(arstrValue);
		}
		return;
	}

	string cMessageRepository::getGeneralSymbol(const string& arstrSymbolName)
	{
		map<string, string>::const_iterator itrTemp = m_mapGeneralSymbols.find(arstrSymbolName);
		return itrTemp == m_mapGeneralSymbols.end() ? Constants::STR_UNDEFINED : itrTemp->second;
	}

	void cMessageRepository::clearMessageFieldValues()	
	{
		if (m_mapMessageFieldValues.size()) {
			m_mapMessageFieldValues.clear();
			//a way to be sure of full deletion is below, but it is not included for performance reasons
			//map<string, string>().swap(m_mapMessageFieldValues);
		}
		return;		
	}
	
	const DLXML_MessageIdentifierStructure* cMessageRepository:: \
		getIdentifierOfMessageName(DLXML_DataLinkNetwork aeMessageNetwork,\
			const std::string& arstrMessageName)
	{
		const DLXML_MessageIdentifierStructure* pstMessageIdentifier = 0;
		for (size_t j = 0; j < m_rvecMessageIdentifiers.size(); ++j) {
			if (!m_rvecMessageIdentifiers[j].strMessageName.compare(arstrMessageName) &&\
					m_rvecMessageIdentifiers[j].eMessageNetwork == aeMessageNetwork) {
				pstMessageIdentifier = &m_rvecMessageIdentifiers[j];
				break;
			}
		}
		return pstMessageIdentifier;
	}
	
	const DLXML_MessageIdentifierStructure* cMessageRepository:: \
		getIdentifierOfEncodedMessage(DLXML_DataLinkNetwork aeMessageNetwork,\
			const std::string& arstrEncodedMessage)
	{
		const DLXML_MessageIdentifierStructure* pstMessageIdentifier = 0;
		string::size_type sztPos = 0;
		
		for (size_t j = 0; j < m_rvecMessageIdentifiers.size(); ++j) {
			if (aeMessageNetwork != m_rvecMessageIdentifiers[j].eMessageNetwork) {
				continue;
			}
			if (arstrEncodedMessage.size() != \
					m_rvecMessageIdentifiers[j].strMessageTemplate.size()) {
				continue;
			}
			
			sztPos = 0;//try to match with message template
			while (sztPos < m_rvecMessageIdentifiers[j].strMessageTemplate.size()) {
				if (m_rvecMessageIdentifiers[j].strMessageTemplate[sztPos] != '*') {
					if (m_rvecMessageIdentifiers[j].strMessageTemplate[sztPos] != \
							arstrEncodedMessage[sztPos]) {
						break;
					}
				}
				++sztPos;
			}//end of while

			if (sztPos == arstrEncodedMessage.size()) {
				pstMessageIdentifier = &m_rvecMessageIdentifiers[j];
				break;
			}
		} //end of m_rvecMessageIdentifiers traverse loop
		return pstMessageIdentifier;
	}

}
