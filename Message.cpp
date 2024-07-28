#include "Message.h"
#include <iostream>
#include <utility>
unsigned int Message::counter = 0;

Message::Message(std::string msg): message(std::move(msg)){
    id = ++counter;
}
unsigned int Message::getId(){
    return id;
}
void Message::printMessage(){
    std::cout << "Message id: "<< id <<"\n Message: "<< message <<"\n" << "Counter: "<<counter<<"\n\n";
}

std::string Message::getMessage() {
    return message;
}