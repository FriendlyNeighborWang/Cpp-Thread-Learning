#include <future>
#include <iostream>

int find_the_answer_to_ltuae();
void do_other_stuff();

int main() {
	std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
	// �������߳���������
	std::future<int> another_answer1 = std::async(std::launch::async, find_the_answer_to_ltuae);
	// �������ڵ�ǰ�߳��Ӻ�ִ�У�ֱ��std::future�������get()��wait()ʱ
	std::future<int> another_answer2 = std::async(std::launch::deferred, find_the_answer_to_ltuae);
	// ����ʵ������ѡ�����з�ʽ
	std::future<int> another_answer3 = std::async(std::launch::deferred | std::launch::async, find_the_answer_to_ltuae);
	do_other_stuff();
	std::cout << "The answer is" << the_answer.get() << std::endl;
}