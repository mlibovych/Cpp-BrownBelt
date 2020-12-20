#include "test_runner.h"

#include <forward_list>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
	using BucketList = forward_list<Type>;

private:
	vector<BucketList> data;
	const Hasher m_hasher;
	
public:
	explicit HashSet(
		size_t num_buckets,
		const Hasher& hasher = {}
	);

	void Add(const Type& value);
	bool Has(const Type& value) const;
	void Erase(const Type& value);
	const BucketList& GetBucket(const Type& value) const {
		return data[m_hasher(value) % data.size()];
	}
	BucketList& GetBucket(const Type& value) {
		return data[m_hasher(value) % data.size()];
	}
};

template <typename Type, typename Hasher>
HashSet<Type, Hasher>::HashSet(size_t num_buckets, const Hasher& hasher) :
								data(num_buckets), m_hasher(hasher)
{

}

template <typename Type, typename Hasher>
void HashSet<Type, Hasher>::Add(const Type& value) {
	BucketList& blist = GetBucket(value);

	if (Has(value)) {
		return;
	}
	blist.push_front(value);
}

template <typename Type, typename Hasher>
void HashSet<Type, Hasher>::Erase(const Type& value) {
	BucketList& blist = GetBucket(value);
	
	blist.remove(value);
}

template <typename Type, typename Hasher>
bool HashSet<Type, Hasher>::Has(const Type& value) const {
	const BucketList& blist = GetBucket(value);
	
	if (!blist.empty()) {
		if (std::find(blist.begin(), blist.end(), value) != blist.end()) {
			return 1;
		}
	}
	return 0;
}
