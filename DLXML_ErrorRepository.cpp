#include "DLXML_ErrorRepository.h"
#include "DLXML_EnumRepository.h"
#include "DLXML_Constants.h"
#include <iomanip>

using std::string;
using std::vector;
using std::ostream;

namespace DLXML
{
	ostream& operator<<(ostream& arostStream, const cErrorRepository& arcErrorRepository)
	{
		if (arcErrorRepository.m_vecErrors.size()) {
			arostStream << Constants::STR_STARS << "\nError List:\n";
			for (size_t j = 0; j < arcErrorRepository.m_vecErrors.size(); ++j) {
				//not very effective
				arostStream << "\tError " << std::setw(3) << j \
					<< ". Type ='" \
					<< cEnumRepository::getInstance().EnumToStr(Constants::STR_ENUM_DLXML_ERROR_TYPE,\
							arcErrorRepository.m_vecErrors[j].enmErrorType) \
					<< "'\tLevel = '" \
					<< cEnumRepository::getInstance().EnumToStr(Constants::STR_ENUM_DLXML_ERROR_LEVEL,\
							arcErrorRepository.m_vecErrors[j].enmErrorLevel) \
					<< "'\n\t\tMessage = " << arcErrorRepository.m_vecErrors[j].strErrorMessage.c_str() \
					<< "\n";				
				/*
				arostStream << "\tError " << std::setw(3) << j \
				<< ". Type ='" << std::setw(3) <<  arcErrorRepository.m_vecErrors[j].enmErrorType \
				<< "'\tLevel = '" << std::setw(3) << arcErrorRepository.m_vecErrors[j].enmErrorLevel \
				<< "'\n\t\tMessage = " << arcErrorRepository.m_vecErrors[j].strErrorMessage.c_str() << std::endl;
				*/
			}
			arostStream << Constants::STR_STARS << std::endl;
			
			//now vector could be cleared, we do not have to store old errors
			cErrorRepository& rcErrors = const_cast<cErrorRepository&>(arcErrorRepository);
			rcErrors.clear();
		}//end of if
		return arostStream;
	}

	cErrorRepository::cErrorRepository(void) : m_sztVectorIndex(0), m_bUndetailedErrorHasBeenAdded(false)
	{
	}

	cErrorRepository::~cErrorRepository(void)
	{
	}
	
	void cErrorRepository::addError(DLXML_ErrorType aeErrorType,
			DLXML_ErrorLevel aeErrorLevel, const string& arstrErrorMessage)
	{
		DLXML_ErrorStructure stError;
		stError.enmErrorType = aeErrorType;
		stError.enmErrorLevel = aeErrorLevel;
		stError.strErrorMessage = arstrErrorMessage;
		m_vecErrors.push_back(stError);
		return;
	}

	//Normally we would not need this detail, but probably because of
	//the poor design, this method has been added.
    void cErrorRepository::addErrorDetail(const std::string& arstrMessageName,\
    		const std::string& arstrFieldIdentifer)
    {
    	if (m_vecErrors.empty()) { //size should be greater than zero
    		return;
    	}
    	
    	while (m_sztVectorIndex < m_vecErrors.size()) {
    		DLXML_ErrorStructure& stError = m_vecErrors[m_sztVectorIndex++];
    		if (stError.enmErrorType != ETP_XML_RULE_FILE) {
    			stError.strErrorMessage.insert(0,\
    					arstrMessageName + " - " + arstrFieldIdentifer + ", ");
    		}
    	}
    	return;
    }

	void cErrorRepository::clear()
	{
		if (m_vecErrors.size()) {
			m_vecErrors.clear();
			m_sztVectorIndex = 0;
			//a way to be sure of full deletion is below, but it is not included for performance reasons
			//vector<DLXML_ErrorStructure>().swap(m_vecErrors);
		}
		return;
	}
}
