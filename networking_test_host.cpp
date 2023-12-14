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

    string messageSend = "Hallo hier ist der Host";
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

                Packet packetSend;
                packetSend << "q";

                socket.send(packetSend);

                socket.setBlocking(false);

                window.close();
            }
            else if (event.type == Event::MouseButtonPressed)
            {
                socket.setBlocking(true);

                // pack the message to ensure it gets send correctly
                Packet packetSend;
                packetSend << messageSend;

                // send a message to the connected host
                socket.send(packetSend);

                socket.setBlocking(false);
            }
        }

        Packet packetReceive;
        // receive a message from the client
        if (socket.receive(packetReceive) == Socket::Done)
        {
            // if the message gets unpacked correctly
            if (packetReceive >> messageReceive)
            {
                // print the message
                cout << messageReceive << endl;

                // if the message was 'q', close the window
                if (messageReceive == 'q')
                {
                    window.close();
                }
            }
        }
    }
}