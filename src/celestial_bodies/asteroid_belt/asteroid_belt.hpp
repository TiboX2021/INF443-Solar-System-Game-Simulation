#pragma once

// Handle drawing asteroids using instancing
// This class does not handle the physics, just the drawing
// TODO : see how we can combine this ? Simply add it to the simulation handler as a drawable.
// TODO : change the simulation handler with booleans in order not to add drawables if they are already handled.
// TODO : this class will only handle planet instances. It would be far more complex to try and generalize
// TODO : handle low poly drawing
// TODO : for asteroids, instead of Planet instances, store Objects, and a list of models ? Separate into simple components

#include "celestial_bodies/overrides/star.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "utils/display/drawable.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include <memory>
#include <vector>

// ************************************************** //
//                  ASTEROID CONSTANTS                //
// ************************************************** //

constexpr float ASTEROID_RADIUS = SATURN_RADIUS / 20; // TODO : scale this
const float ASTEROID_DISPLAY_RADIUS = Object::scaleRadiusForDisplay(ASTEROID_RADIUS);
constexpr float ASTEROID_MASS = 1e22;
constexpr float DISTANCE = SATURN_RADIUS * 4000; // Orbit distance : 1 billion meters, for saturn. TODO : update this for generic use

constexpr perlin_noise_parameters ASTEROID_NOISE_PARAMS{
    0.1f,
    0.1f, // Influence of small frequencies
    6,    // Level of detail
    0.4f,
    1.0f, // Global noise scale
};

class AsteroidBelt : public Drawable
{
public:
    AsteroidBelt();
    // Initialize member meshs
    virtual void initialize() override;

    // Draw function
    virtual void draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

    // Simulation
    void simulateStep();

    // TODO : function to simulate step

    // Setters & getters useless in this case
    virtual void setPosition(cgp::vec3 position) override{};
    virtual cgp::vec3 getPosition() const override { return cgp::vec3{}; };

    // Setters
    void setAttractor(Object *attractor) { this->attractor = attractor; };

private:
    void generateRandomAsteroids(int n);

    Object *attractor; // Pointer to the attractor object of the simulation

    Star debugShadable;

    // Random asteroid models
    std::vector<mesh_drawable> asteroid_mesh_drawables;
    std::vector<mesh_drawable> low_poly_asteroid_mesh_drawables;

    // Objects
    std::vector<Object> objects; // Asteroid physical objects
    // TODO : create a class to associate to meshes ? How to simplify the instanced factorized calls ?
    // Comment faire pour gérer les low poly ? A chaque frame, selon la distance, on recompose les vecteurs associés à chaque truc ?
};