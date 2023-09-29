#ifndef DLXML_MESSAGEDECODER_H
#define DLXML_MESSAGEDECODER_H

#include "DLXML_ErrorRepository.h"
#include "DLXML_MessageRepository.h"

#include <string>
#include <vector>

namespace DLXML
{
	/**
	 * @class cMessageDecoder
	 * 
	 * @brief <p>cMessageDecoder class defines the main interface for decoding data link messages.
	 * It provides a method for decoding data link messages in its public interface. Other
	 * methods in the public interface simply returns references to member classes
	 * cMessageRepository and cErrorRepository. Its private interface consists of helper
	 * controller methods used by the public interface. Decoding process for a message
	 * is started and controlled from this class.</p>
	 **/
	class cMessageDecoder
	{
		friend class cMessageManager;
	public:
		/**
		 * <p>Decodes a number of encoded data link messages and returns them in a vector
		 * where each string of the result vector represents a decoded data link message
		 * as an XML Stream. Throws DLXML_Exception on errors and exceptional cases.</p>
		 * 
		 * @param arvecEncodedMessages : Encoded data link messages
		 * @return Decoded data link messages
		 * @throw DLXML_Exception
		 */
		std::vector<std::string> decodeMessages(\
				const std::vector<std::string>& arvecEncodedMessages) throw (DLXML_Exception);

		/**
         * <p>Returns a reference to member object m_objMessageRepository of type cMessageRepository</p>
         *
         * @return Reference to member object named "m_objMessageRepository"
         */
        cMessageRepository& getMessageRepository();

        /**
         * <p>Returns a reference to member object m_objErrorRepository of type cErrorRepository</p>
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
		cMessageDecoder(const DLXML_MessageIdentifierVector& arvecMessageIdentifiers);

		/**
         * <p>Class destructor</p>
         */
		virtual ~cMessageDecoder(void);
		
		/**
		 * <p>Decodes an encoded data link message</p>
		 *  
		 * @param arstrEncodedMessage : Encoded Message represented as a string consisting of 0's and 1's.
		 * @param apstMessageIdentifier : A pointer to the Message Identifier Structure
		 * that will be used while decode message
		 * @return A decoded data link message as an XML Stream.
		 */ 
		std::string decodeMessage(const std::string& arstrEncodedMessage,\
				const DLXML_MessageIdentifierStructure* apstMessageIdentifier);

		/**
		 * <p>This method is used for checking whether the encoded messages are in order and
		 * ready to be decoded. Checking involves controlling the messages in a sequence whether
		 * they obey transmit rules of their Data Link Network. For Ex. a sequence in Link11
		 * should start with a PPLI message, and each AMPLIFY message should follow a normal
		 * message. Errors that could possibly occur during these operations will be thrown
		 * as a DLXML_Exception set with related exception message. Throws DLXML_Exception on
		 * errors and exceptional cases.</p>
		 *
		 * @param arvecEncodedMessages : Encoded Data Link Messages
		 * @param areDataLinkType : Type of the Data Link Network enumeration (Link11, Link16, IJMS...)
		 * @return void
		 * @throw DLXML_Exception
		 */
		void checkMessages(const std::vector<std::string>& arvecEncodedMessages,
				DLXML_DataLinkNetwork& areDataLinkType) throw (DLXML_Exception);

		/**
         * <p>Private copy constructor. Prevents memberwise initialisation.</p>
         */
		cMessageDecoder(const cMessageDecoder&);

		/**
		 * <p>Private assignment operator. Prevents memberwise assignment.</p>
		 */
		cMessageDecoder& operator=(const cMessageDecoder&);
		
        /**
		 * <p>A member of type cMessageRepository</p>
         */
        cMessageRepository m_objMessageRepository;

        /**
		 * <p>A member of type cErrorRepository</p>
         */
        cErrorRepository m_objErrorRepository;
	};
}
#endif
