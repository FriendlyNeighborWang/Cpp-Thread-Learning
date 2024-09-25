# include<iostream>
# include<thread>

void sayhello() {
	std::cout << "Hello Thread World" << std::endl;
}

int main() {
	std::thread th(sayhello);
	th.join();
}