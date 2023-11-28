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
    TcpListener listener;
    listener.listen(55001);
    // Wait for a connection
    TcpSocket socket;
    listener.accept(socket);
    cout << "New client connected: " << socket.getRemoteAddress() << endl;

    socket.setBlocking(false);

    Packet packetSend;
    string messageSend = "Hallo hier ist der Host";
    Packet packetReceive;
    string messageReceive;

    RenderWindow window(VideoMode(400, 400), "Networking Test Host");

    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                socket.setBlocking(true);

                packetSend << "q";

                socket.send(packetSend);

                socket.setBlocking(false);

                cin.ignore();

                window.close();
            }
            else if (event.type == Event::MouseButtonPressed)
            {
                socket.setBlocking(true);

                // pack the message to ensure it gets send correctly
                packetSend << messageSend;

                // send a message to the connected host
                socket.send(packetSend);

                socket.setBlocking(false);
            }
        }

        // receive a message from the client
        if (socket.receive(packetReceive) == Socket::Done)
        {

            // unpack the message
            packetReceive >> messageReceive;
            cout << messageReceive << endl;
            if (messageReceive == 'q')
            {
                window.close();
            }
        }
    }
}