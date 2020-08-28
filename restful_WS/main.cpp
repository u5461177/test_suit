
#include <memory>
#include <iostream>
#include "wsserver.h"


int main( const int, const char** )
{
    auto websockerserver = std::make_shared<ws_server::WebSocketServer>();
   std::thread t([websockerserver](){
       websockerserver->execute_ws();
   });
   t.detach();
   std::string input_str ={};
       std::cout << "Chat Begin:" << std::endl;
       while(getline(std::cin, input_str)){
           std::cout << "Self: "<< input_str << std::endl;
           websockerserver->send_messages(input_str);
       }

}