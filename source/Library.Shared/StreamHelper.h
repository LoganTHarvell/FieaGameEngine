#pragma once

namespace Library
{
	class Transform;

	class OutputStreamHelper final
	{
	public:
		explicit OutputStreamHelper(std::ostream& stream);
		~OutputStreamHelper() = default;
		OutputStreamHelper(const OutputStreamHelper&) = delete;
		OutputStreamHelper& operator=(const OutputStreamHelper&) = delete;
		OutputStreamHelper(OutputStreamHelper&&) = delete;
		OutputStreamHelper& operator=(OutputStreamHelper&&) = delete;

		std::ostream& Stream();
		const std::ostream& Stream() const;

		OutputStreamHelper& operator<<(const std::int32_t value);
		OutputStreamHelper& operator<<(const std::int64_t value);
		OutputStreamHelper& operator<<(const std::uint32_t value);
		OutputStreamHelper& operator<<(const std::uint64_t value);
		OutputStreamHelper& operator<<(const float value);
		OutputStreamHelper& operator<<(const std::string& value);
		OutputStreamHelper& operator<<(const glm::mat4x4& value);
		OutputStreamHelper& operator<<(const Transform& value);
		OutputStreamHelper& operator<<(const bool value);
		
	private:
		template <typename T>
		static void WriteObject(std::ostream& stream, T value);

		std::ostream& mStream;
	};

	class InputStreamHelper final
	{
	public:
		explicit InputStreamHelper(std::istream& stream);
		~InputStreamHelper() = default;
		InputStreamHelper(const InputStreamHelper&) = delete;
		InputStreamHelper& operator=(const InputStreamHelper&) = delete;
		InputStreamHelper(InputStreamHelper&&) = delete;
		InputStreamHelper& operator=(InputStreamHelper&&) = delete;

		std::istream& Stream();
		const std::istream& Stream() const;

		InputStreamHelper& operator>>(std::int32_t& value);
		InputStreamHelper& operator>>(std::int64_t& value);
		InputStreamHelper& operator>>(std::uint32_t& value);
		InputStreamHelper& operator>>(std::uint64_t& value);
		InputStreamHelper& operator>>(float& value);
		InputStreamHelper& operator>>(std::string& value);
		InputStreamHelper& operator>>(glm::mat4x4& value);
		InputStreamHelper& operator>>(bool& value);
		
	private:
		template <typename T>
		static void ReadObject(std::istream& stream, T& value);

		std::istream& mStream;
	};
}