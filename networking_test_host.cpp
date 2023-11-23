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

    bool proceed;
    while (proceed)
    {
        // receive a message from the client
        char buffer[1024];
        size_t received = 0;
        socket.receive(buffer, sizeof(buffer), received);
        cout << "The client said: " << buffer << endl;
        if (buffer == "q")
        {
            proceed = false;
        }
    }
    // press any key to proceed
    cin.ignore();
}