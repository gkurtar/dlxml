#ifndef DLXML_VOCABULARYREPOSITORY_H
#define DLXML_VOCABULARYREPOSITORY_H

#include "DLXML_CommonTypes.h"

namespace DLXML
{
	/**
	 * @class cVocabularyRepository
	 * 
	 * @brief <p>cVocabularyRepository class acts as a container for "Vocabularies".
     * These "Vocabularies" represent some mappings for alphanumeric characters
     * used throughout Data Link Message field definitions. So a typical Vocabulary
     * enrty is like use "00100" for "A". This class provides methods for both adding
     * vocabulary definitions and for retrieving them.</p>
     */
	class cVocabularyRepository
	{
		friend class cMessageManager;
	public:		
		/**
         * <p>Class destructor</p>
         */
		virtual ~cVocabularyRepository(void);

        /**
         * <p>Given vocabulary name and entry key name this method look up this entry and
         * if it finds this entry key in the key value pairs of the vocabulary named
         * "arstrVocabularyName" it simply returns value of the found key value pair,
         * if it does not find the entry in the specified vocabulary an exception is thrown.</p>
         * 
         * @param arstrVocabularyName : Name of the Vocabulary structure.
         * @param achEntryKey : Key to be searched in the vocabulary.
         * @return Entry value for the searched key.
         * @throw DLXML_Exception
         */
        static std::string lookupEntryKey(const std::string& arstrVocabularyName,
        		char achEntryKey) throw (DLXML_Exception);

        /**
         * <p>Given vocabulary and entry value name this method look up this entry and
         * if it finds a matching entry value in the vocabulary it simply returns 
         * correspoding key, if it does not find entry value among the values in the
         * specified vocabulary an exception is thrown.
         * Searched map has entries where each key value pair is unique, i.e. had we
         * defined a function from keys to values than it would be a one to one function.
         * So this means searching for a value in the key value pairs of the vocabulary,
         * would eventually result in one key or none.</p>
         * 
         * @param arstrVocabularyName : Name of the Vocabulary structure.
         * @param arstrEntryValue : Value to be searched in the vocabulary.
         * @return Entry Key for the searched Entry Value.
         * @throw DLXML_Exception
         */
        static char lookupEntryValue(const std::string& arstrVocabularyName,
        		const std::string& arstrEntryValue) throw (DLXML_Exception);

        /**
         * simply returns contents used for test purposes
         */
        static std::string contents();
  
	private:
		/**
         * <p>Default constructor</p>
         */
		cVocabularyRepository(void);
		
		/**
		 * <p>Private copy constructor, to prevent memberwise initialization</p>
		 */
		cVocabularyRepository(const cVocabularyRepository&);

		/**
		 * <p>Private assignment operator, to prevent memberwise assignment</p>
		 */
		cVocabularyRepository& operator=(const cVocabularyRepository&);
		
		/**
		 * <p>Adds a DLXML_Vocabulary structure into the member "m_vecVocabularyVector"</p>
		 *
         * @param arstrVocName :  Name of the Vocabulary
         * @param armapVocabulary : A dictionary of type std::map<char, char*>
         * @param aeErrorToReport : DLXML_ErrorType enumeration
         * @return void
         */
        static void addVocabulary(const std::string& arstrVocName,\
        		const std::map<char, char*>& armapVocabulary,\
        		DLXML_ErrorType aeErrorToReport);

        /**
		 * <p>Adds a DLXML_Vocabulary structure into the member "m_vecVocabularyVector"</p>
		 *
         * @param arstVocabulary :  DLXML_Vocabulary structure to be added
         * @return void
         */
        static void addVocabulary(const DLXML_Vocabulary& arstVocabulary);
		
		/**
         * <p>A std::vector which acts as a container for DLXML_Vocaulary Structures.</p>
         */
		static DLXML_VocabularyVector m_vecVocabularyVector;
	};
}
#endif
