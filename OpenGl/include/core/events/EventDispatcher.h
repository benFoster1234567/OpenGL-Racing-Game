#pragma once
#include <functional>


namespace Engine::Core {


	template<typename... Args>
	class EventDispatcher final
	{
	private:

		struct Listener
		{
			size_t id;
			std::function<void(Args...)> f;
		};

	public:
		using CallBackFunc = std::function<void(Args...)>;
		using TokenId = size_t;

		EventDispatcher() = default;
		~EventDispatcher() = default;

		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;

		EventDispatcher(EventDispatcher&&) = default;
		EventDispatcher& operator=(EventDispatcher&&) = default;

		TokenId subscribe(CallBackFunc func)
		{
			auto id = curid++;
			callbacks.emplace_back(Listener{ id, func });
			return id;
		}

		void unsubscribe(TokenId id)
		{
			std::erase_if(callbacks, [id](const Listener& l) { return l.id == id; });
		}

		void invoke(Args... args)
		{
			for (const auto& l : callbacks)
			{
				l.f(args...);
			}
		}

	private:
		size_t curid{ 0 };
		std::vector<Listener> callbacks{};
	};

} 