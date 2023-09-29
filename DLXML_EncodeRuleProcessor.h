#ifndef DLXML_ENCODERULEPROCESSOR_H
#define DLXML_ENCODERULEPROCESSOR_H

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
	 * @class cEncodeRuleProcessor
	 * 
	 * @brief <p>cEncodeRuleProcessor class provides static methods for encoding
	 * data link message fields. Public interface of this class contains one method named 
	 * 'applyEncodingRule' which in turn calls other private methods responsible for
	 * encoding of different message field types. These 'called' methods  operate upon
	 * different kinds of data link message fields and tries to encode a message field
	 * by suplied arguments. Besides these methods, there exist helper methods which are
	 * used during encoding process. Although these helper methods (checkField, getExpressions,
	 * getOperand, calculateOperation) exist with same name content and access level in
	 * cDecodeRuleProcessor class, their occurence is happened as a design decision.
	 * These helper methods are called during encoding to check various conditions
	 * such as detecting whether a message field is eligible to decode or not.</p>
	 **/
	class cEncodeRuleProcessor
	{
	public:
		/**
		 * <p>Encodes the input in the 'arstrInputValue' parameter and replaces the
		 * substring of "arstrBitMessage" parameter with the encoded message field value.
		 * This method is called for each field to be decoded by the relevant method
		 * in cMessageEncoder class. Depending on the type of Message Field, this method
		 * in turn calls related process(FieldType)encoding methods defined in the
		 * private interface of this class. The exceptions that are possibly thrown
		 * in the call-stack after this method is caught here and related error message
		 * is added to 'arcErrorRepository'.</p>
		 *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for
		 * the message field which is being processed
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant
		 * Bit is at the higest index of a message field or not
		 * @param arstrBitMessage : std::string reference that represents Message to be encoded
		 * @param arstrInputValue : Input Value to be processed
		 * @param arcMessageRepository : cMessageRepository object reference 
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @return whether Field is processed or not
		 */
		static bool applyEncodingRule(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
				bool abMSB_AtHighestIndex,\
				std::string& arstrBitMessage,\
				const std::string& arstrInputValue,\
				cMessageRepository& arcMessageRepository,\
				cErrorRepository& arcErrorRepository);

	private:
		/**
		 * <p>Encodes a 'Binary' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param arstrBitMessage : std::string reference that represents Message to be encoded
		 * @param abDataExists : Whether data exist or not
		 * @param arstrInputValue : Input Value to be processed 
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference 
		 * @return whether Field is processed or not
		 */
        static bool processBinaryEncoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		std::string& arstrBitMessage,\
        		bool abDataExists,\
        		const std::string& arstrInputValue,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository);

	    /**
         * <p>Encodes a 'Logical' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrBitMessage : std::string reference that represents Message to be encoded
		 * @param abDataExists : Whether data exist or not
		 * @param arstrInputValue : Input Value to be processed 
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference 
		 * @return whether Field is processed or not
         */
        static bool processLogicalEncoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		bool abMSB_AtHighestIndex,\
        		std::string& arstrBitMessage,\
        		bool abDataExists,\
        		const std::string& arstrInputValue,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository);

        /**
         * <p>Encodes a 'Character-Based' Message Field according to the definition
         * supplied with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         * 
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrBitMessage : std::string reference that represents Message to be encoded
		 * @param abDataExists : Whether data exist or not
		 * @param arstrInputValue : Input Value to be processed 
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference 
		 * @return whether Field is processed or not
         */
        static bool processCharStreamEncoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		bool abMSB_AtHighestIndex,\
        		std::string& arstrBitMessage,\
        		bool abDataExists,\
        		const std::string& arstrInputValue,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository);
        
        /**
         * <p>Encodes a 'Numeric' Message Field according to the definition supplied
         * with "apdndFieldNode" argument which represents a <Field> element
         * in the correspoding data link message rule file.</p>
         *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param abMSB_AtHighestIndex : Represents whether position of the Most Significant Bit is
		 * at the higest index of a message field or not
		 * @param arstrBitMessage : std::string reference that represents Message to be encoded
		 * @param abDataExists : Whether data exist or not
		 * @param arstrInputValue : Input Value to be encoded 
		 * @param arcMessageRepository : cMessageRepository object reference
		 * @param arcErrorRepository : cErrorRepository object reference
		 * @return whether Field is processed or not
         */
        static bool processNumericEncoding(const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,\
        		bool abMSB_AtHighestIndex,\
        		std::string& arstrBitMessage,\
        		bool abDataExists,\
        		const std::string& arstrInputValue,\
        		cMessageRepository& arcMessageRepository,\
        		cErrorRepository& arcErrorRepository);

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
         * The element written above defines a condition that is evaluated as a bool value
         * which decides whether the field of the message is to be processed or not.
         * Evaluation is simple: Find the operand values and eval relational expression
         * with these values. So this simply means, Whenever M_81_ENV_AIR and M_81_SI_ZERO
         * are both true than this <CaseCondition> evaluates to true.
         * For all other values of <Operand>s this logical expression evaluates to false.
         * </p>
		 *
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
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
         * Message Field Rules. For example the relational expression named 'DLRPLat_LT_60',
         * <RelationalExpression Name="DLRPLat_LT_60" LeftOperand="GS::DLRP_Latitude" 
         * Operator="LT" RightOperand="1.04719"/>
         * uses two operands: GS::DLRP_Latitude and a constant number 1.04719.
         * This method is responsible for returning the value of requested operand.</p>
         *
		 * @param arstrOperandName : Name of the Operand to search
         * @param arcMessageRepository : cMessageRepository object reference
         * @return Value of the operand
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
         * @return Value of the performed conditional operation.
		 */
		static bool performOperation(const XERCES_CPP_NAMESPACE::DOMNode* apdndBoolNode,\
				const std::map<std::string, bool>& armapEvaluatedRelExps);
	};
}
#endif
