#pragma once
#include <list>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace concurrency {

    template <typename K, typename V>
    class ConcurrentLRUCache {
    public:
        explicit ConcurrentLRUCache(size_t capacity)
            : capacity_(capacity) {}

        bool get(const K& key, V& value) {
            {
                // Acquire shared lock for fast read
                std::shared_lock<std::shared_mutex> rlock(mutex_);
                auto it = cache_map_.find(key);
                if (it == cache_map_.end()) {
                    return false;
                }

                value = it->second->second;
            }

            // Upgrade to exclusive lock to move node to front
            {
                std::unique_lock<std::shared_mutex> wlock(mutex_);
                auto it = cache_map_.find(key);
                if (it != cache_map_.end()) {
                    cache_.splice(cache_.begin(), cache_, it->second);
                    return true;
                }
            }

            return false; // key disappeared between locks
        }

        void put(const K& key, const V& value) {
            std::unique_lock<std::shared_mutex> wlock(mutex_);
            auto it = cache_map_.find(key);

            if (it != cache_map_.end()) {
                it->second->second = value;
                cache_.splice(cache_.begin(), cache_, it->second);
                return;
            }

            // Insert new
            cache_.push_front({key, value});
            cache_map_[key] = cache_.begin();

            // Evict if needed
            if (cache_.size() > capacity_) {
                auto last_it = std::prev(cache_.end());
                cache_map_.erase(last_it->first);
                cache_.pop_back();
            }
        }

    private:
        size_t capacity_;
        mutable std::shared_mutex mutex_;  // allow shared read / exclusive write
        std::list<std::pair<K, V>> cache_;
        std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache_map_;
    };

} // namespace concurrency
