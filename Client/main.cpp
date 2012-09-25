// Client

#include <iostream>
#include <stdio.h>
#include <string>
#include <enet/enet.h>

int main()
{
	ENetHost * client;
	ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    bool Connected = false;
    bool Connecting = true;
    
    client = enet_host_create (NULL /* create a client host */,
                1 /* only allow 1 outgoing connection */,
                2 /* allow up 2 channels to be used, 0 and 1 */,
                0 /* 56K modem with 56 Kbps downstream bandwidth */,
                0 /* 56K modem with 14 Kbps upstream bandwidth */);
    if (client == NULL)
    {
        std::cout << "Unable to create client. \n";
        enet_host_destroy(client);
        return 0;
    }
    
    std::string Name;
    std::cout << "Enter a username: "; std::cin >> Name;
    
    enet_address_set_host (&address, "localhost");
    address.port = 9876;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (client, &address, 2, 0);    
    
    if (peer == NULL)
    {
       fprintf (stderr, 
                "No available peers for initiating an ENet connection.\n");
       exit (EXIT_FAILURE);
    }
    while (Connecting == true)
    {
    /* Wait up to 5 seconds for the connection attempt to succeed. */
		if (enet_host_service (client, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
		{
			std::cout << "Connected\n";
			Connected = true;
			Connecting = false;
			ENetPacket * packet = enet_packet_create (Name.c_str(), Name.length(), ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send (event.peer , 0 , packet);
		}
	

    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);
        puts ("Connection failed.");
        Connecting = false;
    }
	}
	while (Connected = true)
	{
		while (enet_host_service (client, & event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
				/*printf ("A packet of length %u containing \"%s\" was received from %s on channel %u.\n",
						event.packet -> dataLength,
						event.packet -> data,
						event.peer -> data,
						event.channelID);*/
						
						std::cout << "\n" << event.packet->data << std::flush;
						
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);
            
				break;
           
			case ENET_EVENT_TYPE_DISCONNECT:
				printf ("Server disconected.\n", event.peer -> data);
				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}	
	}

	enet_host_destroy(client);
	return 0;
}
