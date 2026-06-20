#include <mutex>
#include <string>
#include <unordered_map>
#include <chrono>
#include <optional>
#include <iostream>

using namespace std;

class CacheManager {
private:
    struct CacheEntry {
        string value;
        chrono::steady_clock::time_point expiry;
        bool has_ttl;
    };

    unordered_map<string, CacheEntry> cache_;
    mutex mutex_;

    CacheManager() = default;

public:
    CacheManager(const CacheManager&) = delete;
    CacheManager& operator=(const CacheManager&) = delete;

    static CacheManager& getInstance() {
        static CacheManager instance;
        return instance;
    }

    void put(const string& key, const string& value, long ttlSeconds = 0) {
        lock_guard<mutex> lock(mutex_);
        CacheEntry entry;
        entry.value = value;
        entry.has_ttl = ttlSeconds > 0;
        if (entry.has_ttl) {
            entry.expiry = chrono::steady_clock::now()
                         + chrono::seconds(ttlSeconds);
        }
        cache_[key] = move(entry);
    }

    optional<string> get(const string& key) {
        lock_guard<mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it == cache_.end()) return nullopt;
        if (it->second.has_ttl && chrono::steady_clock::now() > it->second.expiry) {
            cache_.erase(it);
            return nullopt;
        }
        return it->second.value;
    }

    void remove(const string& key) {
        lock_guard<mutex> lock(mutex_);
        cache_.erase(key);
    }

    int size() {
        lock_guard<mutex> lock(mutex_);
        auto now = chrono::steady_clock::now();
        for (auto it = cache_.begin(); it != cache_.end(); ) {
            if (it->second.has_ttl && now > it->second.expiry)
                it = cache_.erase(it);
            else
                ++it;
        }
        return static_cast<int>(cache_.size());
    }
};

// --- Main ---
int main() {
    // Both references point to the same CacheManager instance
    CacheManager& cache1 = CacheManager::getInstance();
    CacheManager& cache2 = CacheManager::getInstance();

    cout << "Same instance? " << (&cache1 == &cache2) << endl; // 1 (true)

    // Component A caches data
    cache1.put("user:42", "{name: 'Alice'}", 5); // 5-second TTL
    cache1.put("config:theme", "dark");           // no expiry

    // Component B reads from the same cache
    auto user = cache2.get("user:42");
    cout << "user:42 = " << user.value_or("null") << endl;     // {name: 'Alice'}
    auto theme = cache2.get("config:theme");
    cout << "config:theme = " << theme.value_or("null") << endl; // dark
    cout << "Cache size: " << cache2.size() << endl;             // 2

    return 0;
}

    //             ┌────────────┐
    //             │   main()   │
    //             └─────┬──────┘
    //                   │
    //                   ▼
    //   CacheManager::getInstance() called
    //                   │
    //       Is static instance created?
    //            ┌──────┴───────┐
    //            │              │
    //          No               Yes
    //            │              │
    //            ▼              │
    //   Create CacheManager      │
    //   using private ctor       │
    //            │              │
    //            └──────┬───────┘
    //                   ▼
    //       Return reference to instance
    //                   │
    //       ┌───────────┴───────────┐
    //       │                       │
    //       ▼                       ▼
    //   cache1 reference       cache2 reference
    //       │                       │
    //       └───────────┬───────────┘
    //                   │
    //       Both point to SAME object
    //                   │
    //                   ▼
    //   (&cache1 == &cache2) ? → true
    //                   │
    //                   ▼
    //    cache1.put("user:42","Alice",5)
    //                   │
    //       lock_guard locks mutex_
    //                   │
    //                   ▼
    //      Create CacheEntry object
    //                   │
    //   has_ttl = true, expiry = now+5 sec
    //                   │
    //                   ▼
    //    Store entry in unordered_map
    //                   │
    //                   ▼
    //   cache1.put("config:theme","dark")
    //                   │
    //                   ▼
    //   Store entry without expiration
    //                   │
    //                   ▼
    //          cache2.get("user:42")
    //                   │
    //           lock mutex_
    //                   │
    //           Search key in map
    //                   │
    //      Is key present and not expired?
    //             ┌─────┴─────┐
    //             │           │
    //            No          Yes
    //             │           │
    //             ▼           ▼
    //         return        return
    //         nullopt       value
    //                         │
    //                         ▼
    //                 "{name:'Alice'}"
    //                         │
    //                         ▼
    //          cache2.get("config:theme")
    //                         │
    //                         ▼
    //                     returns "dark"
    //                         │
    //                         ▼
    //               cache2.size() called
    //                         │
    //                Remove expired entries
    //                         │
    //                         ▼
    //               Return current size = 2
    //                         │
    //                         ▼
    //                      Program Ends