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
		std::lock(lhs.m, rhs.m);  // std::lockͬʱ������������������ֹ������������
		// ����������lock_guard�������ͨ�������Լ����֤���������������;��Ϊ�쳣�ж϶����ƻ�
		// std::adopt_lock�������lock_guard�ڶ��󴴽�ǰ���⣨std::mutex�����Ѿ�����
		std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);  
		std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
		swap(lhs.some_detail, rhs.some_detail);
	}

	// ����Ľ������������������C++17�����std::scoped_lock�Ľ�,scope_lockͬʱ֧����ʽ��ģ������Ƶ�����

	/*friend void swap(X& lhs, X& rhs) {
		if (&lhs == &rhs)
			return;
		std::scoped_lock(lhs.m, rhs.m);
		swap(lhs.some_detail, rhs.some_detail);
	}*/

};