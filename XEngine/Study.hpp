#if 0
template <typename T>
class ComPtr
{
public:
	typedef T InterfaceType;

protected:
	InterfaceType *ptr_;
	template<class U> friend class ComPtr;

	void InternalAddRef() const throw()
	{
		if (ptr_ != nullptr)
		{
			ptr_->AddRef();
		}
	}

	unsigned long InternalRelease() throw()
	{
		unsigned long ref = 0;
		T* temp = ptr_;

		if (temp != nullptr)
		{
			ptr_ = nullptr;
			ref = temp->Release();
		}

		return ref;
	}

public:
#pragma region constructors
	ComPtr() throw() : ptr_(nullptr)
	{
	}

	ComPtr(decltype(__nullptr)) throw() : ptr_(nullptr)
	{
	}

	template<class U>
	ComPtr(_In_opt_ U *other) throw() : ptr_(other)
	{
		InternalAddRef();
	}

	ComPtr(const ComPtr& other) throw() : ptr_(other.ptr_)
	{
		InternalAddRef();
	}

	// copy constructor that allows to instantiate class when U* is convertible to T*
	template<class U>
	ComPtr(const ComPtr<U> &other, typename Details::EnableIf<__is_convertible_to(U*, T*), void *>::type * = 0) throw() :
		ptr_(other.ptr_)
	{
		InternalAddRef();
	}

	ComPtr(_Inout_ ComPtr &&other) throw() : ptr_(nullptr)
	{
		if (this != reinterpret_cast<ComPtr*>(&reinterpret_cast<unsigned char&>(other)))
		{
			Swap(other);
		}
	}

	// Move constructor that allows instantiation of a class when U* is convertible to T*
	template<class U>
	ComPtr(_Inout_ ComPtr<U>&& other, typename Details::EnableIf<__is_convertible_to(U*, T*), void *>::type * = 0) throw() :
		ptr_(other.ptr_)
	{
		other.ptr_ = nullptr;
	}
#pragma endregion

#pragma region destructor
	~ComPtr() throw()
	{
		InternalRelease();
	}
#pragma endregion

#pragma region assignment
	ComPtr& operator=(decltype(__nullptr)) throw()
	{
		InternalRelease();
		return *this;
	}

	ComPtr& operator=(_In_opt_ T *other) throw()
	{
		if (ptr_ != other)
		{
			ComPtr(other).Swap(*this);
		}
		return *this;
	}

	template <typename U>
	ComPtr& operator=(_In_opt_ U *other) throw()
	{
		ComPtr(other).Swap(*this);
		return *this;
	}

	ComPtr& operator=(const ComPtr &other) throw()
	{
		if (ptr_ != other.ptr_)
		{
			ComPtr(other).Swap(*this);
		}
		return *this;
	}

	template<class U>
	ComPtr& operator=(const ComPtr<U>& other) throw()
	{
		ComPtr(other).Swap(*this);
		return *this;
	}

	ComPtr& operator=(_Inout_ ComPtr &&other) throw()
	{
		ComPtr(static_cast<ComPtr&&>(other)).Swap(*this);
		return *this;
	}

	template<class U>
	ComPtr& operator=(_Inout_ ComPtr<U>&& other) throw()
	{
		ComPtr(static_cast<ComPtr<U>&&>(other)).Swap(*this);
		return *this;
	}
#pragma endregion

#pragma region modifiers
	void Swap(_Inout_ ComPtr&& r) throw()
	{
		T* tmp = ptr_;
		ptr_ = r.ptr_;
		r.ptr_ = tmp;
	}

	void Swap(_Inout_ ComPtr& r) throw()
	{
		T* tmp = ptr_;
		ptr_ = r.ptr_;
		r.ptr_ = tmp;
	}
#pragma endregion

	operator Details::BoolType() const throw()
	{
		return Get() != nullptr ? &Details::BoolStruct::Member : nullptr;
	}

	T* Get() const throw()
	{
		return ptr_;
	}

	InterfaceType* operator->() const throw()
	{
		return ptr_;
	}

	Details::ComPtrRef<ComPtr<T>> operator&() throw()
	{
		return Details::ComPtrRef<ComPtr<T>>(this);
	}

	const Details::ComPtrRef<const ComPtr<T>> operator&() const throw()
	{
		return Details::ComPtrRef<const ComPtr<T>>(this);
	}

	T* const* GetAddressOf() const throw()
	{
		return &ptr_;
	}

	T** GetAddressOf() throw()
	{
		return &ptr_;
	}

	T** ReleaseAndGetAddressOf() throw()
	{
		InternalRelease();
		return &ptr_;
	}

	T* Detach() throw()
	{
		T* ptr = ptr_;
		ptr_ = nullptr;
		return ptr;
	}

	void Attach(_In_opt_ InterfaceType* other) throw()
	{
		if (ptr_ != nullptr)
		{
			auto ref = ptr_->Release();
			DBG_UNREFERENCED_LOCAL_VARIABLE(ref);
			// Attaching to the same object only works if duplicate references are being coalesced. Otherwise
			// re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
			__WRL_ASSERT__(ref != 0 || ptr_ != other);
		}

		ptr_ = other;
	}

	unsigned long Reset()
	{
		return InternalRelease();
	}

	// Previously, unsafe behavior could be triggered when 'this' is ComPtr<IInspectable> or ComPtr<IUnknown> and CopyTo is used to copy to another type U. 
	// The user will use operator& to convert the destination into a ComPtrRef, which can then implicit cast to IInspectable** and IUnknown**. 
	// If this overload of CopyTo is not present, it will implicitly cast to IInspectable or IUnknown and match CopyTo(InterfaceType**) instead.
	// A valid polymoprhic downcast requires run-time type checking via QueryInterface, so CopyTo(InterfaceType**) will break type safety.
	// This overload matches ComPtrRef before the implicit cast takes place, preventing the unsafe downcast.
	template <typename U>
	HRESULT CopyTo(Details::ComPtrRef<ComPtr<U>> ptr, typename Details::EnableIf<
		((Details::IsSame<T, IInspectable>::value && !Details::IsSame<U, IUnknown>::value) || (Details::IsSame<T, IUnknown>::value))
		&& !Details::IsSame<U*, T*>::value, void *>::type * = 0) const throw()
	{
		return ptr_->QueryInterface(__uuidof(U), ptr);
	}

	HRESULT CopyTo(_Outptr_result_maybenull_ InterfaceType** ptr) const throw()
	{
		InternalAddRef();
		*ptr = ptr_;
		return S_OK;
	}

	HRESULT CopyTo(REFIID riid, _Outptr_result_nullonfailure_ void** ptr) const throw()
	{
		return ptr_->QueryInterface(riid, ptr);
	}

	template<typename U>
	HRESULT CopyTo(_Outptr_result_nullonfailure_ U** ptr) const throw()
	{
		return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void**>(ptr));
	}

	// query for U interface
	template<typename U>
	HRESULT As(_Inout_ Details::ComPtrRef<ComPtr<U>> p) const throw()
	{
		return ptr_->QueryInterface(__uuidof(U), p);
	}

	// query for U interface
	template<typename U>
	HRESULT As(_Out_ ComPtr<U>* p) const throw()
	{
		return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
	}

	// query for riid interface and return as IUnknown
	HRESULT AsIID(REFIID riid, _Out_ ComPtr<IUnknown>* p) const throw()
	{
		return ptr_->QueryInterface(riid, reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
	}

	HRESULT AsWeak(_Out_ WeakRef* pWeakRef) const throw()
	{
		return ::Microsoft::WRL::AsWeak(ptr_, pWeakRef);
	}

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
	HRESULT AsAgile(_Out_ AgileRef* pAgile) const throw()
	{
		return ::Microsoft::WRL::AsAgile(ptr_, pAgile);
	}
#endif // (NTDDI_VERSION >= NTDDI_WINBLUE)

};    // ComPtr
#endif

#if 0

#endif
