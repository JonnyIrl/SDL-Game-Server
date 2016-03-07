#pragma once
#ifndef NETCODE_H
#define NETCODE_H

#include "SDL_net.h"
#include "PacketType.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Netcode
{
public:
	Netcode();
	~Netcode();
	bool Init(const string &IP, int32_t port, int32_t localPort);

	bool CreatePacket(int32_t packetSize);
	bool OpenPort(int32_t port);
	bool InitSDL_Net();
	void ResetPacket();
	void ResetSpawnPacket();
	void ResetDefencePacket();
	void ResetRepairPacket();
	void ResetShieldPacket();
	void ResetGameOverPacket();
	void CheckForData();
	bool SetIPAndPort(const std::string &ip, uint16_t port);
	bool Send(const string &str);
	bool SendInitialConnectConfirmation(const string &playerIP, int playerNumber);
	bool SendReconnectedMessage(const string &str, int playerNumber);
	void AddNewConnection(string IPAddress);
	bool CheckClientAlreadyExists(string IP);
	int FindWhichPlayerSentMessage(string data);

	void SendTurretInformation(const string & Data, const string & playerIP);

	void SendSpawnInformation(const string & Data, const string & playerIP);
	void SendDefenceInformation(const string & Data, const string & playerIP);
	void SendSecondPlayerConnectedInformation(const string & Data, const string & playerIP);
	void SendRepairInformation(const string & Data, const string & playerIP);
	void SendShieldInformation(const string & Data, const string & playerIP);
	void SendGameOverInformation(const string & Data, const string & playerIP);


	//Assign the port, create the socket.
	UDPsocket SDLNET_UDP_OPEN(Uint16 port);

	//Resolve A host to connect to.
	int SDLNet_ResolveHose(IPaddress* ip, const char* host, Uint16 port);

	void SetPacket(UDPpacket packet) { m_packet = &packet; }
	void SetSocket(UDPsocket socket) { m_socket = socket; }

private:
	const char* m_host;
	Uint16 m_port;
	UDPpacket* m_packet;
	UDPpacket* m_spawnPacket;
	UDPpacket* m_defencePacket;
	UDPpacket* m_repairPacket;
	UDPpacket* m_shieldPacket;
	UDPpacket* m_gameOverPacket;
	UDPsocket m_socket;
	string m_ip;
	IPaddress m_IPAddress;
	bool m_quit;
	bool m_waitForMessage = false;
	vector<string> connectedClients;
	string m_PlayerOneIP;
	string m_PlayerTwoIP;

};





#endif
