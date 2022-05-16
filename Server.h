#ifndef _SERVER_H_
#define _SERVER_H_

#include <SFML/Network.hpp>

class Game;
class Entity;
class Player;

class Server {

public:
	Server(Player* main_player, Game* game, std::string server_address, unsigned short server_port, unsigned short client_port);

	bool join(std::string name);
	void updatePosition();
	void updatePlayerPosition(sf::Packet packet);


	void addPlayer(sf::Packet packet);

	void run();
	void update();

	bool isRunning();



	void setMainPlayer(Player* player);

	sf::Vector2f getCoordinates();

private:
	bool running;

	int seq;
	int server_seq;

	sf::UdpSocket socket;
	sf::IpAddress server_address;

	sf::IpAddress server_address_to_send;
	unsigned short server_port_to_send;

	std::map<std::string, uint8_t> state_codes;

	Game* game;
	Player* main_player;
	std::map<std::string, Entity*> players;
	unsigned short port;


};
#endif