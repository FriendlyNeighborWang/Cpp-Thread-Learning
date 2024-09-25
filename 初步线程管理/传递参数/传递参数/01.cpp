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
	// 下面这段代码会发生编译错误，因为在创建新线程时会忽视传入函数的参数需求，直接复制
	// std::thread th(change_data, num);
	
	// 同一个程序使用的多个线程共享同一个虚拟内存映射机制
	std::thread th1(change_data, std::ref(num));
	th1.join();
	std::cout << "num outside is " << num << std::endl;

	// 使用成员函数，需要传入对象实例地址(左值)(根据C++对象模型，是用于为函数提供this指针)
	std::unique_ptr<X> x_ptr(new X());
	std::thread th2(&X::mem_func, std::move(x_ptr));  //使用智能指针与移动函数优化性能
	th2.join();
}