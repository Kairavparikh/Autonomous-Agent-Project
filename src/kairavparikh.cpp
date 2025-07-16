#include "raylib.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <resource_dir.h>
#include <algorithm>

const int numBuses = 5;
const int numTargets = 50;
const float maxSpeed = 150.0f;
const float fuelPickupAmount = 30.0f;
const float wanderSpeed = 50.0f;
const float wanderRadius = 100.0f;

struct Target {
    Vector2 position;
    bool active;
};

struct Bus {
    Vector2 position;
    Vector2 velocity;
    float fuel;
    float maxFuel;
    Vector2 target;
    bool wandering;
    float wanderTimer;
    bool speedBoost;
    float powerUpTimer;
    int score;
};

bool compareBuses(const Bus& a, const Bus& b) {
    return a.score > b.score;
}

Vector2 norm(Vector2 vect, float max) {
    float length = sqrtf(vect.x * vect.x + vect.y * vect.y);
    if (length > 0) {
        vect.x = (vect.x / length) * max;
        vect.y = (vect.y / length) * max;
    }
    return vect;
}

bool CirclesOverlap(Vector2 newCircle, Vector2 existingCircle, float radius) {
    float distance = sqrtf((newCircle.x - existingCircle.x) * (newCircle.x - existingCircle.x) +
                           (newCircle.y - existingCircle.y) * (newCircle.y - existingCircle.y));
    return distance < (2 * radius);
}
void RefuelBus(Bus& bus, std::vector<Target>& targets) {
    bus.fuel += fuelPickupAmount;
    if (bus.fuel > bus.maxFuel) bus.fuel = bus.maxFuel;
    if (bus.fuel > 0) bus.wandering = false;
    bus.target = targets[GetRandomValue(0, numTargets - 1)].position;
}

void ResetTargets(std::vector<Target>& targets) {
    for (int i = 0; i < numTargets; i++) {
        Vector2 newPosition;
        bool validPosition = false;

        while (!validPosition) {
            newPosition = { static_cast<float>(GetRandomValue(50, 1200)), static_cast<float>(GetRandomValue(50, 725)) };
            validPosition = true;

            for (int j = 0; j < i; j++) {
                if (targets[j].active && CirclesOverlap(newPosition, targets[j].position, 25.0f)) {
                    validPosition = false;
                    break;
                }
            }
        }

        targets[i].position = newPosition;
        targets[i].active = true;
    }
}

int main() {
    InitWindow(1280, 800, "Hello Raylib");
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    srand(static_cast<unsigned int>(time(nullptr)));

    SearchAndSetResourceDir("resources");

    Image image = LoadImage("rap.png");
    ImageResize(&image, 100, 100);
    Texture2D busTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    Image newImage = LoadImage("busfr.png");
    ImageResize(&newImage, 100, 100);
    Texture2D newBusTexture = LoadTextureFromImage(newImage);
    UnloadImage(newImage);

    std::vector<Target> targets(numTargets);
    ResetTargets(targets);

    std::vector<Bus> buses(numBuses);
    for (int i = 0; i < numBuses; i++) {
        buses[i].position = {static_cast<float>(GetRandomValue(100, 1180)),
                             static_cast<float>(GetRandomValue(100, 700))};
        buses[i].fuel = 100.0f;
        buses[i].maxFuel = 100.0f;
        buses[i].wandering = false;
        buses[i].wanderTimer = 0.0f;
        buses[i].speedBoost = false;
        buses[i].powerUpTimer = 0.0f;
        buses[i].target = targets[GetRandomValue(0, numTargets - 1)].position;
        buses[i].score = 0;
    }

    float targetResetTimer = 0.0f;
    int stage = 1;

    while (!WindowShouldClose()) {
        float t = GetFrameTime();
        targetResetTimer += t;

        if (targetResetTimer >= 20.0f) {
            ResetTargets(targets);
            targetResetTimer = 0.0f;
            stage += 1;

            std::sort(buses.begin(), buses.end(), compareBuses);
            for (int i = 0; i < numBuses; i++) {
                if (i == 0) buses[i].maxFuel += 40;
                else if (i == 1) buses[i].maxFuel += 30;
                else if (i == 2) buses[i].maxFuel += 20;
                else if (i == 3) buses[i].maxFuel += 10;
                else if (i == 4) buses[i].maxFuel += 5;

                if (buses[i].maxFuel > 140.0f) buses[i].maxFuel = 140.0f;
            }

            int busesToRemove = 1;
            if (buses.size() > busesToRemove) {
                buses.erase(buses.end() - busesToRemove, buses.end());
            }

            for (int i = 0; i < buses.size(); i++) {
                buses[i].target = targets[GetRandomValue(0, numTargets - 1)].position;
                buses[i].fuel = buses[i].maxFuel;
            }
        }

        for (int i = 0; i < buses.size(); i++) {
            if (buses[i].fuel > 0) {
                buses[i].fuel -= 20.0f * t;
                if (buses[i].fuel < 0) {
                    buses[i].fuel = 0;
                    buses[i].wandering = true;
                }
            }

            int rand = GetRandomValue(1, 2);
            if (rand == 1) {
                buses[i].speedBoost = true;
                buses[i].powerUpTimer = 0;
            }

            if (buses[i].speedBoost) {
                buses[i].powerUpTimer += t;
                if (buses[i].powerUpTimer > 2.0f) {
                    buses[i].speedBoost = false;
                    buses[i].powerUpTimer = 0.0f;
                }
            }

            Vector2 path = {buses[i].target.x - buses[i].position.x, buses[i].target.y - buses[i].position.y};
            float speed = buses[i].speedBoost ? maxSpeed : 150.0f;

            path = norm(path, speed);

            if (!buses[i].wandering) {
                Vector2 movingSpeed = {path.x - buses[i].velocity.x, path.y - buses[i].velocity.y};
                movingSpeed = norm(movingSpeed, speed);

                buses[i].velocity.x += movingSpeed.x * t * 3;
                buses[i].velocity.y += movingSpeed.y * t * 3;

                buses[i].velocity = norm(buses[i].velocity, speed);

                buses[i].position.x += buses[i].velocity.x * t;
                buses[i].position.y += buses[i].velocity.y * t;

                for (auto &target : targets) {
                    if (target.active && CirclesOverlap(buses[i].position, target.position, 25.0f)) {
                        target.active = false;
                        buses[i].score += 1;
                        RefuelBus(buses[i], targets);
                        break;
                    }
                }
            } else {
                buses[i].wanderTimer += t;
                if (buses[i].wanderTimer >= 3.0f) {
                    buses[i].wanderTimer = 0.0f;
                    Vector2 randDirection = {static_cast<float>(GetRandomValue(-1, 1)), static_cast<float>(GetRandomValue(-1, 1))};
                    float length = sqrtf(randDirection.x * randDirection.x + randDirection.y * randDirection.y);
                    if (length > 0) {
                        randDirection.x /= length;
                        randDirection.y /= length;
                    }
                    buses[i].velocity.x = randDirection.x * wanderSpeed;
                    buses[i].velocity.y = randDirection.y * wanderSpeed;
                }

                buses[i].position.x += buses[i].velocity.x * t;
                buses[i].position.y += buses[i].velocity.y * t;

                for (auto &target : targets) {
                    if (target.active && CirclesOverlap(buses[i].position, target.position, 25.0f)) {
                        target.active = false;
                        buses[i].score += 1;
                        RefuelBus(buses[i], targets);

                        break;
                    }
                }
            }
            if (buses[i].position.x <= 0 || buses[i].position.x >= 1260) {
                buses[i].velocity.x *= -1;
            }
            if (buses[i].position.y <= 0 || buses[i].position.y >= 780) {
                buses[i].velocity.y *= -1;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < buses.size(); i++) {
            if (i == 0) {
                double angle = atan2f(buses[i].velocity.y, buses[i].velocity.x) * (180.0f / M_PI);
                Vector2 hitPoint = {newBusTexture.width / 2.0f, newBusTexture.height / 2.0f};
                Rectangle map = {0, 0, (float)newBusTexture.width, (float)newBusTexture.height};
                Vector2 busCenter = {buses[i].position.x - hitPoint.x, buses[i].position.y - hitPoint.y};
                Vector2 offset = {hitPoint.x, hitPoint.y};

                DrawTexturePro(newBusTexture, map, (Rectangle){busCenter.x, busCenter.y, (float)newBusTexture.width, (float)newBusTexture.height}, offset, angle + 90, WHITE);
            } else {
                double angle = atan2f(buses[i].velocity.y, buses[i].velocity.x) * (180.0f / M_PI);
                Vector2 hitPoint = {busTexture.width / 2.0f, busTexture.height / 2.0f};
                Rectangle map = {0, 0, (float)busTexture.width, (float)busTexture.height};
                Vector2 busCenter = {buses[i].position.x - hitPoint.x, buses[i].position.y - hitPoint.y};
                Vector2 offset = {hitPoint.x, hitPoint.y};

                DrawTexturePro(busTexture, map, (Rectangle){busCenter.x, busCenter.y, (float)busTexture.width, (float)busTexture.height}, offset, angle + 90, WHITE);
            }

            DrawText(TextFormat("Fuel: %.0f / %.0f", buses[i].fuel, buses[i].maxFuel), buses[i].position.x - 20, buses[i].position.y - 60, 20, BLACK);
            DrawText(TextFormat("Score: %d", buses[i].score), buses[i].position.x - 20, buses[i].position.y - 80, 20, BLACK);
        }

        for (const auto &target : targets) {
            if (target.active) {
                DrawCircle(target.position.x, target.position.y, 25.0f, BLUE);
            }
        }

        DrawText(TextFormat("Stage: %i", stage), 10, 10, 20, BLACK);


        EndDrawing();
    }

    UnloadTexture(busTexture);
    UnloadTexture(newBusTexture);
    CloseWindow();
    return 0;
}
