# include<iostream>
# include<thread>
# include<numeric>
# include<vector>

//accumlate函数是一个能够计算容器内所有元素总和的函数，可以使用多线程进行加速这一过程，与std::reduce函数等价
template<typename Iterator, typename T>
struct accumlate_block {
	void operator()(Iterator first, Iterator last, T& result) {
		result = std::accumlate(first, last, result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
	unsigned long length = std::distance(first, last);

	if (!length)
		return init;
	unsigned long const min_per_thread = 25;  // 每个线程处理的元素的最低数量
	unsigned long const max_thread = (length + min_per_thread - 1) / min_per_thread;  //需要的最大线程数量

	// 限定创建线程数不要超过系统最大线程数量
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_thread);
	unsigned long const block_size = length / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1), ++i) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumlate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}

	//注：前面的()是为了实例化一个accumulate_block对象（因为他是一个结构体），后面是调用operator()
	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

	for (auto& entry : threads)
		entry.join();
	return std::accumulate(result.begin(), result.end(), init);
}

int main() {

}