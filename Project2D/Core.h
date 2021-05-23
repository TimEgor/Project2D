#pragma once

class Core final {
private:
	Core() = default;

public:
	static Core& get();

	bool init();
	void release();

	void run();
};