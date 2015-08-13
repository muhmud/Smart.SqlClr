
#ifndef _SMART_SQLCLR_UTILS__COLLECTIONALGORITHM_HPP_
#define _SMART_SQLCLR_UTILS__COLLECTIONALGORITHM_HPP_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Types { 
		ref class List;
		ref class Dictionary;
		ref class Enumerator;
	}

namespace Utils {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	/**
	 * @class CollectionAlgorithms
	 *
	 * @brief Provides algorithm functions that can be used with collection types
	 *
	 * This class contains many useful function for working with
	 * collection and should help to reduce the need to perform iteration
	 * in SQL, thereby providing better performance.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	public ref class CollectionAlgorithm sealed {
	public:
		/**
		 * Returns sum of the items in the list
		 *
		 * The supplied list should contain values of a numeric data type.
		 *
		 * @param list The list on which to apply the operation
		 *
		 * @returns A numeric value
		 *
		 */
		static Object^ Sum(Types::Enumerator^ enumerator);

		/**
		 * Returns the number of distinct items in an enumerator
		 *
		 * @param enumerator The enumerator on which to apply the operation
		 *
		 * @returns An integer value
		 *
		 */
		static SqlInt32 CountDistinct(Types::Enumerator^ enumerator);

		/**
		 * Returns average of the items in an enumeration
		 *
		 * The supplied enumeration should contain values of a numeric data type.
		 *
		 * @param enumerator The enumerator on which to apply the operation
		 *
		 * @returns A numeric value
		 *
		 */
		static Object^ Avg(Types::Enumerator^ enumerator);

		/**
		 * Returns the product of the items in an enumeration
		 *
		 * The supplied list should contain values of a numeric data type. It is
		 * possible for calls to the function to overflow.
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns A numeric value
		 *
		 */
		static Object^ Product(Types::Enumerator^ enumerator);

		/**
		 * Returns the minimum of the items in an enumeration
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns The minimum value
		 *
		 */
		static Object^ Min(Types::Enumerator^ enumerator);

		/**
		 * Returns the maximum of the items in an enumeration
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns The maximum value
		 *
		 */
		static Object^ Max(Types::Enumerator^ enumerator);

		/**
		 * Returns the median of the items in an enumeration
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns The median value
		 *
		 */
		static Object^ Median(Types::Enumerator^ enumerator);

		/**
		 * Returns the concatenation of the items in an enumerator
		 *
		 * The supplied enumerator should contain string values.
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns A string value
		 *
		 */
		static SqlString Concatenate(Types::Enumerator^ enumerator);

		/**
		 * Returns the concatenation of the items in an enumerator with a separator
		 *
		 * The supplied enumerator should contain string values.
		 *
		 * @param enumerator The enumerator to operate on
		 *
		 * @returns A string value
		 *
		 */
		static SqlString ConcatenateWithSeparator(Types::Enumerator^ enumerator, SqlString separator);

		/**
		 * Remove duplicate items in a list
		 *
		 * @param list The list to be processed
		 *
		 * @returns A list with duplicates removed
		 *
		 */
		static Types::List^ RemoveDuplicates(Types::List^ list);

		/**
		 * Creates a sorted copy of a list
		 *
		 * @param list The list to make the sorted copy of
		 *
		 * @returns A sorted list
		 *
		 */
		static Types::List^ SortCopyList(Types::List^ list);

		/**
		 * Creates a sorted copy of a dictionary
		 *
		 * @param dictionary The dictionary to make the sorted copy of
		 *
		 * @returns A sorted dictionary
		 *
		 */
		static Types::Dictionary^ SortCopyDictionary(Types::Dictionary^ dictionary);

		/**
		 * Creates a sorted copy of an enumerator
		 *
		 * @param enumerator The enumerator to make the sorted copy of
		 *
		 * @returns A sorted enumerator
		 *
		 */
		static Types::Enumerator^ SortCopyEnumerator(Types::Enumerator^ enumerator);

		/**
		 * Converts an enumerator to a list
		 *
		 * @param enumerator The enumerator to be copied
		 *
		 * @returns A list
		 *
		 */
		static Types::List^ ConvertEnumeratorToList(Types::Enumerator^ enumerator);
	};
}}}

#endif // _SMART_SQLCLR_UTILS__COLLECTIONALGORITHM_HPP_
