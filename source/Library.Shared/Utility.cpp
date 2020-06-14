#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Utility.h"

// Standard
#include <string>
#include <algorithm>
#include <codecvt>
#include <locale>

// First Party
#include "Vector.h"
#pragma endregion Includes

namespace Library
{	
	namespace String
	{
		std::string ToLower(const std::string& str)
		{
			std::string lowercaseString = str;
			
			auto toLower = [](const unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
			std::transform(lowercaseString.begin(), lowercaseString.end(), lowercaseString.begin(), toLower);

			return lowercaseString;
		}

		std::string ToUpper(const std::string& str)
		{
			std::string uppercaseString = str;
			
			auto toUpper = [](const unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); };
			std::transform(uppercaseString.begin(), uppercaseString.end(), uppercaseString.begin(), toUpper);

			return uppercaseString;
		}

#pragma warning(push)
#pragma warning(disable: 4996)
		void ToWideString(const std::string& source, std::wstring& dest)
		{
			dest = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(source);
		}

		std::wstring ToWideString(const std::string& source)
		{
			return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(source);
		}

		void ToString(const std::wstring& source, std::string& dest)
		{
			dest = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(source);
		}

		std::string ToString(const std::wstring& source)
		{
			return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(source);
		}
#pragma warning(pop)

		bool EndsWith(const std::string& value, const std::string& ending)
		{
			if (ending.size() > value.size())
			{
				return false;
			}

			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

		bool EndsWith(const std::wstring& value, const std::wstring& ending)
		{
			if (ending.size() > value.size())
			{
				return false;
			}

			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}
	}

	namespace Exception
	{
		AggregateException::AggregateException(const char* message) : std::exception(message)
		{
		}
		
		AggregateException::AggregateException(const char* message, const Vector<Entry>& exceptions) : std::exception(message),
			Exceptions(exceptions)
		{
		}
		
		AggregateException::AggregateException(const char* message, Vector<Entry>&& exceptions) : std::exception(message),
			Exceptions(std::move(exceptions))
		{
		}
	}

	namespace File
	{
		void GetName(const std::string& inputPath, std::string& filename)
		{
			std::string fullPath(inputPath);
			std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

			const std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

			if (lastSlashIndex == std::string::npos)
			{
				filename = fullPath;
			}
			else
			{
				filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
			}
		}

		void GetDirectory(const std::string& inputPath, std::string& directory)
		{
			std::string fullPath(inputPath);
			std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

			const std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

			if (lastSlashIndex == std::string::npos)
			{
				directory = "";
			}
			else
			{
				directory = fullPath.substr(0, lastSlashIndex);
			}
		}

		std::tuple<std::string, std::string> GetNameAndDirectory(const std::string& inputPath)
		{
			std::string fullPath(inputPath);
			std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

			const std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

			std::string directory;
			std::string filename;

			if (lastSlashIndex == std::string::npos)
			{
				directory = "";
				filename = fullPath;
			}
			else
			{
				directory = fullPath.substr(0, lastSlashIndex);
				filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
			}

			return std::make_tuple(filename, directory);
		}

		void LoadBinary(const std::string& filename, Vector<char>& data)
		{
			std::ifstream file(filename.c_str(), std::ios::binary);

			if (!file.is_open())
			{
				throw std::runtime_error("Could not open file.");
			}

			file.seekg(0, std::ios::end);
			const uint32_t size = static_cast<uint32_t>(file.tellg());

			if (size > 0)
			{
				data.Resize(size);
				file.seekg(0, std::ios::beg);
				file.read(&data.Front(), size);
			}

			file.close();
		}
	}
}
