#include "WalkState.h"
#include "IdleState.h"
#include "Entity.h"
#include "RunState.h"
#include "AttackState.h"

WalkState::WalkState(Entity* entity) : State(entity) {
	name = "Walk";
	add_stam_timer = 0;
}

void WalkState::update(float delta_time) {
	add_stam_timer += delta_time;
	sound_timer += delta_time;
	sf::Vector2f pos = entity->getObjectPosition();
	
	if (add_stam_timer > 0.2 && entity->getStaminaValue() + entity->getStamPerTick() < entity->getMaxStaminaValue()) {
		entity->setStaminaValue(entity->getStaminaValue() + entity->getStamPerTick());
		entity->updateStaminaBar();
		add_stam_timer = 0;
	}

	if (entity->getStaminaBar()) entity->getStaminaBar()->setPosition(pos.x + (entity->getDirection() == "East" ? 0.5 : -0.5), pos.y + 4 + (entity->getDirection() == "South" ? 0.25 : -0.25));
	if (entity->getHealthBar()) entity->getHealthBar()->setPosition(pos.x + (entity->getDirection() == "East" ? 0.5 : -0.5), pos.y + 2 + (entity->getDirection() == "South" ? 0.25 : -0.25));
	entity->updateAnimator(delta_time, this);
}

void WalkState::update(float delta_time, Entity* target, Map* map) {
	entity->update(delta_time);

	float distance = entity->calcDistance(target->getObjectPosition() + target->getHitboxPosition());

	float X0 = entity->getObjectPosition().x; // Координата X - от левого верхнего угла спрайта
	float Y0 = entity->getObjectPosition().y; // Координата Y - от левого верхнего угла спрайта
	float X = entity->getObjectPosition().x + entity->getHitboxPosition().x; // Координата X - от левого верхнего угла хитбокса
	float Y = entity->getObjectPosition().y + entity->getHitboxPosition().y; // Координата Y - от левого верхнего угла хитбокса
	float targetX = target->getObjectShape()->getPosition().x + target->getHitboxPosition().x;
	float targetY = target->getObjectShape()->getPosition().y + target->getHitboxPosition().y;


	sf::Vector2f pos(targetX, targetY);

	float adjucentLeg = abs(targetX - X); // Прилежащий катет
	float opposingLeg = abs(targetY - Y); // Противолежащий катет
	int signX = entity->sign(targetX - X);
	int signY = entity->sign(targetY - Y);
	float stepX = entity->getSpeedValue() * 300 * signX * delta_time;
	float stepY = entity->getSpeedValue() * 300 * signY * delta_time;


	if (walk_only_Y && entity->calcDistance(pos) > entity->getHitboxShape()->getSize().x) {
		if (elapsed_dist_y <= dist_y) {

			entity->setPosition(X0, Y0 + abs(stepY) * sign_y);
			if (sign_y > 0) {
				entity->setDirection("South");
			}
			else {
				entity->setDirection("North");
			}

			if (!map->checkCollisionWithMap(sf::Vector2f(X + stepX, Y), entity)) {
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
	if (walk_only_X && entity->calcDistance(pos) > entity->getHitboxShape()->getSize().y) {
		if (elapsed_dist_x <= dist_x) {
			if (sign_x > 0) {
				entity->setDirection("East");
			}
			else {
				entity->setDirection("West");
			}
			entity->setPosition(X0 + abs(stepX) * sign_x, Y0);
			if (!map->checkCollisionWithMap(sf::Vector2f(X, Y + stepY), entity)) {
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

	int distX = entity->getHitboxShape()->getSize().x + 1; // Расстояние, при котором враг уже сталкивается с объектом по X
	int distY = entity->getHitboxShape()->getSize().y + 1; // Расстояние, при котором враг уже сталкивается с объектом по Y

	if (entity->calcDistance(pos) < distX && adjucentLeg > opposingLeg || entity->calcDistance(pos) < distY && opposingLeg > adjucentLeg) {
		entity->setState(new AttackState(entity));
		return;
	}

	if (adjucentLeg <= abs(stepX + entity->getHitboxShape()->getSize().x * signX)) {
		if (signY < 0) entity->setDirection("North");
		else entity->setDirection("South");
		int gameobj_coll_numb = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepY), entity);
		if (gameobj_coll_numb < 0) {
			entity->setPosition(X0, Y0 + stepY);
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
		if (distance > 100 && distance < 300) {
			entity->setState(new RunState(entity));
			return;
		}
		else if (distance > 300) {
			entity->setState(new IdleState(entity));
			return;
		}
		return;
	}

	if (opposingLeg <= abs(stepY + entity->getHitboxShape()->getSize().y * signY)) {
		if (signX < 0) entity->setDirection("West");
		else entity->setDirection("East");
		int gameobj_coll_numb = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepX, 0), entity);
		if (gameobj_coll_numb < 0) {
			entity->setPosition(X0 + stepX, Y0);
		}
		else {
			GameObject* game_obj = map->getGameObjectByIndex(gameobj_coll_numb);
			if (abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) < abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) &&
				!map->checkCollisionWithMap(sf::Vector2f(X, Y + (game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y - entity->getHitboxShape()->getSize().y * 2)), entity)) {
				dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) + entity->getHitboxShape()->getSize().y;
				sign_y = -1;
			}
			else {
				dist_y = abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) + entity->getHitboxShape()->getSize().y;
				sign_y = 1;
			}
			walk_only_Y = true;
			elapsed_dist_y = 0;
		}
		if (distance > 100 && distance < 300) {
			entity->setState(new RunState(entity));
			return;
		}
		else if (distance > 300) {
			entity->setState(new IdleState(entity));
			return;
		}
		return;
	}

	float tgAlpha = ((double)opposingLeg) / adjucentLeg;
	float alpha = atan(tgAlpha);

	float stepXtg = signX * entity->getSpeedValue() * cos(alpha) * delta_time * 300;
	float stepYtg = signY * entity->getSpeedValue() * sin(alpha) * delta_time * 300;

	float mapEndX = map->getSize().x * map->getTilesetSize().x;
	float mapEndY = map->getSize().y * map->getTilesetSize().y;

	if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, stepYtg), entity)) {
		if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY)
			entity->setPosition(X0 + stepXtg, Y0 + stepYtg);
	}
	else {
		if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, 0), entity)) {
			int number = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepYtg), entity);
			if (number > 0) {
				GameObject* game_obj = map->getGameObjectByIndex(number);

				if (abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X) < abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + game_obj->getHitboxShape()->getSize().x - X) &&
					entity->calcDistance(pos, sf::Vector2f(X + abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x - X), Y)) < entity->calcDistance(pos, sf::Vector2f(X + abs(game_obj->getObjectPosition().x + game_obj->getHitboxPosition().x + entity->getHitboxShape()->getSize().x - X), Y))) {
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

			if (entity->calcDistance(pos, sf::Vector2f(X + stepXtg, Y)) < entity->calcDistance(pos, sf::Vector2f(X + stepX, Y))) {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY) {
					entity->setPosition(X0 + stepXtg, Y0);
				}
			}
			else {
				if (X + stepX > 0 && X + stepX < mapEndX && Y + stepY > 0 && Y + stepY < mapEndY) {
					if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(entity->getHitboxShape()->getSize().x + 2, Y + stepY), entity)) {
						entity->setPosition(X0 + stepX, Y0);
					}
					else {
						entity->setPosition(X0 - stepX, Y0);
					}
				}
			}
		}
		else if (!map->checkCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(0, stepYtg), entity)) {
			int number = map->returnCollisionWithMap(sf::Vector2f(X, Y) + sf::Vector2f(stepXtg, 0), entity);

			if (number > 0) {
				GameObject* game_obj = map->getGameObjectByIndex(number);

				if (abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y) < abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y) &&
					entity->calcDistance(pos, sf::Vector2f(X, Y + abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y - Y))) < entity->calcDistance(pos, sf::Vector2f(X, Y + abs(game_obj->getObjectPosition().y + game_obj->getHitboxPosition().y + game_obj->getHitboxShape()->getSize().y - Y)))) {
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

			if (entity->calcDistance(pos, sf::Vector2f(X, Y + stepYtg)) < entity->calcDistance(pos, sf::Vector2f(X, Y + stepY))) {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepYtg > 0 && Y + stepYtg < mapEndY) {
					entity->setPosition(X0, Y0 + stepYtg);
				}
			}
			else {
				if (X + stepXtg > 0 && X + stepXtg < mapEndX && Y + stepY > 0 && Y + stepY < mapEndY) {
					entity->setPosition(X0, Y0 + stepY);
				}
			}
		}
		else {
			entity->setPosition(X0 - stepX, Y0 - stepY);
		}
	}

	if (opposingLeg > adjucentLeg) { // Если по Y идти больше чем X, то 
		if (signY > 0)
			entity->setDirection("South"); //Идёт вниз 
		else
			entity->setDirection("North"); //Идёт вверх
	}
	else {
		if (signX > 0) entity->setDirection("East"); //Идёт вправо
		else entity->setDirection("West"); //Идёт вверх
	}

	if (entity->getSpeedValue() > 0) {
		entity->setState(new WalkState(entity));
		return;
	}

	else if (entity->getSpeedValue() == 0) {
		entity->setState(new IdleState(entity));
		return;
	}

	if (distance > 100 && distance < 300) {
		entity->setState(new RunState(entity));
		return;
	}
	else if (distance > 300) {
		entity->setState(new IdleState(entity));
		return;
	}
}
