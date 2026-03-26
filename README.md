💬 TCP Chat Server (C++ / Winsock)
📌 Overview

Acest proiect implementează un server TCP multi-client în C++ folosind Winsock. Serverul permite conectarea mai multor clienți simultan și suportă comunicare în timp real prin broadcast și mesaje private.

⚙️ Features
conexiuni multiple simultane (folosind select())
broadcast către toți clienții conectați
mesaje private (/msg <id> <text>)
detectare deconectare client
afișare IP + port pentru fiecare client
interacțiune directă din consola serverului
🧠 How it works
serverul creează un socket TCP și ascultă conexiuni
fiecare client este gestionat într-un array de socket-uri
select() este folosit pentru multiplexare (I/O non-blocant)
mesajele primite sunt:
retransmise (echo / broadcast)
sau direcționate către un client specific
💻 Commands
[msg] /msg <client_id> <message>   # mesaj privat
[text] orice alt text             # broadcast
🏗️ Architecture
Server
├── socket setup (WSAStartup, bind, listen)
├── connection handling (accept)
├── multiplexing (select)
├── message handling
│   ├── broadcast()
│   └── private_message()
🚀 Technologies
C++
Winsock2
TCP/IP
select() (I/O multiplexing)
⚠️ Limitations
număr maxim de clienți limitat (FD_SET_SIZE)
fără autentificare
protocol simplu (text-based)
🔮 Possible Improvements
multi-threading în loc de select()
criptare (TLS)
username-uri pentru clienți
GUI client
logare în fișier
👤 Author

Marin Laurențiu-Andrei
