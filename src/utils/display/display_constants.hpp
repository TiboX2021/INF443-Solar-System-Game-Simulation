#pragma once

// Constants for display (phong coefficients, etc)

// Asteroids
constexpr float ASTEROID_PHONG_AMBIENT = 0.25;
constexpr float ASTEROID_PHONG_DIFFUSE = 0.6;

// Player
constexpr float PLAYER_SHIELD_RADIUS = 1; // Display radius. Divide by PHYSICS_SCALE to get physics radius