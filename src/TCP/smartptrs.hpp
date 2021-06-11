#pragma once

template<class T>
	class unique_ptr;

template<class T>
	class shared_ptr;

template<class T>
	class weak_ptr;


template <class T>
class unique_ptr
{
  public:
	unique_ptr(T *ptr = NULL):ptr(ptr) {}
	virtual ~unique_ptr() throw() { delete ptr; }

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
	unique_ptr<T> &operator=(const unique_ptr<T> &);
	unique_ptr(const unique_ptr<T> &);
};

template <class T>
struct ControlBlock
{
	size_t shared_nb;
	size_t weak_nb;
	T *ptr;
	ControlBlock(T *ptr = NULL):shared_nb(1), weak_nb(0), ptr(ptr) {}
};

template <class T>
class shared_ptr
{
  public:
	shared_ptr(T *ptr = NULL)
	{
		shared = new ControlBlock<T>(ptr);
	}
	shared_ptr(const shared_ptr<T> &old)
	{
		shared = old.shared;
		++shared->shared_nb;
	}
	shared_ptr(const weak_ptr<T> &old)
	{
		shared = old.shared;
		++shared->shared_nb;
	}
	~shared_ptr() throw () { reset(); }

	void reset() throw ()
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
	shared_ptr &operator=(const shared_ptr<T> &);

	friend class weak_ptr<T>;
};

template <class T>
class weak_ptr
{
  public:
	weak_ptr(const shared_ptr<T> &old)
	{
		shared = old.shared;
		++shared->weak_nb;
	}
	weak_ptr(const weak_ptr<T> &old)
	{
		shared = old.shared;
		++shared->weak_nb;
	}
	~weak_ptr() throw() { reset(); }

	shared_ptr<T> lock() const throw()
	{
		return shared->shared_nb ? shared_ptr<T>(*this) : shared_ptr<T>(NULL);
	}

	void reset() throw()
	{
		if (!--shared->weak_nb && !shared->shared_nb)
			delete shared;
	}

  private:
	ControlBlock<T> *shared;
	weak_ptr &operator=(const weak_ptr<T> &);

	friend class shared_ptr<T>;
};
