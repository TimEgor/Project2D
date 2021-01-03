#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadItemQueue final {
private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable condVar;

public:
	ThreadItemQueue() = default;
	
	void push(const T& val);
	void push(T&& val);
	void pop();

	T& front();
	const T& front() const;

	bool tryGetFrontAndPop(T* val);
	void waitFrontAndPop(T* val);

	bool empty() const;
	size_t size() const;
};

template<typename T>
inline void ThreadItemQueue<T>::push(const T& val) {
	std::lock_guard<std::mutex> locker(mutex);
	queue.push(val);
	condVar.notify_one();
}

template<typename T>
inline void ThreadItemQueue<T>::push(T&& val) {
	std::lock_guard<std::mutex> locker(mutex);
	queue.push(val);
	condVar.notify_one();
}

template<typename T>
inline void ThreadItemQueue<T>::pop() {
	std::lock_guard<std::mutex> locker(mutex);
	queue.pop();
}

template<typename T>
inline T& ThreadItemQueue<T>::front() {
	std::lock_guard<std::mutex> locker(mutex);
	return queue.front();
}

template<typename T>
inline const T& ThreadItemQueue<T>::front() const {
	std::lock_guard<std::mutex> locker(mutex);
	return queue.front();
}

template<typename T>
inline bool ThreadItemQueue<T>::tryGetFrontAndPop(T* val) {
	std::lock_guard<std::mutex> locker(mutex);

	if (queue.empty()) {
		*val = nullptr;
		return false;
	}

	new (val) T(front());
	pop();

	return true;
}

template<typename T>
inline void ThreadItemQueue<T>::waitFrontAndPop(T* val) {
	std::unique_lock<std::mutex> locker(mutex);

	condVar.wait(locker, [this]() { return !queue.empty(); });

	new (val) T(front());
	pop();
}

template<typename T>
inline bool ThreadItemQueue<T>::empty() const {
	std::lock_guard<std::mutex> locker(mutex);
	return queue.empty();
}

template<typename T>
inline size_t ThreadItemQueue<T>::size() const {
	std::lock_guard<std::mutex> locker(mutex);
	return queue.size();
}
