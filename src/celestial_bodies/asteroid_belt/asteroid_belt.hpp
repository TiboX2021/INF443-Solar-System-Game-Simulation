#pragma once

// Handle drawing asteroids using instancing
// This class does not handle the physics, just the drawing
// TODO : see how we can combine this ? Simply add it to the simulation handler as a drawable.
// TODO : change the simulation handler with booleans in order not to add drawables if they are already handled.
// TODO : this class will only handle planet instances. It would be far more complex to try and generalize
// TODO : handle low poly drawing
// TODO : for asteroids, instead of Planet instances, store Objects, and a list of models ? Separate into simple components

#include "celestial_bodies/overrides/star.hpp"
#include "utils/display/drawable.hpp"
class AsteroidBelt : public Drawable
{
public:
    AsteroidBelt();
    // Initialize member meshs
    virtual void initialize() override;

    // Draw function
    virtual void draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

    // Setters & getters useless in this case
    virtual void setPosition(cgp::vec3 position) override{};
    virtual cgp::vec3 getPosition() const override { return cgp::vec3{}; };

private:
    Star debugShadable;
};