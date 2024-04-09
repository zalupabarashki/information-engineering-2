#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class CustomRectangleShape : public sf::RectangleShape {
public:
    CustomRectangleShape(const sf::Vector2f& size, const sf::Vector2f& position) : sf::RectangleShape(size)
    {
        setPosition(position);
    }

    void setSpeed(const int& speed_) {
        speed = speed_;

    }


    void moveInDirection(const sf::Time& elapsed, const sf::Keyboard::Key& key) {


        if (key == sf::Keyboard::Up) {
            speed_y = -speed;
        }
        if (key == sf::Keyboard::Down) {
            speed_y = speed;
        }
        if (key == sf::Keyboard::Left) {
            speed_x = -speed;
        }
        if (key == sf::Keyboard::Right) {
            speed_x = speed;
        }

        sf::Vector2f newPosition = getPosition() + sf::Vector2f(speed_x * elapsed.asSeconds(), speed_y * elapsed.asSeconds());
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if (newPosition.x >= 0 && newPosition.y >= 0 &&
            newPosition.x + rectangle_bounds.width <= r_bound_ && newPosition.y + rectangle_bounds.height <= d_bound_) {
            setPosition(newPosition);
        }
        speed_x = 0;
        speed_y = 0;
    }

    bool isClicked(const sf::Vector2i& mouse_position) {
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if (rectangle_bounds.top <= mouse_position.y && rectangle_bounds.top + rectangle_bounds.height >= mouse_position.y &&
            rectangle_bounds.left <= mouse_position.x && rectangle_bounds.left + rectangle_bounds.width >= mouse_position.x) {
            return true;
        }
        else {
            return false;
        }

    }

    void setBounds(const float& l_bound, const float& r_bound, const float& u_bound, const float& d_bound) {
        l_bound_ = l_bound;
        r_bound_ = r_bound;
        u_bound_ = u_bound;
        d_bound_ = d_bound;
    }
    void selected(bool select_) {
        select = select_;
    }
    bool isSelected() {
        return select;
    }
private:
    bool select = false;
    int speed_x;
    int speed_y;
    int speed = 0;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;

};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    sf::Vector2f size(120.0, 60.0);
    sf::Vector2f position(120.0, 60.0);
    CustomRectangleShape rectangle(size, position);
    rectangle.setFillColor(sf::Color(150, 100, 50));
    rectangle.setSpeed(10);
    sf::Clock clock;
    std::srand(std::time(nullptr));

    std::vector<CustomRectangleShape> rectangles;

    for (int i = 0; i < 10; i++)
    {
        sf::Vector2f size(120.0, 60.0);
        sf::Vector2f position(std::rand() % (window.getSize().x - 120), std::rand() % (window.getSize().y - 60));
        rectangles.emplace_back(CustomRectangleShape(size, position));
    }

    for (auto& rec : rectangles)
    {
        rec.setFillColor(sf::Color(0, 255, 0));
        rec.setBounds(0, window.getSize().x, 0, window.getSize().y);
        rec.setSpeed(10);
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time elapsed = sf::seconds(1.f / 60.f);

        window.clear(sf::Color::Black);

        for (auto& rec : rectangles)
        {
            window.draw(rec);
        }

        for (auto& rec : rectangles)
        {
            if (event.type == sf::Event::MouseButtonPressed && rec.isClicked(sf::Mouse::getPosition(window))) {

                rec.selected(true);
                for (auto& recD : rectangles) {
                    if (&recD != &rec && recD.isSelected()) {
                        recD.selected(false);
                    }
                }


                if (rec.isSelected()) {
                    rec.setFillColor(sf::Color::Red);
                }
                else {
                    rec.setFillColor(sf::Color(0, 255, 0));
                }
            }
        }
        for (auto& rec : rectangles)
        {
            if (event.type == sf::Event::KeyPressed && rec.isSelected()) {
                rec.moveInDirection(elapsed, event.key.code);
            }
        }
        for (auto& rec : rectangles) {
            if (rec.isSelected()) {
                rec.setFillColor(sf::Color::Red);
            }
            else {
                rec.setFillColor(sf::Color(0, 255, 0));
            }
        }
        window.display();
    }




    return 0;
}