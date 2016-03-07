#include "Netcode.h"

Netcode::Netcode()
{
	m_quit = false;
}

Netcode::~Netcode()
{
	SDLNet_FreePacket(m_spawnPacket);
	SDLNet_FreePacket(m_packet);
	SDLNet_Quit();
}

bool Netcode::Init(const string &IP, int32_t port, int32_t localPort)
{
	cout << "Trying to connect to IP: " << IP << endl;
	cout << "Remote Port: " << port << endl;
	cout << "Local Port: " << localPort << endl;

	// Initialize SDL_net
	if (!InitSDL_Net())
		return false;

	if (!OpenPort(localPort))
		return false;

	if (!SetIPAndPort(IP, port))
		return false;

	if (!CreatePacket(1024))
		return false;

	return true;
}

bool Netcode::CreatePacket(int32_t packetSize)
{
	//std::cout << "Creating packet with size " << packetSize << "...\n";

	// Allocate memory for the packet
	m_packet = SDLNet_AllocPacket(packetSize);

	if (m_packet == nullptr)
	{
		std::cout << "\tSDLNet_AllocPacket failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_packet->address.host = m_IPAddress.host;
	m_packet->address.port = m_IPAddress.port;

	// Allocate memory for the packet
	m_spawnPacket = SDLNet_AllocPacket(packetSize);

	if (m_spawnPacket == nullptr)
	{
		std::cout << "\tSDLNet_Alloc***SPAWN Packet*** failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_spawnPacket->address.host = m_IPAddress.host;
	m_spawnPacket->address.port = m_IPAddress.port;

	// Allocate memory for the packet
	m_defencePacket = SDLNet_AllocPacket(packetSize);

	if (m_defencePacket == nullptr)
	{
		std::cout << "\tSDLNet_Alloc***SPAWN Packet*** failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_defencePacket->address.host = m_IPAddress.host;
	m_defencePacket->address.port = m_IPAddress.port;

	// Allocate memory for the packet
	m_shieldPacket = SDLNet_AllocPacket(packetSize);

	if (m_shieldPacket == nullptr)
	{
		std::cout << "\tSDLNet_Alloc***SPAWN Packet*** failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_shieldPacket->address.host = m_IPAddress.host;
	m_shieldPacket->address.port = m_IPAddress.port;

	// Allocate memory for the packet
	m_repairPacket = SDLNet_AllocPacket(packetSize);

	if (m_repairPacket == nullptr)
	{
		std::cout << "\tSDLNet_Alloc***SPAWN Packet*** failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_repairPacket->address.host = m_IPAddress.host;
	m_repairPacket->address.port = m_IPAddress.port;

	// Allocate memory for the packet
	m_gameOverPacket = SDLNet_AllocPacket(packetSize);

	if (m_gameOverPacket == nullptr)
	{
		std::cout << "\tSDLNet_Alloc***SPAWN Packet*** failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	// Set the destination host and port
	// We got these from calling SetIPAndPort()
	m_gameOverPacket->address.host = m_IPAddress.host;
	m_gameOverPacket->address.port = m_IPAddress.port;

	//std::cout << "\tSuccess!\n\n";
	return true;
}

bool Netcode::SetIPAndPort(const std::string &ip, uint16_t port)
{
	std::cout << "Setting IP ( " << ip << " ) " << "and port ( " << port << " )\n";

	// Set IP and port number with correct endianess
	if (SDLNet_ResolveHost(&m_IPAddress, ip.c_str(), port) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	std::cout << "\tSuccess!\n\n";
	return true;
}

int Netcode::FindWhichPlayerSentMessage(string data)
{
	int player = 0;
	for (int i = 0; i < connectedClients.size(); i++)
	{
		if ( i == 0 && data.find(connectedClients.at(i)) != std::string::npos)
		{
			player = 1;
		}

		else if (i == 1 && data.find(connectedClients.at(i)) != std::string::npos)
		{
			player = 2;
		}
	}

	return player;
}

void Netcode::SendTurretInformation(const string & Data, const string &playerIP)
{
	//Gonna hold all the turret information
	//string turret1Info = "", turret2Info = "", turret3Info = "", turret4Info = "", turret5Info = "", turret6Info = "";
	//int turret1ends = 0, turret2ends = 0, turret3ends = 0, turret4ends = 0, turret5ends = 0, turret6ends = 0;
	ResetPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_packet->address.host = m_IPAddress.host;
		m_packet->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_packet->data, msg.c_str(), msg.length());
		m_packet->len = msg.length();

		std::cout << "Sending Turret Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_packet) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent Turret Info!" << endl;
	}


}

void Netcode::SendSpawnInformation(const string & Data, const string &playerIP)
{
	//Gonna hold all the turret information
	//string turret1Info = "", turret2Info = "", turret3Info = "", turret4Info = "", turret5Info = "", turret6Info = "";
	//int turret1ends = 0, turret2ends = 0, turret3ends = 0, turret4ends = 0, turret5ends = 0, turret6ends = 0;
	ResetSpawnPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_spawnPacket->address.host = m_IPAddress.host;
		m_spawnPacket->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_spawnPacket->data, msg.c_str(), msg.length());
		m_spawnPacket->len = msg.length();

		std::cout << "Sending Spawn Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_spawnPacket) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent Spawn Info!" << endl;
	}


}

void Netcode::SendDefenceInformation(const string & Data, const string & playerIP)
{
	//Gonna hold all the turret information
	//string turret1Info = "", turret2Info = "", turret3Info = "", turret4Info = "", turret5Info = "", turret6Info = "";
	//int turret1ends = 0, turret2ends = 0, turret3ends = 0, turret4ends = 0, turret5ends = 0, turret6ends = 0;
	ResetDefencePacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_defencePacket->address.host = m_IPAddress.host;
		m_defencePacket->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_defencePacket->data, msg.c_str(), msg.length());
		m_defencePacket->len = msg.length();

		std::cout << "Sending Defence Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_defencePacket) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent Defence Info!" << endl;
	}
}

void Netcode::SendSecondPlayerConnectedInformation(const string & Data, const string & playerIP)
{
	ResetPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_packet->address.host = m_IPAddress.host;
		m_packet->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_packet->data, msg.c_str(), msg.length());
		m_packet->len = msg.length();

		std::cout << "Sending SECOND CONNECT Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_packet) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent SECOND CONNECT Info!" << endl;
	}
}

void Netcode::SendRepairInformation(const string & Data, const string & playerIP)
{
	ResetRepairPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_repairPacket->address.host = m_IPAddress.host;
		m_repairPacket->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_repairPacket->data, msg.c_str(), msg.length());
		m_repairPacket->len = msg.length();

		std::cout << "Sending Repair Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_repairPacket) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent Repair Info!" << endl;
	}
}

void Netcode::SendShieldInformation(const string & Data, const string & playerIP)
{
	ResetShieldPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_shieldPacket->address.host = m_IPAddress.host;
		m_shieldPacket->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_shieldPacket->data, msg.c_str(), msg.length());
		m_shieldPacket->len = msg.length();

		std::cout << "Sending SHIELD Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_shieldPacket) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent SHIELD Info!" << endl;
	}
}

void Netcode::SendGameOverInformation(const string & Data, const string & playerIP)
{
	ResetGameOverPacket();
	if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
	}
	else
	{
		m_gameOverPacket->address.host = m_IPAddress.host;
		m_gameOverPacket->address.port = m_IPAddress.port;

		std::string msg = Data;

		memcpy(m_gameOverPacket->data, msg.c_str(), msg.length());
		m_gameOverPacket->len = msg.length();

		std::cout << "Sending GAMEOVER Info...." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_gameOverPacket) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
		}

		std::cout << "Sent GAMEOVER Info!" << endl;
	}
}

bool Netcode::Send(const string & str)
{
	ResetPacket();
	// std::stringstreams let us add any data to it using << ( like std::cout ) 
	// We can extract any data from a std::stringstream using >> ( like std::cin )
	//std::cout << "Type a message and hit enter\n";
	std::string msg = "Hello there Alison!";
	//std::cin.ignore();

	memcpy(m_packet->data, msg.c_str(), msg.length());
	m_packet->len = msg.length();

	std::cout << "Sending........ " << endl;

	// Send
	// SDLNet_UDP_Send returns number of packets sent. 0 means error
	if (SDLNet_UDP_Send(m_socket, -1, m_packet) == 0)
	{
		std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n" << endl;
		return false;
	}

	std::cout << "\tSuccess!\n" << endl;

	if (str == "quit")
		m_quit = true;

	return true;
}

bool Netcode::SendInitialConnectConfirmation(const string &playerIP, int playerNumber)
{
	ResetPacket();
	for (int i = 0; i < connectedClients.size(); i++)
	{
		if (SDLNet_ResolveHost(&m_IPAddress, playerIP.c_str(), 9999) == -1)
		{
			std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
			return false;
		}
		else
		{			
			m_packet->address.host = m_IPAddress.host;
			m_packet->address.port = m_IPAddress.port;

			std::string msg = "Connected to the server! Welcome to the chat lobby! You are Player " + to_string(playerNumber) ;

			memcpy(m_packet->data, msg.c_str(), msg.length());
			m_packet->len = msg.length();

			std::cout << "Sending........." << endl;


			if (SDLNet_UDP_Send(m_socket, -1, m_packet) == 0)
			{
				std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
					<< "==========================================================================================================\n";
				return false;
			}

			std::cout << "Success!" << endl;

			return true;
		}
	}
}

bool Netcode::SendReconnectedMessage(const string &str, int playerNumber)
{
	if (SDLNet_ResolveHost(&m_IPAddress, str.c_str(), 9999) == -1)
	{
		std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
		return false;
	}
	else
	{
		m_packet->address.host = m_IPAddress.host;
		m_packet->address.port = m_IPAddress.port;

		std::string msg = "You have been reconnected! Welcome back! Player: " + to_string(playerNumber);

		memcpy(m_packet->data, msg.c_str(), msg.length());
		m_packet->len = msg.length();

		std::cout << "Sending........." << endl;


		if (SDLNet_UDP_Send(m_socket, -1, m_packet) == 0)
		{
			std::cout << "\tSDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n"
				<< "==========================================================================================================\n";
			return false;
		}

		std::cout << "Success!" << endl;
		ResetPacket();
		return true;
	}
}

void Netcode::AddNewConnection(string IPAddress)
{
	if(m_PlayerOneIP.length() <= 0)
	{
		m_PlayerOneIP = IPAddress;
	}

	else
	{
		m_PlayerTwoIP = IPAddress;
	}
	connectedClients.push_back(IPAddress);
}

bool Netcode::CheckClientAlreadyExists(string IP)
{
	bool found = false;
	//Only check if we have something already
	if (connectedClients.size() > 0)
	{

		for (int i = 0; i < connectedClients.size(); i++)
		{
			if (connectedClients.at(i).find(IP) != std::string::npos)
			{
				found = true;
			}
		}
	}

	return found;
}

bool Netcode::OpenPort(int32_t port)
{
	std::cout << "Opening port " << port << "...\n";

	// Sets our sovket with our local port
	m_socket = SDLNet_UDP_Open(port);

	if (m_socket == nullptr)
	{
		std::cout << "\tSDLNet_UDP_Open failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	std::cout << "\tSuccess!\n\n";
	return true;
}

bool Netcode::InitSDL_Net()
{
	std::cout << "Initializing SDL_net...\n";

	if (SDLNet_Init() == -1)
	{
		std::cout << "\tSDLNet_Init failed : " << SDLNet_GetError() << std::endl;
		return false;
	}

	std::cout << "\tSuccess!\n\n";
	return true;
}

void Netcode::ResetPacket()
{
	memcpy(m_packet->data, NULL, NULL);
}

void Netcode::ResetSpawnPacket()
{
	memcpy(m_spawnPacket->data, NULL, NULL);
}

void Netcode::ResetDefencePacket()
{
	memcpy(m_defencePacket->data, NULL, NULL);
}

void Netcode::ResetRepairPacket()
{
	memcpy(m_defencePacket->data, NULL, NULL);
}

void Netcode::ResetShieldPacket()
{
	memcpy(m_shieldPacket->data, NULL, NULL);
}

void Netcode::ResetGameOverPacket()
{
	memcpy(m_gameOverPacket->data, NULL, NULL);
}

void Netcode::CheckForData()
{
	CreatePacket(512);
	while (!m_waitForMessage)
	{
		// Check t see if there is a packet wauting for us...
		if (SDLNet_UDP_Recv(m_socket, m_packet))
		{
			std::cout << "\tData received : " << m_packet->data << "\n";
			string data = (char*)m_packet->data;


			//Check if its inital connect data
			if (data.find("CONNECT,") != std::string::npos)
			{
				string newIP = data.substr(8, data.length() - 8);

				//Retuns false if it doesnt find a match.
				if (!CheckClientAlreadyExists(newIP))
				{
					//If we dont have this client already then we need to add it to the list of clients.
					AddNewConnection(newIP);
					if (FindWhichPlayerSentMessage(newIP) == 1)
					{
						//If the player is connecting then send back a responce
						SendInitialConnectConfirmation(newIP, 1);
					}

					if (FindWhichPlayerSentMessage(newIP) == 2)
					{
						SendInitialConnectConfirmation(newIP, 2);
					}

					if (connectedClients.size() == 2)
					{
						for (int i = 0; i < connectedClients.size(); i++)
						{
							cout << "SENDING 2ND PLAYER CONNECTED" << endl;
							SendSecondPlayerConnectedInformation("SECONDPLAYER,", connectedClients.at(i));
						}
					}
				}

				else
				{
					//If a player reconnects let them know which player they are...
					if (FindWhichPlayerSentMessage(newIP) == 1)
					{
						//If the player is connecting then send back a responce
						//SendReconnectedMessage(newIP, 1);
					}

					if (FindWhichPlayerSentMessage(newIP) == 2)
					{
						//SendReconnectedMessage(newIP, 2);
					}

					if (connectedClients.size() == 2)
					{
						for (int i = 0; i < connectedClients.size(); i++)
						{
							cout << "SENDING 2ND PLAYER CONNECTED" << endl;
							SendSecondPlayerConnectedInformation("SECONDPLAYER,", connectedClients.at(i));
						}
					}
				}				
				
			}

			//Check if its chat Data
			else if (data.find("CHAT,") != std::string::npos)
			{
				string chatMessage = data.substr(5, data.length() - 5);
				if (FindWhichPlayerSentMessage(chatMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					chatMessage = chatMessage.substr(m_PlayerOneIP.length(), chatMessage.length() - m_PlayerOneIP.length());
					cout << "Message from Player 1: " << chatMessage << endl;
				}

				else if (FindWhichPlayerSentMessage(chatMessage) == 2)
				{
					chatMessage = chatMessage.substr(m_PlayerTwoIP.length(), chatMessage.length() - m_PlayerTwoIP.length());
					cout << "Message from Player 2: " << chatMessage << endl;
				}
				
			}

			else if (data.find("TURRETS,") != std::string::npos)
			{
				string turretMessage = data.substr(8, data.length() - 8);
				if (FindWhichPlayerSentMessage(turretMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					turretMessage = turretMessage.substr(m_PlayerOneIP.length(), turretMessage.length() - m_PlayerOneIP.length());
					cout << "TURRET INFO: " << endl << turretMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendTurretInformation("TURRET," + turretMessage, m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(turretMessage) == 2)
				{
					turretMessage = turretMessage.substr(m_PlayerTwoIP.length(), turretMessage.length() - m_PlayerTwoIP.length());
					cout << "TURRET INFO: " << endl << turretMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendTurretInformation("TURRET," + turretMessage, m_PlayerOneIP);
				}
			}

			else if (data.find("SPAWN,") != std::string::npos)
			{
				string spawnMessage = data.substr(6, data.length() - 6);
				if (FindWhichPlayerSentMessage(spawnMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					spawnMessage = spawnMessage.substr(m_PlayerOneIP.length() + 1, spawnMessage.length() - m_PlayerOneIP.length());
					cout << "SPAWN TROOPS INFO: " << endl << spawnMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendSpawnInformation("SPAWN," + spawnMessage, m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(spawnMessage) == 2)
				{
					spawnMessage = spawnMessage.substr(m_PlayerTwoIP.length(), spawnMessage.length() - m_PlayerTwoIP.length());
					cout << "SPAWN TROOPS INFO: " << endl << spawnMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendSpawnInformation("SPAWN," + spawnMessage, m_PlayerOneIP);
				}
			}

			else if (data.find("DEFENCE,") != std::string::npos)
			{
				string spawnMessage = data.substr(8, data.length() - 8);
				if (FindWhichPlayerSentMessage(spawnMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					spawnMessage = spawnMessage.substr(m_PlayerOneIP.length() + 1, spawnMessage.length() - m_PlayerOneIP.length());
					cout << "DEFENCE TROOPS INFO: " << endl << spawnMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendSpawnInformation("DEFENCE," + spawnMessage, m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(spawnMessage) == 2)
				{
					spawnMessage = spawnMessage.substr(m_PlayerTwoIP.length(), spawnMessage.length() - m_PlayerTwoIP.length());
					cout << "DEFENCE TROOPS INFO: " << endl << spawnMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendSpawnInformation("DEFENCE," + spawnMessage, m_PlayerOneIP);
				}
			}

			else if (data.find("REPAIR,") != std::string::npos)
			{
				string repairMessage = data.substr(7, data.length() - 7);
				if (FindWhichPlayerSentMessage(repairMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					repairMessage = repairMessage.substr(m_PlayerOneIP.length() + 1, repairMessage.length() - m_PlayerOneIP.length());
					cout << "REPAIR INFO: " << endl << repairMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendRepairInformation("REPAIR," + repairMessage + ",", m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(repairMessage) == 2)
				{
					repairMessage = repairMessage.substr(m_PlayerTwoIP.length(), repairMessage.length() - m_PlayerTwoIP.length());
					cout << "REPAIR INFO: " << endl << repairMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendRepairInformation("REPAIR," + repairMessage + ",", m_PlayerOneIP);
				}
			}

			else if (data.find("SHIELD,") != std::string::npos)
			{
				string shieldMessage = data.substr(7, data.length() - 7);
				if (FindWhichPlayerSentMessage(shieldMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					shieldMessage = shieldMessage.substr(m_PlayerOneIP.length() + 1, shieldMessage.length() - m_PlayerOneIP.length());
					cout << "SHIELD INFO: " << endl << shieldMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendShieldInformation("SHIELD," + shieldMessage + ",", m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(shieldMessage) == 2)
				{
					shieldMessage = shieldMessage.substr(m_PlayerTwoIP.length(), shieldMessage.length() - m_PlayerTwoIP.length());
					cout << "SHIELD INFO: " << endl << shieldMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendShieldInformation("SHIELD," + shieldMessage + ",", m_PlayerOneIP);
				}
			}

			else if (data.find("GAMEOVER,") != std::string::npos)
			{
				string gameOverMessage = data.substr(9, data.length() - 9);
				if (FindWhichPlayerSentMessage(gameOverMessage) == 1)
				{
					//Take out the players IP Address and display the message.
					gameOverMessage = gameOverMessage.substr(m_PlayerOneIP.length() + 1, gameOverMessage.length() - m_PlayerOneIP.length());
					cout << "GAME OVER INFO: " << endl << gameOverMessage << endl;

					//If we receive the turret info from player 1 then we send that info to player 2
					SendGameOverInformation("GAMEOVER," + gameOverMessage + ",", m_PlayerTwoIP);
				}

				else if (FindWhichPlayerSentMessage(gameOverMessage) == 2)
				{
					gameOverMessage = gameOverMessage.substr(m_PlayerTwoIP.length(), gameOverMessage.length() - m_PlayerTwoIP.length());
					cout << "GAMEOVER INFO: " << endl << gameOverMessage << endl;

					//If we receive the turret info from player 2 then we send that info to player 1
					SendGameOverInformation("GAMEOVER," + gameOverMessage + ",", m_PlayerOneIP);
				}
			}

			m_waitForMessage = true;
			break;
		}

	}

	ResetPacket();
	m_waitForMessage = false;
}

UDPsocket Netcode::SDLNET_UDP_OPEN(Uint16 port)
{
	return UDPsocket();
}

int Netcode::SDLNet_ResolveHose(IPaddress * ip, const char * host, Uint16 port)
{
	//If Fail return -1; If success return 0;
	if (ip->host == INADDR_NONE)
	{
		return -1;
	}

	else
		return 0;
}
