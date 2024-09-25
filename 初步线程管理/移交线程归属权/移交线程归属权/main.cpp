#include <iostream>
#include <thread>

class scoped_thread {
	std::thread t;
public:
	explicit scoped_thread(std::thread t_) :t(std::move(t_)) {
		if (!t.joinable())
			throw std::logic_error("No thread");
	}
	
	~scoped_thread() {
		t.join();
	}
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
};

// 一种在销毁时直接调用join()的线程
// 使用noexcept标识符本身可以优化性能（省略系统对函数是否抛出异常的检查），并且增加一定安全性（发生异常直接调用terminate()）
class joining_thread {
	std::thread t;
public:
	joining_thread() noexcept = default;
	template<typename Callable, typename... Args>
	// 这是在通过原std::thread对象创建时能够向其中添加函数用到的参数时使用的技术
	// 使用完美转发(右值引用&&与std::forward)写法保持参数的左右值性质
	// Args是转发参数包
	explicit joining_thread(Callable&& func, Args&& ... args) :t(std::forward<Callable>(func), std::forward<Args>(args)...) {}
	explicit joining_thread(std::thread t_) noexcept :t(std::move(t_)) {}

	// 移动构造函数与运算符
	joining_thread(joining_thread&& other) noexcept :t(std::move(other.t)) {}
	joining_thread& operator=(joining_thread&& other) noexcept {
		if (joinable())
			join();
		t = std::move(other.t);
		return *this;
	}
	joining_thread& operator=(std::thread other) noexcept {
		if (joinable())
			join();
		t = std::move(other);
		return *this;
	}

	~joining_thread() noexcept{
		if (joinable())
			join();
	}

	void swap(joining_thread& other) {
		t.swap(other.t);
	}

	std::thread::id get_id() const noexcept {
		return t.get_id();
	}

	bool joinable() const noexcept {
		return t.joinable();
	}

	void join() {
		t.join();
	}

	void detach() {
		t.detach();
	}

	std::thread& as_thread() noexcept{
		return t;
	}

	const std::thread& as_thread() const noexcept {
		return t;
	}
};