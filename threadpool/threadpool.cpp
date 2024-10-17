#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
std::mutex cout_mutex;
std::mutex qmutex;
class Task {
public:
	Task(std::string url, int a) : url(url), a(a) {}

	int foo() const {
		try {
			std::this_thread::sleep_for(std::chrono::seconds(10));
			std::lock_guard<std::mutex> lock(cout_mutex);
			std::cout << url << " " << a << std::endl;

			return 0;
		} catch(const std::exception& e) {
			std::cerr << "Exception in thread: " << e.what() << std::endl;
			return -1;
		}
	}
private:
	std::string url;
	int a;
};

class threadpool {
public:
	threadpool(int n) : n(n) {};
	void push_task(Task task) {
		qtasks.push(task);
	};
	void start() {
		for(int i = 0; i < n; i++) {

			Task t1 = qtasks.front();
			qtasks.pop();

			threads.emplace_back([=, &i] {
				while(true) {

					if(!qtasks.empty()) {

					} else {

					}
				}
				//t1.foo();
			});
		}
		for(std::thread& t : threads) {
			if(t.joinable()) {
				t.join();
			}
		}
	}
private:
	int n;
	std::queue<Task> qtasks;
	std::vector<std::thread> threads;
};

int main() {

	std::string strs[100] = {"a", "b", "c", "d", "e", "f", "g", "h", "i",
							 "j", "k", "l", "m", "n", "o", "p", "q", "r",
							 "s", "t", "u", "v", "w", "x", "y", "z"};
	threadpool tp(26);
	for(int i = 0; i < 26; i++) {
		std::string s = strs[i];
		int b = i;
		Task task(s, b);
		tp.push_task(task);
	}
	tp.start();
	return 0;
}