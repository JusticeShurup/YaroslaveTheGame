#ifndef _NICKNAME_H_
#define _NICKNAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Nickname : public sf::Drawable
{
public: 
	Nickname();
	Nickname(std::string name);

	std::string getString() const;
	void setString(std::string name);

	void setPosition(float x, float y);
	sf::Vector2f getPosition();


	sf::RectangleShape& getShape();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::string nickname;

	void initNicknameTexture();

	sf::Text text;
	sf::RectangleShape shape;
	sf::ContextSettings settings;
	sf::RenderTexture render_texture;
	sf::Texture* texture;

};
#endif