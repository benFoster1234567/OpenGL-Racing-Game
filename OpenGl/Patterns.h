#ifndef PATTERNS_H
#define PATTERNS_H

#include <vector>
#include <stack>
#include <memory>

namespace Patterns
{

	template<typename... Args>
	class IObserver
	{
	public:
		virtual void OnNotify(Args... args) = 0;
		virtual ~IObserver() = default;
	};

	template<typename... Args>
	class Observable
	{
	private:
		std::vector<IObserver<Args...>*> observers;

	public:

		void notify(Args... args)
		{
			for (auto observer : observers)
			{
				observer->OnNotify(args...);
			}
		}

		void subscribe(IObserver<Args...>* observer)
		{
			observers.push_back(observer);
		}

		void unsubscribe(IObserver<Args...>* observer)
		{
			observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
		}

		void operator+=(IObserver<Args...>* observer)
		{
			subscribe(observer);
		}

		void operator-=(IObserver<Args...>* observer)
		{
			unsubscribe(observer);
		}
	};





	template<typename T>
	class Singleton
	{
	protected:
		Singleton()
		{
		}
	public:

		static
			T& getInstance()
		{
			static T instance;
			return instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

	};

	struct ICommand
	{
		virtual void execute() = 0;
		virtual void undo() = 0;
		virtual ~ICommand() = default;
	};



	template<typename T = ICommand>
	class CommandManager
	{
	private:
		std::stack<std::unique_ptr<T>> undoStack;
		std::stack<std::unique_ptr<T>> redoStack;

	public:
		void executeCommand(std::unique_ptr<T> cmd)
		{
			cmd->execute();
			undoStack.push(std::move(cmd));
			//purge redo stack
			for (; !redoStack.empty(); redoStack.pop());
		}

		void undo()
		{
			if (undoStack.empty()) return;
			auto c = std::move(undoStack.top());
			undoStack.pop();
			c->undo();
			redoStack.push(std::move(c));
		}

		void redo()
		{
			if (redoStack.empty()) return;
			auto c = std::move(redoStack.top());
			redoStack.pop();
			c->execute();
			undoStack.push(std::move(c));
		}

	};

	template<typename T>
	class IComparable
	{
	private:
		virtual int compareTo(const T& other) const = 0;
	public:

		bool operator==(const T& other) const
		{
			return compareTo(other) == 0;
		}

		bool operator!=(const T& other) const
		{
			return compareTo(other) != 0;
		}

		bool operator<(const T& other) const
		{
			return compareTo(other) < 0;
		}

		bool operator>(const T& other) const
		{
			return compareTo(other) > 0;
		}

		bool operator<=(const T& other) const
		{
			return compareTo(other) <= 0;
		}

		bool operator>=(const T& other) const
		{
			return compareTo(other) >= 0;
		}

	};

}

#endif