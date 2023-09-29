#include "DLXML_EnumRepository.h"
#include "DLXML_Constants.h"

using std::string;
using std::map;

namespace DLXML
{
    typedef map<unsigned int, const char*> ENMAP;

	map<string, ENMAP*> cEnumRepository::m_stamapEnumHolder;
	map<unsigned int, const char*> cEnumRepository::m_stamapRelOpEnum;
	map<unsigned int, const char*> cEnumRepository::m_stamapDataLinkNetEnum;
	map<unsigned int, const char*> cEnumRepository::m_stamapErrorTypeEnum;
	map<unsigned int, const char*> cEnumRepository::m_stamapErrorLevelEnum;
	
	cEnumRepository::cEnumRepository()
	{
		populateEnumVars();
	}

	cEnumRepository& cEnumRepository::getInstance()
	{
		static cEnumRepository objInstance;
	    return objInstance;
	}

	string cEnumRepository::EnumToStr(const string& arstrEnumTypeName, int anEnumVal) throw (DLXML_Exception)
	{
		string strResult;
		
		map<string, ENMAP*>::const_iterator itrEnumName = m_stamapEnumHolder.find(arstrEnumTypeName);
		if (itrEnumName == m_stamapEnumHolder.end()) {
			DLXML_THROW_EX(DLXML_Exception, "Enum kind not found")
		}
		else {
			const map<unsigned int, const char*>& refMap = *itrEnumName->second;
			map<unsigned int, const char*>::const_iterator itrTemp = refMap.find(anEnumVal);
			if (itrTemp == refMap.end() || itrTemp->second == 0) {
				DLXML_THROW_EX(DLXML_Exception, "Enum Value not found/available")
			}
			else {			
				strResult.assign(itrTemp->second);
			}
		}
		return strResult;
	}
	
	int cEnumRepository::StrToEnum(const string& arstrEnumTypeName, const string& arstrEnumVal) throw (DLXML_Exception)
	{
		int nResult = 0;
		map<string, ENMAP*>::const_iterator itrEnumName = m_stamapEnumHolder.find(arstrEnumTypeName);
		if (itrEnumName == m_stamapEnumHolder.end()) {
			DLXML_THROW_EX(DLXML_Exception, "Enum kind not found")
		}
		else {
			const map<unsigned int, const char*>& refMap = *itrEnumName->second;
			map<unsigned int, const char*>::const_iterator itrTemp = refMap.begin();
			while (itrTemp != refMap.end()) {
				if (!strcmp(itrTemp->second, arstrEnumVal.c_str())) {
					break;
				}
				itrTemp++;
			}
			if (itrTemp == refMap.end()) {
				DLXML_THROW_EX(DLXML_Exception, "Enum Value not found")
			}
			nResult = itrTemp->first;
		}
		return nResult;
	}
	
	void cEnumRepository::populateEnumVars()
	{
		m_stamapEnumHolder.insert(make_pair(Constants::STR_ENUM_DLXML_REL_OP, &m_stamapRelOpEnum));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_LT, "LT"));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_LE, "LE"));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_EQ, "EQ"));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_NE, "NE"));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_GE, "GE"));
		m_stamapRelOpEnum.insert(map<unsigned int, const char*>::value_type(OPTR_GT, "GT"));
		
		m_stamapEnumHolder.insert(make_pair(Constants::STR_ENUM_DLXML_DATA_LINK_NET, &m_stamapDataLinkNetEnum));
		m_stamapDataLinkNetEnum.insert(map<unsigned int, const char*>::value_type(DLN_LINK_11, "LINK_11"));
		m_stamapDataLinkNetEnum.insert(map<unsigned int, const char*>::value_type(DLN_LINK_16, "LINK_16"));
		m_stamapDataLinkNetEnum.insert(map<unsigned int, const char*>::value_type(DLN_IJMS, "IJMS"));

		m_stamapEnumHolder.insert(make_pair(Constants::STR_ENUM_DLXML_ERROR_TYPE, &m_stamapErrorTypeEnum));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_XML_RULE_FILE, "ETP_XML_RULE_FILE"));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_EXP_GENERAL, "ETP_EXP_GENERAL"));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_DATA_NOT_EXIST, "ETP_DATA_NOT_EXIST"));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_DATA_INVALID, "ETP_DATA_INVALID"));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_DATA_UNDEFINED, "ETP_DATA_UNDEFINED"));
		m_stamapErrorTypeEnum.insert(map<unsigned int, const char*>::value_type(ETP_DATA_ILLEGAL, "ETP_DATA_ILLEGAL"));
		
		m_stamapEnumHolder.insert(make_pair(Constants::STR_ENUM_DLXML_ERROR_LEVEL, &m_stamapErrorLevelEnum));
		m_stamapErrorLevelEnum.insert(map<unsigned int, const char*>::value_type(ELV_SIMPLE, "ELV_SIMPLE"));
		m_stamapErrorLevelEnum.insert(map<unsigned int, const char*>::value_type(ELV_WARNING, "ELV_WARNING"));
		m_stamapErrorLevelEnum.insert(map<unsigned int, const char*>::value_type(ELV_ERROR, "ELV_ERROR"));
		m_stamapErrorLevelEnum.insert(map<unsigned int, const char*>::value_type(ELV_FATAL, "ELV_FATAL"));
		return;
	}
}
