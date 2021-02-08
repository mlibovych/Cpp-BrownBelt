#include <map>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <numeric>

#include "Common.h"

using namespace std;

class LruCache : public ICache {
	shared_ptr<IBooksUnpacker> unpacker;
	Settings m_settings;
	size_t memory = 0;
	size_t max_rang = 0;
	unordered_map<string, pair<BookPtr, size_t>> books;	
	mutex m_mutex;

	void DeleteLeastUsed() {
		auto it = min_element(
        books.begin(), books.end(),
        	[](const auto& lhs, const auto& rhs) {
          	return lhs.second.second < rhs.second.second;
        });

		memory -= (*it).second.first->GetContent().size();
		books.erase(it);
	}
public:
    LruCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const Settings& settings
    ) : unpacker(move(books_unpacker)), m_settings(settings) {
      	//
    }

    BookPtr GetBook(const string& book_name) override {
		lock_guard guard(m_mutex);
		if (books.count(book_name)) {
			books[book_name].second = max_rang++;
			return books[book_name].first;
		}
		BookPtr book = unpacker->UnpackBook(book_name);

		while (!books.empty() && memory + book->GetContent().size() > m_settings.max_memory) {
			DeleteLeastUsed();
		}
		if (memory + book->GetContent().size() <= m_settings.max_memory) {
			books[book_name] = {book, max_rang++};
			memory += book->GetContent().size();
			return book;
		}
		return book;
    }
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  	return make_unique<LruCache> (books_unpacker, settings);
}
