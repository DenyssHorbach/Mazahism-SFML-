#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <conio.h>
#include <random>
#include <string>
#include <windows.h>

enum Direction { UP1, DOWN1, STOP1 };
Direction dir;
enum Direction2 { UP2, DOWN2, STOP2 };
Direction2 dir2;
enum ballDirection{RIGHT, LEFT, UP, DOWN, STOP};
ballDirection ballDir;
enum ballDirectionY { UP_BALL, DOWN_BALL, STOP_BALL_Y };
ballDirectionY ballDirY;


bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.position.x < b.position.x + b.size.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y;
}

bool gameOverPlayed = false;
float spriteSpeed = 600.f;

float ballSpeedY = 250.f;
float ballSpeedX = 900.f;

int varScore1 = 0;
bool scored1 = false;
int varScore2 = 0;
bool scored2 = false;

float respawnTimer = 0.f;
bool waitingForRespawn = false;
bool lastScoredByLeft = false;


int Draw() {

    sf::RenderWindow window(sf::VideoMode({ 1200, 720 }), "Tennis");

    sf::Clock clock;

    waitingForRespawn = true;
    respawnTimer = 1.5f;
    lastScoredByLeft = false; 
    ballDir = STOP;
    ballDirY = STOP_BALL_Y;


    sf::SoundBuffer over;
    if (!over.loadFromFile("audio/gameOverSound.wav")) {
        return -1;
    }

    sf::SoundBuffer ricoshet;
    if (!ricoshet.loadFromFile("audio/ric.wav")) {
        return -1;
    }

    sf::Texture endTexture;
    if (!endTexture.loadFromFile("images/end.png")) {
        return -1;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("images/TestSprite.png")) {
        return -1;
    }

    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("images/TestBall.png")) {
        return -1;
    }

    sf::Texture palkaTexture;
    if (!palkaTexture.loadFromFile("images/palka.png")) {
        return -1;
    }

    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("images/gameOverScreen.png")) {
        return -1;
    }

    sf::Font impact;
    if (!impact.openFromFile("fonts/impact.ttf")) {
        return -1;
    }

    sf::Text score1(impact);
    score1.setString(sf::String(std::to_string(varScore1)));
    score1.setCharacterSize(50);
    score1.setFillColor(sf::Color::White);
    score1.setStyle(sf::Text::Bold);
    score1.setPosition({1200/2-70.f, 50.f});

    sf::Text score2(impact);
    score2.setString(sf::String(std::to_string(varScore2)));
    score2.setCharacterSize(50);
    score2.setFillColor(sf::Color::White);
    score2.setStyle(sf::Text::Bold);
    score2.setPosition({ (1200 + 70) / 2.f, 50.f });

    sf::Sprite sprite(texture);
    sf::Sprite sprite2(texture);
    sf::Sprite ballSprite(ballTexture);
    sf::Sprite gameOverSprite(gameOverTexture);
    sf::Sprite palka(palkaTexture);
    sf::Sprite tolik(endTexture);


    sf::Sound rico(ricoshet);
    rico.setBuffer(ricoshet);

    sf::Sound endGame(over);
    endGame.setBuffer(over);

    sprite.setPosition({ 30.f, (720 / 2) - 100.f });
    sprite.setScale({ 0.6f, 0.6f });

    sprite2.setPosition({ 1200.f - 30, (720 / 2) - 100.f });
    sprite2.setScale({ 0.6f, 0.6f });

    ballSprite.setScale({ 0.3f, 0.3f });
    sf::FloatRect ballBounds = ballSprite.getGlobalBounds();
    ballSprite.setPosition({ (1200.f - ballBounds.size.x) / 2.f,
                                         (720.f - ballBounds.size.y) / 2.f });
    
    sf::FloatRect palkaBounds = palka.getGlobalBounds();
    palka.setPosition({ (1200 - palkaBounds.size.x) / 2.f, 0.f });
    //palka.setScale({})

    sf::FloatRect goBounds = gameOverSprite.getGlobalBounds();
    gameOverSprite.setScale({ 0.7f, 0.7f });
    gameOverSprite.setPosition({
        1200 / 2.f - goBounds.size.x / 3.f,
        720 / 2.f - goBounds.size.y / 3.f
        });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        float deltaTime = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            dir = UP1;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            dir = DOWN1;
        else 
			dir = STOP1;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            dir2 = UP2;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            dir2 = DOWN2;
		else
			dir2 = STOP2;

        sf::FloatRect spriteBounds = sprite.getGlobalBounds();
        if (dir == UP1 && spriteBounds.position.y > 0.f)
            sprite.move({ 0.f, -spriteSpeed * deltaTime });
        if (dir == DOWN1 && spriteBounds.position.y + spriteBounds.size.y < 720.f)
            sprite.move({ 0.f, spriteSpeed * deltaTime });

        sf::FloatRect sprite2Bounds = sprite2.getGlobalBounds();
        if (dir2 == UP2 && sprite2Bounds.position.y > 0.f)
            sprite2.move({ 0.f, -spriteSpeed * deltaTime });
        if (dir2 == DOWN2 && sprite2Bounds.position.y + sprite2Bounds.size.y < 720.f)
            sprite2.move({ 0.f, spriteSpeed * deltaTime });

        sf::Vector2f ballPos = ballSprite.getPosition();
        
        sprite2Bounds = sprite2.getGlobalBounds();
        spriteBounds = sprite.getGlobalBounds();


        switch (ballDir)
        {
        case LEFT:
            ballSprite.move({ -ballSpeedX * deltaTime, 0.0f });
            break;
        case RIGHT:
            ballSprite.move({ ballSpeedX * deltaTime, 0.0f });
            break;
        case STOP:
            ballSprite.move({ 0.f, 0.f });
            break;
        default:
            break;
        }

        if (!waitingForRespawn) {
            switch (ballDirY)
            {
            case UP_BALL:
                ballSprite.move({ 0.0f, -ballSpeedY * deltaTime });
                break;
            case DOWN_BALL:
                ballSprite.move({ 0.0f, ballSpeedY * deltaTime });
                break;
            case STOP_BALL_Y:
                break;
            }
        }


        


        ballPos = ballSprite.getPosition();
        ballBounds = ballSprite.getGlobalBounds();


        if (ballBounds.position.y <= 0.f)
            ballDirY = DOWN_BALL;
        if (ballBounds.position.y + ballBounds.size.y >= 720.f)
            ballDirY = UP_BALL;




        if (!waitingForRespawn) {
            if (intersects(ballBounds, sprite2Bounds)) {
                if (ballDir == LEFT) {
                    ballDir = RIGHT;
                }
                else {
                    ballDir = LEFT;
                }
                if (dir2 == UP2)
                    ballDirY = UP_BALL;
                else if (dir2 == DOWN2)
                    ballDirY = DOWN_BALL;
                rico.play();
            }
            else if (intersects(ballBounds, spriteBounds)) {
                if (ballDir == LEFT) {
                    ballDir = RIGHT;
                }
                else
                    ballDir = LEFT;
                if (dir == UP1)
                    ballDirY = UP_BALL;
                else if (dir == DOWN1)
                    ballDirY = DOWN_BALL;
                rico.play();
            }
        }
        

        if (!waitingForRespawn) {
            if (ballPos.x <= 0.f && !scored2) {
                varScore2++;
                score2.setString(sf::String(std::to_string(varScore2)));
                scored2 = true;
                lastScoredByLeft = false;
                ballSprite.setPosition({ (1200.f - ballBounds.size.x) / 2.f,
                                         (720.f - ballBounds.size.y) / 2.f });
                ballDir = STOP;
                waitingForRespawn = true;
                respawnTimer = 1.0f;
            }
            else if (ballPos.x + ballBounds.size.x >= 1200.f && !scored1) {
                varScore1++;
                score1.setString(sf::String(std::to_string(varScore1)));
                scored1 = true;
                lastScoredByLeft = true;
                ballSprite.setPosition({ (1200.f - ballBounds.size.x) / 2.f,
                                         (720.f - ballBounds.size.y) / 2.f });
                ballDir = STOP;
                waitingForRespawn = true;
                respawnTimer = 1.0f;
            }
        }
        else {
            respawnTimer -= deltaTime;
            if (respawnTimer <= 0.f) {
                waitingForRespawn = false;
                ballDir = lastScoredByLeft ? RIGHT : LEFT;
                scored1 = scored2 = false;
                int randStart = rand() % 2;
                if (randStart == 0)
                    ballDirY = UP_BALL;
                else
                    ballDirY = DOWN_BALL;
            }
        }

        

        sf::Color color(0, 0, 0);
        window.clear(color);
        window.draw(sprite);
        window.draw(sprite2);
        window.draw(ballSprite);
        window.draw(palka);
        window.draw(score1);
        window.draw(score2);

        if (varScore1 == 11 || varScore2 == 11) {
            window.draw(gameOverSprite);
            ballSprite.setPosition({ (1200.f - ballBounds.size.x) / 2.f,
                                         (720.f - ballBounds.size.y) / 2.f });
            ballDir = STOP;
            if (!gameOverPlayed) {
                endGame.play();
                gameOverPlayed = true;
            }
        }

        window.display();
    }
	return 0;
}


int main()
{
    Draw();
    return 0;
}