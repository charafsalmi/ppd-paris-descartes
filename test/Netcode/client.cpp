#include <iostream>
#include <SFML/Network.hpp>

#include "decl.hpp"

#define SPORT 4567

using namespace std;

int main(){
    Player p(2, 1);
    sf::SocketUDP Socket;
    sf::Packet sSend;
    sf::IPAddress dest("127.0.0.1");

    sSend << p;

    cout << "Envoi des donnees ..." << endl;
    if(Socket.Send(sSend, dest, SPORT) != sf::Socket::Done){
        cerr << "Impossible d'envoyer les données" << endl;
        return 1;
    }
    /*
    char buffer[] = "hello world";
    if(Socket.Send(buffer, sizeof(buffer), dest, SPORT) != sf::Socket::Done){
        cerr << "Impossible d'envoyer les données" << endl;
        return 1;
    }
    */

    return 0;
}
