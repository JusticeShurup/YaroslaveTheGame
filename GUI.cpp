#include "GUI.h"
#include "Camera.h"
#include "Player.h"
#include "TextureContainer/TextureContainer.h"
#include <iostream>
#include <math.h>

sf::Text* initText(std::string string, unsigned int character_size, sf::Color fill_color, sf::Color outline_color, unsigned int thickness) {
	sf::Text* text = new sf::Text;
	text->setFont(TextureContainer::getInstance()->getFont());
	text->setCharacterSize(character_size);
	text->setString(string);
	text->setFillColor(fill_color);
	text->setOutlineColor(outline_color);
	text->setOutlineThickness(thickness);
	text->setLetterSpacing(1);
	return text;
}

sf::RectangleShape initShape(sf::Vector2f size, sf::Color fill_color, sf::Color outline_color, unsigned int thickness = 0) {
	sf::RectangleShape shape(size);
	shape.setFillColor(fill_color);
	shape.setOutlineColor(outline_color);
	shape.setOutlineThickness(thickness);
	return shape;
}

void setTextInCenterShape(sf::Text* text, sf::RectangleShape * shape) {
	text->setPosition(shape->getPosition().x + ((shape->getGlobalBounds().width / 2) - (text->getGlobalBounds().width / 2)),
					 shape->getPosition().y - ((shape->getGlobalBounds().height / 2) - (text->getGlobalBounds().height / 2)) / 2);
}

GUI::GUI(Player* player) : player(player){
	
	can_draw_buttons = false;
	can_update_atributes = false;
	active = false;

	sf::Color background_color(109, 121, 138);
	unsigned int character_size = 45;

	background = sf::RectangleShape(sf::Vector2f(750, 750));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color::Black);
	background.setFillColor(background_color);

	player_icon = sf::RectangleShape(sf::Vector2f(175, 225));
	player_icon.setOutlineThickness(3);
	player_icon.setOutlineColor(sf::Color::Black);
	player_icon.setFillColor(background_color);


	std::string atribute_names[4] = {"Strength", "Fortitude", "Stamina", "Free points"};

	atributes_level[0] = player->getStrength();
	atributes_level[1] = player->getFortitude();
	atributes_level[2] = player->getStamina();
	atributes_level[3] = player->getFreeAtributePoints();
	
	for (int i = 0; i < 4; i++) {
		atributes_text[i] = initText(atribute_names[i], 45, sf::Color::White, sf::Color::Black, 2);
	
		atribute_text_shapes[i] = sf::RectangleShape(sf::Vector2f(300, 45));
		atribute_text_shapes[i].setFillColor(background_color);
		atribute_text_shapes[i].setOutlineColor(sf::Color::Black);
		atribute_text_shapes[i].setOutlineThickness(3);

		atributes_level_text[i] = initText(std::to_string(atributes_level[i]), 45, sf::Color::White, sf::Color::Black, 3);

		atribute_text_level_shapes[i] = sf::RectangleShape(sf::Vector2f(100, 45));
		atribute_text_level_shapes[i].setFillColor(background_color);
		atribute_text_level_shapes[i].setOutlineColor(sf::Color::Black);
		atribute_text_level_shapes[i].setOutlineThickness(3);
	}


	xp_text = initText("XP", 45, sf::Color::White, sf::Color::Black, 3);
	current_xp = initText(std::to_string(player->getXP()), 30, sf::Color::White, sf::Color::Black, 3);
	xp_to_next = initText(std::to_string(player->getXPToNextLevel()), 30, sf::Color::White, sf::Color::Black, 3);
	xp_shape_empty = initShape(sf::Vector2f(560, 34), sf::Color(181, 80, 136), sf::Color::Black, 3);
	xp_shape_full = initShape(sf::Vector2f(560 / player->getXPToNextLevel() * player->getXP(), 34), sf::Color(104, 56, 108), sf::Color::Black);

	health_bar_empty = initShape(sf::Vector2f(200, 46), sf::Color::White, sf::Color::Black, 3);
	health_bar_full = initShape(sf::Vector2f(200 / player->getMaxHPValue() * player->getHealthPointsValue(), 46), sf::Color::Red, sf::Color::Black);
	current_hp = initText(std::to_string(player->getHealthPointsValue()), 30, sf::Color::White, sf::Color::Black, 3);
	texture_hp = new sf::Texture;
	texture_hp->loadFromFile("TextureContainer\\Heart.png");
	health = sf::RectangleShape(sf::Vector2f(96, 96));
	health.setTexture(texture_hp);

	stamina_bar_empty = initShape(sf::Vector2f(200, 46), sf::Color::White, sf::Color::Black, 3);
	stamina_bar_full = initShape(sf::Vector2f(200 / player->getMaxStaminaValue() * player->getStaminaValue(), 46), sf::Color::Yellow, sf::Color::Black);
	current_stamina = initText(std::to_string(int(player->getStaminaValue())), 30, sf::Color::White, sf::Color::Black, 3);
	stamina_texture = new sf::Texture;
	stamina_texture->loadFromFile("TextureContainer\\Stamina.png");
	stamina = sf::RectangleShape(sf::Vector2f(96, 96));
	stamina.setTexture(stamina_texture);

	for (int i = 0; i < 6; i++) {
		inventory_shapes[i] = initShape(sf::Vector2f(90, 90), background_color, sf::Color::Black, 3);
		level_upgrade_buttons[i] = new Button(50, 45, 0, 0, i % 2 == 0 ? "+" : "_", character_size);
		level_upgrade_buttons[i]->hideShape(false);
		level_upgrade_buttons[i]->setShapeColor("idle", background_color);
		level_upgrade_buttons[i]->setShapeColor("hover", background_color);
		level_upgrade_buttons[i]->getShape()->setOutlineThickness(2);
		level_upgrade_buttons[i]->setTextDX(-2.5);
		level_upgrade_buttons[i]->setTextDY(i % 2 == 0 ? -15 : -35);
	}
	

	player_for_anim = new Entity(player_icon.getSize(), sf::Vector2f(0, 0));


	open_timer = 0;
}

GUI::~GUI() {
	for (int i = 0; i < 4; i++) {
		delete atributes_text[i];
		delete atributes_level_text[i];
	}
}

bool GUI::isActive() {
	return active;
}

void GUI::updateAtributes(sf::Event& event, sf::Vector2f mouse_pos, float delta_time) {
	click_timer += delta_time;
	for (int i = 0; i < 6; i++) {
		level_upgrade_buttons[i]->update(mouse_pos, event);
	
		if (i % 2 == 0) {
			if (level_upgrade_buttons[i]->isClicked() && click_timer > 0.1 && atributes_level[3] >= 1) {
				atributes_level_text[int(floor(i / 2))]->setString(std::to_string(++atributes_level[int(floor(i / 2))]));
				atributes_level_text[3]->setString(std::to_string(--atributes_level[3]));
				click_timer = 0;
			}
		}
		else {
			if (level_upgrade_buttons[i]->isClicked() && click_timer > 0.1) {
				if (atributes_level[int(floor(i / 2))] - 1 >= player->getAtributeByName(atributes_text[int(floor(i / 2))]->getString())) {
					atributes_level_text[int(floor(i / 2))]->setString(std::to_string(--atributes_level[int(floor(i / 2))]));
					atributes_level_text[3]->setString(std::to_string(++atributes_level[3]));
					click_timer = 0;
				}
			}
		}
	}

	can_draw_buttons = false;
}

void GUI::open(bool flag) {

	active = flag;

	if (active) {
		xp_shape_full.setSize(sf::Vector2f(560 / player->getXPToNextLevel() * player->getXP(), 34));
		current_xp->setString(std::to_string(player->getXP()));
		xp_to_next->setString(std::to_string(player->getXPToNextLevel()));
	
		atributes_level_text[0]->setString(std::to_string(player->getStrength()));
		atributes_level_text[1]->setString(std::to_string(player->getFortitude()));
		atributes_level_text[2]->setString(std::to_string(player->getStamina()));
		
		atributes_level[3] = player->getFreeAtributePoints();
		atributes_level_text[3]->setString(std::to_string(player->getFreeAtributePoints()));
	}
	else {
		std::string atribute_names[4] = { "Strength", "Fortitude", "Stamina", "Free points" };
		for (int i = 0; i < 3; i++) {
			player->addAtributeByName(atributes_text[i]->getString(), atributes_level[i] - player->getAtributeByName(atributes_text[i]->getString()));
		}
		player->setFreeAtributePoints(atributes_level[3]);
	}
}

void GUI::update(sf::Event& event, Camera* camera, float delta_time) {
	camera->setSize(300, 300);
	current_hp->setString(std::to_string(player->getHealthPointsValue()));
	health_bar_full.setSize(sf::Vector2f((float(200) / player->getMaxHPValue()) * player->getHealthPointsValue(), 46));
	current_stamina->setString(std::to_string(int(player->getStaminaValue())));
	stamina_bar_full.setSize(sf::Vector2f(float(200) / player->getMaxStaminaValue() * player->getStaminaValue(), 46));
	camera->setSize(1920, 1080);


	background.setPosition(camera->getView()->getCenter().x - background.getSize().x / 2, camera->getView()->getCenter().y - background.getSize().y / 2);
	player_icon.setPosition(background.getPosition().x + 10, background.getPosition().y + 9);
		
	player_for_anim->setPosition(player_icon.getPosition());

	for (int i = 0; i < 4; i++) {

		atribute_text_shapes[i].setPosition(player_icon.getPosition().x + player_icon.getSize().x + 10, player_icon.getPosition().y + (56.25 + 12.5 / 4) * i);
		atributes_text[i]->setPosition(player_icon.getPosition().x + player_icon.getSize().x + 10 , player_icon.getPosition().y + (56.25 + 12.25 / 4) * i - 10);

		atribute_text_level_shapes[i].setPosition(background.getPosition().x + background.getSize().x - 225, atribute_text_shapes[i].getPosition().y);
		setTextInCenterShape(atributes_level_text[i], &atribute_text_level_shapes[i]);

	}

	for (int i = 0; i < 6; i++) {
		inventory_shapes[i].setPosition(background.getPosition().x + 5 + i * (inventory_shapes[i].getSize().x + 10), background.getPosition().y + background.getSize().y - 10 - inventory_shapes[i].getSize().y);
		level_upgrade_buttons[i]->setPosition(atribute_text_level_shapes[int(floor(i / 2))].getPosition().x + atribute_text_level_shapes[int(floor(i / 2))].getSize().x + 10 + 60 * (i % 2 == 0 ? 0 : 1), atribute_text_level_shapes[int(floor(i / 2))].getPosition().y);
	}

	xp_text->setPosition(background.getPosition().x + background.getSize().x / 2 - xp_text->getGlobalBounds().width / 2, atributes_text[3]->getPosition().y + atributes_text[3]->getGlobalBounds().height + 25);
	xp_shape_empty.setPosition(background.getPosition().x + background.getSize().x / 2 - xp_shape_empty.getSize().x / 2, xp_text->getPosition().y + xp_text->getGlobalBounds().height + 15);
	xp_shape_full.setPosition(xp_shape_empty.getPosition());
	current_xp->setPosition(background.getPosition().x + background.getSize().x / 2 - current_xp->getGlobalBounds().width * 2, xp_shape_empty.getPosition().y);
	xp_to_next->setPosition(background.getPosition().x + background.getSize().x / 2 + current_xp->getGlobalBounds().width, xp_shape_empty.getPosition().y);

	health_bar_empty.setPosition(xp_shape_empty.getPosition().x, xp_shape_empty.getPosition().y + xp_shape_empty.getSize().y + 30);
	health_bar_full.setPosition(health_bar_empty.getPosition());
	current_hp->setPosition(health_bar_empty.getPosition().x + health_bar_empty.getSize().x / 2 - current_hp->getGlobalBounds().width / 2, health_bar_empty.getPosition().y);
	health.setPosition(health_bar_empty.getPosition().x + health_bar_empty.getSize().x - 10, health_bar_empty.getPosition().y - 20);

	stamina_bar_empty.setPosition(xp_shape_empty.getPosition().x + xp_shape_empty.getSize().x - stamina_bar_empty.getSize().x, xp_shape_empty.getPosition().y + xp_shape_empty.getSize().y + 30);
	stamina_bar_full.setPosition(stamina_bar_empty.getPosition());
	current_stamina->setPosition(stamina_bar_empty.getPosition().x + stamina_bar_empty.getSize().x / 2 - current_stamina->getGlobalBounds().width / 2, stamina_bar_empty.getPosition().y);
	stamina.setPosition(stamina_bar_empty.getPosition().x - 96, stamina_bar_empty.getPosition().y - 10);


	// Проверка кнопок
	sf::Vector2f mouse_pos = sf::Vector2f(camera->getWindow()->mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition())));
	if (player->getFreeAtributePoints() > 0) updateAtributes(event, sf::Vector2f(mouse_pos.x - 5, mouse_pos.y - 24), delta_time);
	camera->setSize(300, 300);

	player_for_anim->update(delta_time);
	player_for_anim->updateAnimator(delta_time, player_for_anim->getState());
}

void GUI::draw(sf::RenderTarget& window, sf::RenderStates states) const{
	window.draw(background);
	window.draw(player_icon);
	
	for (int i = 0; i < 4; i++) {
		window.draw(atribute_text_shapes[i]);
		window.draw(*atributes_text[i]);

	}

	window.draw(atribute_text_level_shapes[3]);
	window.draw(*atributes_level_text[3]);

	window.draw(*xp_text);
	window.draw(xp_shape_empty);
	window.draw(xp_shape_full);
	window.draw(*current_xp);
	window.draw(*xp_to_next);
	window.draw(health_bar_empty);
	window.draw(health_bar_full);
	window.draw(*current_hp);
	window.draw(health);
	window.draw(stamina_bar_empty);
	window.draw(stamina_bar_full);
	window.draw(*current_stamina);
	window.draw(stamina);

	for (int i = 0; i < 6; i++) {
		window.draw(inventory_shapes[i]);
		if (player->getFreeAtributePoints() > 0) window.draw(*level_upgrade_buttons[i]);
		window.draw(atribute_text_level_shapes[int(floor(i / 2))]);
		window.draw(*atributes_level_text[int(floor(i / 2))]);
	}
	
	window.draw(*player_for_anim);
}