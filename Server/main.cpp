// Server 

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <enet/enet.h>

int main()
{
	std::cout << "Initializing enet.\n" << std::flush;
    if (enet_initialize() != 0)
    {
        fprintf (stderr, "Unable to initialize Enet, exiting...");
        atexit(enet_deinitialize);
        return EXIT_FAILURE;
    }
    
    bool Online = true;
    bool NewUser = false;
    std::string Welcome;
    
    ENetAddress address;
    ENetHost * server;
    ENetEvent event;

	address.host = ENET_HOST_ANY;
    address.port = 9876;
    
     server = enet_host_create (  &address /* the address to bind the server host to */, 
								  32      /* allow up to 32 clients and/or outgoing connections */,
                                  2      /* allow up to 2 channels to be used, 0 and 1 */,
                                  0      /* assume any amount of incoming bandwidth */,
                                  0      /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        fprintf (stderr, 
                 "An error occurred while trying to create an ENet server host.\n");
        enet_host_destroy(server);
        return 0;
    }
    
while (Online = true)
{
		/* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service (server, & event, 1000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
        {
            printf ("A new client connected from %x:%u.\n", 
                    event.peer -> address.host,
                    event.peer -> address.port);
                    
                    NewUser = true;
                  
			//event.peer -> data = (void*)event.packet->data;
		}
            break;
        case ENET_EVENT_TYPE_RECEIVE:
        {	
            printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                    event.packet -> dataLength,
                    event.packet -> data,
                    event.peer -> data,
                    event.channelID);
                    
                    //enet_host_broadcast (server, 0, event.packet);
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy (event.packet);
        }   
            break;
           
        case ENET_EVENT_TYPE_DISCONNECT:
        {
            printf ("%s disconected.\n", event.peer -> data);
            /* Reset the peer's client information. */
            event.peer -> data = NULL;
        }
        
        }
        
        if (NewUser == true)
        {
			  Welcome = "\e[1;31mWelcome to the server, "; Welcome += event.packet->data; Welcome += "\e[0m"; 
			
			ENetPacket * packet = enet_packet_create (Welcome.c_str(), Welcome.length(), ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send (event.peer , 0 , packet);
		}
    }
}
    
    enet_host_destroy(server);
    return 0;
}
