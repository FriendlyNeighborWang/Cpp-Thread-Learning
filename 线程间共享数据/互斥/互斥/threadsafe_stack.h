# pragma once
# include <exception>
# include <memory>
# include <mutex>
# include <stack>

struct empty_stack :std::exception {
	// 后面的throw()是早期实现中用来告知编译器不会抛出异常的，先版本已经用noexcept代替
	const char* what() const throw() {
		return "Stack has already been empty";
	}
};

template<typename T>
class threadsafe_stack {
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack(){}

	threadsafe_stack(std::stack<T> s):data(std::move(s)){}

	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard<std::mutex> lock(m);
		data = other.data;
	}

	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data(std::move(new_value));
	}

	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	void pop(T& value) {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		value = data.top();
		data.pop();
	}
	T top() const {
		std::lock_guard<std::mutex> lock(m);
		return data.top();
	}

	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};