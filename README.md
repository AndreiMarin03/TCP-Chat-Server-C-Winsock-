💬 TCP Chat Server (C++ / Winsock)
📌 Overview

Server TCP multi-client implementat în C++ folosind Winsock, care permite comunicare în timp real între clienți prin mesaje broadcast și private.

⚙️ Features
conexiuni multiple simultane (select())
broadcast către toți clienții
mesaje private (/msg <id> <text>)
detectare deconectare clienți
afișare IP și port pentru fiecare conexiune
🧠 How it works

Serverul ascultă conexiuni și gestionează mai mulți clienți folosind select(). Mesajele primite sunt fie trimise tuturor clienților, fie direcționate către un client specific.

💻 Commands
[msg] /msg <client_id> <message>
[text] orice alt text → broadcast
🚀 Technologies

C++ • Winsock2 • TCP/IP

🔮 Improvements
suport pentru username-uri
multi-threading
criptare (TLS)
👤 Author

Marin Laurențiu-Andrei
