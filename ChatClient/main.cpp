#include "Client.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    //std::cout << argv[1] << " " << argv[2] << std::endl;
    Client chatClient(argc,argv);
    chatClient.start();
}
