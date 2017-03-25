#ifndef SAFEQUEUE_HEADER
#define SAFEQUEUE_HEADER

#include <mutex>
#include <condition_variable>
#include <queue>

struct PrintData {
    int row, col, color;
    char str[4];
    static const PrintData* kill;
};

class SafeQueue {
    std::queue<const PrintData*> values;
    
    std::mutex mtx;
    std::condition_variable condition;
    
public:

    const PrintData* read();
    
    void write(const PrintData* data);
    
};

#endif