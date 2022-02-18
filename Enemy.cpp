#include "Enemy.h"
#include <math.h>
#include <thread>
#include <iostream>

int sign(float value) {
	if (value > 0) return 1;
	else if (value == 0) return 0;
	else return -1;
}

Enemy::Enemy(sf::Vector2f entity_size, sf::Vector2f hitbox_size, std::string textures_name, int maxHP, int maxStam, float speed, int damage, std::string name) :
	Entity(entity_size, hitbox_size, textures_name, maxHP, maxStam, speed, damage, name) {
	can_attack = true;
	can_move = true;

	walk_only_X = false;
	walk_only_Y = false;

	dist_x = 0;
	dist_y = 0;

}

void Enemy::update(float delta_time, Entity* entity, Map* map) {
	Entity::update(delta_time);


	float X0 = getObjectPosition().x; // Координата X - от левого верхнего угла спрайта
	float Y0 = getObjectPosition().y; // Координата Y - от левого верхнего угла спрайта
	float X = getObjectPosition().x + getHitboxPosition().x; // Координата X - от левого верхнего угла хитбокса
	float Y = getObjectPosition().y + getHitboxPosition().y; // Координата Y - от левого верхнего угла хитбокса
	float targetX = entity->getObjectShape()->getPosition().x + entity->getHitboxPosition().x;
	float targetY = entity->getObjectShape()->getPosition().y + entity->getHitboxPosition().y;


	sf::Vector2f pos(targetX, targetY);

	float adjucentLeg = abs(targetX - X); // Прилежащий катет
	float opposingLeg = abs(targetY - Y); // Противолежащий катет
	int signX = sign(targetX - X);
	int signY = sign(targetY - Y);
	float stepX = getSpeedValue() * 300 * signX * delta_time;
	float stepY = getSpeedValue() * 300 * signY * delta_time;
	
	
	if (walk_only_Y && calcDistance(pos) > getHitboxShape()->getSize().x) {
		if (elapsed_dist_y <= dist_y) {

			setPosition(X0, Y0 + abs(stepY) * sign_y);
			if (sign_y > 0) {
				setDirection("South");
			}
			else {
				setDirection("North");
			}
			
			if (!map->checkCollisionWithMap(sf::Vector2f(X + stepX, Y), this)) {
				walk_only_Y = false;
				dist_y = 0;
				elapsed_dist_y = 0;
				return;
			}
			elapsed_dist_y += abs(stepY);
			return;
		}
		else {
			walk_only_Y = false;
		}
	}
	if (walk_only_X && calcDistance(pos) > getHitboxShape()->getSize().y) {
		if (elapsed_dist_x <= dist_x) {
			if (sign_x > 0) {
				setDirection("East");
			}
			else {
				setDirection("West");
			}
			setPosition(X0 + abs(stepX) * sign_x, Y0);
			if (!map->checkCollisionWithMap(sf::Vector2f(X, Y + stepY), this)) {
				walk_only_X = false;
				dist_x = 0;
				elapsed_dist_x = 0;
				return;
			}
			elapsed_dist_x += abs(stepX);
			return;
		}
		else {
			walk_only_X = false;
		}
	}

	int distX = getHitboxShape()->getSize().x + 1; // Расстояние, при котором враг уже сталкивается с объектом по X
	int distY = getHitboxShape()->getSize().y + 1; // Расстояние, при котором враг уже сталкивается с объектом по Y

	if (calcDistance(pos) < distX && adjucentLeg > opposingLeg || calcDistance(pos) < distY && opposingLeg > adjucentLeg) {
		setState("Idle");
		return;
	}

	if (adjucentLeg <= abs(stepX + getHitboxShape()->getSize().x * signX)) {
		setState("Walk");
		if (signY < 0) setDirection("North");
		else setDirection("South");
		int gameobj_coll_numb = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepY), this);
		if (gameobj_coll_numb < 0) {
			setPosition(X0, Y0 + stepY);
		}
		else {
			GameObject* game_obj = map->getGameObjectByIndex(gameobj_coll_numb);
			if (abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X) < abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + game_obj->getHitboxShape()->getSize().x - X)) {
				dist_x = abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X);
				if (X < targetX) {
					sign_x = -1;
				}
				else {
					sign_x = 1;
				}
			}
			else {
				dist_x = abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + game_obj->getHitboxShape()->getSize().x - X);
				if (X < targetX) {
					sign_x = -1;
				}
				else {
					sign_x = 1;
				}
			}
			walk_only_X = true;
			elapsed_dist_x = 0;
		}
		return;
	}

	if (opposingLeg <= abs(stepY + getHitboxShape()->getSize().y * signY)) {
		setState("Walk");
		if (signX < 0) setDirection("West");
		else setDirection("East");
		int gameobj_coll_numb = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepX, 0), this);
		if (gameobj_coll_numb < 0) { 
			setPosition(X0 + stepX, Y0); 
		}
		else {
			GameObject* game_obj = map->getGameObjectByIndex(gameobj_coll_numb);
			if (abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) < abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) && 
				!map->checkCollisionWithMap(sf::Vector2f(X, Y + (game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y - getHitboxShape()->getSize().y * 2)), this)) {
					dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) + getHitboxShape()->getSize().y ;
					sign_y = -1;
			}
			else {
				dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) + getHitboxShape()->getSize().y;
				sign_y = 1;
			}
			walk_only_Y = true;
			elapsed_dist_y = 0;
		}
		return;
	}

	float tgAlpha = ((double)opposingLeg) / adjucentLeg;
	float alpha = atan(tgAlpha);

	float stepXtg = signX * getSpeedValue() * cos(alpha) * delta_time * 300;
	float stepYtg = signY * getSpeedValue() * sin(alpha) * delta_time * 300;

	float mapEndX = map->getSize().x * map->getTilesetSize().x;
	float mapEndY = map->getSize().y * map->getTilesetSize().y;

	if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, stepYtg), this)) {
		if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY)
			setPosition(X0 + stepXtg, Y0 + stepYtg);
	}
	else {
		if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, 0), this)) {
			int number = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepYtg), this);
			if (number > 0) {
				GameObject* game_obj = map->getGameObjectByIndex(number);

				if (abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X) < abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + game_obj->getHitboxShape()->getSize().x - X) && 
					calcDistance(pos, sf::Vector2f(X + abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X), Y)) < calcDistance(pos, sf::Vector2f(X + abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + getHitboxShape()->getSize().x - X), Y))) {
					dist_x = abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X) + 10;
					if (X < targetX) {
						sign_x = -1;
					}
					else {
						sign_x = 1;
					}
				}
				else {
					dist_x = abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + game_obj->getHitboxShape()->getSize().x - X) + 10;
					if (X < targetX) {
						sign_x = 1;
					}
					else {
						sign_x = -1;
					}
				}

				walk_only_X = true;
				elapsed_dist_x = 0;
			}

			if (calcDistance(pos, sf::Vector2f(X + stepXtg, Y)) < calcDistance(pos, sf::Vector2f(X + stepX, Y))) {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY) {
					setPosition(X0 + stepXtg, Y0);
				}
			}
			else {
				if (X + stepX > 0 && X + stepX < mapEndX && Y + stepY > 0 && Y + stepY < mapEndY) {
					if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(getHitboxShape()->getSize().x + 2, Y + stepY), this)) {
						setPosition(X0 + stepX, Y0);
					}
					else{
						setPosition(X0 - stepX, Y0);
					}
				}
			}
		}
		else if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepYtg), this)) {
			int number = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, 0), this);

			if (number > 0) {
				GameObject* game_obj = map->getGameObjectByIndex(number);

				if (abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) < abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) && 
					calcDistance(pos, sf::Vector2f(X, Y + abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y))) < calcDistance(pos, sf::Vector2f(X, Y + abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y)))) {
					dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) + 10;
					if (Y < targetY) {
						sign_y = 1;
					}
					else {
						sign_y = -1;
					}
				}
				else {
					dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) + 10;
					if (Y < targetY) {
						sign_y = 1;
					}
					else {
						sign_y = -1;
					}
				}
				walk_only_Y = true;
				elapsed_dist_y = 0;
			}

			if (calcDistance(pos, sf::Vector2f(X, Y + stepYtg)) < calcDistance(pos, sf::Vector2f(X, Y + stepY))) {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY) {
					setPosition(X0, Y0 + stepYtg);
				}
			}
			else {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepY > 0 && Y + stepY < mapEndY) {
					setPosition(X0, Y0 + stepY);
				}
			}
		}
		else {
			setPosition(X0 - stepX, Y0 - stepY);
		}
	}

	if (opposingLeg > adjucentLeg) { // Если по Y идти больше чем X, то 
		if (signY > 0)
			setDirection("South"); //Идёт вниз 
		else
			setDirection("North"); //Идёт вверх
	}
	else {
		if (signX > 0) setDirection("East"); //Идёт вправо
		else setDirection("West"); //Идёт вверх
	}

	if (getSpeedValue() > 0)
		setState("Walk");
	else if (getSpeedValue() == 0) setState("Idle");
}

float Enemy::calcDistance(sf::Vector2f pos) {
	return sqrt(pow(pos.x - (getObjectShape()->getPosition().x + getHitboxPosition().x), 2) + pow(pos.y - (getObjectShape()->getPosition().y + getHitboxPosition().y), 2));
}

float Enemy::calcDistance(sf::Vector2f pos, sf::Vector2f new_pos) {
	return sqrt( pow(pos.x - new_pos.x, 2) + pow(pos.y - new_pos.y, 2) );
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	Entity::draw(target, states);
}