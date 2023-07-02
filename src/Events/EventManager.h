#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <memory>
#include <functional>
#include <queue>

typedef std::uint32_t EventID;

template<typename T>
class CallbackContainer
{
public:
	struct ListenerHandle
	{
		std::uint32_t id;
	};

private:
	std::vector<std::function<void(T&)>> callbacks;
	std::vector<ListenerHandle> free_handles;

public:

	template<typename T_Function>
	auto add_callback(T_Function callback) -> ListenerHandle;
	void remove_callback(ListenerHandle handle);

	void fire(T& event);
};

template<typename T>
inline void CallbackContainer<T>::fire(T& event)
{
	for (auto& callback : callbacks)
		if (callback) callback(event);
}

template<typename T>
template<typename T_Function>
inline auto CallbackContainer<T>::add_callback(T_Function callback) -> ListenerHandle
{
	ListenerHandle handle;

	if (free_handles.empty())
	{
		handle = { static_cast<uint32_t>(callbacks.size()) };
		callbacks.emplace_back(handle.id);
	}
	else
	{
		handle = { free_handles.back() };
		free_handles.pop_back();
		callbacks[handle.id] = callback;
	}

	return handle;
}

template<typename T>
inline void CallbackContainer<T>::remove_callback(ListenerHandle handle)
{
	callbacks[handle] = nullptr;
	free_handles.push_back(handle);
}

class EventManager
{
	template<typename T>
	using CallbackContainers = std::vector<std::unique_ptr<CallbackContainer<T>>>;
public:
	template<typename T, typename T_Function>
	static auto listen(T_Function callback) -> CallbackContainer<T>::ListenerHandle;

	template<typename T, typename T_Instance, typename T_Function>
	static auto listen(T_Instance* instance, T_Function callback) -> CallbackContainer<T>::ListenerHandle;

	template<typename T>
	static void remove_listener(CallbackContainer<T>::ListenerHandle handle);

	template<typename T, typename... T_Args>
	static void fire(T_Args&& ...args);

	template<typename T>
	static void fire(T& event);

	template<typename T>
	static inline CallbackContainer<T>& get_callbacks();

private:
	template<typename T>
	static auto get_event_id() -> EventID;

	template<typename T>
	static auto register_event() -> EventID;

private:
	template<typename T>
	static inline CallbackContainer<T>    s_callbacks;

	static inline EventID               s_next_event_id{ 0u };
};

template<typename T>
inline CallbackContainer<T>& EventManager::get_callbacks()
{
	static CallbackContainer<T> callbacks{};
	return callbacks;
}

template<typename T, typename T_Function>
inline CallbackContainer<T>::ListenerHandle EventManager::listen(T_Function callback)
{
	return s_callbacks<T>.add_callback(callback);
}

template<typename T, typename T_Instance, typename T_Function>
inline CallbackContainer<T>::ListenerHandle EventManager::listen(T_Instance* instance, T_Function callback)
{
	return s_callbacks<T>.add_callback([instance, callback](T* event) { (instance->*callback)(event); });
}

template<typename T>
inline void EventManager::remove_listener(CallbackContainer<T>::ListenerHandle handle)
{
	s_callbacks<T>.remove_callback(handle);
}

template<typename T, typename ...T_Args>
inline void EventManager::fire(T_Args&& ...args)
{
	T event (std::forward<T_Args>(args)...);
	s_callbacks<T>.fire(event);
}

template<typename T>
inline void EventManager::fire(T& event)
{
	s_callbacks<T>.fire(event);
}

template<typename T>
inline EventID EventManager::get_event_id()
{
	static EventID event_id = register_event<T>();
	return event_id;
}

template<typename T>
inline EventID EventManager::register_event()
{
	s_callbacks.emplace_back(std::make_unique<CallbackContainer<T*>>(s_next_event_id));
	return s_next_event_id++;
}