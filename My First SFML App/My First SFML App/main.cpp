
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <string>
#include <iostream>
#include <time.h>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

int scoreDigits(int score_r) {
    int i = 0;
    do {
        ++i;
        score_r /= 10;
    } while (score_r);
    return i;
}

void newGame(const float ball_init_x, sf::RectangleShape &bar_l, sf::RectangleShape &bar_r, sf::CircleShape &ball, const sf::Vector2f &window_size, const float &padding) {
    bar_l.setPosition(padding,window_size.y/2 - bar_l.getSize().y/2);
    bar_r.setPosition(window_size.x - (bar_l.getSize().x + padding), window_size.y/2 - bar_l.getSize().y/2);
    ball.setPosition(ball_init_x, window_size.y/2);
}

int main(int, char const**)
{
    srand(time(NULL));
    const float ball_radius = 15.0f;
    //sf::Vector2f direction(random(), random());
    sf::Vector2f window_size(1280, 750);
    sf::Vector2i score(0, 0);
    float padding = 10.f;
    static const std::string frases[] = {"Si no ganas esta partida te vamos a marginar","¿Quieres ver las fotos de los gintonics?","PRO1 a la cuarta me la saco seguro","Si no apruebo ire al soborno","Conozco a uno del LSI","Si no aprendes catalan te vamos a marginar","¿Pero quieres o no quieres ver las fotos \nde los gintonics?","Los gintonics, tiu."};
    std::vector<std::string> frases_buenas(frases,frases+sizeof(frases)/sizeof(frases[0]));
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode((int)window_size.x, (int)window_size.y, 32), "Pong de Jongh - The Game");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Load textures
    sf::Texture texture_l, texture_r, texture_bar;
    texture_l.loadFromFile(resourcePath() + "bg_l.png");
    texture_r.loadFromFile(resourcePath() + "bg_r.png");
    
    // Define the ball
    sf::CircleShape ball;
    ball.setPosition(window_size.x/2, window_size.y/2);
    ball.setRadius(30);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setFillColor(sf::Color::Yellow);
    ball.setOutlineThickness(2);
    ball.setOutlineColor(sf::Color::Black);
    
    // Define the bars
    float m_ratio = 2.f;
    const sf::Vector2f barSize(window_size.x/80.f, window_size.y/4.f);
    sf::RectangleShape bar_l, bar_r;
    
    bar_l.setFillColor(sf::Color::Yellow);
    bar_l.setOutlineThickness(2);
    bar_l.setOutlineColor(sf::Color::Black);
    bar_r.setFillColor(sf::Color::Yellow);
    bar_r.setOutlineThickness(2);
    bar_r.setOutlineColor(sf::Color::Black);
    
    bar_l.setSize(barSize);
    bar_r.setSize(barSize);
    
    newGame(window_size.x/2.0f, bar_l, bar_r, ball, window_size, padding);
    
    // Define velocity
    sf::Vector2f velocity(400,400);
    
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(resourcePath() + "bg_r.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("\n Pong de Jongh", font, 50);
    std::string frase_random = frases_buenas[rand()%frases_buenas.size()];
    sf::Text text_frases(frase_random, font, 25);
    text.setFillColor(sf::Color::Black);
    text_frases.setFillColor(sf::Color::Black);
    sf::String score_str_l = std::to_string(score.x);
    int scoreFontSize = 70;
    int scoreNumDigits = 1;
    sf::Text score_l(score_str_l, font, scoreFontSize);
    score_l.setFillColor(sf::Color::Yellow);
    score_l.setOutlineColor(sf::Color::Black);
    score_l.setPosition(padding, window_size.y - (padding + scoreFontSize));
    sf::String score_str_r = std::to_string(score.y);
    sf::Text score_r(score_str_r, font, 70);
    score_r.setFillColor(sf::Color::Yellow);
    score_r.setOutlineColor(sf::Color::Black);
    score_r.setPosition(window_size.x - (padding + scoreNumDigits * scoreFontSize), window_size.y - (padding + scoreFontSize));

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() + "main_theme.ogg")) {
        return EXIT_FAILURE;
    }

    // Play the music
    music.play();
    music.setLoop(true);

    // Hacks.
    sf::Clock clock;
    sf::Time ttl = sf::seconds(2);
    
    // Start the game loop
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // Right bar UP (arrow up)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            bar_r.setPosition(bar_r.getPosition().x, std::max(bar_r.getPosition().y - m_ratio, padding));
        }
        
        // Right bar DOWN (arrow down)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            bar_r.setPosition(bar_r.getPosition().x, std::min(bar_r.getPosition().y + m_ratio, window_size.y - (padding + bar_r.getSize().y)));
        }
        
        // Left bar UP (W key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            bar_l.setPosition(bar_l.getPosition().x, std::max(bar_l.getPosition().y - m_ratio, padding));
        }
        
        // Left bar DOWN (S key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            bar_l.setPosition(bar_l.getPosition().x, std::min(bar_l.getPosition().y + m_ratio, window_size.y - (padding + bar_r.getSize().y)));
        }
        
        // Increasing ball speed
        ttl -= sf::seconds(deltaTime);
        if (ttl < sf::Time::Zero && velocity.x < 800) {
            ttl = sf::seconds(2);
            velocity *= 1.01f;
        }
        
        // Save previous ball position
        sf::Vector2f prev_position = ball.getPosition();
        
        // Ball bounce
        ball.setPosition(ball.getPosition() + velocity * deltaTime);
        if (ball.getPosition().x + ball.getRadius() >= window_size.x) {
            ++score.x;
            score_str_l = std::to_string(score.x);
            score_l.setString(score_str_l);
            newGame(2 * ball.getRadius() + padding, bar_l, bar_r, ball, window_size, padding);
        } else if (ball.getPosition().x - ball.getRadius() <= 0) {
            ++score.y;
            score_str_r = std::to_string(score.y);
            score_r.setString(score_str_r);
            scoreNumDigits = scoreDigits(score.y);
            score_r.setPosition(window_size.x - (padding + scoreNumDigits * scoreFontSize * 0.8), score_r.getPosition().y);
            newGame(window_size.x - (2 * ball.getRadius() + padding), bar_l, bar_r, ball, window_size, padding);
        }
        
        if (ball.getPosition().y + ball.getRadius() >= window_size.y || ball.getPosition().y - ball.getRadius() <= 0) {
            velocity.y *= -1;
            // Reset vertical position just to ensure the ball keeps inside the screen
            if (ball.getPosition().y <= ball.getRadius())
                ball.setPosition(ball.getPosition().x, ball.getRadius());
            else
                ball.setPosition(ball.getPosition().x, window_size.y - ball.getRadius());
        }
        
        // Bar collisions
        if (ball.getPosition().x + ball.getRadius() > window_size.x - (padding + barSize.x) && ball.getPosition().y >= bar_r.getPosition().y && ball.getPosition().y <= bar_r.getPosition().y + barSize.y) {
            velocity.x *= -1;
            
        } else if (ball.getPosition().x - ball.getRadius() < padding + barSize.x && ball.getPosition().y >= bar_l.getPosition().y && ball.getPosition().y <= bar_l.getPosition().y + barSize.y) {
            velocity.x *= -1;
        }
        
        // Background depending on the ball's position
        if (prev_position.x <= window_size.x/2 && ball.getPosition().x >= window_size.x/2) {
            sprite.setTexture(texture_r);
        } else if (prev_position.x >= window_size.x/2 && ball.getPosition().x <= window_size.x/2) {
            sprite.setTexture(texture_l);
        }
        
        
        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);
        
        text_frases.setPosition(window_size.x/2 + 0.1 * window_size.x, window_size.y/6);
        if (std::abs(velocity.x) >= 400) {
            if (not (rand() % 6000)) {
                frase_random = frases_buenas[rand()%frases_buenas.size()];
                text_frases.setString(frase_random);
            }
            window.draw(text_frases);
        }
        
        // Draw the ball
        window.draw(ball);
        
        // Draw the bars
        window.draw(bar_l);
        window.draw(bar_r);

        // Draw the strings
        window.draw(text);
        window.draw(score_l);
        window.draw(score_r);

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
