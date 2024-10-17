#include <future>
#include <iostream>

int find_the_answer_to_ltuae();
void do_other_stuff();

int main() {
	std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
	// 启用新线程运行任务
	std::future<int> another_answer1 = std::async(std::launch::async, find_the_answer_to_ltuae);
	// 将任务在当前线程延后执行，直到std::future对象调用get()或wait()时
	std::future<int> another_answer2 = std::async(std::launch::deferred, find_the_answer_to_ltuae);
	// 交由实现自主选择运行方式
	std::future<int> another_answer3 = std::async(std::launch::deferred | std::launch::async, find_the_answer_to_ltuae);
	do_other_stuff();
	std::cout << "The answer is" << the_answer.get() << std::endl;
}