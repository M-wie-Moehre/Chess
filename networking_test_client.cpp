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
    socket.connect("192.168.178.22", 55001);
    std::cout << "Connected to host: " << "192.168.178.22" << endl;

    bool proceed = true;
    string messageSend = "";
    while (proceed || messageSend != "q")
    {
        // input a message
        getline(cin, messageSend);

        // pack the message to ensure it gets send correctly
        Packet packetSend;
        packetSend << messageSend;

        // send a message to the connected host
        socket.send(packetSend);

        // receive a message from the client
        Packet packetReceive;
        socket.receive(packetReceive);

        // unpack the message
        string messageReceive;
        packetReceive >> messageReceive;
        cout << messageReceive << endl;
        if (messageReceive == 'q')
        {
            proceed = false;
        }
    }
}