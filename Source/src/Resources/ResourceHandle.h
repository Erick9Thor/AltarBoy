#include "Resource.h"
#include "../Utils/JsonFormaterValue.h"

template <typename T = Resource>
class ResourceHandle
{
public:
	ResourceHandle() {};
	ResourceHandle(UID ID) : ID(ID) { }
	ResourceHandle(T* resource) : resource(resource)
	{
		resource->instances++;
		ID = resource->GetID();
	}

	~ResourceHandle()
	{
		Free();
	}

	void Set(UID ID)
	{
		if (resource && resource->GetID() != ID)
			Free();

		if (!resource)
			this->ID = ID;
	}

	void Set(T* resource)
	{
		if (this->resource && this->resource != resource)
			Free();

		if (!this->resource)
		{
			this->resource = resource;
			resource->instances++;
			this->ID = resource->GetID();
		}
	}

	void Serialize(const char* name, JsonFormaterValue j_meta)
	{
		//j_meta[name] = ID
	}

	inline T* Get() { return resource ? resource : (resource = RequestResource()); }
	inline const T* Get() const { return resource ? resource : (resource = RequestResource()); }

	//Releases the use of the resource in module resources
	void Free();

	inline UID GetID() const { return ID; }
	inline bool IsLoaded() const { return resource != nullptr; };

private:
	T* RequestResource() const;

private:
	UID ID = 0;
	mutable T* resource = nullptr;
};