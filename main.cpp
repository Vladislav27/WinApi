#include "Heap.h"
#include <ctime>

CHeapManager globalHeapManager;
HANDLE defaultHeapHandle;

template <unsigned int memberSize>
class DefaultHeap {
public:
	void* operator new(unsigned int size) {
		return HeapAlloc(defaultHeapHandle, 0, size * memberSize);
	}

	void operator delete(void* pointer) {
		HeapFree(defaultHeapHandle, 0, pointer);
	}
};

template <unsigned int memberSize>
class MyHeap {
public:
	void* operator new(unsigned int size) {
		return globalHeapManager.Alloc(size * memberSize);
	}

	void operator delete(void* pointer) {
		return globalHeapManager.Free(pointer);
	}
};

class CMyHeapRandom {
public:
	void* operator new(unsigned int size) {
		int memberSize = rand() % (1024 * 1024);
		return globalHeapManager.Alloc(size * memberSize);
	}

	void operator delete(void* pointer) {
		return globalHeapManager.Free(pointer);
	}
};

class CDefaultHeapRandom {
public:
	void* operator new(unsigned int size) {
		int memberSize = rand() % (1024 * 1024);
		return HeapAlloc(defaultHeapHandle, 0, size * memberSize);
	}

	void operator delete(void* pointer) {
		HeapFree(defaultHeapHandle, 0, pointer);
	}
};

void test1() {
	CHeapManager manager;
	manager.Create(1 << 20, 1 << 25);
	int* value = static_cast<int*>(manager.Alloc(sizeof(int)));
	*value = 1000;
	int* array = static_cast<int*>(manager.Alloc(1000 * sizeof(int)));
	for (int i = 0; i < 1000; i++) {
		array[i] = i;
	}
	for (int i = 0; i < 1000; i++) {
		if (array[i] != i) {
			std::cerr << "Error!" << std::endl;
			exit(1);
		}
	}
	if (*value != 1000) {
		std::cerr << "Error!" << std::endl;
		exit(1);
	}
	manager.Free(value);
	for (int i = 0; i < 1000; i++) {
		if (array[i] != i) {
			std::cerr << "Error!" << std::endl;
			exit(1);
		}
	}
	manager.Free(array);
	manager.Destroy();
}

void test2() {
	std::vector<CMyHeapRandom*> pointers;

	globalHeapManager.Create(1 << 12, 1 << 22);
	for (int i = 0; i < 128 * 1024; i++) {
		CMyHeapRandom* pointer = static_cast<CMyHeapRandom*>(new CMyHeapRandom[100]);
		pointers.push_back(pointer);
	}
	for (int i = 0; i < 128 * 1024; i++) {
		delete[] pointers[i];
	}

	globalHeapManager.Destroy();
}

void test3() {
	std::vector<CDefaultHeapRandom*> pointers;

	defaultHeapHandle = HeapCreate(0, 1 << 12, 1 << 22);
	for (int i = 0; i < 128 * 1024; i++) {
		CDefaultHeapRandom* pointer = static_cast<CDefaultHeapRandom*>(new CDefaultHeapRandom);
		pointers.push_back(pointer);
	}
	for (int i = 0; i < 128 * 1024; i++) {
		delete pointers[i];
	}
	HeapDestroy(defaultHeapHandle);
}

void test4() {
	std::vector<CMyHeapRandom*> pointers;

	globalHeapManager.Create(1 << 16, 1 << 24);
	for (int i = 0; i < 1024; i++) {
		CMyHeapRandom* pointer = static_cast<CMyHeapRandom*>(new CMyHeapRandom[100]);
		pointers.push_back(pointer);
	}
	int f;
	std::cin >> f;
	for (int i = 0; i < 1024; i++) {
		delete[] pointers[i];
	}
	globalHeapManager.Destroy();
}

int main(int argc, char **argv) {
	test1();
	unsigned int start_time1 = clock();
	test2();
	unsigned int end_time1 = clock();
	unsigned int time1 = end_time1 - start_time1;
	unsigned int start_time2 = clock();
	test3();
	unsigned int end_time2 = clock();
	unsigned int time2 = end_time2 - start_time2;

	std::cout << "My time: " << time1 << std::endl;
	std::cout << "Heap time: " << time2 << std::endl;

	test4();

	int f;
	std::cin >> f;
	return 0;
}