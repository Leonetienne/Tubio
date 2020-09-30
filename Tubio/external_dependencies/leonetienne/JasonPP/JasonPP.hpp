/*
MIT License

JasonPP, Copyright (c) 2020, Leon Etienne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef _JASONPP_HPP
#define _JASONPP_HPP






namespace JasonPP
{
	enum class JSON_DATA_TYPE
	{
		__NULL__, // Can't use a normal NULL because NULL is already taken lol
		BOOL,
		INT,
		FLOAT,
		STRING,
		JSON,
		ARRAY
	};
}


namespace JasonPP
{
	enum class JSON_ARRAY_SORT_MODE
	{
		NUM_ASC,
		NUM_DESC,
		ALPH_ASC,
		ALPH_DESC
	};
}


namespace JasonPP
{
	namespace Internal
	{
		/// <summary>
		/// Internal helper class. Feel free to use it tho, lol
		/// </summary>
		class Helpers
		{
		public:
			/// <summary>
			/// Will call delete on all elements of a vector, and then call delete of the vector itself
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="vec"></param>
			template <class T>
			static void FreeVector(std::vector<T*>* vec)
			{
				if (vec != nullptr)
				{
					for (std::size_t i = 0; i < vec->size(); i++)
					{
						delete vec->at(i);
						vec->at(i) = nullptr;
					}
					delete vec;
					vec = nullptr;
				}

				return;
			};

			static bool AreSame(const double a, const double b, const double epsilon);
			static bool AreSame(const long double a, const long double b, const double epsilon);
		
			/// <summary>
			/// Returns the minimum of a and b
			/// </summary>
			/// <param name="a">Number a</param>
			/// <param name="b">Number b</param>
			/// <returns></returns>
			template <typename T>
			static T Min(const T a, const T b)
			{
				return (a < b) ? a : b;
			};

			/// <summary>
			/// Returns the maximum of a and b
			/// </summary>
			/// <param name="a">Number a</param>
			/// <param name="b">Number b</param>
			/// <returns></returns>
			template <typename T>
			static T Max(const T a, const T b)
			{
				return (a > b) ? a : b;
			};

			/// <summary>
			/// Returns b to the power of e. May cause unsigned long long int to overflow, so be careful.
			/// This is specifically optimized for integer values.
			/// </summary>
			/// <param name="b"></param>
			/// <param name="e"></param>
			/// <returns></returns>
			static unsigned long long int Powuli(const unsigned long int b, const unsigned long int e);

			/// <summary>
			/// Converts i (base10) to any other base
			/// </summary>
			/// <param name="i">To be converted</param>
			/// <param name="set">New charset</param>
			/// <param name="padding">Minimum string length. Will be expanded with zero-values on the left</param>
			/// <returns>i in the requested base</returns>
			static std::string Base10_2_X(const unsigned long long int i, const std::string set, unsigned int padding = 0);
			
			/// <summary>
			/// Converts num back to base10. Needs its original charset.
			/// </summary>
			/// <param name="num">To be converted</param>
			/// <param name="set">The charset it got converted to originally</param>
			/// <returns>i in base10</returns>
			static unsigned long long int BaseX_2_10(const std::string num, const std::string set);

		};
	}
}



namespace JasonPP
{
	class JsonData;

	namespace Internal
	{
		/// <summary>
		/// A string extension class providing some additional functionality
		/// </summary>
		class Jstring : public std::string
		{
		public:
			/// <summary>
			/// Will initialize as an empty std::string
			/// </summary>
			Jstring();

			/// <summary>
			/// Will initialize as an existing std::string
			/// </summary>
			/// /// <param name="c">The initial string</param>
			Jstring(const std::string str);

			/// <summary>
			/// Will initialize as an std::string with just the character c
			/// </summary>
			/// <param name="c">The character to get converted to string</param>
			Jstring(const char c);

			/// <summary>
			/// Will initialize as an std::string with i as string
			/// </summary>
			/// <param name="i">The integer to get converted to a string</param>
			Jstring(const int i);

			/// <summary>
			/// Will initialize as an std::string with i as string
			/// </summary>
			/// <param name="lli">The integer to get converted to a string</param>
			Jstring(const long long int lli);

			/// <summary>
			/// Will initialize as an std::string with f as string
			/// </summary>
			/// <param name="d">The floating point to get converted to a string</param>
			Jstring(const double d);

			/// <summary>
			/// Will initialize as an std::string with f as string
			/// </summary>
			/// <param name="d">The floating point to get converted to a string</param>
			Jstring(const long double d);

			using std::string::basic_string;
			using std::string::operator=;
			using std::string::operator+=;
			using std::string::operator[];
		};
	}
}

namespace JasonPP
{
	class JsonException : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return message.c_str();
		};

	private:
		std::string message;

		friend class JsonWrongDataTypeException;
		friend class JsonLabelDoesNotExistException;
		friend class JsonLabelAlreadyExistsException;
		friend class JsonParsingMissingBracketsException;
		friend class JsonParsingMissingQuotesException;
		friend class JsonParsingExpectedEOFException;
		friend class JsonParsingGeneralException;
		friend class JsonParsingDuplicateLabelException;
		friend class JsonParsingUnrecognizableDatatypeException;
		friend class JsonParsingUTFSequenceException;
		friend class JsonShorthandInvalidException;
		friend class JsonNoParentAvailableException;
		friend class JsonArrayOutOfRangeException;
	};

	class JsonWrongDataTypeException : public JsonException
	{
	public:
		JsonWrongDataTypeException(const std::string fetchedType, const std::string elementType)
		{
			std::stringstream ss;
			ss << "You tried to fetch the wrong datatype! You tried to get an \"" << fetchedType << "\" from an element of type \"" << elementType << "\"!" << std::endl;

			message = ss.str();
		};
	};

	class JsonLabelDoesNotExistException : public JsonException
	{
	public:
		JsonLabelDoesNotExistException(const std::string label)
		{
			std::stringstream ss;
			ss << "You tried to act upon the non-existent json field \"" << label << "\"!" << std::endl;
			ss << "Use Json::Exists() to check if a label exists, or use Json::Set() to create a field if it does not exist yet!" << std::endl;

			message = ss.str();
		};
	};

	class JsonLabelAlreadyExistsException : public JsonException
	{
	public:
		JsonLabelAlreadyExistsException(const std::string label)
		{
			std::stringstream ss;
			ss << "You tried to create the already existing json field \"" << label << "\"!" << std::endl;
			ss << "Use Json::Exists() to check if a label exists, or use Json::FGet() to overwrite existing fields if they already exist!" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingMissingBracketsException : public JsonException
	{
	public:
		JsonParsingMissingBracketsException()
		{
			std::stringstream ss;
			ss << "The json code that was attempted to parse is missing brackets!" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingMissingQuotesException : public JsonException
	{
	public:
		JsonParsingMissingQuotesException()
		{
			std::stringstream ss;
			ss << "The json code that was attempted to parse is missing string quotation marks!" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingExpectedEOFException : public JsonException
	{
	public:
		JsonParsingExpectedEOFException(const std::string rest)
		{
			std::stringstream ss;
			ss << "Expected end of file, but got \"" << rest << "\" instead." << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingGeneralException : public JsonException
	{
	public:
		JsonParsingGeneralException(const std::string comment)
		{
			std::stringstream ss;
			ss << "The parsing fucked up for some unknown reason. Probably faulty json code. Comment: (" << comment << ")" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingDuplicateLabelException : public JsonException
	{
	public:
		JsonParsingDuplicateLabelException(const std::string label)
		{
			std::stringstream ss;
			ss << "The parsing fucked up because a duplicate label was found. Duplicate label: \"" << label << "\"" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingUnrecognizableDatatypeException : public JsonException
	{
	public:
		JsonParsingUnrecognizableDatatypeException(const std::string value)
		{
			std::stringstream ss;
			ss << "The parsing fucked up because a value is not recognizable. Value: \"" << value << "\"" << std::endl;

			message = ss.str();
		};
	};

	class JsonParsingUTFSequenceException : public JsonException
	{
	public:
		JsonParsingUTFSequenceException(const std::string invalSegment)
		{
			std::stringstream ss;
			ss << "Invalid UTF-8 escape sequence: \"" << invalSegment << "\"" << std::endl;

			message = ss.str();
		};
	};

	class JsonShorthandInvalidException : public JsonException
	{
	public:
		JsonShorthandInvalidException(const std::string shorthand, const std::string message = "")
		{
			std::stringstream ss;
			ss << "The shorthand you've requested is invalid! Shorthand: " << shorthand << std::endl << message << ((message.length() > 0) ? "\n" : "");

			this->message = ss.str();
		};
	};

	class JsonNoParentAvailableException : public JsonException
	{
	public:
		JsonNoParentAvailableException()
		{
			std::stringstream ss;
			ss << "You have requested the parent of an element having no parent!" << std::endl;

			message = ss.str();
		};
	};

	class JsonArrayOutOfRangeException : public JsonException
	{
	public:
		JsonArrayOutOfRangeException(const std::size_t requested, const std::size_t actualLength)
		{
			std::stringstream ss;
			ss << "You have requested " << requested << "th Element (starting at 0) of an array that is only " << actualLength << " elements long!" << std::endl;

			message = ss.str();
		};
	};
}

namespace JasonPP
{
	class JsonData;
	class JsonBlock;
	class JsonElement;

	// A wrapper class for arrays
	class JsonArray
	{
	public:
		/// <summary>
		/// Will construct an empty array
		/// </summary>
		JsonArray();

		/// <summary>
		/// Will construct itself as a clone of otherArr
		/// </summary>
		/// <param name="otherArr">Array to be cloned from</param>
		JsonArray(const JsonArray& otherArr);

		/// <summary>
		/// Will construct itself with the contents of the supplied vector
		/// </summary>
		/// <param name="elements">Vector of elements to be initialized with</param>
		JsonArray(const std::vector<JsonData> elements);

		~JsonArray();

		/// <summary>
		/// Will return the actual JsonData* vector
		/// </summary>
		/// <returns>The vector lmao</returns>
		std::vector<JsonData*>* GetVector();

		/// <summary>
		/// Will return the arrays size
		/// </summary>
		/// <returns>The arrays element count</returns>
		std::size_t Size() const;

		/// <summary>
		/// Will remove all elements in this array
		/// </summary>
		void Clear();

		/// <summary>
		/// Will return the element at index idx
		/// </summary>
		/// <param name="idx">Index to be removed</param>
		/// <returns></returns>
		JsonData& At(const std::size_t idx);

		/// <summary>
		/// Will return the element at index idx
		/// </summary>
		/// <param name="idx">Index to be removed</param>
		/// <returns>The element</returns>
		const JsonData& At(const std::size_t idx) const;

		/// <summary>
		/// Will remove the item at index idx
		/// </summary>
		/// <param name="idx">Index to be removed</param>
		void RemoveAt(const std::size_t idx);

		/// <summary>
		/// Will remove every element identical to a reference.
		/// </summary>
		/// <param name="reference">Reference to be compared against</param>
		/// <returns>Number of elements removed</returns>
		std::size_t RemoveSimilar(const JsonData reference);

		/// <summary>
		/// Will remove every element of a type 'type'.
		/// </summary>
		/// <param name="type">The type to be removed</param>
		/// <returns>Number of elements removed</returns>
		std::size_t RemoveAllOfType(const JSON_DATA_TYPE type);

		/// <summary>
		/// Will remove every element not of type 'type'
		/// </summary>
		/// <param name="type">The type to be kept</param>
		/// <returns>Number of elements removed</returns>
		std::size_t RemoveAllExceptType(const JSON_DATA_TYPE type);

		/// <summary>
		/// Will append (NOT COPYING!!!) the pointer given and it will be freed when 'this' gets destructed
		/// </summary>
		/// <param name="">JsonData to be added</param>
		void InsertExistingtJsonData(JsonData* data);

		/// <summary>
		/// Will add a null value and automatically create a JsonData object
		/// </summary>
		/// <param name="b">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add();

		// <summary>
		/// Will add a boolean value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const bool data);

		// <summary>
		/// Will add all supplied booleans
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<bool> data);

		// <summary>
		/// Will add an integer value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const int data);

		// <summary>
		/// Will add all supplied integers
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<int> data);

		// <summary>
		/// Will add an integer value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const long long int data);

		// <summary>
		/// Will add all supplied integers
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<long long int> data);

		// <summary>
		/// Will add a floating point value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const double data);

		// <summary>
		/// Will add all supplied floats
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<double> data);

		// <summary>
		/// Will add a floating point value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const long double data);

		// <summary>
		/// Will add all supplied floats
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<long double> data);

		// <summary>
		/// Will add a string value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const std::string data);

		// <summary>
		/// Will add a string value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const char data[]);

		// <summary>
		/// Will add all supplied strings
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<std::string> data);

		// <summary>
		/// Will add an array value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const JsonArray data);

		// <summary>
		/// Will add copies of all supplied JsonArray objects
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<JsonArray> data);

		// <summary>
		/// Will add a copy of an existing JsonData
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const JsonData data);

		// <summary>
		/// Will add copies of all supplied JsonData objects
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<JsonData> data);

		// <summary>
		/// Will add a json value and automatically create a JsonData object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& Add(const JsonBlock data);

		// <summary>
		///  Will add copies of all supplied JsonBlock objects
		/// </summary>
		/// <param name="data">To be added</param>
		void Add(const std::vector<JsonBlock> data);

		/// <summary>
		/// Will merge JsonArray other into this, deep-copying all values.
		/// </summary>
		/// <param name="other">Array to merge-copy from</param>
		void Merge(const JsonArray& other);

		/// <summary>
		/// Will append the object given
		/// </summary>
		/// <param name="">Multiple JsonData to be added at once</param>
		void InsertExistingtJsonData(const std::vector<JsonData*> data);

		/// <summary>
		/// Will clone itself and all children from the supplied object
		/// </summary>
		/// <param name="jd">JsonArray to be cloned from</param>
		void CloneFrom(const JsonArray& other);

		/// <summary>
		/// Will clone itself and all children to the supplied object
		/// </summary>
		/// <param name="jd">JsonArray to be cloned to</param>
		void CloneTo(JsonArray& other) const;

		/// <summary>
		/// Will return true if this object is semantically identical to the supplied other one
		/// </summary>
		/// <param name="other">JsonArray to be compared against</param>
		/// <returns>Whether or not the arrays are semantically identical</returns>
		bool IsIdentical(const JsonArray& other) const;

		/// <summary>
		/// Will sort this array after a value in its JsonBlock members.
		/// Members that don't fit the descriptor will not be sorted and moved to the start.
		/// </summary>
		/// <param name="shorthandKey"></param>
		/// <param name="mode"></param>
		/// <param name="shorthandDelimiter"></param>
		void Sort(const std::string shorthandKey, const JSON_ARRAY_SORT_MODE mode, const std::string shorthandDelimiter = ".");

		/// <summary>
		/// Will sort this array. Only use this function for array consisting of DIRECT, ATOMIC VALUES!
		/// </summary>
		/// <param name="mode"></param>
		void Sort(const JSON_ARRAY_SORT_MODE mode);

		/// <summary>
		/// Swaps two array indices
		/// </summary>
		/// <param name="a">to be swapped with b</param>
		/// <param name="b">to be wapped with a</param>
		void Swap(const std::size_t a, const std::size_t b);

		/* Operator overloads */

		/// <summary>
		/// Will return the object at the specified index
		/// </summary>
		/// <param name="idx">index to look at</param>
		/// <returns>Reference to the object</returns>
		JsonData& operator[](const std::size_t idx);

		/// <summary>
		/// Will return the object at the specified index
		/// </summary>
		/// <param name="idx">index to look at</param>
		/// <returns>Reference to the object</returns>
		const JsonData& operator[](std::size_t idx) const;

		/// <summary>
		/// Will attempt to remove all values equal to that integer
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const int data);

		/// <summary>
		/// Will attempt to remove all values equal to that integer
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const long long int data);

		/// <summary>
		/// Will attempt to remove all values equal to that string
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const std::string data);

		/// <summary>
		/// Will attempt to remove all values equal to that string
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const char data[]);

		/// <summary>
		/// Will attempt to remove all values equal to that JsonData
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const JsonData& data);

		/// <summary>
		/// Will attempt to remove all values of that type
		/// </summary>
		/// <param name="data">Reference data to look for</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator-=(const JSON_DATA_TYPE type);

		/// <summary>
		/// Will add this boolean to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const bool data);

		/// <summary>
		/// Will add this integer to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const int data);

		/// <summary>
		/// Will add this integer to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const long long int data);

		/// <summary>
		/// Will add this floating point to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const double data);

		/// <summary>
		/// Will add this floating point to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const long double data);

		/// <summary>
		/// Will add this string to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::string data);

		/// <summary>
		/// Will add this string to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const char data[]);

		/// <summary>
		/// Will add this Array to the array as a single object
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const JsonArray data);

		/// <summary>
		/// Will add this JsonData object to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const JsonData data);

		/// <summary>
		/// Will add this JsonBlock to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const JsonBlock data);

		/// <summary>
		/// Will add all these booleans to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<bool> data);

		/// <summary>
		/// Will add all these integers to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<int> data);

		/// <summary>
		/// Will add all these integers to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<long long int> data);

		/// <summary>
		/// Will add all these floating points to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<double> data);

		/// <summary>
		/// Will add all these floating points to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<long double> data);

		/// <summary>
		/// Will add all these strings to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<std::string> data);

		/// <summary>
		/// Will add all these Arrays to the array as individual objects
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<JsonArray> data);

		/// <summary>
		/// Will add all these JsonData objects to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<JsonData> data);

		/// <summary>
		/// Will add all these JsonBlocks to the array
		/// </summary>
		/// <param name="data">Value to add</param>
		/// <returns>The JsonArray itself</returns>
		JsonArray& operator+=(const std::vector<JsonBlock> data);

		/// <summary>
		/// Compares wether or not these two JsonArrays are semantically identical. => IsIdentical()
		/// </summary>
		/// <param name="other">The object to be compared against</param>
		/// <returns>If the arrays are semantically identical or not</returns>
		bool operator==(const JsonArray& other) const;

		/// <summary>
		/// Compares wether or not these two JsonArrays are NOT semantically identical. => !IsIdentical()
		/// </summary>
		/// <param name="other">The object to be compared against</param>
		/// <returns>If the arrays are NOT semantically identical or not</returns>
		bool operator!=(const JsonArray& other) const;

	private:

		/// <summary>
		/// Internal sort helper. Compares two JsonDatas
		/// </summary>
		/// <param name="a">higher, if true</param>
		/// <param name="b">lower, if true</param>
		/// <param name="mode">Sorting mode</param>
		/// <returns>If a is higher than b</returns>
		bool Sort__Compare(const JsonData& a, const JsonData& b, const JSON_ARRAY_SORT_MODE mode);
		
		// <summary>
		/// Will add copies of all these elements to the vector
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		void CopyJsonDataFromVector_Pointer(const std::vector<JsonData>* data);

		// <summary>
		/// Will add an existing JsonArray object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& AddPointer(JsonArray* data);

		// <summary>
		/// Will add an existing JsonBlock object
		/// </summary>
		/// <param name="data">To be added</param>
		/// <returns>Pointer to new JsonData object</returns>
		JsonData& AddPointer(JsonBlock* data);

		/// <summary>
		/// Will render this component to string
		/// </summary>
		/// <param name="num_tabs">The base number to indent. Basically, how nested are we already?</param>
		/// <param name="minify">Should we render minified or not?</param>
		/// <returns>The rendering result</returns>
		std::string Render(unsigned int num_tabs, const bool minify) const;

		/// <summary>
		/// Will parse code for this component
		/// </summary>
		/// <param name="jsonCode">Code to be parsed</param>
		void Parse(const std::string jsonCode);

		/// <summary>
		/// Simple initializer to be called in constructors
		/// </summary>
		void Init();

		/// <summary>
		/// Will set tha value of its associated JsonData node, and also progagates that value to all direct children
		/// </summary>
		/// <param name="newNode">The new parent node</param>
		void SetAssociatedJsonDataNode(JsonData* newNode);

		std::vector<JsonData*>* content;
		JsonData* associatedJsonDataNode = nullptr;

		friend class JsonData;
		friend class JsonElement;
	};
}


namespace JasonPP
{
	/// <summary>
	/// Will return whether or not the supplied json code is valid
	/// </summary>
	/// <param name="code">The json code to check</param>
	/// <returns>Whether or not this json code is valid</returns>
	bool IsJsonValid(const std::string code);

	/// <summary>
	/// Will convert a json data type to it's string name
	/// </summary>
	/// <param name="type">The json data type to convert</param>
	/// <returns>The json data type in string form</returns>
	std::string JsonDataType2String(const JSON_DATA_TYPE type);
}

/* If defined, JasonPP will print no more warnings */
#ifdef JASONPP_WARN_STFU
	#undef JASONPP_WARN_STFU
	#define JASONPP_WARN_STFU true
#else
	#undef JASONPP_WARN_STFU
	#define JASONPP_WARN_STFU false
#endif

/* How many spaces are prepended per level of indentation */
#ifndef JASONPP_SPACES_PER_INDENT
	#define JASONPP_SPACES_PER_INDENT 2
#endif

/* How similar floating point values have to be, to be considered equal */
#ifndef JASONPP_FLOAT_PRECISION
	#define JASONPP_FLOAT_PRECISION (0.0000001)
#endif

/* The greatest float precision settable via JASONPP_FLOAT_PRECISION or SetFloatPrecision() */
#ifndef JASONPP_MAX_FLOAT_PRECISION
	#define JASONPP_FLOAT_MAX_PRECISION (0.0000000000000000000000000001)
#endif

/* If defined, a JsonData -> string conversion will result in a minified render instead of a pretty one */
#ifdef JASONPP_STRINGCONV_MINIFY
	#undef JASONPP_STRINGCONV_MINIFY
	#define JASONPP_STRINGCONV_MINIFY true
#else
	#undef JASONPP_STRINGCONV_MINIFY
	#define JASONPP_STRINGCONV_MINIFY false
#endif

/* If defined, JasonPP will render JsonBlocks in alphabetical order. Technically, Json standard but tbh i think its kinda unnecesary.
   Anyways, YOU HAVE THE FKIN OPTION :D
   Keep in mind, that this is, of course, slower than normal rendering. */
#ifdef JASONPP_RENDER_SORTED
	#undef JASONPP_RENDER_SORTED
	#define JASONPP_RENDER_SORTED true
#else
#undef JASONPP_RENDER_SORTED
	#define JASONPP_RENDER_SORTED false
#endif

/* If defined, JasonPP will no longer automatically parse UTF-8 escape sequences like "\u00b7" */
#ifdef JASONPP_LEAVE_UTF8_ALONE
	#undef JASONPP_LEAVE_UTF8_ALONE
	#define JASONPP_LEAVE_UTF8_ALONE true
#else
	#undef JASONPP_LEAVE_UTF8_ALONE
	#define JASONPP_LEAVE_UTF8_ALONE false
#endif

/* All this #undef stuff is to make the compiler shut the fuck up i know that i am redifining macros! I intended that! */


namespace JasonPP
{
	namespace Internal
	{
		/// <summary>
		/// Internal helper class. Feel free to use it tho, lol
		/// </summary>
		class StringHelpers
		{
		public:
			/// <summary>
			/// Will replace every occurrence of char find in string str by string subst
			/// </summary>
			/// <param name="str">The original string to work in</param>
			/// <param name="find">The character to find</param>
			/// <param name="subst">The string to replace "find" in "str"</param>
			/// <returns>The string with the values replacesd</returns>
			static std::string Replace(const std::string str, const char find, const std::string subst);

			/// <summary>
			/// Will replace every occurrence of string find in string str by string subst. If find is empty, it will just return str
			/// </summary>
			/// <param name="str">The original string to work in</param>
			/// <param name="find">The substring to find</param>
			/// <param name="subst">The string to replace "find" in "str"</param>
			/// <returns>The string with the values replacesd</returns>
			static std::string Replace(const std::string str, const std::string find, const std::string subst);

			/// <summary>
			/// Will escape characters in a string to be used in json
			/// </summary>
			/// <param name="str">The original string to work in</param>
			/// <returns>The string with escaped characters</returns>
			static std::string Escape(const std::string str);

			/// <summary>
			/// Will unescape json strings
			/// </summary>
			/// <param name="str">The original string to work in</param>
			/// <returns>The string with unescaped characters</returns>
			static std::string Unescape(const std::string str);

			/// <summary>
			/// Will return a string with num spaces. You can set a multiplier via defining JASONPP_SPACES_PER_INDENT, default is 2
			/// </summary>
			/// <param name="num">The number of spaces to indent. Will get multiplied by JASONPP_SPACES_PER_INDENT</param>
			/// <returns></returns>
			static std::string GetIndentation(const std::size_t num);

			/// <summary>
			/// Will return true if the given string consists only of digits (including signage)
			/// </summary>
			/// <param name="str">The string to check</param>
			/// <param name="allowDecimalPoint">Whether or not to see decimal values as numeric values</param>
			/// <returns>Whether or not this string is numerical</returns>
			static bool IsNumeric(const std::string str, const bool allowDecimalPoint = false);

			/// <summary>
			/// Will split a string by a delimiter char. The delimiter will be excluded!
			/// </summary>
			/// <param name="str">The original string to work in</param>
			/// <param name="delimiter">The delimiter to split at. Will not be present in the result</param>
			/// <returns>A vector of split strings</returns>
			static std::vector<std::string> SplitString(const std::string str, const char delimiter);

			/// <summary>
			/// Will split a string by a delimiter string. The delimiter will be excluded!
			/// </summary>
			// <param name="str">The original string to work in</param>
			/// <param name="delimiter">The delimiter to split at. Will not be present in the result</param>
			/// <returns>A vector of split strings</returns>
			static std::vector<std::string> SplitString(const std::string str, const std::string delimiter);

			/// <summary>
			/// Will return minified json code
			/// </summary>
			/// <param name="jsonCode">The json code to minify</param>
			/// <returns>The minified json code</returns>
			static std::string MinifyJson(const std::string jsonCode);

			/// <summary>
			/// Returns whether or not a is alphabetically "higher-ordered" than b
			/// </summary>
			/// <param name="a">Compare 1</param>
			/// <param name="b">Compare 2</param>
			/// <returns>Sorting descriminator</returns>
			static bool SortDescriminator_Alphabetically(const std::string a, const std::string b);

			/// <summary>
			/// Will make a string all lower-case
			/// </summary>
			/// <param name="str">To be lowercasified</param>
			/// <returns></returns>
			static std::string ToLower(const std::string str);

		private:
			/// <summary>
			/// Will parse a 16bit hex value of max-value 0xff (like "007c") to an UTF-8 char. Will return the hex sequence, if it is greater than 0xff
			/// </summary>
			/// <param name="str">The hex string</param>
			/// <returns>A std::string with the unescaped character OR the hex sequence itself, if greater than 0xff</returns>
			static std::string ParseUTF8(const std::string str);

			/// <summary>
			/// Will escape a character to UTF8 notation (\u007c)
			/// </summary>
			/// <param name="str">The string containing weird characters</param>
			/// <returns>The escaped string</returns>
			static std::string EscapeUTF8(const char c);
		};
	}
}

namespace JasonPP
{
	class JsonBlock;
	class JsonElement;
	typedef JsonData Json;

	/// <summary>
	/// The right hand side of a json element
	/// </summary>
	class JsonData
	{
	public:
		/* Constructors */

		/// <summary>
		/// Initializes this JsonData as a null object
		/// </summary>
		JsonData();

		/// <summary>
		/// Will initialize this JsonData as an empty object of type type
		/// </summary>
		/// <param name="type">The type to apply</param>
		JsonData(const JSON_DATA_TYPE type);

		/// <summary>
		/// Initializes this JsonData as an integer object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const long long int data);

		/// <summary>
		/// Initializes this JsonData as an integer object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const int data);

		/// <summary>
		/// Initializes this JsonData as an boolean object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const bool data);

		/// <summary>
		/// Initializes this JsonData as a float object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const long double data);

		/// <summary>
		/// Initializes this JsonData as a float object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const double data);

		/// <summary>
		/// Initializes this JsonData as a string object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const std::string data);

		/// <summary>
		/// Initializes this JsonData as a string object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const char data[]);

		/// <summary>
		/// Initializes this JsonData with a copy of the supplied JsonBlock
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const JsonBlock data);

		/// <summary>
		/// Initializes this JsonData as an array object with the data given
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const std::vector<JsonData> data);

		/// <summary>
		/// Initializes this JsonData with a copy of the supplied JsonArray
		/// </summary>
		/// <param name="data">The data to assign to this object</param>
		JsonData(const JsonArray data);

		/// <summary>
		/// Will initialize itself as a copied of the supplied JsonData object
		/// </summary>
		/// <param name="data">A reference to another JsonData object to deep-copy from</param>
		JsonData(const JsonData& data);

		~JsonData();

		
		/* GETTERS */
		
		/// <summary>
		/// Will return this components effective floating point precision, in this format: (2 decimal places): 0.01
		/// </summary>
		/// <returns>The floating point precision, duh</returns>
		double GetFloatPrecision() const;

		/// <summary>
		/// Returns the data type of the element. That is important! Don't fuck up by trying to pull an integer from a string.
		/// </summary>
		/// <returns>The json data type of this object</returns>
		JSON_DATA_TYPE GetDataType() const { return dataType; };

		/// <summary>
		/// Returns whether or not this JsonData is either of type FLOAT or INT
		/// </summary>
		/// <returns>Whether or not this JsonData is either of type FLOAT or INT</returns>
		bool IsOfNumericType() const;

		/// <summary>
		/// Returns the bool value of this element, exception if of wrong type
		/// </summary>
		/// <returns>The boolean value</returns>
		bool GetBoolData() const;

		/// <summary>
		/// Returns the int value of this element, exception if of wrong type
		/// </summary>
		/// <returns>The integer data</returns>
		long long int GetIntData() const;

		/// <summary>
		/// Returns the floating point value of this element, exception if of wrong type
		/// </summary>
		/// <returns>The float data</returns>
		long double GetFloatData() const;

		/// <summary>
		/// Returns the string value of this element, exception if of wrong type
		/// </summary>
		/// <returns>The string data</returns>
		std::string GetStringData() const;

		/// <summary>
		/// Returns the JsonBlock of this element (Like, another json object), exception if of wrong type
		/// </summary>
		/// <returns>The json data</returns>
		JsonBlock& GetJsonData();

		/// <summary>
		/// Returns the JsonBlock of this element (Like, another json object), exception if of wrong type
		/// </summary>
		/// <returns>The json data</returns>
		const JsonBlock& GetJsonData() const;

		/// <summary>
		/// Returns an the array of JsonData of this element, exception if of wrong type
		/// </summary>
		/// <returns>The array data</returns>
		JsonArray& GetArrayData();

		/// <summary>
		/// Returns an the array of JsonData of this element, exception if of wrong type
		/// </summary>
		/// <returns>The array data</returns>
		const JsonArray& GetArrayData() const;

		/// <summary>
		/// Just for completion. Will always return NULL, exception if of wrong type
		/// </summary>
		/// <returns>NULL</returns>
		short GetNullData();

		/// <summary>
		/// Will return this elements parent object, exception if this object has no parent
		/// </summary>
		/// <returns>A reference to this objects parent</returns>
		JsonData& GetParent();

		/// <summary>
		/// Will return this elements parent object, exception if this object has no parent
		/// </summary>
		/// <returns>A reference to this objects parent</returns>
		const JsonData& GetParent() const;

		/// <summary>
		/// Will return if this object has a parent or not
		/// </summary>
		/// <returns>Whether or not this JsonObject has a parent</returns>
		bool HasParent() const;

		/* SETTERS */

		/// <summary>
		/// Sets this element to be of type bool, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		void SetBoolData(const bool data);

		/// <summary>
		/// Sets this element to be of type int, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		void SetIntData(const long long int data);

		/// <summary>
		/// Sets this element to be of type int, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		void SetIntData(const int data);

		/// <summary>
		/// Sets this element to be of type float, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		void SetFloatData(const long double data);

		/// <summary>
		/// Sets this element to be of type string, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		void SetStringData(const std::string data);

		/// <summary>
		/// Sets this element to be of type json, and initializes it with a copy of the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		JsonBlock& SetJsonData(const JsonBlock data);

		/// <summary>
		/// Sets this element to be of type array, and initializes it with the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		JsonArray& SetArrayData(const std::vector<JsonData> data);

		/// <summary>
		/// Sets this element to be of type array, and initializes it with a copy of the data given
		/// </summary>
		/// <param name="data">The data to be set</param>
		JsonArray& SetArrayData(const JsonArray data);

		/// <summary>
		/// Sets this element to be of type null, and initializes it with 'null'
		/// </summary>
		void SetNull();

		
		/* Controls */

		/// <summary>
		/// Will render a json string for this object.
		/// </summary>
		/// <param name="minify">Set to true if you want to get a minified render</param>
		/// <returns>The rendered string</returns>
		std::string Render(const bool minify = false) const;

		/// <summary>
		/// Will parse the given json code snippet and overwrite itself with the json data given
		/// </summary>
		/// <param name="jsonCode">The json code to be parsed</param>
		void Parse(const std::string jsonCode);

		/// <summary>
		/// Will clone itself and all children by value to the supplied object
		/// </summary>
		/// <param name="other">The JsonData object to deep-copy to. This object will get overwritten!</param>
		void CloneTo(JsonData& other) const;

		/// <summary>
		/// Will clone itself and all children by value from the supplied object
		/// </summary>
		/// <param name="other">The JsonData object get deep-coped from!</param>
		void CloneFrom(const JsonData& other);

		/// <summary>
		/// Compares to another JsonObject, if they are the semantically identical
		/// </summary>
		/// <param name="other">The JsonData object to compare against</param>
		/// <returns>Whether or not these two JsonData objects are semantically identical</returns>
		bool IsIdentical(const JsonData& other) const;

		/// <summary>
		/// Will set this floating points precision for comparisons and rendering. Pass 0.01f if you want a precision of two decimals
		/// </summary>
		/// <param name="precision">The precision to be set in format 0.00001</param>
		void SetFloatPrecision(const double precision);

		/* Operator overloads */

		/// <summary>
		/// Same as calling AsJson[label]
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const std::string label);

		/// <summary>
		/// Same as calling AsJson[label]
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const char label[]);

		/// <summary>
		/// Same as calling AsJson[label]
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const std::string label) const;

		/// <summary>
		/// Same as calling AsJson[label]
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const char label[]) const;

		/// <summary>
		/// Same as calling AsArray[idx]
		/// </summary>
		// <param name="idx">index to look at</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const std::size_t idx);

		/// <summary>
		/// Same as calling AsArray[idx]
		/// </summary>
		// <param name="idx">index to look at</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const int idx);

		/// <summary>
		/// Same as calling AsArray[idx]
		/// </summary>
		// <param name="idx">index to look at</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const std::size_t idx) const;

		/// <summary>
		/// Same as calling AsArray[idx]
		/// </summary>
		// <param name="idx">index to look at</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const int idx) const;

		/// <summary>
		/// Compares wether or not these two JsonData objects are semantically identical. => IsIdentical()
		/// </summary>
		/// <param name="other">The object to be compared against</param>
		/// <returns>If the arrays are semantically identical or not</returns>
		bool operator==(const JsonData& other) const;

		/// <summary>
		/// Compares wether or not these two JsonData objects NOT are semantically identical. => IsIdentical()
		/// </summary>
		/// <param name="other">The object to be compared against</param>
		/// <returns>If the arrays are semantically identical or not</returns>
		bool operator!=(const JsonData& other) const;

		/// <summary>
		/// Will append ele to this objects json data. Will throw an exception, if not of type json data
		/// </summary>
		/// <param name="ele">Eelement to be added</param>
		/// <returns>A reference to the new JsonData object, sitting in the JsonBlock</returns>
		JsonData& operator+=(const JsonElement ele);

		/// <summary>
		/// Will deep-copy from another Json
		/// </summary>
		/// <param name="other">Data to be cloned from</param>
		void operator=(const JsonData other);

		/// <summary>
		/// Will set a boolean value
		/// </summary>
		/// <param name="b">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const bool b);

		/// <summary>
		/// Will set an integer value
		/// </summary>
		/// <param name="i">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const int i);

		/// <summary>
		/// Will set an integer value
		/// </summary>
		/// <param name="lli">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const long long int lli);

		/// <summary>
		/// Will set a floating point value
		/// </summary>
		/// <param name="d">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const double d);

		/// <summary>
		/// Will set a floating point value
		/// </summary>
		/// <param name="d">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(long double ld);

		/// <summary>
		/// Will set a string value
		/// </summary>
		/// <param name="s">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const std::string s);

		/// <summary>
		/// Will set a string value
		/// </summary>
		/// <param name="s">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const char s[]);

		/// <summary>
		/// Will set a JsonBlock value
		/// </summary>
		/// <param name="jb">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const JsonBlock jb); // Not using ref because of inline declaration

		/// <summary>
		/// Will set an array value
		/// </summary>
		/// <param name="arr">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const JsonArray arr); // Not using ref because of inline declaration

		/// <summary>
		/// Will set an array value
		/// </summary>
		/// <param name="arr">Value to be set</param>
		/// <returns>The json object itself</returns>
		JsonData& operator=(const std::vector<JsonData> arr); // Not using ref because of inline declaration

		/// <summary>
		/// Will attempt to get the objects boolean data
		/// </summary>
		operator bool() const;

		/// <summary>
		/// Will attempt to get the objects integer data
		/// </summary>
		operator int() const;

		/// <summary>
		/// Will attempt to get the objects integer data
		/// </summary>
		operator long int() const;

		/// <summary>
		/// Will attempt to get the objects integer data
		/// </summary>
		operator long long int() const;

		/// <summary>
		/// Will attempt to get the objects floating point data
		/// </summary>
		operator float() const;

		/// <summary>
		/// Will attempt to get the objects floating point data
		/// </summary>
		operator double() const;

		/// <summary>
		/// Will attempt to get the objects floating point data
		/// </summary>
		operator long double() const;

		/// <summary>
		/// Will return the objects rendered json OR get it's string value, if of type String
		/// </summary>
		operator std::string() const;

	private:
		/// <summary>
		/// Will set this element to be of type ARRAY and sets it's arrayData pointer. Frees old pointer
		/// </summary>
		/// <param name="p">The pointer to be set</param>
		/// <returns>The pointer as a reference</returns>
		JsonArray& SetArrayDataAsPointer(JsonArray* p);

		/// <summary>
		/// Will set this element to be of type JSON and sets it's jsonData pointer. Frees old pointer
		/// </summary>
		/// <param name="p">The pointer to be set</param>
		/// <returns>The pointer as a reference</returns>
		JsonBlock& SetJsonDataAsPointer(JsonBlock* p);

		/// <summary>
		/// Simple initializer to be called in constructors
		/// </summary>
		void Init();

		/// <summary>
		/// Throw an exception
		/// </summary>
		/// <param name="toFetch">The json type the user requested</param>
		void ThrowDataTypeException(const JSON_DATA_TYPE toFetch) const;

		/// <summary>
		/// Will render a json string for this object.
		/// </summary>
		/// <param name="num_tabs">The number of tabs be already indented</param>
		/// <param name="minify">Whether or not render minified</param>
		/// <returns>The rendered string</returns>
		std::string Render(unsigned int num_tabs, const bool minify) const;

		long long int intData; // Will also use intVal as boolVal (intVal != 0)
		long double floatData;
		std::string stringData;
		JsonBlock* jsonData;
		JsonArray* arrayData;

		JSON_DATA_TYPE dataType;
		JsonData* parentNode = nullptr;
		double customFloatPrecision = -1;

		friend class JsonElement;
		friend class JsonBlock;
		friend class JsonArray;
		friend std::ostream& operator<<(std::ostream& os, const JsonData& jd);
	};
}

namespace JasonPP
{
	class JsonBlock;

	/// <summary>
	/// A single json element. Complete with label and value
	/// </summary>
	class JsonElement
	{
	public:

		/* Constructors*/

		/// <summary>
		/// Initializes json element "label" as a null element
		/// </summary>
		/// <param name="label">Name of the element</param>
		JsonElement(const std::string label);

		/// <summary>
		/// Initializes json element "label" as a bool element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const bool data);

		/// <summary>
		/// Initializes json element "label" as an int element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const long long int data);

		/// <summary>
		/// Initializes json element "label" as an int element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const int data);

		/// <summary>
		/// Initializes json element "label" as an float element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const long double data);

		/// <summary>
		/// Initializes json element "label" as an float element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const double data);

		/// <summary>
		/// Initializes json element "label" as an string element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const std::string data);

		/// <summary>
		/// Initializes json element "label" as an string element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const char data[]); // Compatibility for normal char strings

		/// <summary>
		/// Initializes json element "label" as an struct element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const JsonBlock data);

		/// <summary>
		/// Initializes json element "label" as an array element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const std::vector<JsonData> data);

		/// <summary>
		/// Initializes json element "label" as an array element
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const JsonArray data);

		/// <summary>
		/// Initializes json element "label" by existing data
		/// </summary>
		/// <param name="label">The label of the element</param>
		/// <param name="data">The data of the element</param>
		JsonElement(const std::string label, const JsonData data);

		/// <summary>
		/// Will initialize as a deep-copy copy of an existing JsonElement
		/// </summary>
		/// <param name="other">The JsonElement to get deep-copied</param>
		JsonElement(const JsonElement& other);

		~JsonElement();

		/* Getters */

		/// <summary>
		/// Will return this elements JsonData
		/// </summary>
		/// <returns>The JsonData object</returns>
		JsonData* GetData() { return data; }

		/// <summary>
		/// Will return this elements JsonData
		/// </summary>
		/// <returns>The JsonData object</returns>
		const JsonData* GetData() const { return data; };

		/// <summary>
		/// Will return this elements label / key
		/// </summary>
		/// <returns>The label / key</returns>
		std::string GetLabel() const { return label; };

		/* Setters */

		/// <summary>
		/// Sets the label of this json element
		/// </summary>
		/// <param name="label">The new label to be set</param>
		void SetLabel(const std::string label) { this->label = label; };

	private:
		/// <summary>
		/// Will render a json string for this object
		/// </summary>
		/// <param name="num_tabs">The number of tabs be already indented</param>
		/// <param name="minify">Whether or not render minified</param>
		/// <returns>The rendered string</returns>
		std::string Render(unsigned int num_tabs, const bool minify) const;

		/// <summary>
		/// Will set the JsonData pointer. Frees old pointer
		/// </summary>
		/// <param name="p">New pointer</param>
		void SetJsonDataPointer(JsonData* p);

		/// <summary>
		/// Will parse the given json code snippet of a json element and overwrite itself with the json data given
		/// </summary>
		/// <param name="jsonCode">The json code to be parsed</param>
		void Parse(const std::string jsonCode);

		JsonData* data;
		std::string label;

		friend class JsonBlock;
	};
}

namespace JasonPP
{
	/// <summary>
	/// A complete json object with multiple elements
	/// </summary>
	class JsonBlock
	{
	public:
		/// <summary>
		/// Will construct an empty Json object
		/// </summary>
		JsonBlock();

		/// <summary>
		/// Will construct itself as a clone of another given JsonBlock object
		/// </summary>
		/// <param name="other">The reference to be copied</param>
		JsonBlock(const JsonBlock& other);

		~JsonBlock();

		/// <summary>
		/// Will construct a Json object with initial elements
		/// </summary>
		/// <param name="initialElements">A vector of elements to be copied from</param>
		JsonBlock(const std::vector<JsonElement> initialElements);

		/// <summary>
		/// Returns wether or not a json element of a certain label exists in this level
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be checked against</param>
		/// <returns>If it exists</returns>
		bool DoesExist(const std::string label) const;

		/// <summary>
		/// Will return a json field by it's label or create an empty field if that label does not exist yet.
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be checked against</param>
		/// <returns>The new or existing element</returns>
		JsonData& Set(const std::string label);

		/// <summary>
		/// Will overwrite or create an existing JsonElement based on the label of the supplied JsonElement.
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be checked against</param>
		/// <returns>The new or existing element</returns>
		JsonData& Set(const JsonElement ele);

		/// <summary>
		/// Will get an element by its label and will throw an exception if that label does not exist. Use Json::Exists()!
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be checked against</param>
		/// <returns>The existing element</returns>
		JsonData& Get(const std::string label);

		/// <summary>
		/// Will get an element by its label and will throw an exception if that label does not exist. Use Json::Exists()!
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be checked against</param>
		/// <returns>The existing element</returns>
		const JsonData& Get(const std::string label) const;

		/// <summary>
		/// Will create a new element by a label and return its data object. Will throw an exception if that label is already taken! Use Json::Exists()!
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be added</param>
		/// <returns>The new element</returns>
		JsonData& Add(const std::string label);

		/// <summary>
		/// Will create a new element by an existing element. Will throw an exception if that label is already taken! Use Json::Exists()!
		/// </summary>
		/// <param name="ele">The element to be added</param>
		/// <returns>The new element</returns>
		JsonData& Add(const JsonElement ele);

		/// <summary>
		/// Will remove an element given it's label. Will throw an exception if it does not exist
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be removed</param>
		void Remove(const std::string label);

		/// <summary>
		/// Will try to remove an element. Will return false if it fails for any reason but will NOT throw JasonPP-based exceptions
		/// </summary>
		/// <param name="label">The label, a.k.a. field name, a.k.a. key to be removed</param>
		bool TryRemove(const std::string label);

		/// <summary>
		/// Returns how many direct(!!) children this json object has
		/// </summary>
		/// <returns>The number of direct children</returns>
		std::size_t GetNumElements() const;

		/// <summary>
		/// Returns the raw vector of elements.
		/// </summary>
		/// <returns>The raw vector of elements</returns>
		std::vector<JsonElement*>* GetRawElements() { return content; };

		/// <summary>
		/// Returns the raw vector of elements.
		/// </summary>
		/// <returns>The raw vector of elements</returns>
		const std::vector<JsonElement*>* GetRawElements() const { return content; };

		/// <summary>
		/// Returns wether or not a given shorthand is valid. F.E. lights.sets.kitchen
		/// </summary>
		/// <returns>If it exists</returns>
		bool DoesShorthandExist(const std::string shorthand, const  char delimiter = '.') const;

		/// <summary>
		/// Returns wether or not a given shorthand is valid. F.E. lights.sets.kitchen
		/// </summary>
		/// <returns>If it exists</returns>
		bool DoesShorthandExist(const std::string shorthand, const std::string delimiter) const;

		/// <summary>
		/// Will return a json element via a shorthand. F.E. lights.sets.kitchen Will throw an exception if the shorthand is invalid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing element</returns>
		JsonData& ShorthandGet(const std::string shorthand, const char delimiter = '.');

		/// <summary>
		/// Will return a json element via a shorthand. F.E. lights.sets.kitchen Will throw an exception if the shorthand is invalid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing element</returns>
		const JsonData& ShorthandGet(const std::string shorthand, const char delimiter = '.') const;

		/// <summary>
		/// Will return a json element via a shorthand. F.E. lights.sets.kitchen Will throw an exception if the shorthand is invalid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing element</returns>
		const JsonData& ShorthandGet(const std::string shorthand, const std::string delimiter) const;

		/// <summary>
		/// Will return a json element via a shorthand. F.E. lights.sets.kitchen Will throw an exception if the shorthand is invalid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing element</returns>
		JsonData& ShorthandGet(const std::string shorthand, const std::string delimiter);

		/// <summary>
		/// Will create a json element via a shorthand. F.E. lights.sets.kitchen Will create json structs for every nonexistent path segment. Will throw an exception if the last segment already exists or a path segment is non-json
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The new element</returns>
		JsonData& ShorthandAdd(const std::string shorthand, const char delimiter = '.');

		/// <summary>
		/// Will create a json element via a shorthand. F.E. lights.sets.kitchen Will create json structs for every nonexistent path segment. Will throw an exception if the last segment already exists or a path segment is non-json
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The new element</returns>
		JsonData& ShorthandAdd(const std::string shorthand, const std::string delimiter);

		/// <summary>
		/// Will return a json element via a shorthand or create an empty one at that position. F.E. lights.sets.kitchen Will create json structs for every nonexistent path segment. Will throw an exception if a path segment is non-json
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing or new element</returns>
		JsonData& ShorthandSet(const std::string shorthand, const char delimiter = '.');

		/// <summary>
		/// Will return a json element via a shorthand or create an empty one at that position. F.E. lights.sets.kitchen Will create json structs for every nonexistent path segment. Will throw an exception if a path segment is non-json
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <returns>The existing or new element</returns>
		JsonData& ShorthandSet(const std::string shorthand, const std::string delimiter);

		/// <summary>
		/// Will remove a json element by its shorthand. Only valid for direct members of a json struct! Will throw an exception if the shorthand is not valid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		/// <param name="delimiter"></param>
		void ShorthandRemove(const std::string shorthand, const char delimiter = '.');

		/// <summary>
		/// Will remove a json element by its shorthand. Only valid for direct members of a json struct! Will throw an exception if the shorthand is not valid
		/// </summary>
		/// <param name="shorthand">The shorthand to be used. Eg my.complex.element</param>
		/// <param name="delimiter">The delimiter to split the shorthand into "path"-segments</param>
		void ShorthandRemove(const std::string shorthand, const std::string delimiter);

		/// <summary>
		/// Will remove all elements from this object
		/// </summary>
		void Clear();

		/// <summary>
		/// Will clone itself and all children by value to the supplied Object
		/// <param name="other">The object to be cloned to. This object will get overwritten!</param>
		/// </summary>
		void CloneTo(JsonBlock& other) const;

		/// <summary>
		/// Will clone itself and all children by value from the supplied Object
		/// <param name="other">The object to be cloned from</param>
		/// </summary>
		void CloneFrom(const JsonBlock& other);

		/// <summary>
		/// Compares to another JsonBlock, if they are semantically identical
		/// </summary>
		/// <param name="other">The JsonBlock to be compared against</param>
		/// <returns>Whether or not these two JsonBlocks are semantically identical</returns>
		bool IsIdentical(const JsonBlock& other) const;

		/* Operator overloads */

		/// <summary>
		/// Will return an existing element by its label or create a new one
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const std::string label);

		/// <summary>
		/// Will return an existing element by its label or create a new one
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		JsonData& operator[](const char label[]);

		/// <summary>
		/// Will return an existing element by its label or create a new one
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const std::string label) const;

		/// <summary>
		/// Will return an existing element by its label or create a new one
		/// </summary>
		/// <param name="label">The label to create or check against</param>
		/// <returns>The new or existing element</returns>
		const JsonData& operator[](const char label[]) const;

		/// <summary>
		/// Will add a new element of type null
		/// </summary>
		/// <param name="label">The label of the new element</param>
		/// <returns>The new element</returns>
		JsonData& operator+=(const std::string label);

		/// <summary>
		/// Will add a new Element by cloning an existing one
		/// </summary>
		/// <param name="ele">The element to be cloned from</param>
		/// <returns>The new element</returns>
		JsonData& operator+=(const JsonElement ele);

		/// <summary>
		/// Will remove an existing element by its label
		/// </summary>
		/// <param name="label">The label to be removed</param>
		/// <returns>Itself</returns>
		JsonBlock& operator-=(const std::string label);

		/// <summary>
		/// Compares to another JsonBlock, if they are semantically identical
		/// </summary>
		/// <param name="other">The JsonBlock to be compared against</param>
		/// <returns>Whether or not these two JsonBlocks are semantically identical</returns>
		bool operator==(const JsonBlock& other) const;

		/// <summary>
		/// Compares to another JsonBlock, if they are NOT semantically identical
		/// </summary>
		/// <param name="other">The JsonBlock to be compared against</param>
		/// <returns>Whether or not these two JsonBlocks are NOT semantically identical</returns>
		bool operator!=(const JsonBlock& other) const;

	private:
		/// <summary>
		/// Will render a json string for this object
		/// </summary>
		/// <param name="num_tabs">The number of tabs be already indented</param>
		/// <param name="minify">Whether or not render minified</param>
		/// <returns>The rendered string</returns>
		std::string Render(unsigned int num_tabs, const bool minify) const;

		/// <summary>
		/// Will parse the given json code and overwrite the current object with it
		/// </summary>
		/// <param name="json">The code to parse</param>
		void Parse(const std::string jsonCode);

		/// <summary>
		/// Will set tha value of its associated JsonData node, and also progagates that value to all direct children
		/// </summary>
		/// <param name="newNode">A pointer of the JsonData node to set to itself and all direct children</param>
		void SetAssociatedJsonDataNode(JsonData* newNode);

		/// <summary>
		/// Will return a pointer to a std::vector containing pointers to all elements, but in an alphabetical order.
		/// You have to free the pointer, but don't delete the elements!!!
		/// </summary>
		/// <returns>A pointer to a std::vector of pointers to all elements, but in alphabetical order </returns>
		std::vector<JsonElement*>* GetElementsSortedByLabel() const;

		std::vector<JsonElement*>* content = nullptr;
		JsonData* associatedJsonDataNode = nullptr; // This is the JsonData element associated with this JsonBlock

		friend class JsonData;
	};
}

#define JASONPP_VERSION (1.022)

namespace JasonPP
{
	typedef JsonData Json;
}

namespace JasonPP
{
	typedef JSON_DATA_TYPE JDType;
}


namespace JasonPP
{
	typedef std::basic_stringstream<char16_t> u16stringstream;
	typedef std::basic_ofstream<char16_t> u16ofstream;
	typedef std::basic_ifstream<char16_t> u16ifstream;
	typedef std::basic_fstream<char16_t> u16fstream;

	class UTF16Compatibility
	{
	public:
		/// <summary>
		/// Will parse values like '\u2121' all the way to '\uffff'
		/// </summary>
		/// <param name="str"></param>
		/// <returns></returns>
		static std::u16string ParseUTF16(const std::string str);

		/// <summary>
		/// Will escape utf8-characters to fit into a normal std::string
		/// </summary>
		/// <param name="str">The utf16 string</param>
		/// <returns>A std::string with escaped utf-16 characters</returns>
		static std::string EscapeUTF16(const std::u16string str);

		/// <summary>
		/// Will convert str to an std::wstring
		/// </summary>
		/// <param name="str">To be converted</param>
		/// <returns>The wide string</returns>
		static std::u16string U8_2_U16(const std::string str);

		/// <summary>
		/// Will convert str to an std::string
		/// </summary>
		/// <param name="str">The utf16-string to be converted</param>
		/// <returns>The regular std::string</returns>
		static std::string U16_2_U8(const std::u16string str);
	};
}

#define AsNull GetNullData()
#define AsBool GetBoolData()
#define AsInt GetIntData()
#define AsFloat GetFloatData()
#define AsString GetStringData()
#define AsJson GetJsonData()
#define AsArray GetArrayData()

#define Arr std::vector<::JasonPP::JsonData>
#define Ele ::JasonPP::JsonElement

#endif

