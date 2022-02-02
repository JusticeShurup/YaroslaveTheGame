#ifndef _TILESET_H_
#define _TILESET_H_
#include <SFML/Graphics.hpp>
class Tileset : public sf::Drawable
{
public: 
	Tileset();
	Tileset(sf::Vector2f position);
	Tileset(std::string filename, std::string sign, sf::Vector2f size, sf::Vector2f position, uint8_t sost);
	Tileset(sf::Texture* texture, std::string sign, sf::Vector2f size, sf::Vector2f position, uint8_t sost);

	~Tileset();

	void copyTexture(sf::Texture* texture);

	void setImage(sf::Texture* texture);
	void setTexture(std::string filename);
	void setTexture(sf::Texture* texture);
	void setTempTexture(sf::Texture* texture);

	sf::Texture* getTempTexture();
	sf::Texture* getTexture();
	sf::Image getImage();


	void setSign(std::string sign);
	std::string getSign();

	void setSost(uint8_t);
	uint8_t getSost();

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);
	sf::Vector2f getPosition();

	sf::RectangleShape* getShape();


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private: 
	static const int WIDHT = 32;
	static const int HEIGHT = 32;
	sf::RectangleShape *shape;

	sf::Vector2f size;
	
	sf::Image image;
	sf::Texture* texture;
	sf::Texture* temp_texture;
	std::string sign;
	uint8_t sost;
};
#endif 