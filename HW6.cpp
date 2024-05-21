#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(const int fps, const std::string& path) : fps_(fps)
    {
        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }

        setTexture(texture_);
        setTextureRect(sf::IntRect(0, 0, 50, 37));
        setScale(2, 2);
    }

    void setSpeed(const int& x_speed, const int& y_speed, const int& ro_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }




    bool Collision_L(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left <= wall_bounds.left + wall_bounds.width) && (guy_bounds.left > wall_bounds.left)
            && (guy_bounds.top + guy_bounds.height > wall_bounds.top) && (guy_bounds.top < wall_bounds.top + wall_bounds.height))
        {
            return 1;
        }
        else { return 0; }

    }

    bool Collision_R(sf::FloatRect wall_bounds, sf::FloatRect guy_bounds)
    {
        if ((guy_bounds.left + guy_bounds.width >= wall_bounds.left) && (guy_bounds.left + guy_bounds.width <= wall_bounds.left + wall_bounds.width)
            && (guy_bounds.top + guy_bounds.height > wall_bounds.top) && (guy_bounds.top < wall_bounds.top + wall_bounds.height))
        {
            return 1;
        }
        else { return 0; }

    }



    void moveCharacter(sf::Time& elapsed, const std::vector<sf::Sprite>& obstacles) {
        float dt = elapsed.asSeconds();
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        float y_position = getPosition().y;
        bool  left = 0, top = 0, right = 0;
        for (auto& obstacle : obstacles)
        {
            sf::FloatRect guy_bounds = getGlobalBounds();
            sf::FloatRect wall_bounds = obstacle.getGlobalBounds();
            if (Collision_L(wall_bounds, guy_bounds) == 1) { left = 1; }
            if (Collision_R(wall_bounds, guy_bounds) == 1) { right = 1; }
        }
        if (!isOnGround(obstacles) && !jumping_) {
            float fall_height = ((fallSpeed_)*dt + (0.5 * g * dt * dt));
            move(0, fall_height);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !left && rectangle_bounds.left>=-50) {
            animate(elapsed);

            move(-x_speed_ * elapsed.asSeconds(), 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !right && rectangle_bounds.left + rectangle_bounds.width <= 800) {

            animate(elapsed);
            move(x_speed_ * elapsed.asSeconds(), 0);
        }
        jump(elapsed, obstacles);
        if (jumping_)
        {
            animate(elapsed);
            y_position -= ((300.0) * dt + (0.5 * g * dt * dt));
            jump_time_ += dt;


            setPosition(getPosition().x, y_position);
            if (jump_time_ >= maxJumpTime_) {
                jump_time_ = 0.0;
                jumping_ = false;
            }

        }
    }

    void animate(const sf::Time& elapsed) {
      
        float dt = elapsed.asSeconds();
        t_ += dt;
        if (t_ >= 1.0 / fps_) {
            t_ = 0;
            ++fragments_index;
            if (fragments_index >= frames_.size()) {
                fragments_index = 0;
            }
            setTextureRect(frames_[fragments_index]);
        }

    }

    void setBounds(const float& l_bound, const float& r_bound, const float& u_bound, const float& d_bound) {
        l_bound_ = l_bound;
        r_bound_ = r_bound;
        u_bound_ = u_bound;
        d_bound_ = d_bound;
    }

    void add_animation_frame(const sf::IntRect& frame) {
        frames_.push_back(frame);
    }
    void jump(const sf::Time& elapsed, const std::vector<sf::Sprite>& obstacles) {
        if (isOnGround(obstacles) && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            jumping_ = true;
            jump_time_ = 0.0;
        }
    }



    bool isOnGround(const std::vector<sf::Sprite>& obstacles) {
        sf::FloatRect characterBounds = getGlobalBounds();

        for (const auto& obstacle : obstacles) {
            sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();


            if (characterBounds.top + characterBounds.height + 2 >= d_bound_ ||
                characterBounds.top + characterBounds.height + 2 >= obstacleBounds.top &&
                characterBounds.top + characterBounds.height - 2 <= obstacleBounds.top + obstacleBounds.height &&
                characterBounds.left + characterBounds.width >= obstacleBounds.left &&
                characterBounds.left <= obstacleBounds.left + obstacleBounds.width) {
                return true;
            }
        }

        return false;
    }


  

private:
    sf::Texture texture_;
    int fps_;
    double x_speed_ = 200;
    double y_speed_ = 200;
    int ro_speed_ = 0;
    float l_bound_ = -50;
    float r_bound_ = 750;
    float u_bound_ = 0;
    float d_bound_ = 600;
    float t_ = 0.0;
    float t_falling = 0.0;
    unsigned int fragments_index = 0;
    float g = 9.8;
    std::vector<sf::IntRect> frames_;
    float inJumpSpeed = 300;
    bool jumping_ = false;
    float jump_time_ = 0.0;
    const float maxJumpTime_ = 0.8;
    float fallSpeed_ = 200;
    void bouncce() {
        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if (rectangle_bounds.top <= u_bound_) {
            y_speed_ = abs(y_speed_);
        }

        if (rectangle_bounds.top + rectangle_bounds.height >= d_bound_) {
            y_speed_ = abs(y_speed_) * -1;
        }

        if (rectangle_bounds.left <= l_bound_) {
            x_speed_ = abs(x_speed_);
        }

        if (rectangle_bounds.left + rectangle_bounds.width >= r_bound_) {
            x_speed_ = abs(x_speed_) * -1;
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    AnimatedSprite hero(10, "character.png");
    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);
    hero.setSpeed(200, 200, 0);

    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37));
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37));

    sf::Texture wall_tex;
    if (!wall_tex.loadFromFile("wall.png")) { return 1; }
    wall_tex.setRepeated(true);
    std::vector<sf::Sprite> walls;

    sf::Sprite wall1;
    wall1.setTexture(wall_tex);
    wall1.setPosition(50, 200);
    wall1.setTextureRect(sf::IntRect(0, 0, 300, 50));
    walls.push_back(wall1);

    sf::Sprite wall2;
    wall1.setTexture(wall_tex);
    wall1.setPosition(400, 400);
    wall1.setTextureRect(sf::IntRect(0, 0, 300, 50));
    walls.push_back(wall1);
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        hero.moveCharacter(elapsed, walls);
        window.clear(sf::Color::Black);

        sf::Texture texture;
        if (!texture.loadFromFile("grass.png")) {
            std::cerr << "Could not load texture" << std::endl;
            return 1;
        }
        texture.setRepeated(true);

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setScale(0.3, 0.3);
        sprite.setTextureRect(sf::IntRect(0, 0, 2700, 2000));

        window.draw(sprite);
        for (auto& wall : walls) {
            window.draw(wall);
        }

        window.draw(hero);
     
        window.display();
    }

    return 0;
}
