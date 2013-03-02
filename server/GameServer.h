#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "config.h"
#include "network.h"
#include "data_manager.h"
#include "deck_manager.h"
#include <set>
#include <unordered_map>
#include "RoomManager.h"

#include "netserver.h"

namespace ygo
{

struct GameServerStats
{
    int pid;
    int rooms;
    int players;
    GameServerStats();
};


class GameServer
{
private:
    const static int MAXPLAYERS = 100;
    std::unordered_map<bufferevent*, DuelPlayer> users;
    unsigned short server_port;
    evconnlistener* listener;
    int server_fd;
    char net_server_read[0x2000];
    char net_server_write[0x2000];
    unsigned short last_sent;
    //event* keepAliveEvent;
    volatile bool isAlive;
    static void keepAlive(evutil_socket_t fd, short events, void* arg);
    static int CheckAliveThread(void* parama);
    void RestartListen();
    bool isListening;



public:
    event_base* volatile net_evbase;
    RoomManager roomManager;
    GameServer(int server_fd);
    ~GameServer();
    bool StartServer();
    void StopServer();
    void StopListen();
    static void ServerAccept(evconnlistener* listener, evutil_socket_t fd, sockaddr* address, int socklen, void* ctx);
    static void ServerAcceptError(evconnlistener *listener, void* ctx);
    static void ServerEchoRead(bufferevent* bev, void* ctx);
    static void ServerEchoEvent(bufferevent* bev, short events, void* ctx);
    static int ServerThread(void* param);
    void DisconnectPlayer(DuelPlayer* dp);
    void HandleCTOSPacket(DuelPlayer* dp, char* data, unsigned int len);
};

}

#endif //NETSERVER_H
