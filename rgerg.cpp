#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class CustomSprite : public sf::Sprite
{
public:

    void setBounds(int left, int right, int top, int bottom)
    {
        bound_top = top;
        bound_bottom = bottom;
        bound_right = right;
        bound_left = left;
    }


    bool Collision_T(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.top + guy_bounds.height >= wall_bounds.top - 1) && (guy_bounds.top < wall_bounds.top)
            && (guy_bounds.left + guy_bounds.width > wall_bounds.left + 1) && (guy_bounds.left < wall_bounds.left + wall_bounds.width - 3))
        {
            return 1;
        }
        else { return 0; }

    }
    bool Collision_L(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left <= wall_bounds.left + wall_bounds.width +1 ) && (guy_bounds.left > wall_bounds.left)
            && (guy_bounds.top + guy_bounds.height > wall_bounds.top) && (guy_bounds.top < wall_bounds.top + wall_bounds.height))
        {
            return 1;
        }
        else { return 0; }

    }
    bool Collision_B(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.top <= wall_bounds.top + wall_bounds.height +1) && (guy_bounds.top + guy_bounds.height > wall_bounds.top )
            && (guy_bounds.left + guy_bounds.width > wall_bounds.left) && (guy_bounds.left < wall_bounds.left + wall_bounds.width))
        {
            return 1;
        }
        else { return 0; }

    }
    bool Collision_R(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left + guy_bounds.width >= wall_bounds.left -1) && (guy_bounds.left + guy_bounds.width <= wall_bounds.left + wall_bounds.width)
            && (guy_bounds.top + guy_bounds.height > wall_bounds.top) && (guy_bounds.top < wall_bounds.top + wall_bounds.height))
        {
            return 1;
        }
        else { return 0; }

    }

   


    void moveInDirection(const sf::Time& elapsed, const std::vector<sf::Sprite>& obstacles)
    {


        bool top = 0, left = 0, bottom = 0, right = 0;
        for (auto& obstacle : obstacles)
        {
            sf::FloatRect guy_bounds = getGlobalBounds();
            sf::FloatRect wall_bounds = obstacle.getGlobalBounds();
            if (Collision_T(wall_bounds, guy_bounds) == 1) { top = 1; }
            if (Collision_L(wall_bounds, guy_bounds) == 1) { left = 1; }
            if (Collision_B(wall_bounds, guy_bounds) == 1) { bottom = 1; }
            if (Collision_R(wall_bounds, guy_bounds) == 1) { right = 1; }
        }

        
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rectangle_bounds.top >= bound_top && bottom != 1 )
        {
            move(0, -m_speed_y * elapsed.asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && rectangle_bounds.top + rectangle_bounds.height <= bound_bottom && top != 1)
        {
            move(0, m_speed_y * elapsed.asSeconds());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && rectangle_bounds.left + rectangle_bounds.width <= bound_right && right != 1)
        {
            move(m_speed_x * elapsed.asSeconds(), 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && rectangle_bounds.left >= bound_left && left != 1)
        {
            move(-m_speed_x * elapsed.asSeconds(), 0);
        }
        std::cout << "Position: (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;

    }


private:
    int m_speed_x = 200;
    int m_speed_y = 200;
    int bound_top = 0;
    int bound_bottom = 600;
    int bound_left = 0;
    int bound_right = 800;
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Clock clock;
    CustomSprite spr;
    sf::Texture guy_tex;
    if (!guy_tex.loadFromFile("guy.png")) { return 1; }

    sf::Texture grass_tex;
    if (!grass_tex.loadFromFile("grass.png")) { return 1; }

    sf::Texture wall_tex;
    if (!wall_tex.loadFromFile("wall.png")) { return 1; }
    wall_tex.setRepeated(true);
    CustomSprite guy;
    guy.setTexture(guy_tex);
    
   
    grass_tex.setRepeated(true);
    sf::Sprite grass;
    grass.setTexture(grass_tex);
   
    grass.setTexture(grass_tex);
    grass.setScale(0.3, 0.3);
    grass.setTextureRect(sf::IntRect(0, 0, 2700, 2000));


    std::vector<sf::Sprite> walls;
   
    sf::Sprite wall1;
    wall1.setTexture(wall_tex);
    wall1.setPosition(100, 200);
    wall1.setTextureRect(sf::IntRect(0, 0, 50, 300));
    walls.push_back(wall1);

    sf::Sprite wall2;
    wall2.setTexture(wall_tex);
    wall2.setPosition(100, 200);
    wall2.setTextureRect(sf::IntRect(0, 0, 200, 50));
    walls.push_back(wall2);
    sf::Sprite wall3;
    wall3.setTexture(wall_tex);
    wall3.setPosition(100, 300);
    wall3.setTextureRect(sf::IntRect(0, 0, 50, 300));
    walls.push_back(wall3);
    sf::Sprite wall4;
    wall4.setTexture(wall_tex);
    wall4.setPosition(400, 200);
    wall4.setTextureRect(sf::IntRect(0, 0, 50, 300));
    walls.push_back(wall4);
    sf::Sprite wall5;
    wall5.setTexture(wall_tex);
    wall5.setPosition(400, 450);
    wall5.setTextureRect(sf::IntRect(0, 0, 300, 50));
    walls.push_back(wall5);
    sf::Sprite wall6;
    wall6.setTexture(wall_tex);
    wall6.setPosition(650, 100);
    wall6.setTextureRect(sf::IntRect(0, 0, 50, 350));
    walls.push_back(wall6);



    while (window.isOpen())
    {

        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        guy.setBounds(0, window.getSize().x, 0, window.getSize().y);
        guy.moveInDirection(elapsed, walls);

        
        window.draw(grass);
        window.draw(guy);
       
    for (auto& wall : walls) {
         window.draw(wall);
      }
       

        window.display();
    }

    return 0;
}