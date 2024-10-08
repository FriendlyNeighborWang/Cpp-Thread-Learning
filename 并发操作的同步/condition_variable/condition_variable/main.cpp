#include <condition_variable>
#include <mutex>
#include <queue>

struct data_chunk{};
data_chunk prepare_data();
bool more_data_to_prepare();
void process(data_chunk);
bool is_last_chunk(data_chunk);

std::mutex mut;
std::queue<data_chunk> data_queue;
std::condition_variable data_cond;
void data_preparation_thread() {
	while (more_data_to_prepare()) {
		data_chunk data = prepare_data();
		{
			// 这里是RAII的使用方法，离开代码块后lk自动销毁
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(data);
		}
		// 通过conditional_variable进行消息发送
		data_cond.notify_one();
	}
}

void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> lk(mut);
		// wait()函数接受一个互斥变量和一个可调用对象（下面是lambda表达式）
		// 初次会调用可调用对象，若条件判断失败则会对前面的互斥解锁，并进入休眠状态
		// 前面的notify_one()会将wait唤醒，互斥上锁，并重新调用可调用对象
		// 注：1、因为wait会反复对互斥上锁或解锁，所以使用较为灵活的unique_lock
		//     2、在休眠期间要对互斥解锁是为了让工作进程能正常对保护对象进行上锁与访问
		data_cond.wait(lk, [] {return !data_queue.empty(); });
		data_chunk data = data_queue.front();
		data_queue.pop();
		lk.unlock();
		process(data);
		if (is_last_chunk(data))
			break;
	}
}