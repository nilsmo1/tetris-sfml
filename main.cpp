#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "Tetraminos.hpp"
#include <cstring>

const int xTiles = 10;
const int yTiles = 18;
const int tileSize = 32;
sf::Color backgroundColor = sf::Color::Black;

typedef std::array<std::array<int, xTiles>, yTiles> FieldArray;

bool inBounds(const Tetramino&, const int, const int);
void getRotation(Tetramino&, const Tetramino&);
bool doneDropping(const Tetramino&, const FieldArray&, const int, const int);
void addToField(FieldArray&, const Tetramino&, const int, const int, const int);
bool colliding(const Tetramino&, const FieldArray&, const int, const int);
int generateNextPiece(Tetramino& piece);
void displayPiece(sf::RenderWindow&, sf::Sprite&, const Tetramino&, const int, const int, const int);
void displayField(sf::RenderWindow&, sf::Sprite&, const FieldArray&);
bool removeLines(FieldArray&);
void moveFieldDown(FieldArray& field, const int line);
void initializeField(FieldArray&);

int main(int argc, char** argv)
{

    sf::RenderWindow window(sf::VideoMode(xTiles * tileSize, yTiles * tileSize), "Tetris");
    window.setFramerateLimit(30);

    sf::Texture tileTexture;
    if (argc > 2 && !strcmp(argv[1], "--color"))
    {
        if (!strcmp(argv[2], "white"))
        {
            tileTexture.loadFromFile("assets/white_tiles.png");
        }
        else if (!strcmp(argv[2], "color"))
        {
            tileTexture.loadFromFile("assets/__tiles.png");
        }
        else if (!strcmp(argv[2], "tiles"))
        {
            tileTexture.loadFromFile("assets/_tiles.png");
        }
        else if (!strcmp(argv[2], "minimal"))
        {
            tileTexture.loadFromFile("assets/minimal.png");
        }
        else if (!strcmp(argv[2], "light-minimal"))
        {
            tileTexture.loadFromFile("assets/light_minimal.png");
            backgroundColor = sf::Color::White;
        }
        else
        {
            tileTexture.loadFromFile("assets/__tiles.png");
        }
    }
    else
    {
        tileTexture.loadFromFile("assets/__tiles.png");
    }
    sf::Sprite sprite(tileTexture);

    Tetramino piece;
    //Tetramino nextPiece;

    std::srand(time(0));

    int pieceN = generateNextPiece(piece);
    FieldArray field {};
    initializeField(field);
    int rotation = 0;
    int tmpRotation;
    sprite.setTextureRect(sf::IntRect(tileSize * pieceN, 0, tileSize, tileSize));

    int xOffset = 4;
    int yOffset = 0;
    bool dropNextPiece = false;
    bool hardDrop = false;
    bool manualDrop = false;

    bool gameOver = false;
    const int tickRateValue = 500;
    const int keyPressRateValue = 30;
    const int pauseTime(tickRateValue / 2);
    const std::chrono::milliseconds tickRate(tickRateValue);
    const std::chrono::milliseconds keyPressRate(keyPressRateValue);
    std::chrono::time_point<std::chrono::steady_clock> previousTick = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> elapsedTickTime;
    std::chrono::time_point<std::chrono::steady_clock> previousKeyPress = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> elapsedKeyPressTime;

    while (window.isOpen() && !gameOver)
    {
        window.clear(backgroundColor);
        displayField(window, sprite, field);
        displayPiece(window, sprite, piece, pieceN, xOffset, yOffset);
        window.display();
        manualDrop = false;

        elapsedKeyPressTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTickTime - previousKeyPress) >= keyPressRate)
        {
            const sf::Keyboard keyPressed;
            if (keyPressed.isKeyPressed(sf::Keyboard::Q))
            {
                gameOver = true;
            }
            if (keyPressed.isKeyPressed(sf::Keyboard::Left))
            {
                if (inBounds(piece, xOffset - 1, yOffset) &&
                    !colliding(piece, field, xOffset - 1, yOffset))
                    xOffset -= 1;
            }
            if (keyPressed.isKeyPressed(sf::Keyboard::Right))
            {
                if (inBounds(piece, xOffset + 1, yOffset) &&
                    !colliding(piece, field, xOffset + 1, yOffset))
                    xOffset += 1;
            }
            if (keyPressed.isKeyPressed(sf::Keyboard::Down))
            {
                if (inBounds(piece, xOffset, yOffset + 1) &&
                    !colliding(piece, field, xOffset, yOffset + 1))
                {
                    yOffset += 1;
                    manualDrop = true;
                }
                else
                    manualDrop = false;
                hardDrop = doneDropping(piece, field, xOffset, yOffset + 1);
            }
            previousKeyPress = std::chrono::steady_clock::now();
        }


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                gameOver = true;
                break;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Q:
                        gameOver = true;
                        break;

                    case sf::Keyboard::Z:
                        tmpRotation = rotation;
                        rotation += 1;
                        rotation %= 4;
                        if (!inBounds(ALL_TETRAMINOS[pieceN][rotation], xOffset, yOffset) ||
                             colliding(ALL_TETRAMINOS[pieceN][rotation], field, xOffset, yOffset))
                        {
                            // TODO: adjust offset
                            rotation = tmpRotation;
                            break;
                        }
                        getRotation(piece, ALL_TETRAMINOS[pieceN][rotation]);
                        break;

                    case sf::Keyboard::X:
                        rotation -= 1;
                        rotation %= 4;
                        if (rotation < 0)
                            rotation += 4;
                        if (!inBounds(ALL_TETRAMINOS[pieceN][rotation], xOffset, yOffset) ||
                             colliding(ALL_TETRAMINOS[pieceN][rotation], field, xOffset, yOffset))
                        {
                            // TODO: adjust offset
                            rotation = tmpRotation;
                            break;
                        }
                        getRotation(piece, ALL_TETRAMINOS[pieceN][rotation]);
                        break;

                    default:
                        break;
                }
            }
        }
        elapsedTickTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTickTime - previousTick) >= tickRate || hardDrop)
        {
            if (hardDrop)
            {
                hardDrop = false;
                dropNextPiece = false;
                addToField(field, piece, pieceN, xOffset, yOffset);
                displayPiece(window, sprite, piece, pieceN, xOffset, yOffset);
                displayField(window, sprite, field);
                window.display();
                const bool didRemoveLines = removeLines(field);
                if (didRemoveLines)
                {
                    sf::sleep(sf::milliseconds(pauseTime));
                    previousKeyPress = std::chrono::steady_clock::now();
                }
                pieceN = generateNextPiece(piece);
                xOffset = 4;
                yOffset = 0;
                rotation = 0;
                if (doneDropping(piece, field, xOffset, yOffset))
                    gameOver = true;
                previousTick = std::chrono::steady_clock::now();
            }
            else if (dropNextPiece)
            {
                dropNextPiece = false;
                addToField(field, piece, pieceN, xOffset, yOffset);
                displayPiece(window, sprite, piece, pieceN, xOffset, yOffset);
                displayField(window, sprite, field);
                window.display();
                const bool didRemoveLines = removeLines(field);
                if (didRemoveLines)
                {
                    sf::sleep(sf::milliseconds(pauseTime));
                    previousKeyPress = std::chrono::steady_clock::now();
                }
                pieceN = generateNextPiece(piece);
                xOffset = 4;
                yOffset = 0;
                rotation = 0;
                if (doneDropping(piece, field, xOffset, yOffset))
                    gameOver = true;
                previousTick = std::chrono::steady_clock::now();
            }
            else if (doneDropping(piece, field, xOffset, yOffset + 1))
            {
                dropNextPiece = true;
            }
            else if (!manualDrop)
            {
                yOffset += 1;
                previousTick = std::chrono::steady_clock::now();
            }
        }
    }
    window.clear(backgroundColor);
    displayField(window, sprite, field);
    displayPiece(window, sprite, piece, pieceN, xOffset, yOffset);
    window.display();
    window.close();
    return 0;
}

bool inBounds(const Tetramino& tetramino,
              const int xOffset,
              const int yOffset)
{
    for (size_t i = 0; i < tetramino.size(); ++i)
    {
        const int x = tetramino[i].xPos + xOffset;
        if (x < 0 || x >= xTiles)
            return false;
        const int y = tetramino[i].yPos + yOffset;
        if (y < 0 || y >= yTiles)
            return false;
    }
    return true;
}

void getRotation(Tetramino& piece,
                 const Tetramino& tetramino)
{
    for (size_t i = 0; i < tetramino.size(); ++i)
        piece[i] = tetramino[i];
}

bool doneDropping(const Tetramino& tetramino,
                  const FieldArray& field,
                  const int xOffset,
                  const int yOffset)
{
    for (size_t i = 0; i < tetramino.size(); ++i)
    {
        if (tetramino[i].yPos + yOffset >= yTiles)
            return true;
    }

    for (size_t i = 0; i < tetramino.size(); ++i)
    {
        for (size_t j = 0; j < field.size(); ++j)
        {
            const int xt = tetramino[i].xPos + xOffset;
            const int yt = tetramino[i].yPos + yOffset;
            if (field[yt][xt] >= 0) return true;
        }
    }
    return false;
}

void addToField(FieldArray& field,
                const Tetramino& tetramino,
                const int pieceN,
                const int xOffset,
                const int yOffset)
{
    for (size_t i = 0; i < tetramino.size(); ++i)
        field[tetramino[i].yPos + yOffset][tetramino[i].xPos + xOffset] = pieceN;
}

bool colliding(const Tetramino& tetramino,
               const FieldArray& field,
               const int xOffset,
               const int yOffset)
{
    for (size_t i = 0; i < tetramino.size(); ++i)
    {
        const int xt = tetramino[i].xPos + xOffset;
        const int yt = tetramino[i].yPos + yOffset;
        if (field[yt][xt] >= 0) return true;
    }
    return false;
}

int generateNextPiece(Tetramino& piece)
{
    int pieceN = std::rand() % 7;
    const int rotation = 0;
    for (size_t i = 0; i < 4; ++i)
        piece[i] = ALL_TETRAMINOS[pieceN][rotation][i];
    return pieceN;
}

void displayPiece(sf::RenderWindow& window,
                  sf::Sprite& sprite,
                  const Tetramino& piece,
                  const int pieceN,
                  const int xOffset,
                  const int yOffset)
{
    for (size_t i = 0; i < piece.size(); ++i)
    {
        sprite.setTextureRect(sf::IntRect(tileSize * pieceN, 0, tileSize, tileSize));
        sprite.setPosition((piece[i].xPos + xOffset) * tileSize, (piece[i].yPos + yOffset) * tileSize);
        window.draw(sprite);
    }
}

void displayField(sf::RenderWindow& window,
                  sf::Sprite& sprite,
                  const FieldArray& field)
{
    window.clear(backgroundColor);
    for (size_t y = 0; y < yTiles; ++y)
    {
        for (size_t x = 0; x < xTiles; ++x)
        {
            if (field[y][x] < 0) continue;
            sprite.setTextureRect(sf::IntRect(tileSize * field[y][x], 0, tileSize, tileSize));
            sprite.setPosition(x * tileSize, y * tileSize);
            window.draw(sprite);
        }
    }
}

bool removeLines(FieldArray& field)
{
    std::vector<int> linesToRemove{};
    for (int y = yTiles - 1; y >= 0; --y)
    {
        bool rowFull = true;
        for (int x = 0; x < xTiles; ++x)
        {
            if (field[y][x] < 0)
            {
                rowFull = false;
                break;
            }
        }
        if (rowFull)
            linesToRemove.push_back(y);
    }

    const bool didRemoveLines = linesToRemove.size() > 0;
    for (int y = linesToRemove.size() - 1; y >= 0; --y)
    {
        const int line = linesToRemove[y];
        moveFieldDown(field, line);
    }
    return didRemoveLines;

}

void moveFieldDown(FieldArray& field, const int line)
{
    for (int y = line; y > 0; --y)
    {
        for (int x = 0; x < xTiles; ++x)
        {
            field[y][x] = field[y - 1][x];
        }
    }
}

void initializeField(FieldArray& field)
{
    for (int y = 0; y < yTiles; ++y)
    {
        for (int x = 0; x < xTiles; ++x)
        {
            field[y][x] = -1;
        }
    }

}
