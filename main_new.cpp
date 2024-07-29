#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Message.h"

using namespace std;
struct Queue {
    vector<Message> mVector;
    std::mutex mQueue;
    std::condition_variable cv;
    bool done = false;
};
void createMess(Queue &q, int i) {
    {
        std::unique_lock<std::mutex> lock(q.mQueue);
        q.mVector.push_back(Message("thread_" + to_string(getpid()) + " Mess text " + to_string(i)));
    }
    q.cv.notify_one();
}
void workerProducer(Queue& q) {
    for (int i = 0; i < 50; ++i) {
        createMess(q, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    {
        std::unique_lock<std::mutex> lock(q.mQueue);
        q.done = true;
    }
    q.cv.notify_all();
}
void printAndDropMess(Queue &q) {
        Message message = q.mVector.back();
        q.mVector.pop_back();
        message.printMessage();
   }

void workerConsumer(Queue& q) {
    while (true) {
        std::unique_lock<std::mutex> lock(q.mQueue);
        q.cv.wait(lock, [&q] { return !q.mVector.empty() || q.done; });

        if (!q.mVector.empty()) {
            printAndDropMess(q);
        } else if (q.done) {
            break;
        }
    }
}

int main() {
    Queue queue;

    std::thread producerThread(workerProducer, std::ref(queue));
    std::thread consumerThread(workerConsumer, std::ref(queue));

    producerThread.join();
    consumerThread.join();

    return 0;
}