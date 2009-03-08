#include <iostream>
#include <SFML/Network.hpp>

#include "decl.hpp"

#define LPORT 4567

using namespace std;

int main(){
    Player p;
    sf::SocketUDP Socket;
    sf::Packet Received;
    sf::IPAddress sender;
    char buffer[256];

    cout << "Ecoute du port " << LPORT << "..." << endl;
    if (!Socket.Bind(LPORT)){
        cerr << "Impossible d'écouter le port " << LPORT << endl;
        return 1;
    }

    cout << "En attente de reception de donnees ..." << endl;
    if(Socket.Receive(Received, sender) != sf::Socket::Done){
        cerr << "Erreur de reception" << endl;
    }
    Received >> p;

    cout << "p = " << p.x << " " << p.y << endl;

    /*
    size_t count;
    sf::IPAddress sender;

    cout << "En attente de reception de donnees ..." << endl;
    if(Socket.Receive(buffer, sizeof(buffer), count, sender) != sf::Socket::Done){
        cerr << "Erreur de reception" << endl;
    }
    cout << sender << endl << buffer << endl;
    */

    Socket.Close();
    return 0;
}
