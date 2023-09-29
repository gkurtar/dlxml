#ifndef DLXML_MESSAGEMANAGER_H
#define DLXML_MESSAGEMANAGER_H

#include "DLXML_CommonTypes.h"

XERCES_CPP_NAMESPACE_BEGIN
class XercesDOMParser;
class DOMDocument;
XERCES_CPP_NAMESPACE_END

XERCES_CPP_NAMESPACE_USE

namespace DLXML
{
	//Forward Declarations
	class cMessageEncoder;
	class cMessageDecoder;
	class cParserErrorHandler;
	class cErrorRepository;

	/**
	 * @class cMessageManager
	 * 
	 * @brief <p>This class defines the main interface of DLXML library. It implements the
	 * Meyer's singleton and also could be thought as it has imlemented kind of a 
	 * facade pattern. Its public interface consists of a virtual destructor,
	 * a singleton object returning method, a general symbol adder, and three other
	 * member class returning methods where these classes are of type cMessageEncoder,
	 * cMessageDecoder and cErrorRepository. Its private interface consists of
	 * initializer and controller methods where most of them is called during
	 * construction of this object. An exception is thrown if an error occurs
	 * during construction of this object.</p>
	 */
	class cMessageManager
	{
	public:
		/**
         * <p>Class destructor</p>
         */
		virtual ~cMessageManager(void);

		/**
		 * <p>This method implements Meyer's singleton pattern. Although there exists problems
		 * with these approach it saves the day and meet requirements of DLXML library.</p>
		 * 
		 * @return Reference to unique instance of cMessageManager class
		 */
		static cMessageManager& getInstance();
		
		/**
         * <p>Defines general parameters to be used throughly in the application.</p>
         *
         * @param arstrSymbolName : Symbol Name to define
         * @param arstrValue Value : Symbol Value of related symbol name
         * @return void
         */
        void addGeneralSymbol(const std::string& arstrSymbolName, const std::string& arstrValue);
		
        /**
         * <p>Returns Const reference to object pointed by m_pcErrorRepository member.</p>
         *
         * @return const reference to cErrorRepository object
         */
		const cErrorRepository& getErrorRepository();
		
		/**
		 * <p>Returns member pointer m_pcMessageEncoder of type cMessageEncoder*</p>
		 *
		 * @return member pointer m_pcMessageEncoder
		 */
		cMessageEncoder* getMessageEncoder();
		
		/**
		 * <p>Returns member pointer m_pcMessageDecoder of type cMessageDecoder*</p>
		 *
		 * @return member pointer m_pcMessageDecoder
		 */
		cMessageDecoder* getMessageDecoder();
		
	private:
		/**
         * <p>Default constructor</p>
         */
		cMessageManager(void);
		
		/**
		 * <p>
		 * Parses Configuration file, Data Link Rule Files and Vocabulary File.
		 * Configuration params are read from config file and Vocabularies used in Message
		 * encoding/decoding operation are read from vocabulary file. Also parsed Data Link
		 * Rule files are stored in "m_vecMessageSpecifications" member.
		 * </p>
		 *
		 * @return void
		 * @throw DLXML_Exception
		 */
		void parseXMLFiles() throw (DLXML_Exception);

		/**
         * <p>Checks whether "Order" Attributes of <Field> elements of parsed XML Rule files
         * are in order or not. Also checks whether no statement string values have the same
         * length with the reserved bit length of the field of which it is defined for.
         * Throws a DLXML_Exception whenever an inconsistency is encountered.</p>
         *
         * @return void
         */
		void checkRuleFilesFurther();

		/**
         * <p>Reads Message Identifier Information from XML Rule Files and constructs
         * the relevant data structures.</p>
         *
         * @return void
         */
		void getMessageIdentifiers();
		
		/**
         * <p>Private copy constructor, to prevent memberwise initialisation</p>
         */
		cMessageManager(const cMessageManager&);

		/**
		 * <p>Private assignment operator, to prevent memberwise assignment</p>
		 */
		cMessageManager& operator=(const cMessageManager&);

		/**
		 * <p>A member of type cErrorRepository*</p>
         */
        cErrorRepository* m_pcErrorRepository;
		
        /**
		 * <p>A member of type cMessageEncoder*</p>
         */
		cMessageEncoder* m_pcMessageEncoder;
		
		/**
		 * <p>A member of type cMessageDecoder*</p>
         */
		cMessageDecoder* m_pcMessageDecoder;

		/**
		 * <p>A member of type XercesDOMParser* which is used for parsing Data Link Rule Files</p>
		 */
		XercesDOMParser* m_pxdpDomParser;

		/**
		 * <p>A member of type std::vector<DOMDocument*> where each vector element
		 * represents a DOMDocument* pointing to a parsed Data Link Rule File.</p>
		 */
		std::vector<DOMDocument*> m_vecMessageSpecifications;
		
		/**
		 * <p>A member vector consisting of "DLXML_MessageIdentifierStructure" structures</p>
		 */
		DLXML_MessageIdentifierVector m_vecMessageIdentifiers; 
	};
}
#endif
