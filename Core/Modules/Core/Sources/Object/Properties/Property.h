#pragma once

#include "Templates/Functions.h"

#include "Threading/CriticalSection.h"
#include "Threading/ScopeLock.h"

#include "Object/Collector/ObjectCollector.h"

#include <type_traits>

class CObject;
class CObjectLink;

namespace Objects::Properties
{
	class CProperty
	{
	public:
		CORE_API explicit CProperty(CObject* InParent);
		CORE_API virtual ~CProperty();
		
		CORE_API CObject* Parent() const;
		
	protected:
		CORE_API virtual void ReleaseLinks() = 0;
		CORE_API virtual void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const = 0;

	private:
		CObject* _parent;
		
		friend class ::CObjectCollector;
	};

	template<typename TObject = CObject>
	class TProperty : public CProperty
	{
		static_assert(std::is_base_of_v<CObject, TObject>);

		using Super = CProperty;

	public:
		TProperty(CObject* InParent)
		:	Super(InParent)
		{
			//
		}

		~TProperty() override = default;

		std::nullptr_t operator=(std::nullptr_t)
		{
			SetObject(nullptr);

			return nullptr;
		}

		TObject* operator=(TObject* InOther)
		{
			SetObject(InOther);

			return InOther;
		}

		operator TObject*()
		{
			return Object();
		}

		TObject* operator->()
		{
			return Object();
		}

	private:
		TObject* _object;
		CObjectLink* _link;
		SCriticalSection _criticalSection;

		inline void SetObject(TObject* InObject)
		{
			SScopeLock Lock(_criticalSection);

			if (_object != nullptr && _link != nullptr)
			{
				GObjectCollector.RemoveObjectLink(_link);

				_object = nullptr;
				_link = nullptr;
			}

			if (InObject != nullptr)
			{
				_object = InObject;

				_link = GObjectCollector.AddObjectLink(InObject, this);
			}
		}

		inline TObject* Object() const
		{
			SScopeLock Lock(_criticalSection, true);

			return _object;
		}

		inline void ReleaseLinks() override
		{
			SScopeLock Lock(_criticalSection, true);

			GObjectCollector.RemoveObjectLink(_link);

			_object = nullptr;
			_link = nullptr;
		}

		inline void EnumerateLinks(const TFunction<void (CObjectLink*)>& InFunc) const override
		{
			SScopeLock Lock(_criticalSection, true);

			if (_link != nullptr)
			{
				InFunc(_link);
			}
		}
	};
}

#define PROPERTY(TypeName, Name) Objects::Properties::TProperty< TypeName > Name = this