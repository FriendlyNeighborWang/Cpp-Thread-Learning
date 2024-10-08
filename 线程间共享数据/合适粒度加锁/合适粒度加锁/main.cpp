#include <mutex>

// ��������к����������Ƚ��ٵģ������ȿ����ٱȽϣ�������Ϊ����ռ�ݽϳ�ʱ�䡣
// ͬʱ�����������һ���̶ȱ�������
// ��ͬʱ��������»ᵼ����֮ǰ�ȽϷ�ʽ����ȫ��ͬ�����壬��Ϊ���1��2֮���м�������������������ϵ�ͬʱ��Ƚ�
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