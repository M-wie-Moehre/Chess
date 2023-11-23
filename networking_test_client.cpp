// include SFML library headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

int main()
{
    // Create a socket and connect it to 192.168.178.62 on port 55001
    TcpSocket socket;
    socket.connect("192.168.178.62", 55001);
    std::cout << "Connected to host: " << "192.168.178.62" << endl;

    string message = "";
    while (message != "q")
    {
        // input a message
        cin >> message;
        // send a message to the connected host
        socket.send(message.c_str(), message.size() + 1);
    }
    // press any key to proceed
    cin.ignore();
}