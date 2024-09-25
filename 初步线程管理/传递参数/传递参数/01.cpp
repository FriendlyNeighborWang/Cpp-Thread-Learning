#include <iostream>
#include <thread>
#include <memory>

void change_data(int& num) {
	num++;
	std::cout << "num in thread is " << num << std::endl;
}

class X {
public:
	void mem_func() {
		std::cout << "This is member function" << std::endl;
	}
};


int main() {
	int num = 1;
	// ������δ���ᷢ�����������Ϊ�ڴ������߳�ʱ����Ӵ��뺯���Ĳ�������ֱ�Ӹ���
	// std::thread th(change_data, num);
	
	// ͬһ������ʹ�õĶ���̹߳���ͬһ�������ڴ�ӳ�����
	std::thread th1(change_data, std::ref(num));
	th1.join();
	std::cout << "num outside is " << num << std::endl;

	// ʹ�ó�Ա��������Ҫ�������ʵ����ַ(��ֵ)(����C++����ģ�ͣ�������Ϊ�����ṩthisָ��)
	std::unique_ptr<X> x_ptr(new X());
	std::thread th2(&X::mem_func, std::move(x_ptr));  //ʹ������ָ�����ƶ������Ż�����
	th2.join();
}