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

// һ��������ʱֱ�ӵ���join()���߳�
// ʹ��noexcept��ʶ����������Ż����ܣ�ʡ��ϵͳ�Ժ����Ƿ��׳��쳣�ļ�飩����������һ����ȫ�ԣ������쳣ֱ�ӵ���terminate()��
class joining_thread {
	std::thread t;
public:
	joining_thread() noexcept = default;
	template<typename Callable, typename... Args>
	// ������ͨ��ԭstd::thread���󴴽�ʱ�ܹ���������Ӻ����õ��Ĳ���ʱʹ�õļ���
	// ʹ������ת��(��ֵ����&&��std::forward)д�����ֲ���������ֵ����
	// Args��ת��������
	explicit joining_thread(Callable&& func, Args&& ... args) :t(std::forward<Callable>(func), std::forward<Args>(args)...) {}
	explicit joining_thread(std::thread t_) noexcept :t(std::move(t_)) {}

	// �ƶ����캯���������
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