#include <future>
#include <iostream>
#include <memory>
#include <ostream>
#include "tcp_client.h"

int main()
{
    auto client_arc = TcpClient::connect(3000);
    auto client_clone = client_arc;
    auto future = std::async(std::launch::async, [client_clone]() {
        client_clone->listen();                             
    });
    
    std::cout << "Hello World" << std::endl;
    future.wait();
    std::cout << "Bye World" << std::endl;
    return 0;
}
