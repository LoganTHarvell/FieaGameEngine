#pragma once

#include <string>
#include <cstddef>

namespace Library
{
	class RTTI
	{
	public:
		using IdType = std::size_t;
		static IdType TypeIdClass() { return 0; }

		virtual ~RTTI() = default;

		virtual Library::RTTI::IdType TypeIdInstance() const = 0;

		virtual RTTI* QueryInterface(const IdType)
		{
			return nullptr;
		}

		virtual bool Set(IdType) const
		{
			return false;
		}

		virtual bool Set(const std::string&) const
		{
			return false;
		}

		template <typename T>
		const T* Get() const
		{
			return (Set(T::TypeIdClass()) ? reinterpret_cast<const T*>(this) : nullptr);
		}

		template <typename T>
		T* Get()
		{
			return (Set(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}

		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#define RTTI_DECLARATIONS(Type, ParentType)																						\
		public:																													\
			static std::string TypeName() { return std::string(#Type); }														\
			static Library::RTTI::IdType TypeIdClass() { return sRunTimeTypeId; }										\
			Library::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }								\
			Library::RTTI* QueryInterface(const RTTI::IdType id) override												\
            {																													\
				return (id == sRunTimeTypeId ? reinterpret_cast<Library::RTTI*>(this) : ParentType::QueryInterface(id)); \
            }																													\
			bool Set(Library::RTTI::IdType id) const override																\
			{																													\
				return (id == sRunTimeTypeId ? true : ParentType::Set(id));														\
			}																													\
			bool Set(const std::string& name) const override																		\
			{																													\
				return (name == TypeName() ? true : ParentType::Set(name));														\
			}																													\
			private:																											\
				static const Library::RTTI::IdType sRunTimeTypeId;

#define RTTI_DEFINITIONS(Type) const Library::RTTI::IdType Type::sRunTimeTypeId = reinterpret_cast<Library::RTTI::IdType>(&Type::sRunTimeTypeId);
}