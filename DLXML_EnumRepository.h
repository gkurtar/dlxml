
#ifndef DLXML_ENUMREPOSITORY_H
#define DLXML_ENUMREPOSITORY_H

#include "DLXML_CommonTypes.h"

namespace DLXML
{
	/**
	 * @class cEnumRepository
	 * 
	 * @brief <p>cEnumRepository class provides methods for converting enums to strings
	 * and vice versa. This class implements Meyer's singleton pattern.
	 * Since current C++ standard does not allow conversion of enums to strings,
	 * this class is a workaround for this problem. The solution here is holding
	 * a std::map<int, cons char*> for each kind of enum that is to be used in
	 * DLXML library. Apart from this mappings of enums to strings there exist
	 * another std::map which stores a pair of a string and a std::map<int, cons char*>.
	 * This map maps enum names to  actual enum value-string pairs. Whenever a lookup
	 * is requested by calling the methods EnumToStr or StrToEnum the enum type name
	 * is also provided as an argument. By this name a lookup is done on the map
	 * 'm_stamapEnumHolder', and after finding the related enum map of type
	 * std::map<int, cons char*>, requested operation is done on this map.</p>
	 **/
	class cEnumRepository {
	public:
		/**
		 * <p>Returns string form of enum value supplied with the parameter 'anEnumVal'
		 * which has type name 'arstrEnumTypeName'. Throws DLXML_Exception on errors
		 * and exceptional cases.</p>
		 *
		 * @param arstrEnumTypeName : Enum Type Name
		 * @param anEnumVal : Value of enum.
		 * @return : String form of enum valued 'anEnumVal'
		 * @throw DLXML_Exception
		 */
		std::string EnumToStr(const std::string& arstrEnumTypeName, int anEnumVal) throw (DLXML_Exception);

		/**
		 * <p>Returns enumeration which has the string form supplied with the 
		 * parameter 'arstrEnumVal' and which has type name 'arstrEnumTypeName'.
		 * Return type is int since enums could be converted into ints and vice versa.
		 * Throws DLXML_Exception on errors and exceptional cases.</p>
		 *
		 * @param arstrEnumTypeName : Enum Type Name
		 * @param arstrEnumVal : String form of enum to be converted.
		 * @return Enum value of 'arstrEnumVal' converted to int
		 * @throw DLXML_Exception
		 */
		int StrToEnum(const std::string& arstrEnumTypeName,\
				const std::string& arstrEnumVal) throw (DLXML_Exception);

		/**
		 * <p>Returns an instance of this class. This method implements Meyer's 
		 * singleton pattern.</p>
		 *
		 * @return cEnumRepository : An instance of cEnumRepository class
		 */
		static cEnumRepository& getInstance();

	private:
		/**
		 * <p>Default constructor.</p>
		 */
		cEnumRepository();
		
		/**
		 * <p>Private copy constructor, to prevent memberwise initialization.</p>
		 */
		cEnumRepository(const cEnumRepository&);
		
		/**
		 * <p>Private assignment operator, to prevent memberwise assignment.</p>
		 */
		cEnumRepository& operator=(const cEnumRepository&);

		/**
		 * <p>Populates member data structures with Enums and their string forms.</p>
		 *
		 * @return void
		 */
		void populateEnumVars();

		/**
         * <p>A std::map which holds pairs of <enum type name, std::map>.</p>
         */
		static std::map<std::string, std::map<unsigned int, const char*>*> m_stamapEnumHolder;

		/**
         * <p>A std::map which holds pairs of <DLXML_RelationalOperator, std::string>
         * where string value here is the string form of enumeration value.</p>
         */
		static std::map<unsigned int, const char*> m_stamapRelOpEnum;

		/**
         * <p>A std::map which holds pairs of <DLXML_DataLinkNetwork, std::string>
         * where string value here is the string form of enumeration value.</p>
         */
		static std::map<unsigned int, const char*> m_stamapDataLinkNetEnum;

		/**
         * <p>A std::map which holds pairs of <DLXML_ErrorType, std::string>
         * where string value here is the string form of enumeration value.</p>
         */
		static std::map<unsigned int, const char*> m_stamapErrorTypeEnum;

		/**
         * <p>A std::map which holds pairs of <DLXML_ErrorLevel, std::string>
         * where string value here is the string form of enumeration value.</p>
         */
		static std::map<unsigned int, const char*> m_stamapErrorLevelEnum;
	};
}
#endif
