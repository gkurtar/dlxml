#include "DLXML_VocabularyRepository.h"

using std::string;
using std::vector;
using std::map;

namespace DLXML
{
	DLXML_VocabularyVector cVocabularyRepository::m_vecVocabularyVector;

	cVocabularyRepository::cVocabularyRepository(void)
	{
	}
	
	cVocabularyRepository::~cVocabularyRepository(void)
	{
	}

	void cVocabularyRepository::addVocabulary(const DLXML_Vocabulary& arstVocabulary)
	{
		m_vecVocabularyVector.push_back(arstVocabulary);
		return;
	}

	void cVocabularyRepository::addVocabulary(const string& arstrVocName,\
        const map<char, char*>& armapVocabulary, DLXML_ErrorType aeErrorToReport)
	{
		DLXML_Vocabulary stVocabulary;
		stVocabulary.strName.assign(arstrVocName);
		stVocabulary.eErrorToReport = aeErrorToReport;
		stVocabulary.mapVocabulary = armapVocabulary;
		m_vecVocabularyVector.push_back(stVocabulary);
		return;
	}

	string cVocabularyRepository::lookupEntryKey(
			const string& arstrVocabularyName,
			char achEntryKey) throw (DLXML_Exception)
	{
		string strResult;
		bool bIsLookUpSuccessful = false;
		map<char, char*>::const_iterator itrTemp = 0;

		for (size_t j = 0; j < m_vecVocabularyVector.size(); ++j) {
			const DLXML_Vocabulary& stVocabulary = m_vecVocabularyVector[j];

			if (!arstrVocabularyName.compare(stVocabulary.strName)) {
				itrTemp = stVocabulary.mapVocabulary.find(achEntryKey);
				if (itrTemp != stVocabulary.mapVocabulary.end()) {

					if (itrTemp->second == 0) { // if it is null
						DLXML_THROW_EX(DLXML_Exception, "Invalid value for look up key is found")
					}

					bIsLookUpSuccessful = true;
					strResult.assign(itrTemp->second);
					break;
				}
			}
		}//end of for
		
		if (!bIsLookUpSuccessful) {
			DLXML_THROW_EX(DLXML_Exception, "Could not find value for looked up key")
		}
		return strResult;
	}

	char cVocabularyRepository::lookupEntryValue(
			const std::string& arstrVocabularyName,
			const std::string& arstrEntryValue) throw (DLXML_Exception)
	{
		char chResult = 0;
		bool bIsLookUpSuccessful = false;
		map<char, char*>::const_iterator itrTemp = 0;
		
		for (size_t j = 0; j < m_vecVocabularyVector.size(); ++j) {
			const DLXML_Vocabulary& stVocabulary = m_vecVocabularyVector[j];
			
			if (!arstrVocabularyName.compare(stVocabulary.strName)) {
				itrTemp = stVocabulary.mapVocabulary.begin();
				while (itrTemp != stVocabulary.mapVocabulary.end()) {
					if (!strcmp(arstrEntryValue.c_str(), itrTemp->second)) {
						chResult = itrTemp->first;
						bIsLookUpSuccessful = true;
						break;
					}
					itrTemp++;
				}//end of while
			}
		}//end of for
		
		if (!bIsLookUpSuccessful) {
			DLXML_THROW_EX(DLXML_Exception, "Could not find key for the value searched")
		}

		return chResult;
	}

	string cVocabularyRepository::contents()
	{
		string strContents;

		for (size_t k = 0; k < m_vecVocabularyVector.size(); ++k) {			
			const DLXML_Vocabulary& stVocabulary = m_vecVocabularyVector[k];

			strContents.append("\nName = ");
			strContents.append(stVocabulary.strName);
			strContents.append("\n\tEntries:");

			std::map<char, char*>::const_iterator itrStart = stVocabulary.mapVocabulary.begin();
			std::map<char, char*>::const_iterator itrEnd = stVocabulary.mapVocabulary.end();
			
			while (itrStart != itrEnd) {
				strContents.append("\n\t\t");
				strContents.append(1, itrStart->first);
				strContents.append(" = ");
				strContents.append(itrStart->second);				
				++itrStart;
			}
		}

		if (!strContents.size()) {
			strContents.assign("Empty Contents...");
		}
		return strContents;
	}
	
}
