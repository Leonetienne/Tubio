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
#include "JasonPP.hpp"


using namespace JasonPP;
using namespace JasonPP::Internal;

bool Helpers::AreSame(const double a, const double b, const double epsilon)
{
	return fabs(a - b) < epsilon;
}

bool Helpers::AreSame(const long double a, const long double b, const double epsilon)
{
	return fabsl(a - b) < epsilon;
}

unsigned long long int Helpers::Powuli(const unsigned long int b, const unsigned long int e)
{
	unsigned long long int buf = 1;

	for (unsigned long int i = 0; i < e; i++)
	{
		buf *= b;
	}
	return buf;
}

std::string Helpers::Base10_2_X(const unsigned long long int i, const std::string set, unsigned int padding)
{
	if (set.length() == 0) return ""; // Return empty string, if set is empty. Play stupid games, win stupid prizes.

	std::stringstream ss;
	
	if (i != 0)
	{
		{
			unsigned long long int buf = i;
			while (buf != 0)
			{
				unsigned long long int mod = buf % set.length();
				buf /= set.length();
				ss << set[(std::size_t)mod];
			}
		}
		{
			std::string buf = ss.str();
			ss.str("");
			for (long long int i = buf.length() - 1; i >= 0; i--) ss << buf[(std::size_t)i];
		}
	}
	else
	{
		ss << set[0]; // If i is 0, just pass a null-value. The algorithm would hang otherwise.
	}

	// Add as much null-values to the left as requested.
	if (ss.str().length() < padding)
	{
		std::size_t cachedLen = ss.str().length();
		std::string cachedStr = ss.str();
		ss.str("");
		for (std::size_t i = 0; i < padding - cachedLen; i++)
		{
			ss << set[0];
		}
		ss << cachedStr;
	}

	return ss.str();
}

unsigned long long int Helpers::BaseX_2_10(const std::string num, const std::string set)
{
	// No special cases for num / set length 0 needed. Already taken care of by for-loops.

	unsigned long long int buf = 0;
	for (std::size_t i = 0; i < num.length(); i++)
	{
		for (std::size_t j = 0; j < set.length(); j++)
		{
			if (num[i] == set[j])
			{
				buf += Powuli((unsigned long int)set.length(), (unsigned long int)(num.length() - (i + 1))) * j;
				break;
			}
		}
	}

	return buf;
}

using namespace JasonPP;
using namespace JasonPP::Internal;

Jstring::Jstring()
{
	return;
}

Jstring::Jstring(const std::string str)
{
	this->operator=(str);
	return;
}

Jstring::Jstring(const char c)
{
	std::stringstream ss;
	ss << c;
	this->operator=(ss.str());
	return;
}

Jstring::Jstring(const int i)
{
	std::stringstream ss;
	ss << i;
	this->operator=(ss.str());
	return;
}

Jstring::Jstring(const long long int lli)
{
	std::stringstream ss;
	ss << lli;
	this->operator=(ss.str());
	return;
}

Jstring::Jstring(const double d)
{
	std::stringstream ss;
	ss << d;
	this->operator=(ss.str());
	return;
}

Jstring::Jstring(const long double d)
{

	std::stringstream ss;
	ss << d;
	this->operator=(ss.str());
	return;
}

using namespace JasonPP;
using namespace JasonPP::Internal;

JsonArray::JsonArray()
{
	Init();
	return;
}

JsonArray::JsonArray(const JsonArray& other)
{
	Init();
	CloneFrom(other);

	return;
}

JsonArray::JsonArray(const std::vector<JsonData> data)
{
	Init();
	Add(data);

	return;
}

void JsonArray::Init()
{
	content = new std::vector<JsonData*>();
	associatedJsonDataNode = nullptr;

	return;
}

JsonArray::~JsonArray()
{
	Helpers::FreeVector(content);

	return;
}

std::vector<JsonData*>* JsonArray::GetVector()
{
	return content;
}

std::size_t JsonArray::Size() const
{
	return content->size();
}

void JsonArray::Clear()
{
	Helpers::FreeVector(content);
	content = new std::vector<JsonData*>();

	return;
}

JsonData& JsonArray::At(const std::size_t idx)
{
	if (idx >= content->size()) throw JsonArrayOutOfRangeException(idx, content->size());
	return *content->at(idx);
}

const JsonData& JsonArray::At(const std::size_t idx) const
{
	if (idx >= content->size()) throw JsonArrayOutOfRangeException(idx, content->size());
	return *content->at(idx);
}

void JsonArray::RemoveAt(const std::size_t idx)
{
	if (idx >= content->size()) throw JsonArrayOutOfRangeException(idx, content->size());

	delete content->at(idx);
	content->erase(content->begin() + idx);

	return;
}

std::size_t JsonArray::RemoveSimilar(const JsonData reference)
{
	std::size_t counter = 0;
	for (long long int i = content->size() - 1; i >= 0; i--)
	{
		if (content->at((std::size_t)i)->IsIdentical(reference))
		{
			RemoveAt((std::size_t)i);
			counter++;
		}
	}

	return counter;
}

std::size_t JsonArray::RemoveAllOfType(const JDType type)
{
	std::size_t counter = 0;
	for (long long int i = content->size() - 1; i >= 0; i--)
	{
		if (content->at((std::size_t)i)->GetDataType() == type)
		{
			RemoveAt((std::size_t)i);
			counter++;
		}
	}

	return counter;
}

std::size_t JsonArray::RemoveAllExceptType(const JDType type)
{
	std::size_t counter = 0;
	for (long long int i = content->size() - 1; i >= 0; i--)
	{
		if (content->at((std::size_t)i)->GetDataType() != type)
		{
			RemoveAt((std::size_t)i);
			counter++;
		}
	}
	return counter;
}

void JsonArray::InsertExistingtJsonData(JsonData* data)
{
	content->push_back(data);
	return;
}

JsonData& JsonArray::Add()
{
	JsonData* newData = new JsonData;
	InsertExistingtJsonData(newData);
	return *newData;
}

JsonData& JsonArray::Add(const bool data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<bool> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const long long int data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<long long int> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const int data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<int> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const double data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<double> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const long double data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<long double> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const std::string data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

JsonData& JsonArray::Add(const char data[])
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<std::string> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const JsonArray data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<JsonArray> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const JsonData data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<JsonData> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

JsonData& JsonArray::Add(const JsonBlock data)
{
	JsonData* newData = new JsonData(data);
	InsertExistingtJsonData(newData);
	return *newData;
}

void JsonArray::Add(const std::vector<JsonBlock> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		Add(data[i]);
	}
	return;
}

void JsonArray::InsertExistingtJsonData(const std::vector<JsonData*> data)
{
	for (std::size_t i = 0; i < data.size(); i++)
	{
		InsertExistingtJsonData(data[i]);
	}

	return;
}

void JsonArray::SetAssociatedJsonDataNode(JsonData* newNode)
{
	associatedJsonDataNode = newNode;

	for (std::size_t i = 0; i < content->size(); i++)
	{
		content->at(i)->parentNode = newNode;
	}

	return;
}

void JsonArray::CloneFrom(const JsonArray& other)
{
	Clear();

	for (std::size_t i = 0; i < other.content->size(); i++)
	{
		// We do not have to set the new parent element here, because it gets set set at SetArrayData()
		JsonData* newData = new JsonData(*other.content->at(i));
		InsertExistingtJsonData(newData);
	}

	return;
}

void JsonArray::CloneTo(JsonArray& other) const
{
	other.CloneFrom(*this);

	return;
}

bool JsonArray::IsIdentical(const JsonArray& other) const
{
	if (content->size() != other.content->size()) return false;
	if (content->size() == 0) return true; // Both vectors are empty => identical

	for (unsigned i = 0; i < content->size(); i++)
	{
		if (content->at(i)->dataType != other.content->at(i)->dataType) return false;
		if (!content->at(i)->IsIdentical(*other.content->at(i))) return false;
	}

	return true;
}

void JsonArray::Sort(const std::string shorthandKey, const JSON_ARRAY_SORT_MODE mode, const std::string shorthandDelimiter)
{
	// Want some bubble  t e a   with your bubble   s o r t? ^_^
	for (std::size_t i = 0; i < Size(); i++)
	{
		for (std::size_t j = 0; j < Size() - i - 1; j++)
		{
			// First, cache array entries
			const JsonData* a = &At(j);
			const JsonData* b = &At(j + 1);

			// Check if they are of type json (this is the json sorter) (deep sort)
			if ((a->GetDataType() == JDType::JSON) && (b->GetDataType() == JDType::JSON))
			{
				// Check if the requested key even exists
				if ((a->GetJsonData().DoesShorthandExist(shorthandKey, shorthandDelimiter)) &&
					(b->GetJsonData().DoesShorthandExist(shorthandKey, shorthandDelimiter)))
				{
					// only then, get and compare them
					a = &a->GetJsonData().ShorthandGet(shorthandKey, shorthandDelimiter);
					b = &b->GetJsonData().ShorthandGet(shorthandKey, shorthandDelimiter);

					if (Sort__Compare(*a, *b, mode))
					{
						Swap(j, j + 1);
					}
				}
				else Swap(j, j + 1); // We gotta swap here aswell, to prevent invalid types from fucking up the order
			}
			else Swap(j, j + 1); // We gotta swap here aswell, to prevent invalid types from fucking up the order
		}
	}

	return;
}

void JsonArray::Sort(const JSON_ARRAY_SORT_MODE mode)
{
	for (std::size_t i = 0; i < Size(); i++)
	{
		for (std::size_t j = 0; j < Size() - i - 1; j++)
		{
			// First, cache array entries
			const JsonData& a = At(j);
			const JsonData& b = At(j + 1);

			// Only if neither a or b's are neither of type json or array
			// This is the "shallow"-sort
			if (!(((a.GetDataType() == JDType::JSON) || (a.GetDataType() == JDType::ARRAY)) &&
				((b.GetDataType() == JDType::JSON) || (b.GetDataType() == JDType::ARRAY))))
			{
				if (Sort__Compare(a, b, mode))
				{
					Swap(j, j + 1);
				}
			}
			else Swap(j, j + 1);
		}
	}

	return;
}

bool JsonArray::Sort__Compare(const JsonData& a, const JsonData& b, const JSON_ARRAY_SORT_MODE mode)
{
	switch (mode)
	{
	case JSON_ARRAY_SORT_MODE::ALPH_ASC:
		// I have to scope this because of c++ reasons
	{
		std::string aStr;
		std::string bStr;

		// This basically means:
		// If of type STRING, just get its string value.
		// If it's BOOL, INT or FLOAT, just get it's value as string. Eg "53", "53.2" or "false"
		// This way numerics can still be sorted alphabetically
		// Also allows for sorting after bools
		if (a.GetDataType() == JDType::STRING) aStr = a.GetStringData();
		else if ((a.GetDataType() != JDType::JSON) && (a.GetDataType() != JDType::ARRAY)) aStr = a.Render();
		else return true; // Datatype invalid. Swap, to keep the others in order.
		if (b.GetDataType() == JDType::STRING) bStr = b.GetStringData();
		else if ((b.GetDataType() != JDType::JSON) && (b.GetDataType() != JDType::ARRAY)) bStr = b.Render();
		else return true; // Datatype invalid. Swap, to keep the others in order.

		return StringHelpers::SortDescriminator_Alphabetically(aStr, bStr);
	}
		break;

	case JSON_ARRAY_SORT_MODE::ALPH_DESC:
		// I have to scope this because of c++ reasons
	{
		std::string aStr;
		std::string bStr;

		// This basically means:
		// If of type STRING, just get its string value.
		// If it's BOOL, INT or FLOAT, just get it's value as string. Eg "53", "53.2" or "false"
		// This way numerics can still be sorted alphabetically
		// Also allows for sorting after bools
		if (a.GetDataType() == JDType::STRING) aStr = a.GetStringData();
		else if ((a.GetDataType() != JDType::JSON) && (a.GetDataType() != JDType::ARRAY)) aStr = a.Render();
		else return true; // Datatype invalid. Swap, to keep the others in order.
		if (b.GetDataType() == JDType::STRING) bStr = b.GetStringData();
		else if ((b.GetDataType() != JDType::JSON) && (b.GetDataType() != JDType::ARRAY)) bStr = b.Render();
		else return true; // Datatype invalid. Swap, to keep the others in order.

		return StringHelpers::SortDescriminator_Alphabetically(bStr, aStr);
	}
	break;

	case JSON_ARRAY_SORT_MODE::NUM_ASC:
		// I have to scope this because of c++ reasons
	{
		long double dataA = 0;
		long double dataB = 0;

		switch (a.GetDataType())
		{
		case JDType::INT:
			dataA = (long double)a.GetIntData();
			break;
		case JDType::FLOAT:
			dataA = a.GetFloatData();
			break;
		case JDType::BOOL:
			dataA = a.GetBoolData() ? 1.0 : 0.0;
			break;
		default:
			return true;
		}
		switch (b.GetDataType())
		{
		case JDType::INT:
			dataB = (long double)b.GetIntData();
			break;
		case JDType::FLOAT:
			dataB = b.GetFloatData();
			break;
		case JDType::BOOL:
			dataB = b.GetBoolData() ? 1.0 : 0.0;
			break;
		default:
			return true;
		}

		return dataA >= dataB;
	}
	break;
	case JSON_ARRAY_SORT_MODE::NUM_DESC:
		// I have to scope this because of c++ reasons
	{
		long double dataA = 0;
		long double dataB = 0;

		switch (a.GetDataType())
		{
		case JDType::INT:
			dataA = (long double)a.GetIntData();
			break;
		case JDType::FLOAT:
			dataA = a.GetFloatData();
			break;
		case JDType::BOOL:
			dataA = a.GetBoolData() ? 1.0 : 0.0;
			break;
		default:
			return true;
		}
		switch (b.GetDataType())
		{
		case JDType::INT:
			dataB = (long double)b.GetIntData();
			break;
		case JDType::FLOAT:
			dataB = b.GetFloatData();
			break;
		case JDType::BOOL:
			dataB = b.GetBoolData() ? 1.0 : 0.0;
			break;
		default:
			return true;
		}

		return dataB >= dataA;
	}
	break;
	}

	return true;
}

void JsonArray::Swap(const std::size_t a, const std::size_t b)
{
	JsonData* ptrA = content->at(a);
	content->at(a) = content->at(b);
	content->at(b) = ptrA;

	return;
}

JsonData& JsonArray::operator[](const std::size_t idx)
{
	return At(idx);
}

const JsonData& JsonArray::operator[](const std::size_t idx) const
{
	return At(idx);
}

JsonArray& JsonArray::operator-=(const int data)
{
	RemoveSimilar(JsonData(data));
	return *this;
}

JsonArray& JsonArray::operator-=(const long long int data)
{
	RemoveSimilar(JsonData(data));
	return *this;
}

JsonArray& JsonArray::operator-=(const std::string data)
{
	RemoveSimilar(JsonData(data));
	return *this;
}

JsonArray& JsonArray::operator-=(const char data[])
{
	RemoveSimilar(JsonData(data));
	return *this;
}

JsonArray& JsonArray::operator-=(const JsonData& data)
{
	RemoveSimilar(data);
	return *this;
}

JsonArray& JsonArray::operator-=(const JDType type)
{
	RemoveAllOfType(type);
	return *this;
}

JsonArray& JsonArray::operator+=(const bool data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const int data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const long long int data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const double data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const long double data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::string data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const char data[])
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const JsonData data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const JsonBlock data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const JsonArray data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<bool> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<int> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<long long int> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<double> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<long double> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<std::string> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<JsonArray> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<JsonData> data)
{
	Add(data);
	return *this;
}

JsonArray& JsonArray::operator+=(const std::vector<JsonBlock> data)
{
	Add(data);
	return *this;
}

bool JsonArray::operator==(const JsonArray& other) const
{
	return IsIdentical(other);
}

bool JsonArray::operator!=(const JsonArray& other) const
{
	return !operator==(other);
}

void JsonArray::CopyJsonDataFromVector_Pointer(const std::vector<JsonData>* data)
{
	for (std::size_t i = 0; i < data->size(); i++)
	{
		Add(data->at(i));
	}
	return;
}

JsonData& JsonArray::AddPointer(JsonArray* data)
{
	JsonData* newData = new JsonData(*data);
	InsertExistingtJsonData(newData);
	return *newData;
}

JsonData& JsonArray::AddPointer(JsonBlock* data)
{
	JsonData* newData = new JsonData(*data);
	InsertExistingtJsonData(newData);
	return *newData;
}

std::string JsonArray::Render(unsigned int num_tabs, const bool minify) const
{
	std::stringstream ss;

	if (content->size() == 0) return "[]";

	ss << "[" << (minify ? "" : "\n");

	num_tabs++;

	for (std::size_t i = 0; i < content->size(); i++)
	{
		if (!minify) ss << StringHelpers::GetIndentation(num_tabs);

		ss << content->at(i)->Render(num_tabs, minify) << (i < (content->size() - 1) ? "," : "") << (minify ? "" : "\n");
	}

	num_tabs--;

	if (!minify) ss << StringHelpers::GetIndentation(num_tabs);
	ss << "]";

	return ss.str();
}

void JsonArray::Parse(const std::string jsonCode)
{
	// Minify code for easier parsing
	const std::string minifiedCode = StringHelpers::MinifyJson(jsonCode);

	if (minifiedCode.length() == 0) throw JsonParsingGeneralException("JsonArray-Parser got json code snippet of length 0!");

	// Get code snippet of each array entry
	std::vector<std::string> dataJsonSnippets;
	{
		std::size_t start = std::string::npos;
		std::size_t end = std::string::npos;
		int arrayBracketLevel = 0; // defines how deep we are in array brackets
		int curlyBracketLevel = 0; // defines how deep we are in curly brackets
		bool areWeInString = false;
		bool areWeInCode = false;
		bool isCharEscaped = false;
		bool areWeBetweenCommaAndValue = false; // Has the parser found a comma, but is still looking for the next value?

		for (std::size_t i = 0; i < minifiedCode.length(); i++)
		{
			const char c = minifiedCode[i];

			if ((!areWeInString) && (!areWeInCode) && (arrayBracketLevel == 1) && (curlyBracketLevel == 0))
			{
				start = i;
				areWeInCode = true;
				areWeBetweenCommaAndValue = true;
			}

			else if ((!areWeInString) && (areWeInCode) && (arrayBracketLevel == 1) && (curlyBracketLevel == 0) && ((c == ',') || (i == minifiedCode.length() - 1)))
			{
				if (c != ',') areWeBetweenCommaAndValue = false;
				end = i;
				areWeInCode = false;

				std::string codeSnippet = minifiedCode.substr(start, end - start);
				if (codeSnippet.length() > 0) // A json data snippet can't be of size 0!
				{
					dataJsonSnippets.push_back(codeSnippet);
				}
				else
				{
					// JsonElement too short to be valid.
					throw JsonParsingGeneralException(std::string("Fucked up a comma around position ") + Jstring((long long int)start));
				}
			}

			if ((!areWeInString) && (c == ']') && (arrayBracketLevel == 1) && (i != minifiedCode.length() - 1)) throw JsonParsingExpectedEOFException(minifiedCode.substr(i, minifiedCode.length() - i));

			if ((c == '\"') && (!areWeInString)) areWeInString = true;
			else if ((c == '\"') && (!isCharEscaped) && (areWeInString)) areWeInString = false;

			// No need to check for char escaping since we are already checking if we are in a string or not. Chars are ever escaped outside of strings
			if ((c == '[') && (!areWeInString)) arrayBracketLevel++;
			else if ((c == ']') && (!areWeInString)) arrayBracketLevel--;
			else if ((c == '{') && (!areWeInString)) curlyBracketLevel++;
			else if ((c == '}') && (!areWeInString)) curlyBracketLevel--;

			isCharEscaped = false;
			if ((c == '\\') && (!isCharEscaped) && (areWeInString)) isCharEscaped = true; // Should only matter for the next char!!
		}

		// Someone fucked up his json code
		if (arrayBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (curlyBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (areWeBetweenCommaAndValue) throw JsonParsingGeneralException("Unexpected EOF. Don't put a comma after the last value of a json array!");
		if (areWeInString) throw JsonParsingMissingQuotesException();
	}

	// Delegate further parsing to another instance of the JsonData component and initialize the array vector
	{
		Helpers::FreeVector(content);
		content = new std::vector<JsonData*>();
		for (std::size_t i = 0; i < dataJsonSnippets.size(); i++)
		{
			JsonData* newJsonData = new JsonData();
			newJsonData->Parse(dataJsonSnippets[i]);
			content->push_back(newJsonData);
		}
	}

	return;
}

bool JasonPP::IsJsonValid(const std::string code)
{
	try
	{
		JsonData jd;
		jd.Parse(code);
	}
	catch (const JsonException&)
	{
		return false;
	}

	return true;
}

std::string JasonPP::JsonDataType2String(const JDType type)
{
	switch (type)
	{
	case JDType::__NULL__:
		return std::string("NULL");

	case JDType::BOOL:
		return std::string("BOOL");

	case JDType::INT:
		return std::string("INT");

	case JDType::FLOAT:
		return std::string("FLOAT");

	case JDType::STRING:
		return std::string("STRING");

	case JDType::JSON:
		return std::string("JSON");

	case JDType::ARRAY:
		return std::string("ARRAY");
	}

	return std::string("UNREGISTERED - You dun fucked up");
};

using namespace JasonPP;
using namespace JasonPP::Internal;

std::string StringHelpers::Replace(const std::string str, const char find, const std::string subst)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != find) ss << str[i];
        else ss << subst;
    }

    return ss.str();
}

std::string StringHelpers::Replace(const std::string str, const std::string find, const std::string subst)
{
    if (find.length() == 0) return str;

    std::stringstream ss;

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(find, posFound);

        if (posFound != std::string::npos)
        {
            ss << str.substr(lastFound, posFound - lastFound) << subst;
            posFound += find.length();
        }
        else
        {
            ss << str.substr(lastFound, (str.length()) - lastFound);
        }
    }

    return ss.str();
}

std::string StringHelpers::Escape(const std::string str)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        switch (str[i])
        {
        case '\n':
            ss << "\\n";
            break;
        case '\r':
            ss << "\\r";
            break;
        case '\t':
            ss << "\\t";
            break;
        case '\f':
            ss << "\\f";
            break;
        case '\b':
            ss << "\\b";
            break;
        case '\"':
            ss << "\\\"";
            break;
        case '\\':
            ss << "\\\\";
            break;
        default:
            if (str[i] < 0) ss << EscapeUTF8(str[i]);
            else ss << str[i];
        }
    }

    return ss.str();
}

std::string StringHelpers::Unescape(const std::string str)
{
    std::stringstream ss;
    bool isEscaped = false;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        // If the current character is escaped
        if (isEscaped)
        {
            switch (str[i])
            {
            case 'n':
                ss << '\n';
                break;
            case 'r':
                ss << '\r';
                break;
            case 't':
                ss << '\t';
                break;
            case 'f':
                ss << '\f';
                break;
            case 'b':
                ss << '\b';

            case 'u':
                // Special procedure for UTF-8 sequences

                if (JASONPP_LEAVE_UTF8_ALONE) // Skip UTF-8 parsing if defined
                {
                    ss << '\\' << str[i];
                    break;
                }
                else
                {
                    if (str.length() < i + 5)
                    {
                        throw JsonParsingUTFSequenceException(str);
                    }
                    else
                    {
                        ss << ParseUTF8(str.substr(i + 1, 4));
                        i += 4;
                    }
                    break;
                }
            default:
                // Just read the character after the "\" without switching it out for anything
                ss << str[i];
                break;
            }
            isEscaped = false;
        }
        // The current character is the escape slash
        else if (str[i] == '\\')
        {
            isEscaped = true;
        }
        else // Character is not escaped. Just read it 'as is'
        {
            ss << str[i];
            isEscaped = false;
        }
    }

    return ss.str();
}

std::string StringHelpers::GetIndentation(const std::size_t num)
{
    std::stringstream ss;
    for (std::size_t i = 0; i < num * JASONPP_SPACES_PER_INDENT; i++) ss << ' ';
    return ss.str();
}

bool StringHelpers::IsNumeric(const std::string str, const bool allowDecimalPoint)
{
    if (str.length() == 0) return false;

    bool alreadyParsedDecimalPoint = false;
    std::size_t digitCount = 0;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if (!(
            ((str[i] >= '0') && (str[i] <= '9')) ||
            ((str[i] == '-') && (i == 0)) ||
            ((str[i] == '.') && (allowDecimalPoint) && (!alreadyParsedDecimalPoint) && (digitCount > 0))
            )) return false;

        // Here we just have to check for the character. Not for any other conditions.
        // Why? Because if these conditions failed, the function would have already returned false.
        if (((str[i] >= '0') && (str[i] <= '9'))) digitCount++;
        if (str[i] == '.') alreadyParsedDecimalPoint = true;
    }

    // Even if we did not find any invalid chars, we should still return false, if we found no digits at all.
    return digitCount > 0;
}

std::vector<std::string> StringHelpers::SplitString(const std::string str, const char delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    return SplitString(str, Jstring(delimiter));
}

std::vector<std::string> StringHelpers::SplitString(const std::string str, const std::string delimiter)
{
    if (str.length() == 0) return std::vector<std::string>();

    std::vector<std::string> parts;

    if (delimiter.length() == 0) // If the delimiter is "" (empty), just split between every single char. Not useful, but logical
    {
        for (std::size_t i = 0; i < str.length(); i++)
        {
            parts.push_back(Jstring(str[i]));
        }
        return parts;
    }

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find(delimiter, posFound);

        std::string found;

        if (posFound != std::string::npos)
        {
            found = str.substr(lastFound, posFound - lastFound);
            posFound += delimiter.length();
        }
        else
        {
            found = str.substr(lastFound, str.length() - lastFound);
        }

        parts.push_back(found);
    }

    return parts;
}

std::string StringHelpers::MinifyJson(const std::string jsonCode)
{
    std::stringstream ss;
    bool areWeInString = false;
    bool isCharEscaped = false;
    for (std::size_t i = 0; i < jsonCode.length(); i++)
    {
        const char c = jsonCode[i];

        if (areWeInString)
        {
            ss << c;
        }
        else
        {
            // Skip these parsing-irrelevant characters!
            if ((c != ' ') && (c != '\t') && (c != '\n') && (c != '\r')) ss << c;
        }

        if ((c == '\"') && (!areWeInString)) areWeInString = true;
        else if ((c == '\"') && (areWeInString) && (!isCharEscaped)) areWeInString = false;

        isCharEscaped = false;
        if ((c == '\\') && (!isCharEscaped) && (areWeInString)) isCharEscaped = true; // Should only matter for the next char!!
    }

    // Someone fucked up his json code
    if (areWeInString) throw JsonParsingMissingQuotesException();

    return ss.str();
}

bool StringHelpers::SortDescriminator_Alphabetically(const std::string a, const std::string b)
{
    std::string modA = ToLower(a);
    std::string modB = ToLower(b);

    std::stringstream ss;
    for (std::size_t i = 0; i < modA.length(); i++)
    {
        // Yes, fuggin negative chars. Checking against ae, ue, oe, etc
             if (modA[i] == -28) ss << 'a';
        else if (modA[i] == -10) ss << 'o';
        else if (modA[i] == -4 ) ss << 'u';
        else if (modA[i] == -33) ss << 's';
        else ss << modA[i];
    }
    modA = ss.str();
    ss.str("");
    for (std::size_t i = 0; i < modB.length(); i++)
    {
             if (modB[i] == -28) ss << 'a';
        else if (modB[i] == -10) ss << 'o';
        else if (modB[i] == -4 ) ss << 'u';
        else if (modB[i] == -33) ss << 's';
        else ss << modB[i];
    }
    modB = ss.str();
    ss.str("");

    for (std::size_t i = 0; i < Helpers::Min<std::size_t>(modA.length(), modB.length()); i++)
    {
        if (modA[i] != modB[i])
        {
            return modA[i] > modB[i];
        }
    }

    return a.length() > b.length();
}

std::string StringHelpers::ToLower(const std::string str)
{
    std::stringstream ss;
    for (std::size_t i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) ss << (char)(((int)str[i]) + 32);
        else if (str[i] == -60) ss << (char)-28; // AE => ae
        else if (str[i] == -42) ss << (char)-10; // OE => oe
        else if (str[i] == -36) ss << (char)-4;  // UE => ue
        else ss << str[i];
    }

    return ss.str();
}

std::string StringHelpers::ParseUTF8(const std::string str)
{
    // Expects input like "00ff" or "00b7".
    // Does NOT expect: "\u00ff" or "u00ff" or "b7" or anything other than [0-f]{4}

    std::stringstream ss;
    if (str.length() != 4) throw JsonParsingUTFSequenceException(str);

    // And now, ladies and gentlemen, for the grande finale,
    // let's parse that utf8 btch
    std::string utf8Hex = ToLower(str);

    // Validate that it is actually valid hex
    bool isValidHex = true;
    for (std::size_t i = 0; i < utf8Hex.length(); i++)
    {
        if (!(((utf8Hex[i] >= '0') && (utf8Hex[i] <= '9')) ||
            ((utf8Hex[i] >= 'a') && (utf8Hex[i] <= 'f')))) isValidHex = false;
    }

    if (isValidHex)
    {
        unsigned int unsignedVal = (unsigned int)Helpers::BaseX_2_10(utf8Hex, "0123456789abcdef");
        // It just   w o r k s

        // Check if the parsed value fits in the 8-byte char
        if (unsignedVal <= 0xff)
        {
            ss << (char)((char)unsignedVal - 0x100);
        }
        else
        {
            // The utf-8 value exceeds 8 bits. "Oh fuck put it back"
            ss << "\\u" << utf8Hex;
        }
    }
    else
    {
        // Invalid escape sequence...
        throw JsonParsingUTFSequenceException(utf8Hex);
    }

    return ss.str();
}

std::string StringHelpers::EscapeUTF8(const char c)
{
    std::stringstream ss;

    int intval = 0;

    if (c < 0)
    {
        intval = (int)c + 0x100;
    }
    else
    {
        intval = (int)c;
    }

    ss << "\\u" << Helpers::Base10_2_X((unsigned long long int)intval, "0123456789abcdef", 4);

    return ss.str();
}

using namespace JasonPP;
using namespace JasonPP::Internal;

/* Constructors */

JsonData::JsonData()
{
	Init();
	SetNull();

	return;
}

// Set default data per type
JsonData::JsonData(const JDType type)
{
	Init();

	switch (type)
	{
	case JDType::__NULL__:
		// Default value is already NULL
		break;
	
	case JDType::BOOL:
		SetBoolData(false);
		break;

	case JDType::INT:
		SetIntData(0);
		break;

	case JDType::FLOAT:
		SetFloatData(0);
		break;

	case JDType::STRING:
		SetStringData("");
		break;

	case JDType::JSON:
		SetJsonDataAsPointer(new JsonBlock());
		break;

	case JDType::ARRAY:
		SetArrayDataAsPointer(new JsonArray());
		break;
	}

	return;
}

JsonData::JsonData(const bool data)
{
	Init();
	SetBoolData(data);

	return;
}

JsonData::JsonData(const long long int data)
{
	Init();
	SetIntData(data);

	return;
}

JsonData::JsonData(const int data)
{
	Init();
	SetIntData(data);

	return;
}

JsonData::JsonData(const long double data)
{
	Init();
	SetFloatData(data);

	return;
}

JsonData::JsonData(const double data)
{
	Init();
	SetFloatData((const long double)data);

	return;
}

JsonData::JsonData(const std::string data)
{
	Init();
	SetStringData(data);

	return;
}

JsonData::JsonData(const char data[])
{
	Init();
	SetStringData(std::string(data));

	return;
}

JsonData::JsonData(const JsonBlock data)
{
	Init();
	SetJsonData(data);

	return;
}

JsonData::JsonData(const std::vector<JsonData> data)
{
	Init();
	SetArrayData(data);

	return;
}

JsonData::JsonData(const JsonArray data)
{
	Init();
	SetArrayData(data);

	return;
}

JsonData::JsonData(const JsonData& other)
{
	Init();
	CloneFrom(other);

	return;
}

void JsonData::SetFloatPrecision(const double precision)
{
	if (precision <= 0)
	{
		if (!JASONPP_WARN_STFU) std::cout << "[JasonPP] Warning: Please only use float precisions > 0 <= 1! (0.001 would be a precision of 3 decimals) Applying JASONPP_FLOAT_MAX_PRECISION..." << std::endl;
		customFloatPrecision = JASONPP_FLOAT_MAX_PRECISION;
	}
	else if (precision > 1.0)
	{
		if(!JASONPP_WARN_STFU) std::cout << "[JasonPP] Warning: Please only use float precisions > 0 <= 1! (0.001 would be a precision of 3 decimals) Applying 1.0..." << std::endl;
		customFloatPrecision = 1;
	}
	else customFloatPrecision = precision;

	return;
}

JsonArray& JsonData::SetArrayDataAsPointer(JsonArray* p)
{
	dataType = JDType::ARRAY;

	if (arrayData != nullptr)
	{
		delete arrayData;
	}
	
	arrayData = p;
	arrayData->SetAssociatedJsonDataNode(this);

	return *arrayData;
}

JsonBlock& JsonData::SetJsonDataAsPointer(JsonBlock* p)
{
	dataType = JDType::JSON;

	if (jsonData != nullptr)
	{
		delete jsonData;
	}

	jsonData = p;
	jsonData->SetAssociatedJsonDataNode(this);
	return *jsonData;
}

void JsonData::Init()
{
	dataType = JDType::__NULL__;
	intData = 0l;
	floatData = 0.0f;
	stringData = std::string();
	jsonData = nullptr;
	arrayData = nullptr;

	return;
}

JsonData::~JsonData()
{
	if (arrayData != nullptr)
	{
		delete arrayData;
		arrayData = nullptr;
	}

	if (jsonData != nullptr)
	{
		delete jsonData;
		jsonData = nullptr;
	}

	return;
}

/* GETTERS */

double JsonData::GetFloatPrecision() const
{
	if (customFloatPrecision < 0) // If < 0, it is not set.
	{
		if (parentNode == nullptr) // No parent? Then inhreit from default values
		{
			// Clamp from a max-performance val to 1
			if (JASONPP_FLOAT_PRECISION <= 0.0)
			{
				if (!JASONPP_WARN_STFU) std::cout << "[JasonPP] Warning: Please only use float precisions > 0 <= 1! (0.001 would be a precision of 3 decimals) Applying JASONPP_FLOAT_MAX_PRECISION..." << std::endl;
				return JASONPP_FLOAT_MAX_PRECISION;
			}
			else if (JASONPP_FLOAT_PRECISION > 1.0)
			{
				if (!JASONPP_WARN_STFU) std::cout << "[JasonPP] Warning: Please only use float precisions > 0 <= 1! (0.001 would be a precision of 3 decimals) Applying 1.0..." << std::endl;
				return 1.0;
			}
			else return JASONPP_FLOAT_PRECISION;
		}
		else return parentNode->GetFloatPrecision(); // Inherit from parent
	}
	return customFloatPrecision;
}

bool JsonData::IsOfNumericType() const
{
	return (dataType == JDType::INT) || (dataType == JDType::FLOAT);
}

bool JsonData::GetBoolData() const
{
	JDType typeToGet = JDType::BOOL;

	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);

	return intData != 0;
}

long long int JsonData::GetIntData() const
{
	JDType typeToGet = JDType::INT;

	if (dataType != typeToGet)
	{
		if (dataType == JDType::FLOAT)
		{
			return (long long int)floatData;
		}
		else
		{
			ThrowDataTypeException(typeToGet);
		}
	}

	return intData;
}

long double JsonData::GetFloatData() const
{
	JDType typeToGet = JDType::FLOAT;

	if (dataType != typeToGet)
	{
		if (dataType == JDType::INT)
		{
			return (float)intData;
		}
		else
		{
			ThrowDataTypeException(typeToGet);
		}
	}

	return floatData;
}

std::string JsonData::GetStringData() const
{
	JDType typeToGet = JDType::STRING;

	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);

	return stringData;
}

JsonBlock& JsonData::GetJsonData()
{
	JDType typeToGet = JDType::JSON;
	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);
	return *jsonData;
}

const JsonBlock& JsonData::GetJsonData() const
{
	JDType typeToGet = JDType::JSON;
	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);
	return *jsonData;
}

JsonArray& JsonData::GetArrayData()
{
	JDType typeToGet = JDType::ARRAY;
	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);
	return *arrayData;
}

const JsonArray& JsonData::GetArrayData() const
{
	JDType typeToGet = JDType::ARRAY;
	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);
	return *arrayData;
}

short JsonData::GetNullData()
{
	JDType typeToGet = JDType::__NULL__;

	if (dataType != typeToGet) ThrowDataTypeException(typeToGet);

	return 0;
}

JsonData& JsonData::GetParent()
{
	if (parentNode == nullptr) throw JsonNoParentAvailableException();
	return *parentNode;
}

const JsonData& JsonData::GetParent() const
{
	if (parentNode == nullptr) throw JsonNoParentAvailableException();
	return *parentNode;
}

bool JsonData::HasParent() const
{
	return parentNode != nullptr;
}

/* SETTERS */

void JsonData::SetBoolData(const bool data)
{
	dataType = JDType::BOOL;
	intData = (int)data;

	return;
}

void JsonData::SetIntData(const long long int data)
{
	dataType = JDType::INT;
	intData = data;

	return;
}

void JsonData::SetIntData(const int data)
{
	dataType = JDType::INT;
	intData = data;

	return;
}

void JsonData::SetFloatData(const long double data)
{
	dataType = JDType::FLOAT;
	floatData = data;

	return;
}

void JsonData::SetStringData(const std::string data)
{
	dataType = JDType::STRING;
	stringData = data;

	return;
}

JsonBlock& JsonData::SetJsonData(const JsonBlock data)
{
	return SetJsonDataAsPointer(new JsonBlock(data));
}

JsonArray& JsonData::SetArrayData(const std::vector<JsonData> data)
{
	dataType = JDType::ARRAY;
	JsonArray* newArr = new JsonArray;
	newArr->CopyJsonDataFromVector_Pointer(&data); // Slightly more performant than constructor
	return SetArrayDataAsPointer(newArr);
}

JsonArray& JsonData::SetArrayData(const JsonArray data)
{
	return SetArrayDataAsPointer(new JsonArray(data));
}

void JsonData::SetNull()
{
	dataType = JDType::__NULL__;

	return;
}

/* MISC */

void JsonData::ThrowDataTypeException(const JDType toFetch) const
{
	throw JsonWrongDataTypeException(
		JsonDataType2String(toFetch),
		JsonDataType2String(dataType));

	return;
}

/* Controls */

std::string JsonData::Render(const bool minify) const
{
	return Render(0, minify);
}

std::string JsonData::Render(unsigned int num_tabs, const bool minify) const
{
	std::stringstream ss;

	switch (dataType)
	{
	case JDType::__NULL__:
		ss << "null";
		break;

	case JDType::BOOL:
		ss << ((intData != 0) ? "true" : "false");
		break;

	case JDType::INT:
		ss << intData;
		break;

	case JDType::FLOAT:
		ss.precision((std::streamsize)((-log10(GetFloatPrecision())) + 1));
		ss << floatData;
		break;

	case JDType::STRING:
		ss << "\"" << StringHelpers::Escape(stringData) << "\"";
		break;

	case JDType::JSON:
		ss << jsonData->Render(num_tabs, minify);
		break;

	case JDType::ARRAY:
		ss << arrayData->Render(num_tabs, minify);
		break;
	}

	return ss.str();
}

void JsonData::Parse(const std::string jsonCode)
{
	// Minify code for easier parsing
	const std::string minifiedCode = StringHelpers::MinifyJson(jsonCode);

	if (minifiedCode.length() == 0) throw JsonParsingGeneralException("JsonData-Parser got json code snippet of length 0!");

	if (minifiedCode == "null")  // Datatype null
	{
		SetNull();
	}
	else if ((minifiedCode == "true") || (minifiedCode == "false")) // Datatype bool
	{
		SetBoolData(minifiedCode == "true");
	}
	else if (StringHelpers::IsNumeric(minifiedCode)) // Datatype int
	{
		try
		{
			SetIntData(std::stoll(minifiedCode));
		}
		catch (const std::invalid_argument&)
		{
			throw JsonParsingGeneralException(std::string("std::stoll failed while parsing numeric json value! Value was \"") + minifiedCode + std::string("\""));
		}
		catch (const std::out_of_range&)
		{
			throw JsonParsingGeneralException(std::string("std::stoll failed because the number found was too large for a long long int! Value was \"") + minifiedCode + std::string("\""));
		}
	}
	else if (StringHelpers::IsNumeric(minifiedCode, true)) // Datatype float
	{
		try
		{
			SetFloatData(std::stold(minifiedCode));
		}
		catch (const std::invalid_argument&)
		{
			throw JsonParsingGeneralException(std::string("std::stold failed while parsing numeric json value! Value was \"") + minifiedCode + std::string("\""));
		}
		catch (const std::out_of_range&)
		{
			throw JsonParsingGeneralException(std::string("std::stold failed because the number found was too large for a long double! Value was \"") + minifiedCode + std::string("\""));
		}
	}
	else if (minifiedCode[0] == '\"') // Datatype string
	{
		if (minifiedCode[minifiedCode.length() - 1] != '\"') throw JsonParsingMissingBracketsException();

		std::string strWithoutQuotes = minifiedCode.substr(1, minifiedCode.length() - 2);
		std::string strUnescaped = StringHelpers::Unescape(strWithoutQuotes);
		SetStringData(strUnescaped);
	}
	else if (minifiedCode[0] == '{') // Datatype json
	{
		// Delegate further parsing to the json class
		JsonBlock* newJsonBlock = new JsonBlock;
		newJsonBlock->Parse(minifiedCode);
		SetJsonDataAsPointer(newJsonBlock);
	}
	else if (minifiedCode[0] == '[') // Datatype array
	{
		JsonArray* newJsonArray = new JsonArray;
		newJsonArray->Parse(minifiedCode);
		SetArrayDataAsPointer(newJsonArray);
		
	}
	else throw JsonParsingUnrecognizableDatatypeException(jsonCode);

	return;
}

void JsonData::CloneTo(JsonData& other) const
{
	other.CloneFrom(*this);
	return;
}

void JsonData::CloneFrom(const JsonData& other)
{
	customFloatPrecision = other.customFloatPrecision;

	switch (other.dataType)
	{
	case JDType::__NULL__:
		// Default value is already NULL
		break;

	case JDType::BOOL:
		SetBoolData(other.intData != 0);
		break;

	case JDType::INT:
		SetIntData(other.intData);
		break;

	case JDType::FLOAT:
		SetFloatData(other.floatData);
		break;

	case JDType::STRING:
		SetStringData(other.stringData);
		break;

	case JDType::ARRAY:
		SetArrayData(*other.arrayData);
		break;

	case JDType::JSON:
		SetJsonData(*other.jsonData);
		break;
	}

	return;
}

bool JsonData::IsIdentical(const JsonData& other) const
{
	// Special case for int/float implicit conversion
	if (((dataType == JDType::INT) && (other.dataType == JDType::FLOAT)) ||
		((other.dataType == JDType::INT) && (dataType == JDType::FLOAT)))
	{
		// Here we have to get the float value via the getter because of implicit conversion
		// Use the more precise precision of the two...
		return Helpers::AreSame(GetFloatData(), other.GetFloatData(), Helpers::Min<double>(GetFloatPrecision(), other.GetFloatPrecision()));
	}

	// This check sits down here because we have to consider that a float value can be equal to an int value!
	if (dataType != other.dataType) return false;

	switch (dataType)
	{
	case JDType::__NULL__:
		return true;

	case JDType::BOOL:
		// Values can't be of different type because of the check at the beginning of the function
		return intData == other.intData;
		break;

	case JDType::INT:
		return intData == other.intData;
		break;

	case JDType::FLOAT:
		// Use the more precise precision of the two...
		return Helpers::AreSame(floatData, other.floatData, Helpers::Min<double>(GetFloatPrecision(), other.GetFloatPrecision()));
		break;

	case JDType::STRING:
		return stringData == other.stringData;
		break;

	case JDType::ARRAY:
		return arrayData->IsIdentical(*other.arrayData);
		break;

	case JDType::JSON:
		return jsonData->IsIdentical(*other.jsonData);
		break;
	}

	return false;
}

JsonData& JsonData::operator[](const std::string label)
{
	return GetJsonData().Set(label);
}

JsonData& JsonData::operator[](const char label[])
{
	return this->operator[](std::string(label));
}

const JsonData& JsonData::operator[](const std::string label) const
{
	return GetJsonData().Get(label);
}

const JsonData& JsonData::operator[](const char label[]) const
{
	return this->operator[](std::string(label));
}

JsonData& JsonData::operator[](std::size_t idx)
{
	return GetArrayData().At(idx);
}

JsonData& JsonData::operator[](int idx)
{
	return GetArrayData().At(idx);
}

const JsonData& JsonData::operator[](const std::size_t idx) const
{
	return GetArrayData().At(idx);
}

const JsonData& JsonData::operator[](int idx) const
{
	return GetArrayData().At(idx);
}

bool JsonData::operator==(const JsonData& other) const
{
	return IsIdentical(other);
}

bool JsonData::operator!=(const JsonData& other) const
{
	return !operator==(other);
}

JsonData& JsonData::operator+=(const JsonElement ele)
{
	if (dataType == JDType::JSON)
	{
		return jsonData->Add(ele);
	}
	ThrowDataTypeException(JDType::JSON);
	std::terminate();
}

void JsonData::operator=(const JsonData other)
{
	CloneFrom(other);
	return;
}

JsonData& JsonData::operator=(const bool b)
{
	SetBoolData(b);
	return *this;
}

JsonData& JsonData::operator=(const int i)
{
	SetIntData(i);
	return *this;
}

JsonData& JsonData::operator=(long long int lli)
{
	SetIntData(lli);
	return *this;
}

JsonData& JsonData::operator=(const double d)
{
	SetFloatData(d);
	return *this;
}

JsonData& JsonData::operator=(const long double ld)
{
	SetFloatData(ld);
	return *this;
}

JsonData& JsonData::operator=(const std::string s)
{
	SetStringData(s);
	return *this;
}

JsonData& JsonData::operator=(const char s[])
{
	SetStringData(s);
	return *this;
}

JsonData& JsonData::operator=(const JsonBlock jb)
{
	JsonBlock* newJson = new JsonBlock(jb);
	SetJsonDataAsPointer(newJson);
	return *this;
}

JsonData& JsonData::operator=(const JsonArray arr)
{
	JsonArray* newArr = new JsonArray(arr);
	SetArrayDataAsPointer(newArr);
	return *this;
}

JsonData& JsonData::operator=(const std::vector<JsonData> arr)
{
	JsonArray* newArr = new JsonArray;
	newArr->CopyJsonDataFromVector_Pointer(&arr);
	SetArrayDataAsPointer(newArr);
	return *this;
}

/* Operator overloads */

JsonData::operator bool() const
{
	return GetBoolData();
}

JsonData::operator int() const
{
	return (int)GetIntData();
}

JsonData::operator long int() const
{
	return (long int)GetIntData();
}

JsonData::operator long long int() const
{
	return GetIntData();
}

JsonData::operator float() const
{
	return (float)GetFloatData();
}

JsonData::operator double() const
{
	return (double)GetFloatData();
}

JsonData::operator long double() const
{
	return GetFloatData();
}

JsonData::operator std::string() const
{
	if (dataType == JDType::STRING) return GetStringData();
	else return Render(JASONPP_STRINGCONV_MINIFY);
}

// Extra namespace scope is required for the << overload
namespace JasonPP
{
	std::ostream& operator<<(std::ostream& os, const JsonData& jd)
	{
		if (jd.dataType == JDType::STRING) return os << jd.GetStringData();
		else return os << jd.Render(JASONPP_STRINGCONV_MINIFY);
	}
}

using namespace JasonPP;
using namespace JasonPP::Internal;

JsonElement::JsonElement(const std::string label)
{
	this->label = label;
	this->data = new JsonData;

	return;
}

JsonElement::JsonElement(const std::string label, const bool data)
{
	this->label = label;
	this->data = new JsonData(data);

	return;
}

JsonElement::JsonElement(const std::string label, const long long int data)
{
	this->label = label;
	this->data = new JsonData(data);

	return;
}

JsonElement::JsonElement(const std::string label, const int data)
{
	this->label = label;
	this->data = new JsonData((long long int)data);

	return;
}

JsonElement::JsonElement(const std::string label, const long double data)
{
	this->label = label;
	this->data = new JsonData(data);

	return;
}

JsonElement::JsonElement(const std::string label, const double data)
{
	this->label = label;
	this->data = new JsonData((long double)data);

	return;
}

JsonElement::JsonElement(const std::string label, const std::string data)
{
	this->label = label;
	this->data = new JsonData(data);

	return;
}

JsonElement::JsonElement(const std::string label, const char data[])
{
	this->label = label;
	this->data = new JsonData(std::string(data));

	return;
}

JsonElement::JsonElement(const std::string label, const JsonBlock data)
{
	this->label = label;
	this->data = new JsonData();
	this->data->SetJsonDataAsPointer(new JsonBlock(data));

	return;
}

JsonElement::JsonElement(const std::string label, const JsonData data)
{
	this->label = label;
	this->data = new JsonData(data);

	return;
}

JsonElement::JsonElement(const std::string label, const  std::vector<JsonData> data)
{
	this->label = label;
	this->data = new JsonData();
	JsonArray* newArr = new JsonArray;
	newArr->CopyJsonDataFromVector_Pointer(&data); // Slightly more performant than constructor
	this->data->SetArrayDataAsPointer(newArr);

	return;
}

JsonElement::JsonElement(const std::string label, const JsonArray data)
{
	this->label = label;
	this->data = new JsonData();
	this->data->SetArrayDataAsPointer(new JsonArray(data));

	return;
}

JsonElement::JsonElement(const JsonElement& other)
{
	label = other.label;
	data = new JsonData(*other.data);

	return;
}

JsonElement::~JsonElement()
{
	delete data;
	data = nullptr;

	return;
}

std::string JsonElement::Render(unsigned int num_tabs, const bool minify) const
{
	std::stringstream ss;

	if (!minify) ss << StringHelpers::GetIndentation(num_tabs);

	ss << "\"" << StringHelpers::Escape(label) << "\":" << (minify ? "" : " ") << data->Render(num_tabs, minify);

	return ss.str();
}

void JsonElement::SetJsonDataPointer(JsonData* p)
{
	if (data != nullptr)
	{
		delete data;
	}
	data = p;

	return;
}

void JsonElement::Parse(const std::string jsonCode)
{
	const std::string minifiedCode = StringHelpers::MinifyJson(jsonCode);

	std::string label;
	std::string dataCode;

	std::size_t endLabel = std::string::npos;

	// Get the elements label
	{
		std::size_t start = std::string::npos;
		std::size_t end = std::string::npos;
		bool areWeInString = false;
		bool isCharEscaped = false;
		int arrayBracketLevel = 0; // defines how deep we are in array brackets
		int curlyBracketLevel = 0; // defines how deep we are in curly brackets

		for (std::size_t i = 0; i < minifiedCode.length(); i++)
		{
			const char c = minifiedCode[i];

			if ((c == '\"') && (!areWeInString) && (arrayBracketLevel == 0) && (curlyBracketLevel == 0) && (start == std::string::npos)) start = i;

			else if ((c == '\"') && (areWeInString) && (!isCharEscaped) && (arrayBracketLevel == 0) && (curlyBracketLevel == 0))
			{
				end = i;
				endLabel = end;
				label = StringHelpers::Unescape(minifiedCode.substr(start + 1, (end - start) - 1));
				areWeInString = false;
				break;
			}

			if ((c == '\"') && (!areWeInString)) areWeInString = true;
			else if ((c == '\"') && (areWeInString) && (!isCharEscaped)) areWeInString = false;

			// No need to check for char escaping since we are already checking if we are in a string or not. Chars are ever escaped outside of strings
			if ((c == '[') && (!areWeInString)) arrayBracketLevel++;
			else if ((c == ']') && (!areWeInString)) arrayBracketLevel--;
			else if ((c == '{') && (!areWeInString)) curlyBracketLevel++;
			else if ((c == '}') && (!areWeInString)) curlyBracketLevel--;

			isCharEscaped = false;
			if ((c == '\\') && (!isCharEscaped) && (areWeInString)) isCharEscaped = true; // Should only matter for the next char!!
		}

		// Someone fucked up his json code
		if (arrayBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (curlyBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (areWeInString) throw JsonParsingMissingQuotesException();
		if (minifiedCode[endLabel + 1] != ':') throw JsonParsingGeneralException(std::string("Expected ':', got '") + Jstring(minifiedCode[endLabel + 1]) + "'!");
	}
	if (endLabel < 0) throw JsonParsingGeneralException("No label in json element found.");

	// Get the element data's code
	{
		const std::size_t startData = endLabel + 2; // + endquote of label and double colon. We can assume this is always the case because the json code is minified
		dataCode = minifiedCode.substr(startData, minifiedCode.length() - startData);
	}

	// Apply the label and delegate further parsing to the JsonData class
	this->label = label;
	this->data->Parse(dataCode);

	return;
}

using namespace JasonPP;
using namespace JasonPP::Internal;

JsonBlock::JsonBlock()
{
	content = new std::vector<JsonElement*>();

	return;
}

JsonBlock::JsonBlock(const std::vector<JsonElement> initialElements)
{
	content = new std::vector<JsonElement*>();
	
	for (std::size_t i = 0; i < initialElements.size(); i++)
	{
		bool alreadyExists = false;

		for (std::size_t j = 0; j < content->size(); j++)
		{
			if (initialElements[i].label == content->at(j)->GetLabel()) alreadyExists = true;
		}

		if (alreadyExists) throw JsonLabelAlreadyExistsException(initialElements[i].GetLabel());
		else
		{
			JsonElement* newEle = new JsonElement(initialElements[i].GetLabel());
			newEle->SetJsonDataPointer(new JsonData(*initialElements[i].GetData())); // Slightly more performant than via constructor
			newEle->GetData()->parentNode = associatedJsonDataNode;
			content->push_back(newEle);
		}
	}

	return;
}

JsonBlock::JsonBlock(const JsonBlock& other)
{
	content = new std::vector<JsonElement*>();
	CloneFrom(other);

	return;
}

JsonBlock::~JsonBlock()
{
	Helpers::FreeVector(content);

	return;
}

bool JsonBlock::DoesExist(const std::string label) const
{
	for (std::size_t i = 0; i < content->size(); i++)
	{
		if (content->at(i)->GetLabel() == label) return true;
	}

	return false;
}

JsonData& JsonBlock::Set(const std::string label)
{
	for (std::size_t i = 0; i < content->size(); i++)
	{
		if (content->at(i)->GetLabel() == label)
		{
			return *content->at(i)->GetData();
		}
	}

	JsonElement* newEle = new JsonElement(label);
	newEle->GetData()->parentNode = associatedJsonDataNode;
	content->push_back(newEle);
	return *newEle->GetData();
}

JsonData& JsonBlock::Set(const JsonElement ele)
{
	JsonData& created = Set(ele.label);
	created.CloneFrom(*ele.GetData());
	return created;
}

JsonData& JsonBlock::Get(const std::string label)
{
	return const_cast<JsonData&>(const_cast<const JsonBlock&>(*this).Get(label));
}

const JsonData& JsonBlock::Get(const std::string label) const
{
	for (std::size_t i = 0; i < content->size(); i++)
	{
		if (content->at(i)->GetLabel() == label)
		{
			return *content->at(i)->GetData();
		}
	}

	throw JsonLabelDoesNotExistException(label);
	std::terminate();
}

JsonData& JsonBlock::Add(const std::string label)
{
	if (!DoesExist(label))
	{
		JsonElement* newEle = new JsonElement(label);
		newEle->GetData()->parentNode = associatedJsonDataNode;
		content->push_back(newEle);
		return *newEle->GetData();
	}

	throw JsonLabelAlreadyExistsException(label);
	std::terminate();
}

JsonData& JsonBlock::Add(const JsonElement ele)
{
	if (!DoesExist(ele.label))
	{
		JsonElement* newEle = new JsonElement(ele);
		newEle->GetData()->parentNode = associatedJsonDataNode;
		content->push_back(newEle);
		return *newEle->GetData();
	}

	throw JsonLabelAlreadyExistsException(ele.label);
	std::terminate();
}

void JsonBlock::Remove(const std::string label)
{
	for (std::size_t i = 0; i < content->size(); i++)
	{
		if (content->at(i)->GetLabel() == label)
		{
			// We can just erase within a normal ass for-loop because we return directly after
			delete content->at(i);
			content->at(i) = nullptr;
			content->erase(content->begin() + i);
			return;
		}
	}

	throw JsonLabelDoesNotExistException(label);
	return;
}

bool JsonBlock::TryRemove(const std::string label)
{
	if (!DoesExist(label)) return false; // Let's prevent most Exceptions that would have to be caught right here.

	try
	{
		Remove(label);
	}
	catch(JsonException&)
	{
		return false;
	}

	return true;
}

std::size_t JsonBlock::GetNumElements() const
{
	return content->size();
}

/* Shorthand alternate arguments */
bool JsonBlock::DoesShorthandExist(const std::string shorthand, const char delimiter) const
{
	return DoesShorthandExist(shorthand, Jstring(delimiter));
}

void JsonBlock::ShorthandRemove(const std::string shorthand, const char delimiter)
{
	return ShorthandRemove(shorthand, Jstring(delimiter));
}

JsonData& JsonBlock::ShorthandAdd(const std::string shorthand, const char delimiter)
{
	return ShorthandAdd(shorthand, Jstring(delimiter));
}

JsonData& JsonBlock::ShorthandGet(const std::string shorthand, const char delimiter)
{
	return ShorthandGet(shorthand, Jstring(delimiter));
}

const JsonData& JsonBlock::ShorthandGet(const std::string shorthand, const char delimiter) const
{
	return ShorthandGet(shorthand, Jstring(delimiter));
}

JsonData& JsonBlock::ShorthandSet(const std::string shorthand, const char delimiter)
{
	return ShorthandSet(shorthand, Jstring(delimiter));
}
/* Shorthand alternate arguments END */

bool JsonBlock::DoesShorthandExist(const std::string shorthand, const std::string delimiter) const
{
	if (shorthand.length() == 0) return false; // Now segments.size() cannot be 0
	std::vector<std::string> segments = StringHelpers::SplitString(shorthand, delimiter);

	const JsonBlock* jb = &const_cast<const JsonBlock&>(*this);
	for (std::size_t i = 0; i < segments.size(); i++)
	{
		if ((jb->DoesExist(segments[i])) && ((jb->Get(segments[i]).GetDataType() == JDType::JSON) || (i == segments.size() - 1)))
		{
			if (i == segments.size() - 1) return true; // We are at the end. Let's just return it
			if (jb->Get(segments[i]).GetDataType() == JDType::JSON) jb = &jb->Get(segments[i]).GetJsonData();
			else return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}

JsonData& JsonBlock::ShorthandGet(const std::string shorthand, const std::string delimiter)
{
	return const_cast<JsonData&>(const_cast<const JsonBlock&>(*this).ShorthandGet(shorthand, Jstring(delimiter)));
}

const JsonData& JsonBlock::ShorthandGet(const std::string shorthand, const std::string delimiter) const
{
	if (shorthand.length() == 0) throw JsonShorthandInvalidException(shorthand, "Shorthand was empty!"); // Now segments.size() cannot be 0
	std::vector<std::string> segments = StringHelpers::SplitString(shorthand, delimiter);

	const JsonBlock* jb = &const_cast<const JsonBlock&>(*this);
	for (std::size_t i = 0; i < segments.size(); i++)
	{
		if ((jb->DoesExist(segments[i])) && ((jb->Get(segments[i]).GetDataType() == JDType::JSON) || (i == segments.size() - 1)))
		{
			if (i == segments.size() - 1) return jb->Get(segments[i]); // We are at the end. Let's just return it
			if (jb->Get(segments[i]).GetDataType() == JDType::JSON) jb = &jb->Get(segments[i]).GetJsonData();
			else throw JsonShorthandInvalidException(shorthand);
		}
		else
		{
			throw JsonShorthandInvalidException(shorthand);
		}
	}

	throw JsonShorthandInvalidException(shorthand);
	std::terminate();
}

JsonData& JsonBlock::ShorthandAdd(const std::string shorthand, const std::string delimiter)
{
	if (shorthand.length() == 0) throw JsonShorthandInvalidException(shorthand, "Shorthand was empty!"); // Now segments.size() cannot be 0
	std::vector<std::string> segments = StringHelpers::SplitString(shorthand, delimiter);

	JsonBlock* jb = this;
	for (std::size_t i = 0; i < segments.size(); i++)
	{
		if ((i == segments.size() - 1) && (jb->DoesExist(segments[i]))) throw JsonShorthandInvalidException(shorthand, "Last path segment already exists! Cannot create!");

		if (!jb->DoesExist(segments[i])) // Path segment does not exist yet. Let's create it!
		{
			if (i < segments.size() - 1) // If segments[i] is an actual path segment, create it as such
			{
				jb = &jb->Add(segments[i]).SetJsonData(JsonBlock());
			}
			else // Else, just leave it at null, since it is the head
			{
				return jb->Add(segments[i]);
			}
		}
		else
		{
			if (jb->Get(segments[i]).GetDataType() != JDType::JSON) throw JsonShorthandInvalidException(shorthand, "A path segment already exists and is not of type json!");
			jb = &jb->Get(segments[i]).GetJsonData();
		}
	}

	throw JsonShorthandInvalidException(shorthand);
	std::terminate();
}

JsonData& JsonBlock::ShorthandSet(const std::string shorthand, const std::string delimiter)
{
	if (shorthand.length() == 0) throw JsonShorthandInvalidException(shorthand, "Shorthand was empty!");

	if (DoesShorthandExist(shorthand, delimiter))
	{
		return ShorthandGet(shorthand, delimiter);
	}
	else
	{
		return ShorthandAdd(shorthand, delimiter);
	}

	throw JsonShorthandInvalidException(shorthand);
	std::terminate();
}

void JsonBlock::ShorthandRemove(const std::string shorthand, const std::string delimiter)
{
	if (shorthand.length() == 0) throw JsonShorthandInvalidException(shorthand, "Shorthand was empty!"); // Now segments.size() cannot be 0
	std::vector<std::string> segments = StringHelpers::SplitString(shorthand, delimiter);

	// Rebuild the shorthand without the last segment
	std::stringstream ss;
	for (std::size_t i = 0; i < segments.size() - 1; i++) // Iterate through every segment EXCEPT the last one
	{
		ss << segments[i];
		if (i != segments.size() - 2) ss << delimiter;
	}
	std::string shorthandParent = ss.str();

	// Lets get the parent object of the object to be deleted via shorthand. Let it throw an exception if it wants to
	JsonBlock* parentJson = this;
	if (shorthandParent.length() > 0)
	{
		JsonData* dt = &ShorthandGet(shorthandParent, delimiter);

		// Is the parent object of the object to be deleted even of type json?
		if (dt->GetDataType() != JDType::JSON) throw JsonShorthandInvalidException(shorthand, "The parent of the object to be deleted is not of type json!");
		parentJson = &dt->GetJsonData();
	}

	// Basically avoid a JsonLabelDoesNotExistException and t hrow a more fitting JsonShorthandInvalidException instead
	if (!parentJson->TryRemove(segments[segments.size() - 1])) throw JsonShorthandInvalidException(shorthand, "The object to be deleted does not exist!");

	return;
}

void JsonBlock::Clear()
{
	Helpers::FreeVector(content);
	content = new std::vector<JsonElement*>();

	return;
}

void JsonBlock::CloneTo(JsonBlock& other) const
{
	other.CloneFrom(*this);
	return;
}

void JsonBlock::CloneFrom(const JsonBlock& other)
{
	content->clear();
	for (std::size_t i = 0; i < other.content->size(); i++)
	{
		JsonElement* je = new JsonElement(other.content->at(i)->GetLabel());
		je->SetJsonDataPointer(new JsonData(*other.content->at(i)->GetData())); // Slightly more performant than via constructor
		je->GetData()->parentNode = associatedJsonDataNode;
		content->push_back(je);
	}

	return;
}

bool JsonBlock::IsIdentical(const JsonBlock& jb) const
{
	// If even the member count does not match, they are definitely not identical
	if (content->size() != jb.content->size()) return false;
	if (content->size() == 0) return true; // If both jsons are empty, they are identical
	
	for (std::size_t i = 0; i < content->size(); i++)
	{
		bool isLabelPresent = false;
		JsonData* otherData = nullptr;
		for (std::size_t j = 0; j < jb.content->size(); j++)
		{
			// Not using DoesExist() for performance reasons
			if (content->at(i)->GetLabel() == jb.content->at(j)->GetLabel())
			{
				isLabelPresent = true;
				otherData = jb.content->at(j)->GetData();
			}
		}
		// If any label is not present in the other JsonBlock => not identical
		if (!isLabelPresent) return false;

		// If two elements of the same label are not identical, the whole JsonBlock is not identical!
		if (!content->at(i)->GetData()->IsIdentical(*otherData)) return false;
	}

	return true;
}

void JsonBlock::SetAssociatedJsonDataNode(JsonData* newNode)
{
	associatedJsonDataNode = newNode;

	for (std::size_t i = 0; i < content->size(); i++)
	{
		content->at(i)->GetData()->parentNode = associatedJsonDataNode;
	}

	return;
}

std::vector<JsonElement*>* JsonBlock::GetElementsSortedByLabel() const
{
	std::vector<JsonElement*>* vec = new std::vector<JsonElement*>;

	std::vector<std::string> sortedLabels;
	// Fill with all labels
	for (std::size_t i = 0; i < content->size(); i++) sortedLabels.push_back(content->at(i)->GetLabel());
	
	// Sort via bubble sort
	for (std::size_t i = 0; i < sortedLabels.size(); i++)
	{
		for (std::size_t j = 0; j < sortedLabels.size() - i - 1; j++)
		{
			if (StringHelpers::SortDescriminator_Alphabetically(sortedLabels[j], sortedLabels[j + 1]))
			{
				// Swappp
				std::string buf = sortedLabels[j];
				sortedLabels[j] = sortedLabels[j + 1];
				sortedLabels[j + 1] = buf;
			}
		}
	}

	// Now just fill vec with the contents of content in the order of sortedLabels
	for (std::size_t i = 0; i < sortedLabels.size(); i++)
	{
		for (std::size_t j = 0; j < content->size(); j++)
		{
			if (sortedLabels[i] == content->at(j)->GetLabel())
			{
				vec->push_back(content->at(j));
				continue;
			}
		}
	}

	return vec;
}

std::string JsonBlock::Render(unsigned int num_tabs, const bool minify) const
{
	if (GetNumElements() == 0) return "{}";

	std::stringstream ss;
	
	ss << "{" << (minify ? "" : "\n");

	num_tabs++;

	// This vector will be rendered
	std::vector<JsonElement*>* toRender = nullptr; // To   s h u t   the compiler with unitialized warnings. It will be ffs.

	if (JASONPP_RENDER_SORTED)
	{
		// IF we should render sorted, we will fill it with all elements of content, but sorted by label
		toRender = GetElementsSortedByLabel();
	}
	else
	{
		// if not, just set it equal to content.
		toRender = content;
	}
	
	// Here the actual rendering takes part
	for (std::size_t i = 0; i < toRender->size(); i++)
	{
		ss << toRender->at(i)->Render(num_tabs, minify) << (i < (toRender->size() - 1) ? "," : "") << (minify ? "" : "\n");
	}
	
	num_tabs--;

	if (!minify) ss << StringHelpers::GetIndentation(num_tabs);
	ss << "}";
	
	// If we rendered sorted, toRender is a unique pointer! Free its memory!
	if (JASONPP_RENDER_SORTED)
	{
		delete toRender;
		toRender = nullptr;
	}

	return ss.str();
}

void JsonBlock::Parse(const std::string jsonCode)
{
	const std::string minifiedCode = StringHelpers::MinifyJson(jsonCode);

	// Get the code of all json data elements
	std::vector<std::string> elementCodeSnippets;
	{
		std::size_t start = std::string::npos;
		std::size_t end = std::string::npos;
		int arrayBracketLevel = 0; // defines how deep we are in array brackets
		int curlyBracketLevel = 0; // defines how deep we are in curly brackets
		bool areWeInString = false;
		bool areWeInCode = false;
		bool isCharEscaped = false;
		bool areWeBetweenCommaAndLabel = false; // Has the parser found a comma, but is still looking for the next label?

		for (std::size_t i = 0; i < minifiedCode.length(); i++)
		{
			const char c = minifiedCode[i];

			if ((!areWeInString) && (arrayBracketLevel == 0) && (curlyBracketLevel == 1) && (!areWeInCode))
			{
				if (c == '\"')
				{
					start = i;
					areWeInCode = true;
					areWeBetweenCommaAndLabel = false;
				}
				else if (c == ',')
				{
					// Did not find a label between two commas!
					throw JsonParsingGeneralException(std::string("Fucked up a comma around position ") + Jstring((long long int)i));
				}
			}

			else if ((areWeInCode) && (arrayBracketLevel == 0) && (curlyBracketLevel == 1) && (!areWeInString) && ((c == ',') || (c == '}')))
			{
				end = i;
				areWeInCode = false;
				if (c == ',') areWeBetweenCommaAndLabel = true;

				std::string codeSnippet = minifiedCode.substr(start, end - start);
				if (codeSnippet.length() >= 4) // Minimum length for valid JsonElement code. "":0
				{
					elementCodeSnippets.push_back(codeSnippet);
				}
				else
				{
					// JsonElement too short to be valid.
					throw JsonParsingGeneralException(std::string("Fucked up a comma around position ") + Jstring((long long int)start));
				}
			}

			// Our } at level 1 is not the last char? It should be! EXPECTED_END_OF_FILE_EXCEPTION :)
			if ((!areWeInString) && (c == '}') && (curlyBracketLevel == 1) && (i != minifiedCode.length() - 1)) throw JsonParsingExpectedEOFException(minifiedCode.substr(i, minifiedCode.length() - i));

			if ((c == '\"') && (!areWeInString)) areWeInString = true;
			else if ((c == '\"') && (!isCharEscaped) && (areWeInString)) areWeInString = false;

			// No need to check for char escaping since we are already checking if we are in a string or not. Chars are never escaped outside of strings
			if ((c == '[') && (!areWeInString)) arrayBracketLevel++;
			else if ((c == ']') && (!areWeInString)) arrayBracketLevel--;
			else if ((c == '{') && (!areWeInString)) curlyBracketLevel++;
			else if ((c == '}') && (!areWeInString)) curlyBracketLevel--;

			isCharEscaped = false;
			if ((c == '\\') && (!isCharEscaped) && (areWeInString)) isCharEscaped = true; // Should only matter for the next char!!
		}

		// Someone fucked up his json code
		if (arrayBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (curlyBracketLevel != 0) throw JsonParsingMissingBracketsException();
		if (areWeInString) throw JsonParsingMissingQuotesException();
		if (areWeBetweenCommaAndLabel) throw JsonParsingGeneralException("Unexpected EOF. Don't put a comma after the last value of a json block!");
		if ((elementCodeSnippets.size() == 0) && (minifiedCode.length() > 2)) throw JsonParsingGeneralException("Found no members in json block, but code is too long for no members");
	}
	
	// Create the json content vector and delegate further parsing to the deeper json components
	{
		if (content == nullptr) content = new std::vector<JsonElement*>();

		for (std::size_t i = 0; i < elementCodeSnippets.size(); i++)
		{
			JsonElement* newElement = new JsonElement(""); // The label will get overwritten by the parser
			newElement->Parse(elementCodeSnippets[i]);
			if (DoesExist(newElement->GetLabel())) throw JsonParsingDuplicateLabelException(newElement->GetLabel());
			newElement->GetData()->parentNode = associatedJsonDataNode;
			content->push_back(newElement);
		}
	}

	return;
}

JsonData& JsonBlock::operator[](const std::string label)
{
	return Set(label);
}

JsonData& JsonBlock::operator[](const char label[])
{
	return this->operator[](std::string(label));
}

const JsonData& JsonBlock::operator[](const std::string label) const
{
	return Get(label);
}

const JsonData& JsonBlock::operator[](const char label[]) const
{
	return this->operator[](std::string(label));
}

JsonData& JsonBlock::operator+=(const std::string label)
{
	return Add(label);
}

JsonData& JsonBlock::operator+=(const JsonElement ele)
{
	return Add(ele);
}

JsonBlock& JsonBlock::operator-=(const std::string label)
{
	Remove(label);
	return *this;
}

bool JsonBlock::operator==(const JsonBlock& other) const
{
	return IsIdentical(other);
}

bool JsonBlock::operator!=(const JsonBlock& other) const
{
	return !operator==(other);
}

using namespace JasonPP;
using namespace JasonPP::Internal;

std::u16string UTF16Compatibility::ParseUTF16(const std::string str)
{
    u16stringstream uss;

    std::size_t posFound = 0;
    std::size_t lastFound = 0;

    while (posFound != std::string::npos)
    {
        lastFound = posFound;
        posFound = str.find("\\u", posFound);

        if (posFound != std::string::npos)
        {
            // Check, that we did not find an escaped sequence (eg "\\\\u") because that should not be parsed!
            if ((posFound == 0) || (str[posFound - 1] != '\\'))
            {
                uss << U8_2_U16(str.substr(lastFound, posFound - lastFound));

                // And now, ladies and gentlemen, for the grande finale,
                // let's parse that utf16 btch
                if (str.length() < posFound + 6) throw JsonParsingUTFSequenceException("\\u");
                std::string utf8Hex = StringHelpers::ToLower(str.substr(posFound + 2, 4));

                // Validate that it is actually valid hex
                bool isValidHex = true;
                for (std::size_t i = 0; i < utf8Hex.length(); i++)
                {
                    if (!(((utf8Hex[i] >= '0') && (utf8Hex[i] <= '9')) ||
                        ((utf8Hex[i] >= 'a') && (utf8Hex[i] <= 'f')))) isValidHex = false;
                }

                if (isValidHex)
                {
                    // It just   w o r k s
                    uss << (char16_t)Helpers::BaseX_2_10(utf8Hex, "0123456789abcdef");

                    posFound += 6; // We found a valid hex value. All good.
                }
                else
                {
                    // Invalid escape sequence...
                    throw JsonParsingUTFSequenceException(utf8Hex);
                }
            }
            else
            {
                // Escape sequence is escaped itself (eg "\\\\u213")
                uss << U8_2_U16(str.substr(lastFound, posFound - lastFound));
                posFound += 2;
            }
        }
        else
        {
            // No sequence found, dump the rest of the string in and return
            uss << U8_2_U16(str.substr(lastFound, (str.length()) - lastFound));
        }
    }

    return uss.str();
}

std::string UTF16Compatibility::EscapeUTF16(const std::u16string str)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        if ((str[i]) > 0xff)
        {
            ss << "\\u" << Helpers::Base10_2_X((unsigned long long int)str[i], "0123456789abcdef", 4);
        }
        else
        {
            ss << (char)(((char)str[i]) - 0x100);
        }
    }

    return ss.str();
}

std::u16string UTF16Compatibility::U8_2_U16(const std::string str)
{
    u16stringstream uss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        uss << (char16_t)(((char16_t)str[i]));
    }

    return uss.str();
}

std::string UTF16Compatibility::U16_2_U8(const std::u16string str)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < str.length(); i++)
    {
        ss << (char)(((char)str[i]) - 0x100);
    }

    return ss.str();
}
