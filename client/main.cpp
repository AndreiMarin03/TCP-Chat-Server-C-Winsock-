#include <winsock2.h>
#include <iostream>
#include <conio.h>
#include <cstring>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define MAX_LINE 1024

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;

    string ip;
    int port;

    cout << "Server IP: ";
    cin >> ip;
    cin.ignore();

    cout << "Port: ";
    cin >> port;
    cin.ignore();

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    connect(sock,(sockaddr*)&server,sizeof(server));

    sockaddr_in local_addr;
    sockaddr_in server_addr;
    int len = sizeof(sockaddr_in);

    getsockname(sock,(sockaddr*)&local_addr,&len);
    getpeername(sock,(sockaddr*)&server_addr,&len);

    char* client_ip = inet_ntoa(local_addr.sin_addr);
    int client_port = ntohs(local_addr.sin_port);

    char* server_ip = inet_ntoa(server_addr.sin_addr);
    int server_port = ntohs(server_addr.sin_port);

    char line[MAX_LINE];

    fd_set read_set;

    while(true)
    {
        // verifică dacă serverul a trimis ceva
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(sock, &read_set);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        select(0, &read_set, NULL, NULL, &tv);

        if(FD_ISSET(sock, &read_set))
        {
            int n = recv(sock,line,MAX_LINE,0);

            if(n <= 0)
            {
                cout << "Server disconnected\n";
                break;
            }

            line[n] = '\0';

            cout << "\n[" << server_ip << ":" << server_port << "] -> "
                 << "[" << client_ip << ":" << client_port << "] : "
                 << line << endl;
        }

        // verifică dacă ai tastat ceva
        if(_kbhit())
        {
            cout << "Message: ";
            cin.getline(line,MAX_LINE);

            send(sock,line,strlen(line),0);

            cout << "[" << client_ip << ":" << client_port << "] -> "
                 << "[" << server_ip << ":" << server_port << "] : "
                 << line << endl;
        }
    }
}
