#include "Heap.h"

const int CHeapManager::page_size = CHeapManager::getPageSize();
const int CHeapManager::medium_block_size = CHeapManager::page_size * 32;

int CHeapManager::getPageSize() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwPageSize;
}

CHeapManager::CHeapManager() {
	head_memmory = 0;
}

CHeapManager::~CHeapManager() {
}

void CHeapManager::Create(int minSize, int maxSize) {
	// Äëÿ íà÷àëà îêðóãëèì misSize è maxSize_ äî 4 Êá
	minSize = (minSize + page_size - 1) / page_size * page_size;
	maxSize = (maxSize + page_size - 1) / page_size * page_size;
	//Çàðåçåðâèðóåì íåïðåðûâíûé ðåãèîí ïàìÿòè ðàçìåðîì maxSize
	head_memmory = VirtualAlloc(NULL, maxSize, MEM_RESERVE, PAGE_READWRITE);
	if( head_memmory != NULL) {
		//Commit äëÿ ÷àñòè ýòîãî ðåãèîíà
		void* commit_memmory = VirtualAlloc(head_memmory, minSize, MEM_COMMIT, PAGE_READWRITE);
		if (commit_memmory != NULL) {
			int number_pages = minSize / page_size;
			pages.resize(number_pages, 1);
			add_free_block(head_memmory, maxSize); // äîáàâèì â ìíîæåñòâî ñâîáîäíûõ áëîêîâ íîâûé ñâîáîäíûé áëîê
		}
		else {
			std::cerr << "Error: cannot commit memory!" << std::endl;
			return;
		}
	}
	else {
		std::cerr << "Error: cannot allocate memory!" << std::endl;
		return;
	}
}// Äàåò íîìåðà ñòðàíèö (ñ êàêîé ïî êàêóþ ìû çíèìàåì)
void CHeapManager::interval_pages(void* block, int size, int& begin_page, int& end_page) {
	int byte_start = static_cast<byte*>(block) - static_cast<byte*>(head_memmory);
	begin_page = byte_start / page_size;
	end_page = (byte_start + sizeof(int) + size - 1) / page_size;
}

// Äîáàâëÿåò ñâîáîäíûé êóñîê ïàìÿòè â îäíî èç õðàíèëèù
void CHeapManager::add_free_block(void* start, int size) { 
	if (size < page_size) {
		int begin_page, end_page;
		interval_pages(start, size, begin_page, end_page);
		if (pages[begin_page] == 0) {  
			exit(0);
		}
		memcpy(start, &size, sizeof(int));  // Õðàíèì ðàçìåð áëîêà â ñàìîì áëîêå
		free_small_blocks.insert(start);
	}
	else if (size < medium_block_size) {
		free_medium_blocks.insert(std::make_pair(start, size));
	}
	else {
		free_large_blocks.insert(std::make_pair(start, size));
	}
}

void* CHeapManager::Alloc(int size) {
	int block = sizeof(int);
	size = (size + block - 1) / block * block;  // Îêðóãëÿåì äî 4 áàéò
	void* begin_memmory = NULL;  // àäðåñ äîñòóïíîãî áëîêà
	int memorry_size = 0;  // ðàçìåð äîñòóïíîãî áëîêà
	// èùåì äîñòóïíûé áëîê
	if (size + sizeof(int) < page_size) {
		std::make_pair(std::ref(begin_memmory), std::ref(memorry_size)) = find_free_block(free_small_blocks, size); 
	}
	if (begin_memmory == NULL && size + sizeof(int) < medium_block_size) {
		std::make_pair(std::ref(begin_memmory), std::ref(memorry_size)) = find_free_block(free_medium_blocks, size);
	}
	if (begin_memmory == NULL) {
		std::make_pair(std::ref(begin_memmory), std::ref(memorry_size)) = find_free_block(free_large_blocks, size);
	}
	if (begin_memmory == NULL) {
		std::cerr << "Failed get memory" << std::endl;
		return 0;
	}
	else {
		return allocate_memory(begin_memmory, memorry_size, size);  // Îòùèïëÿåì ïàìÿòü èç ñâîáîäíîãî áëîêà
	}
}

void CHeapManager::Free(void *mem) {
	mem = static_cast<byte*>(mem) - sizeof(int);  // Àäðåñ áëîêà, êîòîðûé õðàíèòñÿ â blocks 
	int memSize;
	memcpy(&memSize, mem, sizeof(int));
	int beginPage, endPage;
	interval_pages(mem, memSize, beginPage, endPage);
	int beginFreeInterval, endFreeInterval;
	beginFreeInterval = beginPage - 1;
	for (int page = beginPage; page <= endPage; page++) {
		--pages[page];
		if (pages[page] == 0) {
			if (beginFreeInterval == beginPage - 1) {
				beginFreeInterval = page;
			}
			endFreeInterval = page;
		}
	}
	// äîáàâèì â ìíîæåñòâî ñâîáîäíûõ áëîêîâ íîâûé ñâîáîäíûé áëîê
	// è ïðè âîçìîæíîñòè îáúåäèíèì åãî ñ ñîñåäíèìè
	add_empty_block_with_merge(mem, memSize + sizeof(int));
	blocks.erase(mem);
	if (beginFreeInterval >= beginPage) {
		VirtualFree(static_cast<byte*>(head_memmory) + beginFreeInterval * page_size, (endFreeInterval - beginFreeInterval + 1)
			* page_size, MEM_DECOMMIT);  // îñâîáîæäàåì ôèçè÷åñêóþ ïàìÿòü, íî ñîõðàíÿåì âèðòóàëüíóþ
	}
}

void CHeapManager::Destroy() {
	for( auto it = blocks.begin(); it != blocks.end(); it++ ) {
		std::cerr << "Unfreed region: start " << *it << " size: " << get_block_size(*it) << std::endl;
	}
	VirtualFree(head_memmory, maxSize, MEM_RELEASE);
	head_memmory = 0;
	pages = std::vector<int>();
	free_small_blocks = std::set<void*>();
	free_large_blocks = free_medium_blocks = std::set<std::pair<void*, int>>();
}

void* CHeapManager::allocate_memory(void* mem, int memSize, int size) {  // îòùèïûâàåì êóñîê ïàìÿòè èç ñâîáîäíîãî áëîêà
	int beginPage, endPage;
	interval_pages(mem, size, beginPage, endPage);
	if (endPage >= pages.size()) {
		pages.resize(endPage + 1, 0);  // 0 áóäåò òîëüêî ó íîâûõ ñòðàíèö
	}
	void* memCommited = VirtualAlloc(mem, size + sizeof(int), MEM_COMMIT, PAGE_READWRITE);  // â íà÷àëî êàæäîãî áëîêà çàïèñûâàåì ðàçìåð
	if (memCommited != NULL) {
		memcpy(mem, &size, sizeof(int));
		for (int page = beginPage; page <= endPage; page++) {
			pages[page] += 1;
		}
		if (size + sizeof(int) < memSize) {
			// ïîñëå òîãî êàê ìû îòùèïèëè êóñî÷åê ïàìÿòè, 
			// îñòàâøóþñÿ ïàìÿòü ñíîâà äåëàåì ñâîáîäíîé
			add_free_block(static_cast<byte*>(mem) + sizeof(int) + size, memSize - size - sizeof(int));
		}
		blocks.insert(mem);
		return static_cast<byte*>(mem) + sizeof(int);  // âîçâðàùàåì óêàçàòåëü íà äàííûå
	}
	else {
		std::cerr << "Error: cannot allocate memory!" << std::endl;
		return 0;
	}
}

// Èùåì ñâîáîäíûé áëîê â ìíîæåñòâå freeBlocks
template <typename setType> std::pair<void*, int> CHeapManager::find_free_block(std::set<setType>& freeBlocks, int size) {
	for (auto it = freeBlocks.begin(); it != freeBlocks.end(); it++) {
		if (get_block_size(*it) >= size + sizeof(int)) {  // ò.ê â ïàìÿòè ïîìèìî äàííûõ õðàíèì è ðàçìåð
			void* block = get_block(*it);
			int size = get_block_size(*it);
			freeBlocks.erase(it);
			return std::make_pair(block, size);
		}
	}
	return std::make_pair(static_cast<void*>(0), 0);
}

int CHeapManager::get_block_size(void* pointer) {
	int result;
	memcpy(&result, pointer, sizeof(int));
	return result;
}

int CHeapManager::get_block_size(std::pair<void*, int> pair) {
	return pair.second;
}

void* CHeapManager::get_block(void* pointer) {
	return pointer;
}

void* CHeapManager::get_block(std::pair<void*, int> pair) {
	return pair.first;
}

void CHeapManager::add_empty_block_with_merge(void* block, int block_size) { // äîáàâëÿåì ñâîáîäíûé áëîê è ïðè âîçìîæíîñòè îáúåäèíÿåì ñ ñîñåäíèì
	void* leftBorder;
	int size = block_size;
	std::pair<void*, int> max_block;
	std::pair<void*, int> compareElement = std::make_pair(block, block_size);
	max_block = get_merge_result(try_left_merge(free_small_blocks, block), try_left_merge(free_medium_blocks, compareElement), try_left_merge(free_large_blocks, compareElement));
	if( max_block.first != 0 ) {
		leftBorder = max_block.first;
		size += max_block.second;
	} else {
		leftBorder = block;
	}
	max_block = get_merge_result(try_right_merge(free_small_blocks, block, block_size), try_right_merge(free_medium_blocks,
		compareElement, block_size), try_right_merge(free_large_blocks, compareElement, block_size));
	if (max_block.first != 0) {
		size += max_block.second;
	}
	add_free_block(leftBorder, size);
}

template <typename setType>
std::pair<void*, int> CHeapManager::try_left_merge(std::set<setType>& targetSet, setType compareElement) {
	// âîçâðàùàåì áëîê ñ êîòîðûì ìîæíî ñëèòüñÿ
	std::set<setType>::iterator found = targetSet.upper_bound(compareElement); // found > compareElement
	if ((targetSet.size() == 0) || found == targetSet.begin() || compareElement < *(--found)) { //òåïåðü found < compareElement
		return std::make_pair(static_cast<void*>(0), 0);
	}
	int leftSize = get_block_size(*found);
	void* leftBlock = get_block(*found);
	void* currentBlock = get_block(compareElement);
	if( static_cast<byte*>(currentBlock) - static_cast<byte*>(leftBlock) == leftSize ) {
		targetSet.erase(found);
		return std::make_pair(leftBlock, leftSize);
	} else {
		return std::make_pair(static_cast<void*>(0), 0);
	}
}

template <typename setType>
std::pair<void*, int> CHeapManager::try_right_merge(std::set<setType>& targetSet, setType compareElement, int currentSize) {
	// âîçâðàùàåì áëîê ñ êîòîðûì ìîæíî ñëèòüñÿ
	std::set<setType>::iterator found = targetSet.upper_bound(compareElement); // found > compareElement
	if (found == targetSet.end()) {
		return std::make_pair(static_cast<void*>(0), 0);
	}
	void* currentBlock = get_block(compareElement);
	void* rightBlock = get_block(*found);
	int rightSize = get_block_size(*found);
	if( static_cast<byte*>(rightBlock) - static_cast<byte*>(currentBlock) == currentSize) {
		targetSet.erase(found);
		return std::make_pair(rightBlock, rightSize);
	} else {
		return std::make_pair(static_cast<void*>(0), 0);
	}
}

// âîçâðàùàåò åäèíñòâåííûé âîçìîæíûé âàðèàíò ñëèÿíèÿ
std::pair<void*, int> CHeapManager::get_merge_result(std::pair<void*, int> first, std::pair<void*, int> second, std::pair<void*, int> third) {
	if (first.first != 0) {
		if (second.first != 0 || third.first != 0) { // ïðè ïðàâèëüíîé ðàáîòå ïðîãðàììû ýòîò ðåçóëüòàò íåâîçìîæåí
			exit(10); //ASSERT
		}
		return first;
	} else if (second.first != 0) {
		if (third.first != 0) { // ïðè ïðàâèëüíîé ðàáîòå ïðîãðàììû ýòîò ðåçóëüòàò íåâîçìîæåí
			exit(10);
		}
		return second;
	} else {
		return third;
	}
}
