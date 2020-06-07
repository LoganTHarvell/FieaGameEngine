#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "StreamHelper.h"

// First Party
#include "Transform.h"
#pragma endregion Includes

namespace Library
{
#pragma region OutputStreamHelper
	OutputStreamHelper::OutputStreamHelper(std::ostream& stream) :
		mStream(stream)
	{
	}

	std::ostream& OutputStreamHelper::Stream()
	{
		return mStream;
	}

	const std::ostream& OutputStreamHelper::Stream() const
	{
		return mStream;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const std::int32_t value)
	{
		WriteObject(mStream, value);

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const std::int64_t value)
	{
		WriteObject(mStream, value);

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const std::uint32_t value)
	{
		WriteObject(mStream, value);

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const std::uint64_t value)
	{
		WriteObject(mStream, value);

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(float value)
	{
		mStream.write(reinterpret_cast<char*>(&value), sizeof(float));

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const std::string& value)
	{
		const std::uint32_t Size = static_cast<std::uint32_t>(value.size());
		WriteObject(mStream, Size);
		mStream.write(&value[0], Size);

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const Transform& value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				*this << value[i][j];
			}
		}

		return *this;
	}

	OutputStreamHelper& OutputStreamHelper::operator<<(const bool value)
	{
		WriteObject(mStream, static_cast<std::uint8_t>(value ? 1 : 0));

		return *this;
	}

	template <typename T>
	void OutputStreamHelper::WriteObject(std::ostream& stream, T value)
	{
		for (std::uint32_t Size = sizeof(T); Size > 0; --Size, value >>= 8)
		{
			stream.put(static_cast<char>(value & 0xFF));
		}
	}
#pragma endregion OutputStreamHelper

#pragma region InputStreamHelper
	InputStreamHelper::InputStreamHelper(std::istream& stream) :
		mStream(stream)
	{
	}

	std::istream& InputStreamHelper::Stream()
	{
		return mStream;
	}

	const std::istream& InputStreamHelper::Stream() const
	{
		return mStream;
	}

	InputStreamHelper& InputStreamHelper::operator>>(std::int32_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(std::int64_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(std::uint32_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(std::uint64_t& value)
	{
		ReadObject(mStream, value);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(float& value)
	{
		mStream.read(reinterpret_cast<char*>(&value), sizeof(float));

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(std::string& value)
	{
		std::uint32_t stringLength;
		ReadObject(mStream, stringLength);

		value.resize(stringLength);
		mStream.read(&value[0], stringLength);

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(Transform& value)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				*this >> value[i][j];
			}
		}

		return *this;
	}

	InputStreamHelper& InputStreamHelper::operator>>(bool& value)
	{
		std::uint8_t boolValue;
		ReadObject(mStream, boolValue);

		value = (boolValue == 1 ? true : false);

		return *this;
	}

	template <typename T>
	void InputStreamHelper::ReadObject(std::istream& stream, T& value)
	{
		value = 0;

		for (std::uint32_t Size = 0; Size < sizeof(T); ++Size)
		{
			value |= static_cast<std::uint64_t>(stream.get()) << (8 * Size);
		}
	}
#pragma endregion InputStreamHelper
}