#ifndef DLXML_DECODER_H
#define DLXML_DECODER_H

#include "DLXML_CommonTypes.h"

namespace DLXML
{
	//forward declarations
	class cErrorRepository;

	/**
	 * @class cDecoder
	 * 
	 * @brief <p>cDecoder class provides static methods for decoding Message Fields.
	 * For each different kind of data link message field there exists a method 
	 * with prefix 'decode' except for binary fields. These methods are called 
	 * from cDecodeRuleProcessor class and after some processing they call
	 * relevant methods from cDataLinkUtil class.</p>
	 **/
	class cDecoder
	{
	public:
		/**
		 * <p>Decodes encoded message field of a <Logical> element and returns the
		 * decoded value in string form.</p>
		 *
		 * @param arstrEncodedBitMessage : Encoded message field in binary string form
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents the
		 * <GenericValues> element of the field that is being decoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant
		 * Bit is at the higest index of a message field string or not
		 * @return decoded value
		 */
		static std::string decodeLogical(const std::string& arstrEncodedBitMessage,
        	const DLXML_RuleGenericStructure& arstGeneric, \
        	cErrorRepository& arcErrorRepository, \
        	bool abMSB_AtHighestIndex = true);

		/**
		 * <p>Decodes encoded message field of a <CharacterStream> element and returns
		 * the decoded value in string form.</p>
		 *
		 * @param arstrEncodedBitMessage : Encoded message field in binary string form
		 * @param arstStringSubparts : DLXML_RuleStringStructure object that represents
		 * the <String> element of the field that is being decoded.
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents the
		 * <GenericValues> element of the field that is being decoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit
		 * is at the higest index of a message field string or not
		 * @return decoded value
		 */
		static std::string decodeCharStream(const std::string& arstrEncodedBitMessage,\
			const DLXML_RuleStringStructure& arstStringSubparts,\
			const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex = true);
		
		/**
		 * <p>Decodes encoded message field of a <Numeric> element and returns the
		 * decoded value in string form.</p>
		 *
		 * @param arstrEncodedBitMessage : Encoded message field in binary string form
		 * @param arstInputMapping : DLXML_RuleInputMappingStructure object that represents
		 * the <InputMapping> element of the field that is being decoded.
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents the
		 * <GenericValues> element of the field that is being decoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit
		 * is at the higest index of a message field string or not
		 * @return decoded value
		 */
		static std::string decodeNumeric(const std::string& arstrEncodedBitMessage,\
			const DLXML_RuleInputMappingStructure& arstInputMapping,\
			const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex = true);

	private:
		/**
		 * <p>Checks whether the input is illegal/undefined or not.</p>
		 *
		 * @param alInput: input value to search
		 * @param arstInvalid : DLXML_InvalidStructure object that represents an
		 * <Illegal> or <Undefined> element.
		 * @return whether alInput is illegal/undefined or not
		 */
		static bool checkValue(long alInput, const DLXML_InvalidStructure& arstInvalid);
		
		/**
		 * <p>Decodes encoded substring according to a vocabulary, and returns it
		 * as a char value. Throws DLXML_Exception on errors and exceptional cases.</p>
		 *
		 * @param arstrEncodedSubstring: input value to search
		 * @param arstrVocabularyName : Name of the Vocabulary
		 * @return Decoded Form of 'arstrEncodedSubstring' according to vocabulary 'arstrVocabulary'
		 * @throw DLXML_Exception
		 */
		static char getDecodedForm(const std::string& arstrEncodedSubstring,\
				const std::string& arstrVocabularyName) throw (DLXML_Exception);
	};
}
#endif
