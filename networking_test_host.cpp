// include SFML library headers
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// use the SFML namespace
using namespace sf;
using namespace std;

#include <iostream>

int main()
{
    // Create a listener to wait for incoming connections on port 55001
    sf::TcpListener listener;
    listener.listen(55001);
    // Wait for a connection
    sf::TcpSocket socket;
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << endl;

    bool proceed = true;
    string messageSend = "";
    while (proceed || messageSend != "q")
    {
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

        // input a message
        getline(cin, messageSend);

        // pack the message to ensure it gets send correctly
        Packet packetSend;
        packetSend << messageSend;

        // send a message to the connected host
        socket.send(packetSend);
    }
}