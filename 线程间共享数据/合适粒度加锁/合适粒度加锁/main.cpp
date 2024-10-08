#include <mutex>

// 如果是类中含有数据量比较少的，可以先拷贝再比较，避免因为锁而占据较长时间。
// 同时这种情况可以一定程度避免死锁
// 但同时这种情况下会导致与之前比较方式不完全相同的语义，因为语句1和2之间有间隔，并不是真正意义上的同时间比较
class Y {
private:
	int some_detail;
	mutable std::mutex m;
	int get_detail() const {
		std::lock_guard<std::mutex> lock_a(m);
		return some_detail;
	}
public:
	Y(int sd):some_detail(sd){}
	friend bool operator==(const Y& lhs, const Y& rhs) {
		if (&lhs == &rhs)
			return true;
		int const lhs_value = lhs.get_detail();  // 1
		int const rhs_value = rhs.get_detail();  // 2
		return lhs_value == rhs_value;
	}
};