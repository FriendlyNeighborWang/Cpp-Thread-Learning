#include <iostream>
#include <thread>
#include <algorithm>
#include <random>
#include <vector>
#include <queue>
#include <condition_variable>
#include <future>
#include <utility>

// 所需数据结构
struct element {
	int ele;
	element(){}
	element(int n):ele(n){}
};
using task_type = std::function<std::pair<bool, element>()>;
std::mutex mut;
std::vector<element> result_list;
std::condition_variable cond_var;
std::queue<task_type> task_list;
std::vector<std::thread> thread_list;


template<typename Iterator, typename T>
std::pair<bool, T> search(Iterator start, Iterator end, int target) {
	auto it = std::find_if(start, end, [target](T& item) {return item.ele == target; });
	if (it != end) 
		return std::make_pair(true, *it);
	return std::make_pair(false, element());
}

void worker_thread_task() {
	while (true) {
		{
			std::lock_guard<std::mutex> lk(mut);
			if (task_list.empty())
				break;
			auto this_pair = task_list.front()();
			task_list.pop();
			if (this_pair.first)
				result_list.push_back(this_pair.second);
		}
	}
}

int main() {
	std::vector<element> list(10000);
	std::random_device rd;
	std::mt19937 mt(rd()); 
	std::uniform_int_distribution<int> dist(1, 200);
	for (int i = 0; i < 10000; i++)
		list[i] = element(dist(mt));
	std::cout << "Data initialized" << std::endl;

	int target = 124;

	int count = 0;
	while (true) {
		auto start = list.begin() + count;
		auto end = (count + 100 <= list.size()) ? start + 100 : list.end();
		task_type taskpack = std::bind(search<decltype(end), element>, start, end, target);
		task_list.push(taskpack);
		if (count + 100 >= list.size())
			break;
		count += 100;
	}
	for (int i = 0; i < 10; i++)
		thread_list.emplace_back(worker_thread_task);

	std::cout << "Searching" << std::endl;

	for (auto& th : thread_list) 
		th.join();


	for (auto& res : result_list) 
		std::cout << res.ele << std::endl;
	
}