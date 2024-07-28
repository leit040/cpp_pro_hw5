#ifndef HW5_MESSAGE_H
#define HW5_MESSAGE_H
#include <string>

class Message {
private:
    static unsigned int counter;
    unsigned int id;
    std::string message;
public:
    Message( std::string);

    static unsigned int getCount();
    unsigned int getId();
    std::string getMessage();
    void printMessage();
};


#endif //HW5_MESSAGE_H
