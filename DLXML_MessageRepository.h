#ifndef DLXML_MESSAGEREPOSITORY_H
#define DLXML_MESSAGEREPOSITORY_H

#include <string>
#include <map>
#include <iosfwd>
#include "DLXML_CommonTypes.h"

namespace DLXML
{
	/**
	 * @class cMessageRepository
	 * 
	 * @brief <p>cMessageRepository class provides methods which are used during encoding 
	 * and decoding process. Data link message field values generated during
	 * encoding and decoding operations and general symbols needed are stored
	 * in this class and its public interface consists of methods to add and query
	 * these values. Also an outputstream operator('operator<<') is defined in this
	 * class for generating output stream of contained values and symbols.
	 * 
	 * Encoding/Decoding of a data link message field sometimes
	 * may need the value of previosly encoded/decoded value of a message field.
	 * This class is emerged from this need. During encoding/decoding process
	 * values of data link message fields are stored in this class so they
	 * are available if needed for other data link message field processing.
	 * </p>
	 **/

    class cMessageRepository
    {
    	/**
         * <p>Overloaded operator for this class to perform ostream operations.</p>
         */
    	friend std::ostream& operator<<(std::ostream&, const cMessageRepository&);
	public:
        /**
         * <p>Constructor</p>
         * 
         * @param arvecMessageIdentifiers : DLXML_MessageIdentifierVector reference 
         */
        cMessageRepository(const DLXML_MessageIdentifierVector& arvecMessageIdentifiers);

		/**
         * <p>Class destructor</p>
         */
        virtual ~cMessageRepository(void);

        /**
         * <p>Adds parameters into the member map "m_mapMessageFieldValues".</p>
         *
         * @param arstrMessageName : Data Link Message Name.
         * @param arstrFieldDFI_DUI : DFI_DUI of related field (Ex: 2500_003)
         * @param arstrFieldValue : Value of related field
         *
         * @return void
         */
        void addMessageFieldValue(const std::string& arstrMessageName,\
        		const std::string& arstrFieldDFI_DUI,\
        		const std::string& arstrFieldValue);

		/**
         * <p>Searchs for the "arstrDFI_DUI" parameter in the member "m_mapMessageFieldValues"
         * and returns its value if it is stored in the "m_mapMessageFieldValues",
         * otherwise it returns "CONST_STR_UNDEFINED".</p>
         *
         * @param arstrDFI_DUI : DFI_DUI to be searched
         * 
         * @return value of key value pair where key is "arstrDFI_DUI" in mapMessageFieldValues.
         */
        std::string getMessageFieldValue(const std::string& arstrDFI_DUI);

		/**
		 * <p>Adds parameters into the member map "m_mapGeneralSymbols".</p>
         *
         * @param arstrSymbolName : Symbol Name to insert as a key into std::map "m_mapMessageFieldValues"
         * @param arstrValue : Symbol Value to insert as a value into std::map "m_mapMessageFieldValues"
         *
         * @return void
         */
        void addGeneralSymbol(const std::string& arstrSymbolName, const std::string& arstrValue);

		/**
		 * <p>Searchs for the "arstrSymbolName" parameter in the member "m_mapGeneralSymbols" and 
         * returns its value if it is stored in the "m_mapGeneralSymbols", otherwise it 
         * returns "CONST_STR_UNDEFINED"</p>
		 *
         * @param arstrSymbolName : SymbolName to look up in "m_mapGeneralSymbols"
         * 
         * @return value of key value pair where key is "arstrSymbolName" in m_mapGeneralSymbols.
         */
        std::string getGeneralSymbol(const std::string& arstrSymbolName);

		/**
		 * <p>Clears the data contained in message repository.</p>
		 * 
		 * @return void
		 */
		void clearMessageFieldValues();
		
		/**
		 * <p>Searchs DLXML_MessageIdentifierStructure vector to find a matching
		 * element regarding DLXML_DataLinkNetwork and Message Name. We assume these
		 * two members of the data type DLXML_MessageIdentifierStructure defines a
		 * 'primary key'. If search operation has resulted in success then this method
		 * returns the address of DLXML_MessageIdentifierStructure else it returns null.</p>
		 * 
		 * @param aeMessageNetwork : MessageNetwork of element being searched.
		 * @param arstrMessageName : MessageName of element being searched.
		 *
		 * @return address of the matching element
		 */
		const DLXML_MessageIdentifierStructure* getIdentifierOfMessageName(\
				DLXML_DataLinkNetwork aeMessageNetwork,\
				const std::string& arstrMessageName);
		
		/**
		 * <p>Searchs DLXML_MessageIdentifierStructure vector to find a matching element
		 * which have the same Message Network and matching Message Template with Encoded
		 * Message provided as arguments. If search operation has resulted in success then
		 * this method returns the address of DLXML_MessageIdentifierStructure else it
		 * returns null.</p>
		 * 
		 * @param aeMessageNetwork : Message Network of element being searched.
		 * @param arstrEncodedMessage : Encoded Message which will be compared to Message Template
		 *
		 * @return address of the matching element
		 */
		const DLXML_MessageIdentifierStructure* getIdentifierOfEncodedMessage(\
				DLXML_DataLinkNetwork aeMessageNetwork,\
				const std::string& arstrEncodedMessage);
		
	private:
		/**
         * <p>Private copy constructor, to prevent memberwise initialization</p>
         */
		cMessageRepository(const cMessageRepository&);
		
		/**
		 * <p>Private assignment operator, to prevent memberwise assignment</p>
		 */
		cMessageRepository& operator=(const cMessageRepository&);

		/**
		 * <p>A member of type std::map<std::string, std::string> which is used as
		 * a dictionary for General Symbols.</p>
		 */
		std::map<std::string, std::string> m_mapGeneralSymbols;

		/**
		 * <p>A member of type std::map<std::string, std::string> which is used as
		 * a dictionary for Encoded Message Field Values.</p>
		 */
        std::map<std::string, std::string> m_mapMessageFieldValues;
        
        /**
	     * <p>A std::vector which stores "DLXML_MessageIdentifierStructure"s.</p>
	     */
		const DLXML_MessageIdentifierVector& m_rvecMessageIdentifiers;
	};
}
#endif
