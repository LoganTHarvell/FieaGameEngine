#pragma once

#include <string>

#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Stack.h"

namespace UnitTests
{
	class JsonParseTestHelper : public Library::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonParseTestHelper, Library::IJsonParseHelper)

	public:
		struct StackFrame
		{
			const std::string& Key;
			const Json::Value& Value;
			const std::size_t Depth;
		};

		class SharedData : public Library::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, Library::JsonParseMaster::SharedData)

				friend JsonParseTestHelper;

#pragma region Shared Data Special Member Functions
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Default destructor.
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Copy constructor.
			/// </summary>
			SharedData(const SharedData& rhs) = default;

			/// <summary>
			/// Copy assignment operator.
			/// </summary>
			SharedData& operator=(const SharedData& rhs) = default;

			/// <summary>
			/// Move constructor.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData(SharedData&& rhs) noexcept;

			/// <summary>
			/// Move assignment operator.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData& operator=(SharedData&& rhs) noexcept;
#pragma endregion Shared Data Special Member Functions

		public:
			virtual gsl::owner<Library::JsonParseMaster::SharedData*> Create() const override;
			virtual void Initialize() override;

			std::size_t GetSize(const std::string& key) const;

		private:
			Library::HashMap<std::string, std::size_t> mDataSizes;
		};

	public:
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual bool StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool) override;
		virtual bool EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key) override;

	private:
		Library::Stack<std::pair<const std::string&, const Json::Value&>> mStack;
	};
}
