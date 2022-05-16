#include "Server.h"
#include <iostream>
#include "Game.h"

#include "States/IdleState.h"
#include "States/WalkState.h"
#include "States/RunState.h"
#include "States/AttackState.h"
#include "States/HurtState.h"

#include <fstream>

Server::Server(Player* main_player, Game* game, std::string server_address, unsigned short server_port, unsigned short client_port) {

	std::ifstream file("address.txt");
	file >> server_address >> server_port >> client_port;
	file.close();

	this->server_address = sf::IpAddress(server_address);

	this->server_address_to_send = server_address;
	this->server_port_to_send = server_port;

	this->game = game;
	this->main_player = main_player;

	socket.bind(client_port);
	this->port = server_port;

	running = false;
	seq = 1;
	server_seq = 1;

	state_codes["Idle"] = 0;
	state_codes["Walk"] = 1;
	state_codes["Run"] = 2;
	state_codes["Attack"] = 3;
	state_codes["Hurt"] = 4;


}

bool Server::join(std::string name) {
	sf::Packet packet;
	sf::Vector2f pos(0, 0);
	uint8_t code = 2;
	uint8_t code_type;
	packet << code << name << pos.x << pos.y;
	static int count = 0;
	socket.send(packet, server_address, port);

	socket.receive(packet, server_address, port);
	packet >> code >> code_type;

	if (code_type == 1) {
		players[name] = main_player;
	}
	return code_type == 1;
}

void Server::updatePosition() {
	sf::Packet packet;

	uint8_t code = 1;
	uint32_t seq_t = seq;

	int posx = int(main_player->getObjectPosition().x * 100);
	int posy = int(main_player->getObjectPosition().y * 100);
	std::string nickname = main_player->getNickname()->getString();
	int8_t dx;
	int8_t dy;
	uint8_t state = state_codes[main_player->getState()->getName()];

	if (main_player->getDirection() == "West" || main_player->getDirection() == "East")  
		dx = main_player->getDirection() == "West" ? -1 : 1;
	else 
		dx = 0;

	if (main_player->getDirection() == "North" || main_player->getDirection() == "South")
		dy = main_player->getDirection() == "North" ? -1 : 1;
	else
		dy = 0;


	packet << code << seq_t << nickname << posx << posy << dx << dy << state;
	seq++;

	socket.send(packet, server_address_to_send, server_port_to_send);

}

void Server::updatePlayerPosition(sf::Packet packet) {
	int seq;
	std::string name;
	sf::Vector2f pos(0, 0);
	int posx;
	int posy;
	int8_t dx;
	int8_t dy;
	uint8_t state; // 0 - IDLE, 1 - WALK, 2 - RUN, 3 - ATTACK, 4 - HURT


	sf::Vector2i deltas;

	packet >> seq >> name;
	auto player = players[name];
	if (seq > server_seq && player) {
		server_seq = seq;


		packet >> posx >> posy >> dx >> dy >> state;

		pos.x = posx / 100;
		pos.y = posy / 100;


		if (player->getNickname() != main_player->getNickname()) {
			player->setPosition(pos);
			if (dx == -1 || dx == 1) {
				player->setDirection(dx == -1 ? "West" : "East");
			}
			else if (dy == -1 || dy == 1) {
				player->setDirection(dy == -1 ? "North" : "South");
			}
			if (state == 0) {
				player->setState(new IdleState(player));
			}
			else if (state == 1) {
				player->setState(new WalkState(player));
			}
			else if (state == 2) {
				player->setState(new RunState(player));
			}
			else if (state == 3) {
				player->setState(new AttackState(player));
			}
		}
	}
}

void Server::run() {
	running = true;
	while (running) {
		update();
	}
}

void Server::update() {
	sf::Packet packet;

	socket.receive(packet, server_address, port);

	if (packet.getDataSize() > 0) {
		uint8_t code;
		packet >> code;
		if (code == 1) {
			updatePlayerPosition(packet);
		}
		else if (code == 2) {
			addPlayer(packet);
		}
	}
}

void Server::addPlayer(sf::Packet packet) {
	std::string name;
	sf::Vector2i pos;

	packet >> name >> pos.x >> pos.y;

	players.emplace(name, new Entity(sf::Vector2f(16, 32), sf::Vector2f(300, 300), "NES_Slave", 100, 100, 0.15, 20, name));
	game->addPlayer(players[name]);
}

void Server::setMainPlayer(Player* player) {
	main_player = player;
	players[main_player->getNickname()->getString()] = player;
}

bool Server::isRunning() {
	return running;
}