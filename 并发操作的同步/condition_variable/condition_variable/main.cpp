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
			// ������RAII��ʹ�÷������뿪������lk�Զ�����
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(data);
		}
		// ͨ��conditional_variable������Ϣ����
		data_cond.notify_one();
	}
}

void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> lk(mut);
		// wait()��������һ�����������һ���ɵ��ö���������lambda���ʽ��
		// ���λ���ÿɵ��ö����������ж�ʧ������ǰ��Ļ������������������״̬
		// ǰ���notify_one()�Ὣwait���ѣ����������������µ��ÿɵ��ö���
		// ע��1����Ϊwait�ᷴ���Ի������������������ʹ�ý�Ϊ����unique_lock
		//     2���������ڼ�Ҫ�Ի��������Ϊ���ù��������������Ա�������������������
		data_cond.wait(lk, [] {return !data_queue.empty(); });
		data_chunk data = data_queue.front();
		data_queue.pop();
		lk.unlock();
		process(data);
		if (is_last_chunk(data))
			break;
	}
}