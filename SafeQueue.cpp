#include "SafeQueue.h"

const PrintData* PrintData::kill = (PrintData*) 0xDEAD;

const PrintData* SafeQueue::read() {
    std::unique_lock<std::mutex> lock(mtx);
    
    while(values.empty()) {
        condition.wait(lock);
    }
    const PrintData* value = values.front();
    values.pop();
    
    return value;
}

void SafeQueue::write(const PrintData* data) {
    std::unique_lock<std::mutex> lock(mtx);
    values.push(data);
    lock.unlock();
    condition.notify_one();
}