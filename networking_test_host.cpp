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
    while (proceed)
    {
        // receive a message from the client
        Packet packet;
        socket.receive(packet);

        // unpack the message
        string message;
        packet >> message;
        cout << message << endl;
        if (message == 'q')
        {
            proceed = false;
        }
    }
}