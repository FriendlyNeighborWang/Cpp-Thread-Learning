#include <mutex>

class some_big_object{};
void swap(some_big_object& lhs, some_big_object& rhs);

class X {
private:
	some_big_object some_detail;
	std::mutex m;
public:
	X(some_big_object const& sd):some_detail(sd){}
	friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;
		std::lock(lhs.m, rhs.m);  // std::lock同时将两个互斥上锁，防止出现死锁现象
		// 还创建两个lock_guard对象可以通过对象的约束保证上锁与解锁不会中途因为异常中断而被破坏
		// std::adopt_lock对象告诉lock_guard在对象创建前互斥（std::mutex）就已经上锁
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);  
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_detail, rhs.some_detail);
	}

	// 上面的交换函数可以用下面的C++17引入的std::scoped_lock改进,scope_lock同时支持隐式类模板参数推导机制

	/*friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;
		std::scoped_lock(lhs.m, rhs.m);
		swap(lhs.some_detail, rhs.some_detail);
	}*/

};