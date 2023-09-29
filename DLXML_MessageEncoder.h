#ifndef DLXML_MESSAGEENCODER_H
#define DLXML_MESSAGEENCODER_H

#include "DLXML_ErrorRepository.h"
#include "DLXML_MessageRepository.h"

#include <string>
#include <vector>

XERCES_CPP_NAMESPACE_BEGIN
class DOMDocument;
class DOMNode;
class XercesDOMParser;
XERCES_CPP_NAMESPACE_END

namespace DLXML {

	//Forward Declarations
	class cParserErrorHandler;

	/**
	 * @class cMessageEncoder
	 * 
	 * @brief <p>cMessageEncoder class defines the main interface for encoding data link messages.
	 * It provides a method for encoding data link messages in its public interface. Other
	 * methods in the public interface simply returns references to member classes
	 * cMessageRepository and cErrorRepository. Its private interface consists of helper
	 * and controller methods used by the public interface. Encoding process for a message
	 * is started and controlled from this class.</p>
	 */
	class cMessageEncoder
	{
		friend class cMessageManager;
	public:
		/**
		 * <p>Encodes a number of data link messages and returns encoded messages where
		 * each string of the result vector represents an encoded data link message in
		 * the string form consisting of '0's and '1's. Throws DLXML_Exception on errors
		 * and exceptional cases.</p></p>
		 * 
		 * @param arvecXMLDataStream : std::vector that contains Input data streams in xml
		 * @return encoded messages
		 * @throw DLXML_Exception
		 */
		std::vector<std::string> encodeMessages(\
				const std::vector<std::string>& arvecXMLDataStreams) throw (DLXML_Exception);

		/**
         * <p>Returns a reference to member object m_objMessageRepository.</p>
         *
         * @return Reference to member object named "m_objMessageRepository"
         */
        cMessageRepository& getMessageRepository();

        /**
         * <p>Returns a reference to member object m_objErrorRepository.</p>
         *
         * @return Reference to member object named "m_objErrorRepository"
         */
        cErrorRepository& getErrorRepository();

	private:
		/**
         * <p>Constructor</p>
         * 
         * @param arvecMessageIdentifiers : DLXML_MessageIdentifierVector reference 
         */
		cMessageEncoder(const DLXML_MessageIdentifierVector& arvecMessageIdentifiers);

		/**
         * <p>Class destructor</p>
         */
		virtual ~cMessageEncoder(void);
		
		/**
		 * <p>Encodes the data link message which is passed as a stream reference to this
		 * member function, Returns encoded string which is consisting of 0's and 1's.
		 * Throws DLXML_Exception on errors and exceptional cases.</p>
		 *
         * @param apddocInputStream : Pointer to a DOMDocument which stores parsed XML Stream
         * representing link message field data
         * @return encoded data link message
         * @throw DLXML_Exception
         */
        std::string encodeMessage(const XERCES_CPP_NAMESPACE::DOMDocument* apddocInputStream)\
        		throw (DLXML_Exception);

        /**
         * <p>Parses XML Data Streams via XercesDomParser. Stores DomDocument pointers of parsed
         * input data streams in the member vector 'm_vecInputStreamDomDocuments'. Throws exception
         * if any error occurs during parsing.</p>
		 * 
         * @param arvecXML_DataStreams : A string vector where each element represents an Input
         * Data Stream for a Data Link Message.
         * @return void
         * @throw DLXML_Exception
         */
        void parseXMLDataStreams(const std::vector<std::string>& arvecXML_DataStreams)\
        		throw (DLXML_Exception);
		
		/**
         * <p>Searchs the data value which has a DFI_DUI of "arstrDFI_DUI_Name" in DomDocumnet 
         * pointed by apddocInputStream, and returns this value. If it does not find any data it
         * returns empty string. Throws DLXML_Exception on errors and exceptional cases.</p>
         *
         * @param apddocInputStream : Pointer to DomDocument that represents a parsed Input Data Stream
         * @param arstrDFI_DUI_Name : DFI_DUI value to be searched
		 * @return Data associated with the DFI_DUI in XML Data Stream
		 * @throw DLXML_Exception
         */
		std::string getMessageFieldData(\
				const XERCES_CPP_NAMESPACE::DOMDocument* apddocInputStream,\
				const std::string& arstrDFI_DUI_Name) throw (DLXML_Exception);
		
		
		/**
		 * <p>Checks whether input data is of type "input data type" deduced from the DataType
		 * attribute of the Field Node. If input data is invalid then it is reported through
		 * error repository and "0" is returned as result. Otherwise trimmed input data is returned.
		 * 
		 * @param apdndFieldNode : DOMNode* that defines the encoding/decoding rule for the message
		 * field which is being processed
		 * @param arstrInputData : Input Data to be checked
         * @return Trimmed and checked input value
		 */
		std::string checkInputData(
				const XERCES_CPP_NAMESPACE::DOMNode* apdndFieldNode,
				const std::string& arstrInputData);
		
		/**
         * <p>Private copy constructor, to prevent memberwise initialisation.</p>
         */
		cMessageEncoder(const cMessageEncoder&);

		/**
		 * <p>Private assignment operator, to prevent memberwise assignment.</p>
		 */
		cMessageEncoder& operator=(const cMessageEncoder&);
		
		/**
		 * <p>A member of type cMessageRepository</p>
         */
        cMessageRepository m_objMessageRepository;
        
        /**
		 * <p>A member of type cErrorRepository</p>
         */
        cErrorRepository m_objErrorRepository;

        /**
		 * <p>A member of type cParserErrorHandler* which is used as the
		 * Error Handler during parse operations.</p>
		 */
		cParserErrorHandler* m_pehErrorHandler;
        
		/**
		 * <p>A member of type XercesDOMParser* which is used for parsing Input Data Streams.</p>
		 */
		XERCES_CPP_NAMESPACE::XercesDOMParser* m_pxdpDataDomParser;
		
		/**
		 * <p>A member of type std::vector<DOMDocument*> where each vector element represents
		 *  a DOMDocument* pointing the parsed Input Data Streams</p>
		 */
		std::vector<XERCES_CPP_NAMESPACE::DOMDocument*> m_vecInputStreamDomDocuments;
	};
}
#endif
