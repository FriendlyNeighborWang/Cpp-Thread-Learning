# include<mutex>
# include<list>
# include<algorithm>
# include<deque>
# include<vector>
# include<iostream>
# include"threadsafe_stack.h"

std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value) {
	// std::lock_guard�����ڴ���ʱ���Զ�����mutex.lock()
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
	std::lock_guard<std::mutex> guard(some_mutex);
	return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
} 

std::mutex loc;
int result = 0;

int main() {
	std::deque<int> deq = { 1,2,3,4 };
	std::stack<int> stack_origin(deq);
	threadsafe_stack<int> my_stack(stack_origin);

	// ע�����Ҫ����Ƿ�ᷢ����ջ�������⣬һ��Ҫÿ����ز���ʱ������һ���쳣����
	auto operation = [&my_stack]() {
		try {
			std::lock_guard<std::mutex> lock(loc);
			std::cout << my_stack.top() << std::endl;
			result += *my_stack.pop();
		}
		catch (empty_stack& error) {
			std::cout << error.what() << std::endl;
		}
	};
		

	std::vector<std::thread> th_list;
	for (int i = 0; i < 3; i++) {
		th_list.push_back(std::thread(operation));
	}

	operation();
	
	//��֤�ڴ�ӡresultǰ�����̶߳������
	for (auto& th : th_list) {
		th.join();
	}

	std::cout << "final result is "<< result << std::endl;

	
}