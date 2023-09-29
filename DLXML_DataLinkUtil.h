#ifndef DLXML_DATALINKUTIL_H
#define DLXML_DATALINKUTIL_H

#include "DLXML_CommonTypes.h"

XERCES_CPP_NAMESPACE_BEGIN
class DOMNode;
class DOMElement;
XERCES_CPP_NAMESPACE_END

using XERCES_CPP_NAMESPACE::DOMNode;
using XERCES_CPP_NAMESPACE::DOMElement;

namespace DLXML
{
	/**
	 * @class cDataLinkUtil
	 * 
	 * @brief <p>cDataLinkUtil class provides static utility methods to other classes 
	 * in DLXML namespace. It has various kinds of methods ranging from simple 
	 * converting methods -from binary representation to decimal and vice versa- 
	 * to DLXML specific ones where "evalBoolExpression" may be a good example 
	 * for this kind, and others which are mostly related with XML Document 
	 * processing.</p>
	 **/
	class cDataLinkUtil
	{
	public:
		/**
		 * <p>This method converts a decimal value to its binary representation. 
		 * For ex, arguments (23, 5, true) would mean "represent decimal value 23 in 5 bits with
		 * most significant bit at the HighestIndex" and this method would return "10111" as result</p>
		 *
		 * @param alValue : Input Value that will be represented as a binary number which will
		 * be returned in the string form.
		 * @param asNumberOfBits : Number of chars that will be used in the string
		 * representation of input value.
		 * @param abMSB_AtHighestIndex : Determines orientation of encoding, default
		 * value is 'true' which represents
         * right oriented output(Most Significant Bit is at the highest index of the resultant string).
         * @return input value converted to binary string.
         */
        static std::string convertDecimalToBinaryString(long alValue, \
        		short asNumberOfBits, bool abMSB_AtHighestIndex = true);

        /**
         * <p>Converts a binary number represented by a std::string to its
         * equivalent base 10's representation.</p>
         *
         * @param arstrBinaryString : A string consisting of 0's and 1's that represent
         * an encoded data link message field.
         * @param bool abMSB_AtHighestIndex : Determines position of orientation of
         * Most Significant Bit and has a default
         * value. when it is 'true' that would mean input is right oriented
         * (Most Significant Bit is at the highest index of the input string).
         * 
         * @return Decimal value of input string.
         */
        static long convertBinaryStringToDecimal(const std::string& arstrBinaryString,\
        		bool abMSB_AtHighestIndex = true);

		/**
         * <p>Evaluates boolean expression represented by input paramaters (Left Operand
         * Right Operand, Expression Operator(LT, GT, EQ, vs..)), and returns the result.
         * Throws DLXML_Exception on errors and exceptional cases.</p>
         *
		 * @param aenmExpressionOperator : An Enum which represents boolean
		 * operators, ("LT, GT, EQ, NE, LE, GE")
		 * @param adLeftOperand : Left Operand of expression to be evaluated
		 * @param adRightOperand : Right Operand of expression to be evaluated
		 * @return result of evaluation of boolean expression
		 * @throw DLXML_Exception 
		 */
        static bool evalBoolExpression(DLXML_RelationalOperator aenmExpressionOperator,\
        		double adLeftOperand, double adRightOperand) throw (DLXML_Exception);

		/**
		 * <p>Returns whether element named "arstrElementName" is contained
		 * within the DOMNode "apdndSearchNode"</p>
		 *
         * @param apdndSearchNode : DOMNode to search in 
         * @param arstrElementName : ElementName to search for
         * @return whether element named "arstrElementName" exists within "apdndSearchNode"
         * @throw DLXML_Exception
         */
		static bool hasElementNamed(const DOMNode* apdndSearchNode,\
				const std::string& arstrElementName) throw (DLXML_Exception);

		/**
		 * <p>Returns the first element of the DOMNodeList which is the result of calling
		 * getElementsByTagName with parameter arstrElementName. Throws DLXML_Exception on
		 * errors and exceptional cases.</p>
		 *
         * @param apdndSearchNode : DOMNode to search in 
         * @param arstrElementName : ElementName to search for
         * @return pointer to first element in a depth first traversal of elements
         * @throw DLXML_Exception
         */
		static DOMElement* getElementByName(const DOMNode* apdndSearchNode,\
				const std::string& arstrElementName) throw (DLXML_Exception);

		/**
		 * <p>Returns the content of an XML Element. Throws DLXML_Exception
		 * on errors and exceptional cases.</p>
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @return Element Content
         * @throw DLXML_Exception
         */
		static std::string getElementContent(const DOMNode* apdndSearchNode) throw (DLXML_Exception);
		
		/**
		 * <p>Returns the first Child of apdndSearchNode whose NodeName is same with arstrElementName.</p>
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @param arstrElementName : ElementName to search for
         * @return DOMNode pointer whose node name is equal to arstrElementName
         */
		static DOMNode* getChildNodeByName(const DOMNode* apdndSearchNode,\
				const std::string& arstrElementName);

		/**
		 * <p>Returns the encoding/decoding rule defining element (one of the numeric, binary,
		 * logical, charstream inside the "<Field>" element represented by "apdndSearchNode".
		 * Throws DLXML_Exception on errors and exceptional cases.</p>
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @return DOMNode pointer
         * @throw DLXML_Exception
         */
		static DOMNode* getFieldChildNode(const DOMNode* apdndSearchNode) throw (DLXML_Exception);
		
		/**
		 * <p>Returns whether "apdndSearchNode" has any attribute named "apchAttributeName" </p>
		 *
         * @param apdndSearchNode : DOMNode to search in 
         * @param apchAttributeName : Attribute Name to search for
         * @return whether Searched DOMNode has specified attribute or not.
         */
		static bool hasAttributeNamed(const DOMNode* apdndSearchNode, const char* apchAttributeName);

		/**
		 * <p>Returns the content of the attribute named "apchAttributeName" in "apdndSearchNode".
		 * Throws DLXML_Exception on errors and exceptional cases.</p>
		 *
		 * @param apdndSearchNode : DOMNode to search in
		 * @param apchAttributeName : Attribute Name to search for
		 * @return Attribute Content
		 * @throw DLXML_Exception
		 */
		static std::string getAttributeValue(const DOMNode* apdndSearchNode,\
				const char* apchAttributeName) throw (DLXML_Exception);
		
		/**
		 *<p>Reads in the XML element named "Generic" and constructs a similar structure
		 * DLXML_RuleGenericStructure(c++ struct) and returns it.</p> 
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @return DLXML_RuleGenericStructure object populated by this method
         */
		static DLXML_RuleGenericStructure getGenericStructure(const DOMNode* apdndSearchNode);

		/**
		 * <p>Reads in the XML element named "String" and constructs a similar structure
		 * DLXML_RuleStringStructure(c++ struct) and returns it</p>
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @return DLXML_RuleStringStructure object populated by this method
         */
		static DLXML_RuleStringStructure getStringStructure(const DOMNode* apdndSearchNode);

		/**
		 * 
		 * <p>Reads in the XML element named "InputMapping" and constructs a similar structure
		 * DLXML_RuleInputMappingStructure(c++ struct) and returns it </p>
		 *
         * @param apdndSearchNode : DOMNode to search in
         * @return DLXML_RuleInputMappingStructure object populated by this method
         */
		static DLXML_RuleInputMappingStructure getInputMappingStructure(const DOMNode* apdndSearchNode);
	};
}
#endif
