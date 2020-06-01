#pragma once

#pragma region Includes
// Standard
#include <stdint.h>

// Third Party
#pragma warning(disable : 26812)
#include <json/json.h>
#pragma warning(default : 26812)

#include <gsl/gsl>

// First Party
#include "RTTI.h"
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
	class IJsonParseHelper;
	
	/// <summary>
	/// JSON parser master class for managing IJsonParserHelper handles and parsing JSON data.
	/// </summary>
	class JsonParseMaster final
	{
#pragma region Shared Data
	public:
		/// <summary>
		/// Abstract base class for data to be filled with parsed Json data.
		/// </summary>
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS_ABSTRACT(SharedData, RTTI)
			
			friend JsonParseMaster;

#pragma region Shared Data Special Member Functions
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Default destructor.
			/// </summary>
			virtual ~SharedData() override = default;

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
			
#pragma region Shared Data Accessors
		public:
			/// <summary>
			/// Gets the JsonParseMaster this SharedData is associated with.
			/// </summary>
			/// <returns>Pointer to the JsonParseMaster instance associated with this SharedData.</returns>
			const JsonParseMaster* GetJsonParseMaster() const;

			/// <summary>
			/// Gets the depth of the current depth of the SharedData as it is being filled during parsing.
			/// </summary>
			/// <returns>Depth of the JSON data being parsed into the SharedData.</returns>
			std::size_t Depth() const;
#pragma endregion Shared Data Accessors

#pragma region Shared Data Modifiers
		public:
			/// <summary>
			/// Virtual initializer method called before parsing to perform any needed steps.
			/// </summary>
			virtual void PreParse() {};
			
			/// <summary>
			/// Virtual initializer method called after parsing to perform any needed steps.
			/// </summary>
			virtual void PostParse() {};

			/// <summary>
			/// Sets the JsonParseMaster associated with this SharedData.
			/// </summary>
			/// <param name="master">Pointer to the JsonParseMaster instance to be set.</param>
			void SetJsonParseMaster(JsonParseMaster* master);

		private:
			/// <summary>
			/// Increments the Json parse depth counter.
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decrements the Json parse depth counter.
			/// </summary>
			void DecrementDepth();
#pragma endregion Shared Data Modifiers
					
#pragma region Shared Data Data Members
		private:
			/// <summary>
			/// JsonParseMaster instance associated with the SharedData instance.
			/// </summary>
			JsonParseMaster* mMaster{ nullptr };

			/// <summary>
			/// Current depth level of the Json data as it is parsed into the SharedData.
			/// </summary>
			std::size_t mDepth{ 0 };
#pragma endregion Shared Data Data Members
		};
#pragma endregion Shared Data

#pragma region Special Member Functions
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="sharedData">Pointer to an instance of a SharedData subclass.</param>
		explicit JsonParseMaster(SharedData* sharedData=nullptr);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		JsonParseMaster(const JsonParseMaster&) = delete;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		JsonParseMaster& operator=(const JsonParseMaster&) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">JsonParseMaster to be moved.</param>
		JsonParseMaster(JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">JsonParseMaster to be moved.</param>
		JsonParseMaster& operator=( JsonParseMaster&& rhs) noexcept;
#pragma endregion Special Member Functions

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// Creates a JsonParseMaster instance.
		/// Creates new heap allocated SharedData and Helper instances from the original instances.
		/// </summary>
		/// <returns>Pointer to the new JsonParseMaster instance.</returns>
		gsl::owner<JsonParseMaster*> Clone() const;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
		/// <summary>
		/// Gets the filename of the last file parsed.
		/// </summary>
		/// <returns>Reference to the filename std::string instance.</returns>
		const std::string& GetFilename() const;

		/// <summary>
		/// Gets the SharedData instance associated with the JsonParseMaster.
		/// </summary>
		/// <returns>Pointer to the SharedData instance.</returns>
		const SharedData* GetSharedData() const;
#pragma endregion Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Gets the SharedData instance associated with the JsonParseMaster.
		/// </summary>
		/// <param name="sharedData">Reference to a SharedData instance.</param>
		void SetSharedData(SharedData& sharedData);

		/// <summary>
		/// Adds a parse helper to the JsonParseMaster.
		/// </summary>
		/// <param name="helper">Reference to the IJsonParseHelper subclass.</param>
		void AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Removes a parse helper to the JsonParseMaster.
		/// </summary>
		/// <param name="helper">Reference to the IJsonParseHelper subclass.</param>
		bool RemoveHelper(IJsonParseHelper& helper);
#pragma endregion Modifiers
		
#pragma region Parse Methods
	public:
		/// <summary>
		/// Parses a JSON string.
		/// </summary>
		/// <param name="string">JSON string to be parsed.</param>
		void Parse(const std::string& string);

		/// <summary>
		/// Parses an input stream containing JSON data.
		/// </summary>
		/// <param name="inputStream">A std::istream instance containing JSON data.</param>
		void Parse(std::istream& inputStream);

		/// <summary>
		/// Parses a JSON file.
		/// </summary>
		/// <param name="filename">Filename of the JSON file to be parsed.</param>
		void ParseFromFile(std::string filename);
#pragma endregion Parse Methods

#pragma region Parse Helper Methods
	private:
		/// <summary>
		/// Iterates through the members of JSON value object and parses the members.
		/// </summary>
		/// <param name="value">Reference to a JSON value object to be parsed.</param>
		void ParseMembers(const Json::Value& value);

		/// <summary>
		/// Parses a JSON value.
		/// </summary>
		/// <param name="key">Key associated with the JSON value.</param>
		/// <param name="value">Reference to the JSON value.</param>
		/// <param name="isArray">Boolean representing if the JSON value is an array.</param>
		void Parse(const std::string& key, const Json::Value& value);
#pragma endregion Parse Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to a SharedData subclass instance to be filled with parsed data.
		/// </summary>
		SharedData* mSharedData;

		/// <summary>
		/// List of pointers to parse helpers subclasses.
		/// </summary>
		Vector<IJsonParseHelper*> mHelpers;

		/// <summary>
		/// Filename of the last file parsed.
		/// </summary>
		std::string mFilename;

		/// <summary>
		/// Describes if the SharedData is owned by the JsonParseMaster due to cloning.
		/// </summary>
		bool mOwnsSharedData{ false };

		/// <summary>
		/// List of indices in the list of helpers that contain owned Helper instances.
		/// </summary>
		Vector<std::size_t> mOwnedHelperIndices;
#pragma endregion Data Members
	};
}

#include "JsonParseMaster.inl"
