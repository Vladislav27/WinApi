#pragma once

#include <Windows.h>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

class CHeapManager {
public:
    CHeapManager();
    ~CHeapManager();
    void Create(int minSize, int maxSize);
    void* Alloc(int size);
    void Free(void* mem);
    void Destroy();
private:
	std::set<void*> free_small_blocks;  // Ýòî ìíîæåñòâî ñâîáîäíûõ áëîêîâ ðàçìåðîì < 4 Êá. Ðàçìåð õðàíèòüñÿ â ñàìèõ áëîêàõ

	// Ýòî äâà ìíîæåñòâà ñâîáîäíûõ áëîêîâ ðàçìåðîì îò 4 äî 128 Êá è áîëüøå 128 Êá. Ðàçìåð õðàíèòñÿ âòîðûì ýëåìåíòîì ïàðû
	std::set<std::pair<void*, int>> free_medium_blocks, free_large_blocks;
	std::set<void*> blocks;  // Ýòî ìíîæåñòâî çàíÿòûõ áëîêîâ
	std::vector<int> pages; // Ýòî ìàññèâ â êîòîðîì äëÿ êàæäîé ñòðàíèöû õðàíèòüñÿ êîëè÷åñòâî çàíÿòûõ áëîêîâ, ëåæàùèõ íà ýòîé ñòðàíèöå
	void* head_memmory; // Íà÷àëî çàðåçåðâèðîâàííîé ïàìÿòè
	int maxSize; // ìàêñèìàëüíûé ðàçìåð ïàìÿòè, êîòîðûé äîñòóïåí êó÷å

	void add_free_block(void* start, int size);
	void interval_pages(void* block, int size, int& beginPage, int& endPage);
	void* allocate_memory(void* mem, int memSize, int size);
	template <typename setType> std::pair<void*, int> CHeapManager::find_free_block(std::set<setType>& freeBlocks, int size);

	int get_block_size(void* pointer);
	int get_block_size(std::pair<void*, int> pair);
	void* get_block(void* pointer);
	void* get_block(std::pair<void*, int> pair);
	void add_empty_block_with_merge(void* block, int BlockSize);
	template <typename setType>	std::pair<void*, int> try_left_merge(std::set<setType>& targetSet, setType compareElement);
	template <typename setType>	std::pair<void*, int> try_right_merge(std::set<setType>& targetSet, setType compareElement, int BlockSize);
	std::pair<void*, int> get_merge_result(std::pair<void*, int> first, std::pair<void*, int> second, std::pair<void*, int> third);

	static const int page_size, medium_block_size;
	static int getPageSize();
};
