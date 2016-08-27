#pragma once

#include <functional>
#include <vector>
#include <set>
#include <algorithm>

template <typename... Args> class EventTempl;

template <typename... Args>
class  ListenerTempl
{
	typedef EventTempl<Args...> Event;
	std::set<Event*> _events;
	typename Event::F _function;


	void onDestroyEvent(Event& event) { _events.erase(&event); }
public:
	ListenerTempl(){}

	explicit ListenerTempl(Event& event, const typename Event::F& f) : _function(f)
	{
		_events.insert(&event);
		event.add(this);
	}

	ListenerTempl(const ListenerTempl& r)
	{
		_events = r._events;
		_function = r._function;
		for (auto& event : _events)
		{
			event->add(this);
		}
	}

	ListenerTempl(const typename Event::F& f) : _function(f)
	{
	}


	~ListenerTempl()
	{
		clear();
	}

	void set(Event& event, const typename Event::F& f)
	{
		clear();
		_events.insert(&event);
		_function = f;
		event.add(this);
	}

	void set(const typename Event::F& f)
	{
		clear();
		_function = f;
	}

	void add(Event& event)
	{
		_events.insert(&event);
		event.add(this);
	}

	void remove(Event& event)
	{
		event.remove(this);
		_events.erase(&event);
	}

	void clear()
	{
		for (auto& e : _events)
			e->remove(this);
		_events.clear();
	}

	size_t getSize()
	{
		return _events.size();
	}

	ListenerTempl& operator =(const ListenerTempl& r)
	{
		if (&r == this)
			return *this;

		_events = r._events;
		_function = r._function;
		for (auto& e : _events)
		{
			e->add(this);
		}
		return *this;
	}

	ListenerTempl& operator=(const typename Event::F& f)
	{
		set(f);
		return *this;
	}

	ListenerTempl& operator += (Event& event)
	{
		add(event);
		return *this;
	}

	ListenerTempl& operator -= (Event& event)
	{
		remove(event);
		return *this;
	}

	void internal_add(Event& event)
	{
		_events.insert(&event);
	}

	template <typename... A>
	friend class EventTempl;
};

template <typename... Args>
class EventTemplHandler
{
public:
	void addEventListener(const ListenerTempl<Args...>& listener)
	{
		_listeners.push_back(listener);
	}

	void removeEventListener(const ListenerTempl<Args...>& listener)
	{
		_listeners.remove(listener);
	}
private:
	std::list<ListenerTempl<Args...> >			_listeners;
};


template <typename... Args> class EventTempl
{
public:
	typedef ListenerTempl<Args...>			L;
	typedef ListenerTempl<Args...>			Listener;
	typedef EventTemplHandler<Args...>		Handler;

private:
	typedef EventTempl<Args...>			 MyType;
	typedef std::function<void(Args...)> F;
	typedef L*							 Entry;

	template <typename... A>
	friend class  ListenerTempl;

private:
	std::vector<typename MyType::Entry> _callback;
public:
	EventTempl() { }

	EventTempl(const EventTempl& evn)
	{
		operator=(evn);
	}

	~EventTempl()
	{
		for (auto e : _callback)
			e->onDestroyEvent(*this);
	}

	EventTempl& operator=(const EventTempl& evn)
	{
		if (this == &evn)
			return *this;

		_callback = evn._callback;
		std::for_each(_callback.begin(), _callback.end(),
			[this](MyType::Entry& entry)
		{
			entry->internal_add(*this);
		});
		return *this;
	}

private:
	void add(L* l)
	{
		if (std::find_if(_callback.begin(), _callback.end(), [l](const Entry& e){ return e == l; }) != _callback.end())
			return;
		_callback.push_back(l);
	}

	Entry* find(const L* l)
	{
		auto res = std::find_if(std::begin(_callback), std::end(_callback), [&l](const Entry& e){return e == l; });
		return res != _callback.end() ? &(*res) : nullptr;
	}

	void remove(L* l)
	{
		_callback.erase(std::remove_if(std::begin(_callback), std::end(_callback), [&l](const Entry& e){return e == l; }), std::end(_callback));
	}
public:
	void operator()(Args... args)
	{
		auto callbacks = _callback;
		for (auto& f : callbacks)
		{
			f->_function(args...);
		}
	}
};

/*
Usage examples:

class Model
{
public:
Event<const int&> callback;
Event<> zcallback;

void z() { std::cout << "zcallback\n";}

void test()
{
{
Event<>::Listener l(zcallback, std::bind(&Model::z, this));
zcallback();
std::cout << "end scope" << std::endl;
}
zcallback();
callback(42);
}
};


int main(int argc, _TCHAR* argv[])
{
Event<const int&>::Listener list;
auto func = [](int v) -> void
{
std::cout << "res: " << v << '\n';
};
Model m;

list.set(m.callback, func);

try
{
m.test();
}
catch(std::bad_function_call& e)
{
std::cerr<< e.what() << std::endl;
}

std::cout << "cleanup\n" ;

list.remove();

m.test();

{
Event<const std::string&, double>::Listener l2;

{
Event<const std::string&, double> e2;
l2.set(e2,  [](const std::string& s, double d) { std::cout << s.c_str() << d << std::endl; } );
e2("String and double args", 666.666);
}

//l2.remove();
}

char s;
std::cin >> s;

return 0;
}
*/
