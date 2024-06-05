#include <SFML/Graphics.hpp>
#include <iostream>

#define ROW 5
#define COL 5
#define MAX_SPHERE_D 240.0f

sf::CircleShape circle_vector[ROW][COL];

void updateCircleOpacity(sf::CircleShape& circle, int alpha) {
    sf::Color currentColor = circle.getOutlineColor();
    currentColor.a = static_cast<sf::Uint8>(alpha);
    circle.setOutlineColor(currentColor);
    currentColor = circle.getFillColor();
    currentColor.a = static_cast<sf::Uint8>(alpha);
    circle.setFillColor(currentColor);
}

bool isPointInCircle(sf::Vector2f point, sf::Vector2f circlePos, float radius) {
    float dx = point.x - circlePos.x;
    float dy = point.y - circlePos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= radius;
}

void drawCircles(sf::RenderWindow& window, float variationX, float variationY, bool init = false) {
    const float maxRadius = 48.f;
    int alpha = 255;

    for (int row = 0; row < ROW; ++row) {
        float yPos = row * 2 * maxRadius + maxRadius + variationY; // Y position of the row

        for (int col = 0; col < COL; ++col) {
            float xPos = col * 2 * maxRadius + maxRadius + variationX; // X position of the column

            // Shift odd rows by half radius on the x-axis
            if (row % 2 != 0)
                xPos += maxRadius;

            // Calculate the distance from the center
            float centerX = 240.f; // Center X coordinate
            float centerY = 240.f; // Center Y coordinate
            float distance = std::sqrt(std::pow(xPos - centerX, 2) + std::pow(yPos - centerY, 2));

            // Calculate the radius using a linear function
            float radius = maxRadius - (distance / MAX_SPHERE_D) * maxRadius;

            // Special case for the center circle
            if (distance == 0.f) distance = 0.00001f;

            // Calculate the movement towards the center using a logarithmic function
            float maxMovement = distance / 3.f;
            float movement = maxMovement * (1.f - std::log(radius / maxRadius + 1) / std::log(2.f));

            // Move the circle towards the center
            float newXPos = xPos - (xPos - centerX) * movement / distance;
            float newYPos = yPos - (yPos - centerY) * movement / distance;

            if (radius < 0) radius = 0;
            // Calculate the opacity based on the radius of the element
            alpha = static_cast<int>(255.0f * (-1) * std::atan(-radius / 20));
            if (alpha > 255.0f) alpha = 255.0f;

            // TODO fix colour managment
            circle_vector[row][col].setRadius(radius);
            circle_vector[row][col].setPosition(newXPos - radius, newYPos - radius);
            if (init)
            {
                circle_vector[row][col].setOutlineColor(sf::Color(200, 200, 200, static_cast<sf::Uint8>(alpha)));
                circle_vector[row][col].setFillColor(sf::Color::Transparent);
                circle_vector[row][col].setOutlineThickness(2.f);
            }
            else
            {
                updateCircleOpacity(circle_vector[row][col], alpha);
            }
            window.draw(circle_vector[row][col]);
        }
    }
}

int main()
{
    // Request anti-aliasing
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16; // Set the desired level of anti-aliasing

    sf::RenderWindow window(sf::VideoMode(480, 480), "Drawer");
    window.setFramerateLimit(30);

    // Load the image
    sf::Image iconBackImage;
    if (!iconBackImage.loadFromFile("images/i_back.png")) {
        std::cerr << "Error loading image" << std::endl;
        return -1;
    }

    // Invert the colors of the image
    sf::Vector2u imageSize = iconBackImage.getSize();
    for (unsigned int x = 0; x < imageSize.x; ++x) {
        for (unsigned int y = 0; y < imageSize.y; ++y) {
            sf::Color pixelColor = iconBackImage.getPixel(x, y);
            pixelColor.r = 255 - pixelColor.r;
            pixelColor.g = 255 - pixelColor.g;
            pixelColor.b = 255 - pixelColor.b;
            iconBackImage.setPixel(x, y, pixelColor);
        }
    }

    // Create a texture from the inverted image
    sf::Texture iconBackTexture;
    iconBackTexture.loadFromImage(iconBackImage);
    iconBackTexture.setSmooth(true);

    // Create a sprite for the image
    sf::Sprite iconBackSprite;
    iconBackSprite.setTexture(iconBackTexture);

    // Set the scale to resize the image to 50x50 pixels
    iconBackSprite.setScale(25.f / iconBackTexture.getSize().x, 25.f / iconBackTexture.getSize().y);

    // Set the position to the bottom of the window
    iconBackSprite.setPosition(225.f, 445.f); // 480 - 25 = 455 (bottom position)

    // Variables for animation
    bool isBouncing = false;
    float bounceOffset = 0.f;
    float bounceSpeed = 0.2f;
    float bounceHeight = 10.f;
    float originalY = iconBackSprite.getPosition().y;

    // Mouse position
    bool isDragging = false;
    sf::Vector2i previousMousePos;
    float variationX = 0.0f;
    float variationY = 0.0f;

    // Initialize circles in a 5x5 grid
    drawCircles(window, variationX, variationY, true);

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::FloatRect spriteBounds = iconBackSprite.getGlobalBounds();

                if (spriteBounds.contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    isBouncing = true;
                }

                isDragging = true; // Start dragging
                previousMousePos = sf::Mouse::getPosition(window); // Initialize previous position
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the current mouse position and calculate the movement
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2i moveVector = currentMousePos - previousMousePos;

                if (moveVector.x < 10 || moveVector.y < 10)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    std::cout << "Mouse Position - x: " << mousePos.x << ", y: " << mousePos.y << std::endl;
                    for (int row = 0; row < ROW; ++row) {
                        for (int col = 0; col < COL; ++col) {
                            sf::Vector2f circlePos = circle_vector[row][col].getPosition() + sf::Vector2f(circle_vector[row][col].getRadius(), circle_vector[row][col].getRadius());;
                            float radius = circle_vector[row][col].getRadius();
                            if (isPointInCircle(sf::Vector2f(mousePos), circlePos, radius)) {
                                if (circle_vector[row][col].getFillColor() == sf::Color::White)
                                    circle_vector[row][col].setFillColor(sf::Color::Transparent);
                                else
                                    circle_vector[row][col].setFillColor(sf::Color::White);
                                break;
                            }
                        }
                    }
                }
                isDragging = false; // Stop dragging
            }
            else if (event.type == sf::Event::MouseMoved && isDragging)
            {
                // Get the current mouse position
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);

                // Calculate the movement vector
                sf::Vector2i moveVector = currentMousePos - previousMousePos;
                float moveLength = std::sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y);
                float moveAngle = std::atan2(moveVector.y, moveVector.x); // Angle in radians

                // Calculate the x and y variations
                variationX = moveLength * std::cos(moveAngle);
                variationY = moveLength * std::sin(moveAngle);

                // Print the mouse position, translation offsets, and movement vector info
                /*std::cout << "Mouse Position - x: " << currentMousePos.x << ", y: " << currentMousePos.y << std::endl;
                std::cout << "Move Length: " << moveLength << ", Move Angle (radians): " << moveAngle << std::endl;
                std::cout << "Variation X: " << variationX << ", Variation Y: " << variationY << std::endl;*/

            }
        }

        window.clear();

        // Update animation if bouncing
        if (isBouncing) {
            bounceOffset += bounceSpeed;
            // Stop bouncing after one full cycle
            if (bounceOffset >= 2 * 3.14159f) {
                bounceOffset = 0.f;
                isBouncing = false;
                iconBackSprite.setPosition(225.f, originalY);
            }
            else if (bounceOffset >= 3.14159f) {
                float newY = originalY - std::abs(std::sin(bounceOffset)) * bounceHeight;
                iconBackSprite.setPosition(225.f, newY);
            }
            else {
                float newY = originalY - std::abs(1.2 * std::sin(bounceOffset)) * bounceHeight;
                iconBackSprite.setPosition(225.f, newY);
            }

            
        }

        // Draw the background sprite
        window.draw(iconBackSprite);
        
        // Re-Draw circles in a 5x5 grid
        drawCircles(window, variationX, variationY);

        window.display();
    }

    return 0;
}