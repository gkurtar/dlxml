#ifndef DLXML_PARSERERRORHANDLER_H
#define DLXML_PARSERERRORHANDLER_H

#include <xercesc/sax/HandlerBase.hpp>

XERCES_CPP_NAMESPACE_BEGIN
class AttributeList;
class SAXParseException;
XERCES_CPP_NAMESPACE_END

using XERCES_CPP_NAMESPACE_QUALIFIER AttributeList;
using XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException;

namespace DLXML
{
	//Forward Declarations
	class cErrorRepository;
	
	/**
	 * @class cParserErrorHandler
	 * 
	 * @brief <p>cParserErrorHandler class extends XERCES_CPP_NAMESPACE::HandlerBase class
	 * and it helps error handling of Xerces Parsers. While parsing XML documents
	 * the methods implemented are triggered by Xerces Parsers. It contains an
	 * error repository object reference as a member. After errors are handled
	 * by related methods of this class, they are stored in supplied error
	 * repository object for logging purposes.</p>
	 **/
	class cParserErrorHandler : public XERCES_CPP_NAMESPACE_QUALIFIER HandlerBase
	{
	public:
	    // -----------------------------------------------------------------------
	    //  Constructors and Destructor
		cParserErrorHandler(cErrorRepository&);
	    ~cParserErrorHandler();

	    // -----------------------------------------------------------------------
	    //  Getter methods
	    unsigned int getElementCount() const
	    {
	        return m_unElementCount;
	    }

	    unsigned int getAttrCount() const
	    {
	        return m_unAttrCount;
	    }

	    unsigned int getCharacterCount() const
	    {
	        return m_unCharacterCount;
	    }

	    bool getSawErrors() const
	    {
	        return m_bSawErrors;
	    }

	    unsigned int getSpaceCount() const
	    {
	        return m_unSpaceCount;
	    }

	    // -----------------------------------------------------------------------
	    //  Handlers for the SAX DocumentHandler interface
		void startElement(const XMLCh* const name, XERCES_CPP_NAMESPACE_QUALIFIER AttributeList& attributes);
	    void characters(const XMLCh* const chars, const unsigned int length);
	    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
	    void resetDocument();

	    // -----------------------------------------------------------------------
	    //  Handlers for the SAX ErrorHandler interface
		void warning(const SAXParseException& arexpSaxParseException);
	    void error(const SAXParseException& arexpSaxParseException);
	    void fatalError(const SAXParseException& arexpSaxParseException);
	    void resetErrors();

	private:
		cParserErrorHandler(const cParserErrorHandler&);
		cParserErrorHandler& operator=(const cParserErrorHandler&);
	    //  Private data members
	    unsigned int    m_unAttrCount;
	    unsigned int    m_unCharacterCount;
	    unsigned int    m_unElementCount;
	    unsigned int    m_unSpaceCount;
	    bool            m_bSawErrors;
	    cErrorRepository& m_rcErrors;
	};
}
#endif
