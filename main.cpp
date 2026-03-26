#include <winsock2.h> //biblioteca de networking windows(include functii precum listen(),socket(),bind(),etc)
#include <iostream>
#include <io.h>
#include <ws2tcpip.h>
#include <cstring>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define FD_SET_SIZE 16
#define MAX_LINE 1024

void broadcast(int sender,int client[], int max_clients,char buffer[], int len)
{
    for(int j = 0; j < max_clients; j++)
    {
        if(client[j] != -1 && j != sender)
        {
            send(client[j], buffer, len, 0);
        }
    }
}
void private_message(int target, char message[], int client[])
{
    if(client[target] != -1)
    {
        send(client[target], message, strlen(message), 0);
    }
}

int main(int argc, char** argv)
{
    int SERVER_PORT;
    cout << "Enter port: ";
    cin >> SERVER_PORT;
    int i, maxfd, server_fd, client_fd; //server_fd este socket-ul serverului iar client_fd este al clientului.
    int num_ready, client[FD_SET_SIZE];//am setat numatul maxim de clienti intr-un vector.
    sockaddr_in client_addr[FD_SET_SIZE];
    int n, err, cli_len;
    fd_set read_set; //structura folosita de select(pentru a monitoriza socket urile.
    char line[MAX_LINE],cha;
    struct sockaddr_in cli_addr, serv_addr;
    //sockaddr_in -> structura de stocare adrese de tip IPv4, ea imi stocheaza IP-ul si portul unei conexiuni.

//Creare socket server
//1.wsastartup
//2.socket()
//3.bind()
//4.listen()
//5.accept()

//1 wsastartup
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2),&wsa)!=0) //pornesc biblioteca Winsock,altfel nu pot folosi socket-uri pe windows
    {
        cout<<"WSAStartup a esuat:("<<endl;
    }
//2 socket
    server_fd=socket(AF_INET,SOCK_STREAM,0);   //AF_INET->foloseste IPv4, SOCK_STREAM->tipul socket-ului, adica TCP

    if(server_fd == INVALID_SOCKET)
    {
        err= WSAGetLastError();        //diverse erori ce pot aparea daca socket-ul nu se creeaza cum trebuie
        cout<<" creearea socket-ului nu a functionat, error= " <<err<<endl;
        if (err == WSAENETDOWN)	cout << "NETDOWN" << endl;
        else if (err == WSANOTINITIALISED) cout << "NOINIT" << endl;
        else if (err == WSAEAFNOSUPPORT) cout << "NOSUPPORT" << endl;
        else if (err == WSAEINPROGRESS) cout << "IN PROGRESS" << endl;
        else if (err == WSAEMFILE) cout << "EM FILE" << endl;
        else if (err == WSAENOBUFS) cout << "NOBUFS" << endl;
        else if (err == WSAEPROTONOSUPPORT) cout << "PROTO" << endl;
        else if (err == WSAEPROTOTYPE) cout << "PROTOTYPE" << endl;
        else if (err == WSAESOCKTNOSUPPORT) cout << "SOCK NO SUPP" << endl;
        else cout <<"Eroare ciudata" <<endl;
        cin >>cha;
        exit(1);
    }
    serv_addr.sin_family = AF_INET; //IPv4.
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //accepta conexiuni de pe orice IP local.
    serv_addr.sin_port = htons(SERVER_PORT); // functie de converteste din host byte in network byte order.

//3 bind
    err=bind(server_fd,(sockaddr*)&serv_addr, sizeof(serv_addr));
    if(err < 0)
    {
        err = WSAGetLastError();
        cout<<" bind() nu a mers nene, error = " <<err<<endl;
        cin>>cha;
        exit(1);
    }
    else
    {
        char hostname[256];
        gethostname(hostname, sizeof(hostname));

        struct hostent* host = gethostbyname(hostname);

        cout << "\n=============================\n";
        cout << "        TCP SERVER\n";
        cout << "=============================\n";

        if(host)
        {
            struct in_addr addr;
            memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
            cout << "Server IP : " << inet_ntoa(addr) << endl;
        }

        cout << "Port      : " << SERVER_PORT << endl;
        cout << "Numar maxim clienti : " << FD_SET_SIZE << endl;
        cout << "Serverul a pornit cu succes\n";
        cout << "Asteptam clienti...\n";
        cout << "=============================\n\n";
    }
//4 listen
    listen(server_fd,5);
    maxfd=server_fd;

    for(i=0; i< FD_SET_SIZE; i++)
    {
        client[i]=-1;
    }
    FD_ZERO(&read_set); //golește lista de file descriptor-uri pe care select() trebuie să le monitorizeze.
    FD_SET(server_fd,&read_set);//aici se adauga socketul serverului în lista pe care select() o va verifica.

//5 accept

    while(1)
    {
        int client_id=0;
        FD_ZERO(&read_set);
        FD_SET(server_fd,&read_set);
        maxfd=server_fd;
        for(i=0; i<FD_SET_SIZE; i++)
        {
            if(client[i] >0)
            {
                FD_SET(client[i],&read_set);
                if(client[i]>maxfd)
                    maxfd=client[i];
            }
        }
        timeval tv;
        tv.tv_sec=0;
        tv.tv_usec=100000;
        num_ready= select(maxfd+1, &read_set, NULL, NULL, &tv); //select() verifică dacă unul dintre socketuri are activitate.

        if(FD_ISSET(server_fd,&read_set)) //verifica daca exista activitate pe server_fd, adica daca exista clienti care vor sa se conecteze
        {
            cli_len=sizeof(cli_addr);
            client_fd=accept(server_fd, (sockaddr*)&cli_addr,&cli_len);
            if(client_fd == INVALID_SOCKET)
                continue;
            cout<<"Conectiune acceptata"<<endl;
            for(i=0; i<FD_SET_SIZE; i++)
            {
                if(client[i]<0)
                {
                    client[i]=client_fd;
                    client_id=i;
                    client_addr[i] = cli_addr;
                    break;
                }
            }
            char*ip=inet_ntoa(cli_addr.sin_addr);
            cout<< "Clientul[" <<i<< "] s-a conectat: "
                <<ip<< ":" <<ntohs(cli_addr.sin_port)
                <<endl;

            if (client_fd >maxfd)
                maxfd=client_fd;

            if(--num_ready<=0)  //verificam daca am procesat toate evenimentele, in cazul nostru toti clientii.
                continue;
        }
        for(i=0; i<FD_SET_SIZE; i++)
        {
            if (client[i] < 0)
                continue;

            if (FD_ISSET(client[i], &read_set))
            {

                n = recv(client[i], line, MAX_LINE, 0);

                if (n <= 0)
                {
                    cout << "Client[" <<i<< "] disconnected" << endl;
                    closesocket(client[i]);
                    FD_CLR(client[i], &read_set);
                    client[i] = -1;
                }
                else
                {
                    line[n] = '\0';

                    char* src_ip = inet_ntoa(client_addr[i].sin_addr);
                    int src_port = ntohs(client_addr[i].sin_port);

                    cout << "Client[" << i << "] "
                         << "[" << src_ip << ":" << src_port << "] -> "
                         << "[SERVER:" << SERVER_PORT << "] : "
                         << line << endl;

                    // trimite mesajul la toti clientii
                    send(client[i], line, n, 0);
                }
                if(--num_ready<=0)
                    break;
            }
        }
        if(_kbhit())
        {
            char server_msg[MAX_LINE];
            cout<< "\nServer command: ";
            cin.getline(server_msg,MAX_LINE);
            string msg(server_msg);
//mesaj privat
            if(msg.rfind("/msg",0)==0)
            {
                int target;
                char text[MAX_LINE];
                sscanf(server_msg,"/msg %d %[^\n]", &target, text);

                private_message(target, text, client);
            }
            else
            {
                // broadcast
                broadcast(-1, client, FD_SET_SIZE, server_msg, strlen(server_msg));
            }

        }
    }
    return 0;
}
