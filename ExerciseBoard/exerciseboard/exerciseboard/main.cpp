#include <chrono>
#include <future>
#include <thread>
#include <iostream>

struct num_pair {
	int a;
	int b;
};
void add_a(num_pair* ptr) {
	for(int i=0;i<100000;i++)
		ptr->a += 1;
}
void add_b(num_pair* ptr) { 
	for (int i = 0; i < 100000; i++)
		ptr->b += 1; 
}

int main() {
	num_pair t = { 0,0 };
	std::future<void> task_a = std::async(std::launch::async, add_a, &t);
	std::future<void> task_b = std::async(std::launch::async, add_b, &t);

	task_a.wait();
	task_b.wait();

	std::cout << t.a << " " << t.b << std::endl;
}