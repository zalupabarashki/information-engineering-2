#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const float PLAYER_SPEED = 200.0f;
const float SPRINT_MULTIPLIER = 1.8f;
float current_max_time = 200.0f;

enum Difficulty { EASY, NORMAL, HARD };
float BULLET_SPEEDS[] = { 200.0f, 300.0f, 400.0f };
float BULLET_SPAWN_RATES[] = { 1.0f, 0.7f, 0.5f };
const int BOUNCES_BY_DIFFICULTY[] = { 1, 2, 9 };

struct Bullet {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    int bounces = 0;
};

struct Explosion {
    sf::Sprite sprite;
    float frame_time;
    int current_frame;
    bool finished = false;

    int rows = 5;
    int columns = 5;
    int total_frames = 25;
    int frame_width;
    int frame_height;

    Explosion(sf::Texture& explosion_texture, sf::Vector2f position) : current_frame(0), frame_time(0) {
        frame_width = explosion_texture.getSize().x / columns;
        frame_height = explosion_texture.getSize().y / rows;

        sprite.setTexture(explosion_texture);
        sprite.setTextureRect(sf::IntRect(0, 0, frame_width, frame_height));
        sprite.setPosition(position);
    }

    void update(float delta_time) {
        frame_time += delta_time;
        if (frame_time > 0.05f) {
            frame_time = 0;
            current_frame++;
            if (current_frame < total_frames) {
                int x = (current_frame % columns) * frame_width;
                int y = (current_frame / columns) * frame_height;
                sprite.setTextureRect(sf::IntRect(x, y, frame_width, frame_height));
            }
            else {
                finished = true;
            }
        }
    }
};

sf::Vector2f random_direction() {
    float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
    return sf::Vector2f(cos(angle), sin(angle));
}

class Game {
public:
    Game()
        : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game"),
        player(),
        difficulty(NORMAL),
        health(10),
        elapsed_time(0.0f),
        time_bar_width(current_max_time),
        spawn_timer(0.0f),
        game_paused(true),
        game_over(false),
        max_bounces(BOUNCES_BY_DIFFICULTY[NORMAL])
    {
        window.setFramerateLimit(60);

        background_texture.loadFromFile("background.jpg");
        starship_texture.loadFromFile("starship.png");
        comet_texture.loadFromFile("comet.png");
        explosion_texture.loadFromFile("explosion.png");
        hp_bar_texture.loadFromFile("hp bar.png");
        time_bar_texture.loadFromFile("time bar.png");

        background.setTexture(background_texture);
        float scale_x = static_cast<float>(WINDOW_WIDTH) / background_texture.getSize().x;
        float scale_y = static_cast<float>(WINDOW_HEIGHT) / background_texture.getSize().y;
        float scale = max(scale_x, scale_y);
        background.setScale(scale, scale);
        background.setPosition(0, 0);

        player.setTexture(starship_texture);
        player.setOrigin(starship_texture.getSize().x / 2, starship_texture.getSize().y / 2);
        player.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        hp_bar.setTexture(hp_bar_texture);
        hp_bar.setPosition(0.0f, 10.0f);
        hp_bar.setScale(1.5f, 1.0f);

        time_bar.setTexture(time_bar_texture);
        time_bar.setPosition(50.0f, 60.0f);

        default_health_bar.setFillColor(sf::Color::Red);
        default_health_bar.setPosition(88.0f, 23.0f);
        default_health_bar.setSize(sf::Vector2f(212.0f, 16.0f));

        default_time_bar.setFillColor(sf::Color::Blue);
        default_time_bar.setPosition(60.0f, 70.0f);
        default_time_bar.setSize(sf::Vector2f(200.0f, 16.0f));

        font.loadFromFile("arial.ttf");

        setup_text(start_text, "Press Enter to Start", sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
        setup_text(exit_text, "Press ESC to Exit", sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50));
        setup_text(difficulty_text, "Difficulty: Normal", sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100));
        setup_text(restart_text, "Press Enter to Play Again", sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
        setup_text(final_score_text, "", sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    }

    void run() {
        srand(static_cast<unsigned>(time(nullptr)));

        while (window.isOpen()) {
            handle_events();
            if (!game_paused && !game_over) {
                update();
            }
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Sprite player;
    sf::Sprite hp_bar;
    sf::Sprite time_bar;
    vector<Bullet> bullets;
    vector<Explosion> explosions;

    sf::RectangleShape default_health_bar;
    sf::RectangleShape default_time_bar;

    sf::Texture background_texture, starship_texture, comet_texture, explosion_texture, hp_bar_texture, time_bar_texture;
    sf::Sprite background;

    Difficulty difficulty;
    int health;
    float elapsed_time;
    float time_bar_width;
    float spawn_timer;
    bool game_paused;
    bool game_over;
    int max_bounces;

    sf::Font font;
    sf::Text start_text;
    sf::Text exit_text;
    sf::Text difficulty_text;
    sf::Text restart_text;
    sf::Text final_score_text;

    sf::Clock clock;

    void setup_text(sf::Text& text, const string& str, const sf::Vector2f& position) {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(position.x - text.getLocalBounds().width / 2, position.y);
    }

    void handle_events() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    game_paused = !game_paused;
                }
                if (game_paused && !game_over) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        game_paused = false;
                    }
                    if (event.key.code == sf::Keyboard::Num1) {
                        difficulty = EASY;
                        difficulty_text.setString("Difficulty: Easy");
                        max_bounces = BOUNCES_BY_DIFFICULTY[EASY];
                    }
                    if (event.key.code == sf::Keyboard::Num2) {
                        difficulty = NORMAL;
                        difficulty_text.setString("Difficulty: Normal");
                        max_bounces = BOUNCES_BY_DIFFICULTY[NORMAL];
                    }
                    if (event.key.code == sf::Keyboard::Num3) {
                        difficulty = HARD;
                        difficulty_text.setString("Difficulty: Hard");
                        max_bounces = BOUNCES_BY_DIFFICULTY[HARD];
                    }
                }
                else if (game_over) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        restart_game();
                    }
                }
            }
        }
    }

    void update() {
        float dt = clock.restart().asSeconds();

        sf::Vector2f move_dir;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move_dir.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move_dir.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move_dir.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move_dir.x += 1;

        float speed = PLAYER_SPEED;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            speed *= SPRINT_MULTIPLIER;
        }

        if (move_dir.x != 0 || move_dir.y != 0) {
            float length = sqrt(move_dir.x * move_dir.x + move_dir.y * move_dir.y);
            move_dir /= length;
        }

        player.move(move_dir * speed * dt);

        if (player.getPosition().x < 0) player.setPosition(0, player.getPosition().y);
        if (player.getPosition().y < 0) player.setPosition(player.getPosition().x, 0);
        if (player.getPosition().x + 60 > WINDOW_WIDTH) player.setPosition(WINDOW_WIDTH - 60, player.getPosition().y);
        if (player.getPosition().y + 60 > WINDOW_HEIGHT) player.setPosition(player.getPosition().x, WINDOW_HEIGHT - 60);

        spawn_timer += dt;
        if (spawn_timer >= BULLET_SPAWN_RATES[difficulty]) {
            spawn_bullet();
            spawn_timer = 0.0f;
        }

        for (auto& bullet : bullets) {
            bullet.sprite.move(bullet.velocity * dt);
            float angle = atan2(bullet.velocity.y, bullet.velocity.x) * 180.0f / 3.14159f;
            bullet.sprite.setRotation(angle + 90);

            if (bullet.sprite.getPosition().x < 0 || bullet.sprite.getPosition().x + bullet.sprite.getGlobalBounds().width > WINDOW_WIDTH) {
                bullet.velocity.x = -bullet.velocity.x;
                bullet.bounces++;
            }
            if (bullet.sprite.getPosition().y < 0 || bullet.sprite.getPosition().y + bullet.sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
                bullet.velocity.y = -bullet.velocity.y;
                bullet.bounces++;
            }
        }

        bullets.erase(remove_if(bullets.begin(), bullets.end(), [&](Bullet& bullet) {
            if (bullet.bounces >= max_bounces || player.getGlobalBounds().intersects(bullet.sprite.getGlobalBounds())) {
                if (player.getGlobalBounds().intersects(bullet.sprite.getGlobalBounds())) {
                    health--;
                    explosions.emplace_back(explosion_texture, bullet.sprite.getPosition());
                }
                return true;
            }
            return false;
            }), bullets.end());

        for (auto& explosion : explosions) {
            explosion.update(dt);
        }

        explosions.erase(remove_if(explosions.begin(), explosions.end(), [](Explosion& e) { return e.finished; }), explosions.end());

        if (time_bar_width <= 0) {
            BULLET_SPEEDS[difficulty] *= 1.1f;
            BULLET_SPAWN_RATES[difficulty] *= 0.9f;
            current_max_time += 5.0f;
            time_bar_width = current_max_time;
        }
        else {
            elapsed_time += dt;
            time_bar_width -= dt * 20.0f;
        }

        default_health_bar.setSize(sf::Vector2f(health * 20.0f, default_health_bar.getSize().y));
        default_time_bar.setSize(sf::Vector2f(time_bar_width, default_time_bar.getSize().y));

        if (health <= 0) {
            game_over = true;
            final_score_text.setString("Time Survived: " + to_string(static_cast<int>(elapsed_time)) + " seconds");
        }
    }

    void render() {
        window.clear();
        window.draw(background);
        window.draw(player);

        for (const auto& bullet : bullets) {
            window.draw(bullet.sprite);
        }

        for (const auto& explosion : explosions) {
            window.draw(explosion.sprite);
        }

        window.draw(hp_bar);
        window.draw(time_bar);
        window.draw(default_health_bar);
        window.draw(default_time_bar);

        if (game_paused && !game_over) {
            window.draw(start_text);
            window.draw(exit_text);
            window.draw(difficulty_text);
        }

        if (game_over) {
            window.draw(restart_text);
            window.draw(final_score_text);
        }

        window.display();
    }

    void spawn_bullet() {
        Bullet bullet;
        bullet.sprite.setTexture(comet_texture);
        bullet.sprite.setOrigin(comet_texture.getSize().x / 2, comet_texture.getSize().y / 2);

        int side = rand() % 4;
        if (side == 0) {
            bullet.sprite.setPosition(0, static_cast<float>(rand() % WINDOW_HEIGHT));
        }
        else if (side == 1) {
            bullet.sprite.setPosition(WINDOW_WIDTH - bullet.sprite.getGlobalBounds().width, static_cast<float>(rand() % WINDOW_HEIGHT));
        }
        else if (side == 2) {
            bullet.sprite.setPosition(static_cast<float>(rand() % WINDOW_WIDTH), 0);
        }
        else if (side == 3) {
            bullet.sprite.setPosition(static_cast<float>(rand() % WINDOW_WIDTH), WINDOW_HEIGHT - bullet.sprite.getGlobalBounds().height);
        }

        bullet.velocity = random_direction() * BULLET_SPEEDS[difficulty];
        bullet.bounces = 0;
        bullets.push_back(bullet);
    }

    void restart_game() {
        health = 10;
        elapsed_time = 0.0f;
        current_max_time = 200.0f;
        time_bar_width = current_max_time;
        spawn_timer = 0.0f;
        game_over = false;
        game_paused = true;
        bullets.clear();
        explosions.clear();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
