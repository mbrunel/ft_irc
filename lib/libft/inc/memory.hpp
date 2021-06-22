#pragma once

namespace ft
{

template<class T>
class uniquePtr;

template<class T>
class sharedPtr;

template<class T>
class weakPtr;

/**
 * Implementation of std::unique_ptr for C++98
 */
template <class T>
class uniquePtr
{
  public:
	uniquePtr(T *ptr = NULL):ptr(ptr) {}
	virtual ~uniquePtr() throw() { delete ptr; }

	T *release() throw()
	{
		T *r = ptr;
		ptr = NULL;
		return r;
	}
	void reset(T *nptr = NULL) throw()
	{
		delete ptr;
		ptr = nptr;
	}

	T *get() const { return ptr; }
	T &operator*() { return *ptr; }
	T *operator->() { return ptr; }
	operator bool() const { return (ptr); }

  private:
	T *ptr;
	uniquePtr<T> &operator=(const uniquePtr<T> &);
	uniquePtr(const uniquePtr<T> &);
};

template <class T>
struct ControlBlock
{
	size_t shared_nb;
	size_t weak_nb;
	T *ptr;
	ControlBlock(T *ptr = NULL):shared_nb(1), weak_nb(0), ptr(ptr) {}
};

/**
 * Implementation of std::shared_ptr for C++98
 */
template <class T>
class sharedPtr
{
  public:
	sharedPtr(T *ptr = NULL)
	{
		shared = new ControlBlock<T>(ptr);
	}
	sharedPtr(const sharedPtr<T> &old)
	{
		shared = old.shared;
		++shared->shared_nb;
	}
	sharedPtr(const weakPtr<T> &old)
	{
		shared = old.shared;
		++shared->shared_nb;
	}
	~sharedPtr() throw() { reset(); }

	void reset() throw()
	{
		if (!--shared->shared_nb)
		{
			delete shared->ptr;
			if (!shared->weak_nb)
				delete shared;
		}
		shared = NULL;
	}
	void reset(T *nptr)
	{
		reset();
		if (nptr)
			shared = new ControlBlock<T>(nptr);
	}

	T *get() const{ return shared ? shared->ptr : NULL; }
	T &operator*() { return *get(); }
	T *operator->() { return get(); }
	operator bool() const { return get(); }

  private:
	ControlBlock<T> *shared;
	sharedPtr &operator=(const sharedPtr<T> &);

	friend class weakPtr<T>;
};

/**
 * Implementation of std::weak_ptr for C++98
 */
template <class T>
class weakPtr
{
  public:
	weakPtr(const sharedPtr<T> &old)
	{
		shared = old.shared;
		++shared->weak_nb;
	}
	weakPtr(const weakPtr<T> &old)
	{
		shared = old.shared;
		++shared->weak_nb;
	}
	~weakPtr() throw() { reset(); }

	sharedPtr<T> lock() const throw()
	{
		return shared->shared_nb ? sharedPtr<T>(*this) : sharedPtr<T>(NULL);
	}

	void reset() throw()
	{
		if (!--shared->weak_nb && !shared->shared_nb)
			delete shared;
	}

  private:
	ControlBlock<T> *shared;
	weakPtr &operator=(const weakPtr<T> &);

	friend class sharedPtr<T>;
};

} /* end of namespace ft */
