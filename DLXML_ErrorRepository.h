#ifndef DLXML_ERRORREPOSITORY_H
#define DLXML_ERRORREPOSITORY_H

#include "DLXML_CommonTypes.h"
#include <iosfwd>

namespace DLXML
{
	/**
	 * @class cErrorRepository
	 * 
	 * @brief <p>cErrorRepository class provides methods for storing errors, warnings,
	 * conditions etc. occurred during DLXML library operations. Errors are stored in a
	 * std::vector member which contains DLXML_ErrorStructures. Also an outputstream
	 * operator('operator<<') is defined in this class for generating output stream of
	 * contained errors.</p>
	 **/
	class cErrorRepository
	{
		/**
         * <p>Overloaded operator for this class to perform ostream operations. This method
         * clears member error vector after it has finished generating requested stream.</p>
         */
		friend std::ostream& operator<<(std::ostream& arostStream, \
				const cErrorRepository& arcErrorRepository);
	public:
		/**
         * <p>Default constructor</p>
         */
		cErrorRepository(void);
		
		/**
         * <p>Class destructor</p>
         */
		virtual ~cErrorRepository(void);

        /**
		 * <p>Adds an error structure into the member "m_vecErrors".</p>
		 *
         * @param aeErrorType : DLXML_ErrorType enumeration 
         * @param aeErrorLevel : DLXML_ErrorLevel enumeration
         * @param arstrErrorMessage : Detail of Error/Warning/Condition
         * @return void
         */
        void addError(DLXML_ErrorType aeErrorType, DLXML_ErrorLevel aeErrorLevel,\
        		const std::string& arstrErrorMessage);

		/**
         * <p>Inserts error detail (Message and Field names) to the last error that has
         * been reported.</p>
         *
		 * @param arstrMessageName : Name of Message which has last been processed
		 * @param arstrFieldIdentifer : DFI_DUI of the Message Field which has last been processed
         * @return void
         */
        void addErrorDetail(const std::string& arstrMessageName,\
        		const std::string& arstrFieldIdentifer);

		/**
		 * <p>Clears the data contained in error repository</p>
		 * 
		 * @return void
		 */
		void clear();
		
	private:
		/**
		 * <p>Private copy constructor, to prevent memberwise initialization</p>
		 */
		cErrorRepository(const cErrorRepository&);

		/**
		 * <p>Private assignment operator, to prevent memberwise assignment</p>
		 */
		cErrorRepository& operator=(const cErrorRepository&);

		/**
         * <p>A std::vector which contains DLXML_ErrorStructures.</p>
         */
        DLXML_ErrorVector m_vecErrors;

        /**
		 * <p>An index variable used for indexing m_vecErrors.</p>
		 */
        size_t m_sztVectorIndex;

        /**
         * <p>A bool variable which represents whether any undetailed error data is
         * arrived.</p>
         */
        bool m_bUndetailedErrorHasBeenAdded;
	};
}
#endif
