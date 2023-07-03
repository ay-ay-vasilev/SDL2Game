#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <memory>
#include <functional>
#include <queue>

namespace events
{
	template<typename T>
	struct CallbackContainer
	{
		using CallbackType = std::function<void(const T&)>;

		struct ListenerHandle
		{
			std::uint32_t id;
		};

		std::vector<CallbackType> callbacks;
		std::vector<ListenerHandle> free_handles;

		auto add_callback(CallbackType callback) -> ListenerHandle;

		void remove_callback(ListenerHandle handle);
	};

	template<typename T>
	inline auto CallbackContainer<T>::add_callback(CallbackType callback) -> CallbackContainer<T>::ListenerHandle
	{
		CallbackContainer<T>::ListenerHandle handle;

		if (free_handles.empty())
		{
			handle = { static_cast<std::uint32_t>(callbacks.size()) };
			callbacks.emplace_back(callback);
		}
		else
		{
			handle = free_handles.back();
			free_handles.pop_back();
			callbacks[handle.id] = callback;
		}

		return handle;
	}

	template<typename T>
	void CallbackContainer<T>::remove_callback(CallbackContainer<T>::ListenerHandle handle)
	{
		callbacks[handle.id] = nullptr;
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
		static void fire(T_Args&&...args);

	private:
		template<typename T>
		static inline CallbackContainer<T> s_callbacks;
	};


	template<typename T, typename T_Function>
	inline CallbackContainer<T>::ListenerHandle EventManager::listen(T_Function callback)
	{
		return s_callbacks<T>.add_callback(callback);
	}

	template<typename T, typename T_Instance, typename T_Function>
	inline CallbackContainer<T>::ListenerHandle EventManager::listen(T_Instance* instance, T_Function callback)
	{
		return s_callbacks<T>.add_callback([instance, callback](const T& event) { (instance->*callback)(&event); });
	}

	template<typename T>
	inline void EventManager::remove_listener(CallbackContainer<T>::ListenerHandle handle)
	{
		s_callbacks<T>.remove_callback(handle);
	}

	template<typename T, typename ...T_Args>
	inline void EventManager::fire(T_Args&& ...args)
	{
		T event{ std::forward<T_Args>(args)... };

		for (auto& callback : s_callbacks<T>.callbacks)
			if (callback) callback(event);
	}
}