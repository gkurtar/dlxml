#ifndef DLXML_DECODERULEPROCESSOR_H
#define DLXML_DECODERULEPROCESSOR_H

#include <xercesc/util/XercesDefs.hpp>
#include <map>
#include <set>
#include <string>

XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
XERCES_CPP_NAMESPACE_END

namespace DLXML
{
	//Forward Declarations
	class cMessageRepository;
	class cErrorRepository;

	/**
	 * @class cDecodeRuleProcessor
	 * 
	 * @brief <p>cDecodeRuleProcessor class provides static methods for decoding encoded
	 * data link message fields. Public interface of this class contains one method named 
	 * 'applyDecodingRule' which in turn calls other private methods responsible for
	 * decoding of different message field types. These 'called' methods  operate upon
	 * different kinds of data link message fields and tries to decode the encoded
	 * string of a message field by supplied arguments. Besides these methods there exist
	 * helper methods which are used during decoding process. Although these helper
	 * methods exist with same name content and access level in cEncodeRuleProcessor class
	 * their occurrence is happened as a design decision. These helper methods are called
	 * during decoding to check various conditions such as detecting whether a message
	 * field is eligible to decode or not.
	 * </p>
	 **/
	class cDecodeRuleProcessor
	{
	public:
		/**
		 * <p>Decodes encoded message field in the 'arstrEncodedMessage' parameter and 
		 * assigns the result to a reference parameter named 'arstrDecodedField'.
		 * This method is called for each field to be decoded by the relevant method
		 * in cMessageDecoder class. Depending on the type of Message Field, this method
		 * in turn calls related process(FieldType)decoding methods defined in the private
		 * interface of this class. The exceptions that are possibly thrown in the 
		 * call-stack after this method is caught here and related error message is
		 * added to 'arcErrorRepository'.</p>
		 * 
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the
		 * message field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant
		 * Bit is at the higest index of a message field or not
		 * @param arstrEncodedMessage : std::string reference that represents Encoded Message
		 * @param arstrDecodedField : std::string reference that will store the Decoded Field.
		 * @return void
		 */
		static void applyDecodingRule(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
				cMessageRepository& arcMessageRepository,\
				cErrorRepository& arcErrorRepository,\
				bool abMSB_AtHighestIndex,\
				const std::string& arstrEncodedMessage,\
				std::string& arstrDecodedField);

	private:
        /**
         * <p>Decodes a 'Binary' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the
		 * message field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @param arstrEncodedMessage : std::string reference that represents encoded Message
		 * @param arstrDecodedField : std::string reference that will store the Decoded Field.
		 * @return void
		 */
        static void processBinaryDecoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository,\
        		const std::string& arstrEncodedMessage,\
        		std::string& arstrDecodedField);

        /**
         * <p>Decodes a 'Logical' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         * 
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrEncodedMessage : std::string reference that represents encoded Message
		 * @param arstrDecodedField : std::string reference that will store the Decoded Field.
		 * @return void
		 */
        static void processLogicalDecoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository,\
        		bool abMSB_AtHighestIndex,\
        		const std::string& arstrEncodedMessage,\
        		std::string& arstrDecodedField);

        /**
         * <p>Decodes a 'Character-Based' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrEncodedMessage : std::string reference that represents encoded Message
		 * @param arstrDecodedField : std::string reference that will store the Decoded Field.
		 * @return void
		 */
        static void processCharStreamDecoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository,\
        		bool abMSB_AtHighestIndex,\
        		const std::string& arstrEncodedMessage,\
        		std::string& arstrDecodedField);

        /**
         * <p>Decodes a 'Numeric' Message Field according to the definition supplied
         * with "apdndFieldNode" argument which represents a <Field> element
         * in the corresponding data link message rule file.</p>
         * 
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrEncodedBitMessage : std::string reference that represents encoded Message
		 * @param arstrDecodedField : std::string reference that will store the Decoded Field.
		 * @return void
		 */
        static void processNumericDecoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository,\
        		bool abMSB_AtHighestIndex,\
        		const std::string& arstrEncodedBitMessage,\
        		std::string& arstrDecodedField);
        
		/**
         * <p>This method checks whether the field that is being inspected is
         * allowed for processing or not. This decision is made after evaluating
         * various relational expressions. If there are no such expressions then
         * it simply returns true. There are two Condition types in the Data Link
         * Message Rule files, namely <SelectiveCondition> and <CaseCondition>. 
         * These provide a representation of Relational Expressions used in the
         * Data Link Message Rule Files.
         *  
         * A simple example may help clarifying this subject. Following is one of
         * the conditions defined in M.81 Message Rule File.
         * <CaseCondition> 
         * 	<And>
         * 		<Operand>M_81_ENV_AIR</Operand>
         * 		<Operand>M_81_SI_ZERO</Operand>
         * 	</And>
         * </CaseCondition>
         * The element written above defines a condition for a message field of M.81.
         * Evaluation is simple: Find the operand values and eval relational expression
         * with these values. So this simply means, Whenever M_81_ENV_AIR and M_81_SI_ZERO
         * are both true than this <CaseCondition> evaluates to true.
         * For all other values of <Operand>s this logical expression evaluates to false.
         * </p>
		 *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the
		 * message field which is being processed
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param abIsSelectiveCondition : Determines the Condition Element Type that will be checked
         * @return whether Field should be processed or not
		 */
		static bool checkField(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
				cMessageRepository& arcMessageRepository,\
				bool abIsSelectiveCondition = true);

		/**
		 * <p>Evaluates relational expressions supplied by the argument arsetExpressionNames
		 * and returns relational expression name and evaluated value pairs in a 
		 * std::map<std::string, bool> object.</p>
		 *
		 * @param apdndRelationalExpressionsNode : DOMNode* representing relational expressions
		 * @param arsetExpressionNames : Relational expressions to be evaluated
         * @param arcMessageRepository : cMessageRepository object reference
         * @return Evaluated expression name value pairs
		 */
		static std::map<std::string, bool> evalRelationalExpressions(\
				const XERCES_CPP_NAMESPACE::DOMNode* apdndRelationalExpressionsNode,\
				const std::set<std::string>& arsetExpressionNames,\
				cMessageRepository& arcMessageRepository);

		/**
         * <p>Returns the value of the 'Operands' used in 'RelationalExpressions' of Link
         * Message Field Rules. For example the RelationalExpression named 'DLRPLat_LT_60',
         * <RelationalExpression Name="DLRPLat_LT_60" LeftOperand="GS::DLRP_Latitude"
         * Operator="LT" RightOperand="1.04719"/>
         * uses two operands: GS::DLRP_Latitude and a constant number 1.04719.
         * This method is responsible for returning the value of requested operand.</p>
         *
		 * @param arstrOperandName : Name of the Operand to be searched
         * @param arcMessageRepository : cMessageRepository object reference
         * @return value of the operand
         */
        static double getOperand(const std::string& arstrOperandName,\
        		cMessageRepository& arcMessageRepository);
        
        /**
         * <p>Performs a conditional operation defined by 'apdndBoolNode' which is an
		 * XML Element consisting of "And" and "Or" xml-elements and relevant operands.
		 * The values of conditional operands are supplied by 'armapEvaluatedRelExps'
		 * argument which is a std::map containing <Conditional Operand, Value> pairs.</p>
		 *
		 * @param apdndBoolNode : DOMNode* representing a Bool expression node in xml rule file
         * @param armapEvaluatedRelExps : <string, bool> pairs where each pair defines a
         * Relational Expression and its value
         * @return value of the performed conditional operation.
		 */
		static bool performOperation(const XERCES_CPP_NAMESPACE::DOMNode* apdndBoolNode,\
				const std::map<std::string, bool>& armapEvaluatedRelExps);
	};
}
#endif
