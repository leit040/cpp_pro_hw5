#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include "Message.h"

using namespace std;

struct  Queue{
    vector<Message> mVector;
    std::mutex mQueue;
  };
void workerProducer(Queue& q){
    int i = 0;
    for (;;) {
        ++i;
        q.mQueue.lock();
        q.mVector.push_back(Message("thread_"+to_string(getpid())+" Mess text "+ to_string(i)));
        q.mQueue.unlock();
    }
}

void workerConsumer(Queue& q){
    for (;;) {
        if (!q.mVector.empty()){
        q.mQueue.lock();
        Message message = q.mVector.back();
        message.printMessage();
        q.mVector.pop_back();
        q.mQueue.unlock();
    }
    }
}

int main() {
    Queue queue;

    std::thread thread1 ([&](){
        workerProducer(queue);
    });

    std::thread thread2 ([&](){
        workerConsumer(queue);
    });
    thread1.join();
    thread2.join();

    for (Message &m: queue.mVector) {
        m.printMessage();
    }
    return 0;
}
