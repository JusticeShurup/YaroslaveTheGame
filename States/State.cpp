#include "State.h"

State::State(Entity* entity) {
	this->entity = entity;
	name = "";
}

std::string State::getName() {
	return name;
}

bool State::operator==(const State& s) {
	return (this->name == s.name);
}