#ifndef DLXML_ENCODER_H
#define DLXML_ENCODER_H

#include "DLXML_CommonTypes.h"

namespace DLXML
{
	//forward declarations
	class cErrorRepository;

	/**
	 * @class cEncoder
	 * 
	 * @brief <p>cEncoder class provides static methods for encoding Message Fields.
	 * For each different kind of data link message field there exists a method
	 * with prefix 'encode' except for binary fields in the public interface of
	 * this class. During encoding process these methods are called by 
	 * cEncodeRuleProcessor class and after some processing they call related
	 * methods from cDataLinkUtil class. Besides these methods there exist two
	 * helper methods in the private interface of this class which are called
	 * by the encode-prefixed methods.</p>
	 **/
	class cEncoder
	{
	public:
		/**
		 * <p>Encodes alInputValue according to <Logical> message field encoding rule and 
		 * returns the encoded value in binary string form.</p>
		 *
		 * @param alInputValue : input number to encode
		 * @param asNumberOfBits : number of bits in the related field
		 * @param abDataExists : whether data exist or not
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents
		 * the <GenericValues> element of the field that is being encoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the most
		 * significant bit is at the higest index of a message field or not
		 * @return Encoded data link message field.
		 */
        static std::string encodeLogical(long alInputValue,\
        		short asNumberOfBits,\
        		bool abDataExists,\
        		const DLXML_RuleGenericStructure& arstGeneric,\
        		cErrorRepository& arcErrorRepository,\
        		bool abMSB_AtHighestIndex);

        /**
		 * <p>Encodes arstrInputValue according to <CharacterStream> message field encoding rule and 
		 * returns the encoded value in binary string form.</p>
		 *
		 * @param arstrInputValue : input stream to encode
		 * @param asNumberOfBits : number of bits in the related field
		 * @param abDataExists : whether data exist or not
		 * @param arstStringSubparts : DLXML_RuleStringStructure object that represents the <String>
		 * element of the field that is being decoded.
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents the <GenericValues>
		 * element of the field that is being encoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @return Encoded data link message field.
		 */
        static std::string encodeCharStream(const std::string& arstrInputValue,\
        	short asNumberOfBits,\
        	bool abDataExists,\
        	const DLXML_RuleStringStructure& arstStringSubparts,\
			const DLXML_RuleGenericStructure& arstGeneric,\
			cErrorRepository& arcErrorRepository,\
			bool abMSB_AtHighestIndex);

        /**
		 * <p>Encodes adInputValue according to <Numeric> message field encoding rule and 
		 * returns the encoded value in binary string form.</p>
		 *
		 * @param adInputValue : input value to encode
		 * @param asNumberOfBits : number of bits in the related field
		 * @param abDataExists : whether data exist or not
		 * @param arstInputMapping : DLXML_RuleInputMappingStructure object that represents the
		 * <InputMapping> element of the field that is being encoded.
		 * @param arstGeneric : DLXML_RuleGenericStructure object that represents the <GenericValues>
		 * element of the field that is being encoded.
		 * @param arcErrorRepository : cErrorRepository class reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit
		 * is at the higest index of a message field or not
		 * @return Encoded value
		 */
        static std::string encodeNumeric(double adInputValue,\
        		short asNumberOfBits,\
        		bool abDataExists,\
        		const DLXML_RuleInputMappingStructure& arstInputMapping,\
        		const DLXML_RuleGenericStructure& arstGeneric,\
        		cErrorRepository& arcErrorRepository,\
        		bool abMSB_AtHighestIndex);

	private:
		/**
		 * <p>Encodes achInput according to a vocabulary, and returns it via a reference
		 * paramater. Return value is a boolean representing the success of the operation</p>
		 *
		 * @param achInput: input character
		 * @param arstrVocabulary : Vocabulary Name
		 * @param arstrEncodedSubstring : encoded form of achInput
		 * @return whether op is success or not
		 */
		static bool getEncodedForm(char achInput, const std::string& arstrVocabulary,\
				std::string& arstrEncodedSubstring);

		/**
		 * <p>Checks whether the input is illegal/undefined or not.</p>
		 *
		 * @param alInput: input value to search
		 * @param arstInvalid : A structure representing the <Illegal> or <Undefined>
		 * element of the relevant field
		 * @return whether alInput is illegal/undefined or not
		 */
		static bool checkInputValue(long alInput, const DLXML_InvalidStructure& arstInvalid);
	};
}
#endif
