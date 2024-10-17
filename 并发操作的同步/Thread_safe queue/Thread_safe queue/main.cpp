#include "threadsafe_queue.h"

struct data_chunk{};
bool more_data_to_prepare();
data_chunk prepare_data();
void process(data_chunk);
bool is_last_chunk(data_chunk);

threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread() {
	while (more_data_to_prepare()) {
		data_chunk const data = prepare_data();
		data_queue.push(data);
	}
}

void data_processing_thread() {
	while (true) {
		data_chunk data;
		data_queue.wait_and_pop(data);
		process(data);
		if (is_last_chunk(data))
			break;
	}
}