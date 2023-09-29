
#include "DLXML_ParserErrorHandler.h"
#include "DLXML_ErrorRepository.h"
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/util/XMLString.hpp>

#include <sstream>

XERCES_CPP_NAMESPACE_USE
using std::ostringstream;

namespace DLXML
{
	//-------------------------------------------------------------------------------------------
	//ParserErrorHandler interface
	cParserErrorHandler::cParserErrorHandler(cErrorRepository& arcErrorRepository) : \
		m_unAttrCount(0), m_unCharacterCount(0), m_unElementCount(0),\
		m_unSpaceCount(0), m_bSawErrors(false), m_rcErrors(arcErrorRepository) 
	{
	}

	cParserErrorHandler::~cParserErrorHandler()
	{
	}

	// ---------------------------------------------------------------------------
	//  cParserErrorHandler: Implementation of the SAX DocumentHandler interface
	void cParserErrorHandler::startElement(const XMLCh* const, AttributeList& attributes)
	{
		m_unElementCount++;
		m_unAttrCount += attributes.getLength();
		return;
	}

	void cParserErrorHandler::characters(const XMLCh* const, const unsigned int length)
	{
		m_unCharacterCount += length;
		return;
	}

	void cParserErrorHandler::ignorableWhitespace(const XMLCh* const, const unsigned int length)
	{
		m_unSpaceCount += length;
		return;
	}

	void cParserErrorHandler::resetDocument()
	{
		m_unAttrCount = m_unCharacterCount = m_unElementCount = m_unSpaceCount = 0;
		return;
	}

	// ---------------------------------------------------------------------------
	//  SAXCountHandlers: Overrides of the SAX ErrorHandler interface
	void cParserErrorHandler::warning(const SAXParseException& arexpSaxParseException)
	{
		char* pchSystemId = XMLString::transcode(arexpSaxParseException.getSystemId());
		char* pchMessage = XMLString::transcode(arexpSaxParseException.getMessage());

	    ostringstream ossTemp;
	    ossTemp << "\nWarning at file " << pchSystemId << ", line " <<\
	    	arexpSaxParseException.getLineNumber() << ", char " <<\
	    	arexpSaxParseException.getColumnNumber() << "\n  Message: " <<\
	    	pchMessage << XERCES_STD_QUALIFIER endl;
	    DLXML_ADD_ERROR(m_rcErrors, ETP_XML_RULE_FILE, ELV_WARNING, ossTemp.str())
	    XMLString::release(&pchSystemId);
	    XMLString::release(&pchMessage);
	    return;
	}
	
	void cParserErrorHandler::error(const SAXParseException& arexpSaxParseException)
	{
		m_bSawErrors = true;
	    char* pchSystemId = XMLString::transcode(arexpSaxParseException.getSystemId());
	    char* pchMessage = XMLString::transcode(arexpSaxParseException.getMessage());

	    ostringstream ossTemp;
	    ossTemp << "\nError at file " << pchSystemId << ", line " <<\
	    	arexpSaxParseException.getLineNumber() << ", char " <<\
	    	arexpSaxParseException.getColumnNumber() << "\n  Message: " <<\
	    	pchMessage << XERCES_STD_QUALIFIER endl;
	    DLXML_ADD_ERROR(m_rcErrors, ETP_XML_RULE_FILE, ELV_ERROR, ossTemp.str())
	    XMLString::release(&pchSystemId);
	    XMLString::release(&pchMessage);
	    //pack this exception into DLXML_Exception and throw
	    //throw arexpSaxParseException;
	    DLXML_THROW_EX(DLXML_Exception, ossTemp.str())
	}

	void cParserErrorHandler::fatalError(const SAXParseException& arexpSaxParseException)
	{
		m_bSawErrors = true;
	    char* pchSystemId = XMLString::transcode(arexpSaxParseException.getSystemId());
	    char* pchMessage = XMLString::transcode(arexpSaxParseException.getMessage());

	    ostringstream ossTemp;
	    ossTemp << "\nFatal Error at file " << pchSystemId 	<< ", line " <<\
	    	arexpSaxParseException.getLineNumber() <<", char " <<\
	    	arexpSaxParseException.getColumnNumber() << "\n  Message: " <<\
	    	pchMessage << XERCES_STD_QUALIFIER endl;
	    DLXML_ADD_ERROR(m_rcErrors, ETP_XML_RULE_FILE, ELV_FATAL, ossTemp.str())
	    XMLString::release(&pchSystemId);
	    XMLString::release(&pchMessage);
	    //pack this exception into DLXML_Exception and throw
	    DLXML_THROW_EX(DLXML_Exception, ossTemp.str())
	}

	void cParserErrorHandler::resetErrors()
	{
		m_bSawErrors = false;
		return;
	}
} // end of namespace

