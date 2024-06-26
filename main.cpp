#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>

#define ROW 5
#define COL 5
#define MAX_SPHERE_D 240.0f

#define ICON_DRAWER 0
#define ICON_CIRCLE 1

sf::CircleShape circle_vector[ROW][COL];
sf::Vector2f initialCenterCirclePos;

void updateCircleOpacity(sf::CircleShape& circle, int alpha) {
    sf::Color currentColor = circle.getOutlineColor();
    currentColor.a = static_cast<sf::Uint8>(alpha);
    circle.setOutlineColor(currentColor);
    currentColor = circle.getFillColor();
    currentColor.a = static_cast<sf::Uint8>(alpha);
    circle.setFillColor(currentColor);
}

bool isCircleVisible(sf::CircleShape& circle) {
	return circle.getOutlineColor().a != 0;
}

bool isPointInCircle(sf::Vector2f point, sf::Vector2f circlePos, float radius) {
    float dx = point.x - circlePos.x;
    float dy = point.y - circlePos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= radius;
}

void decreaseRadiusCircleCenter(sf::RenderWindow& window, float variationX, float variationY, float& animationProgress, int row_i, int col_i) {
    const float maxRadius = 48.f;
    const float maxZoomRadius = 400.f;
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

            float radius;
            float newXPos = xPos;
            float newYPos = yPos;

            // Special case for the center circle
            if (row == row_i && col == col_i) {
                float initialRadius = maxRadius - (distance / MAX_SPHERE_D) * maxRadius;
                radius = initialRadius + (maxZoomRadius - initialRadius) * (1.0f - animationProgress);
                std::cout << radius << std::endl;
                newXPos = centerX; // Keep the center at 240.0f
                newYPos = centerY;
            }
            else {
                // Calculate the radius using a linear function
                radius = maxRadius - (distance / MAX_SPHERE_D) * maxRadius;

                // Special case for very small distances
                if (distance < 0.00001f) distance = 0.00001f;

                // Calculate the movement towards the center using a logarithmic function
                float maxMovement = distance / 3.f;
                float movement = maxMovement * (1.f - std::log(radius / maxRadius + 1) / std::log(2.f));

                // Move the circle towards the center
                newXPos = xPos - (xPos - centerX) * movement / distance;
                newYPos = yPos - (yPos - centerY) * movement / distance;
            }

            if (radius < 0) radius = 0;
            // Calculate the opacity based on the radius of the element
            alpha = static_cast<int>(255.0f * (-1) * std::atan(-radius / 20));
            if (alpha > 255.0f) alpha = 255.0f;

            // Update the circle position, radius, and opacity
            circle_vector[row][col].setPosition(newXPos - radius, newYPos - radius);
            circle_vector[row][col].setRadius(radius);
            updateCircleOpacity(circle_vector[row][col], alpha);

            // Draw the updated circle on the window (for all circles except the center circle)
            if (row != row_i || col != col_i)
                window.draw(circle_vector[row][col]);
        }
    }
    // Draw the center circle (the one with increasing radius) last
    window.draw(circle_vector[row_i][col_i]);
}

void increaseRadiusCircleCenter(sf::RenderWindow& window, float variationX, float variationY, float& animationProgress, int row_i, int col_i) {
    const float maxRadius = 48.f;
    const float maxZoomRadius = 400.f;
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

            float radius;
            float newXPos = xPos;
            float newYPos = yPos;

            // Special case for the center circle
            if (row == row_i && col == col_i) {
                float initialRadius = maxRadius - (distance / MAX_SPHERE_D) * maxRadius;
                radius = initialRadius + (maxZoomRadius - initialRadius) * animationProgress;
                std::cout << radius << std::endl;
                newXPos = centerX; // Keep the center at 240.0f
                newYPos = centerY;
            }
            else {
                // Calculate the radius using a linear function
                radius = maxRadius - (distance / MAX_SPHERE_D) * maxRadius;

                // Special case for very small distances
                if (distance < 0.00001f) distance = 0.00001f;

                // Calculate the movement towards the center using a logarithmic function
                float maxMovement = distance / 3.f;
                float movement = maxMovement * (1.f - std::log(radius / maxRadius + 1) / std::log(2.f));

                // Move the circle towards the center
                newXPos = xPos - (xPos - centerX) * movement / distance;
                newYPos = yPos - (yPos - centerY) * movement / distance;
            }

            if (radius < 0) radius = 0;
            // Calculate the opacity based on the radius of the element
            alpha = static_cast<int>(255.0f * (-1) * std::atan(-radius / 20));
            if (alpha > 255.0f) alpha = 255.0f;

            // Update the circle position, radius, and opacity
            circle_vector[row][col].setPosition(newXPos - radius, newYPos - radius);
            circle_vector[row][col].setRadius(radius);
            updateCircleOpacity(circle_vector[row][col], alpha);

            // Draw the updated circle on the window (for all circles except the center circle)
            if (row != row_i || col != col_i)
                window.draw(circle_vector[row][col]);
        }
    }
    // Draw the center circle (the one with increasing radius) last
    window.draw(circle_vector[row_i][col_i]);
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

                if (row == 2 && col == 2) {
                    initialCenterCirclePos = sf::Vector2f(newXPos - radius, newYPos - radius);
                }
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

    // Create the circles
    const float radius_hours = 30.0f; // Radius of the hours circle
    const float radius_minutes = 20.0f; // Radius of the minutes circle
    const float radius_seconds = 10.0f; // Radius of the seconds circle
    sf::CircleShape hours_circle(radius_hours);
    sf::CircleShape minutes_circle(radius_minutes);
    sf::CircleShape seconds_circle(radius_seconds);

    // Set the origin of the circles to their center
    hours_circle.setOrigin(radius_hours, radius_hours);
    minutes_circle.setOrigin(radius_minutes, radius_minutes);
    seconds_circle.setOrigin(radius_seconds, radius_seconds);

    // Set the fill color of the circles
    hours_circle.setFillColor(sf::Color::Red);
    minutes_circle.setFillColor(sf::Color::Blue);
    seconds_circle.setFillColor(sf::Color::Green);

    // Set the initial position of the circles to the center of the window
    hours_circle.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    minutes_circle.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    seconds_circle.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    const float circle_radius = 100.0f; // Radius of the circular path

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

    sf::FloatRect spriteBounds = iconBackSprite.getGlobalBounds();

    // Variables for animation
    uint8_t row_i, col_i = 0;
    bool zoomAnimation = false;
    bool isBouncing = false;
    float bounceOffset = 0.f;
    float bounceSpeed = 0.2f;
    float bounceHeight = 10.f;
    float originalY = iconBackSprite.getPosition().y;

    float animationProgress = 0.0f; // Initialize the animation progress to 0
    const float animationSpeedZoom = 0.010f; // Adjust this value to control the animation speed

    bool animateBackToInitial = false;
    bool animateBackToDrawer = false;
    float initialVariationX = 0.0f;
    float initialVariationY = 0.0f;

    // Mouse position
    bool isDragging = false;
    bool hasClicked = false;
    sf::Vector2i mousePos;
    sf::Vector2i currentMousePos;
    float variationX = 0.0f;
	float variationXpre = 0.0f;
    float variationY = 0.0f;
    float variationYpre = 0.0f;

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
                mousePos = sf::Mouse::getPosition(window);

                if (spriteBounds.contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    isBouncing = true;
                }

                isDragging = true; // Start dragging
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                // Get the current mouse position and calculate the movement
                currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2i moveVector = currentMousePos - mousePos;

                if (moveVector.x < 10 || moveVector.y < 10)
                {
                    hasClicked = true;
                }

                if (spriteBounds.contains(static_cast<sf::Vector2f>(currentMousePos)))
                {
                    if (!zoomAnimation)
                        animateBackToInitial = true;
                    else
                        animateBackToDrawer = true;
                }
                isDragging = false; // Stop dragging

                variationXpre = variationX;
                variationYpre = variationY;
            }
            else if (event.type == sf::Event::MouseMoved && isDragging)
            {
                // Get the current mouse position
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);

                // Calculate the movement vector
                sf::Vector2i moveVector = currentMousePos - mousePos;
                float moveLength = std::sqrt(moveVector.x * moveVector.x + moveVector.y * moveVector.y);
                float moveAngle = std::atan2(moveVector.y, moveVector.x); // Angle in radians

                // Calculate the x and y variations
                variationX = variationXpre + moveLength * std::cos(moveAngle);
                variationY = variationYpre + moveLength * std::sin(moveAngle);

                // Print the mouse position, translation offsets, and movement vector info
                /*std::cout << "Mouse Position - x: " << currentMousePos.x << ", y: " << currentMousePos.y << std::endl;
                std::cout << "Move Length: " << moveLength << ", Move Angle (radians): " << moveAngle << std::endl;
                std::cout << "Variation X: " << variationX << ", Variation Y: " << variationY << std::endl;*/

            }
        }

        window.clear();

        if (hasClicked)
        {
            static uint8_t stato = 0;

            switch (stato)
            {
            case 0:
                for (int row = 0; row < ROW; ++row) {
                    for (int col = 0; col < COL; ++col) {
                        sf::Vector2f circlePos = circle_vector[row][col].getPosition() + sf::Vector2f(circle_vector[row][col].getRadius(), circle_vector[row][col].getRadius());
                        float radius = circle_vector[row][col].getRadius();
                        if (isPointInCircle(sf::Vector2f(currentMousePos), circlePos, radius) && isCircleVisible(circle_vector[row][col])) {
                            // Calculate the new variations to move the clicked circle to the center
                            row_i = row;
                            col_i = col;
                            stato = 1;
                            break;
                        }
                    }
                }
                if (!stato)
                    hasClicked = false;
			break;
            case 1:
            {
                // Determine the animation speed (adjust as needed)
                const float animationSpeed = 5.0f;

                // Calculate the movement vector from the initially centered circle to the center of the window
                sf::Vector2f movementVector = initialCenterCirclePos - circle_vector[row_i][col_i].getPosition();
                float distance = std::sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);

                // Move all circles according to the movement vector
                if (distance > animationSpeed) {
                    float ratio = animationSpeed / distance;
                    variationX += movementVector.x * ratio;
                    variationY += movementVector.y * ratio;
                }
                else {
                    // Update variationX and variationY to the final position of the clicked circle
                    //variationX = initialCenterCirclePos.x - circle_vector[row_i][col_i].getPosition().x;
                    //variationY = initialCenterCirclePos.y - circle_vector[row_i][col_i].getPosition().y;

                    // Store the current variations for case 2
                    variationXpre = variationX;
                    variationYpre = variationY;

                    stato = 2;
                }
            }
            break;
            case 2:
                zoomAnimation = true;
                if (animationProgress < 1.0f) {
                    // Update the animation progress
                    animationProgress += animationSpeedZoom;

                    increaseRadiusCircleCenter(window, variationXpre, variationYpre, animationProgress, row_i, col_i);
                }
                else {
                    stato = 3;
                    animationProgress = 0.0f;
                }
                
            break;
            case 3:
                if(animateBackToDrawer)
                    stato = 4;
                else
                {
                    // Get the current time
                    time_t current_time = time(NULL);
                    struct tm local_time;
                    localtime_s(&local_time, &current_time);

                    // Calculate the angle of the hours circle
                    float hours_angle = (local_time.tm_hour % 12 + local_time.tm_min / 60.0f) * 30.0f * M_PI / 180.0f;

                    // Calculate the position of the hours circle on the circular path
                    float hours_x = window.getSize().x / 2.0f + circle_radius * cos(hours_angle);
                    float hours_y = window.getSize().y / 2.0f + circle_radius * sin(hours_angle);
                    hours_circle.setPosition(hours_x, hours_y);

                    // Calculate the angle of the minutes circle
                    float minutes_angle = local_time.tm_min * 6.0f * M_PI / 180.0f;

                    // Calculate the position of the minutes circle on the circular path
                    float minutes_x = window.getSize().x / 2.0f + circle_radius * cos(minutes_angle);
                    float minutes_y = window.getSize().y / 2.0f + circle_radius * sin(minutes_angle);
                    minutes_circle.setPosition(minutes_x, minutes_y);

                    // Calculate the angle of the seconds circle
                    float seconds_angle = local_time.tm_sec * 6.0f * M_PI / 180.0f;

                    // Calculate the position of the seconds circle on the circular path
                    float seconds_x = window.getSize().x / 2.0f + circle_radius * cos(seconds_angle);
                    float seconds_y = window.getSize().y / 2.0f + circle_radius * sin(seconds_angle);
                    seconds_circle.setPosition(seconds_x, seconds_y);

                    // Draw the circles
                    window.draw(hours_circle);
                    window.draw(minutes_circle);
                    window.draw(seconds_circle);
                }
            break;
            case 4:
                if (animationProgress < 1.0f) {
                    // Update the animation progress
                    animationProgress += animationSpeedZoom;

                    decreaseRadiusCircleCenter(window, variationXpre, variationYpre, animationProgress, row_i, col_i);
                }
                else {
                    stato = 0;
                    animationProgress = 0.0f;
                    hasClicked = false;
                    animateBackToDrawer = false;
                    zoomAnimation = false;
                }
            break;
            }
        }

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

        // Animation to move circles back to initial positions
        if (animateBackToInitial) {
            // Determine the animation speed (adjust as needed)
            const float animationSpeed = 5.0f;

            // Calculate the movement vector from the initially centered circle to the center of the window
            sf::Vector2f movementVector = initialCenterCirclePos - circle_vector[2][2].getPosition();
            float distance = std::sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);

            // Move all circles according to the movement vector
            if (distance > animationSpeed) {
                float ratio = animationSpeed / distance;
                variationX += movementVector.x * ratio;
                variationY += movementVector.y * ratio;
            }
            else {
                // Reset the variations to move all circles to the center
                variationX = variationY = 0;
                variationXpre = variationYpre = 0;
                animateBackToInitial = false;
            }
        }
        // Draw the background sprite
        window.draw(iconBackSprite);
        
        // Re-Draw circles in a 5x5 grid
        if(!zoomAnimation)
            drawCircles(window, variationX, variationY);

        window.display();
    }

    return 0;
}