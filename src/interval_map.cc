#include <iostream>
#include <map>
#include <limits>
#include <algorithm>
#include <iomanip>

template <typename K, typename V>
class IntervalMap {
private:
    friend void interval_map_test() {}
    V m_val_begin;
    std::map<K, V> m_map;
public:
    explicit IntervalMap(V const& val) : m_val_begin(val) {
        m_map.insert(m_map.end(), std::make_pair(std::numeric_limits<K>::lowest(), val));
    }
    void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        if (keyBegin >= keyEnd) return;

		auto itr_begin = m_map.lower_bound(keyBegin);
		auto itr_end = m_map.lower_bound(keyEnd);

		if (itr_begin->second == val || itr_end->second == val) {
			return;
		}
		--itr_end;
		auto key_after = keyEnd;
		++key_after;
		std::pair<K, V> funeral_pair = {key_after, itr_end->second};

		if (m_map.size() > 1) {
			auto [beg_pair, beg_succeed] = m_map.insert_or_assign(keyBegin, val);
			if (!beg_succeed) {
				m_map.insert(std::move(funeral_pair));
			}
		}
		else {
			m_map.try_emplace(itr_begin, itr_begin->first, val);
		}

	}
    V const& operator[](K const&key) const {
        auto it = m_map.upper_bound(key);
        if(it==m_map.begin()) {
            return m_val_begin;
        }
        else {
            return (--it)->second;
        }
    }

};


int main(int argc, char* argv[]) {

    IntervalMap<int, char> m('A');

	m.assign(1, 2, 'B');
	m.assign(3, 5, 'C');
	m.assign(4, 7, 'E');
	m.assign(4, 6, 'A');



    for (int i = -2; i <= 10; ++i) {
        std::cout << std::setw(2) << i << ' ' << m[i] << '\n';
    }




    return 0;
}
