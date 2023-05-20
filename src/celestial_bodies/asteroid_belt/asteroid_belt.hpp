#pragma once

// Handle drawing asteroids using instancing
// This class does not handle the physics, just the drawing
// TODO : see how we can combine this ? Simply add it to the simulation handler as a drawable.
// TODO : change the simulation handler with booleans in order not to add drawables if they are already handled.
// TODO : this class will only handle planet instances. It would be far more complex to try and generalize
// TODO : handle low poly drawing
// TODO : for asteroids, instead of Planet instances, store Objects, and a list of models ? Separate into simple components

#include "celestial_bodies/overrides/star.hpp"
#include "cgp/geometry/shape/mesh/structure/mesh.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "utils/display/drawable.hpp"
#include "utils/physics/object.hpp"
#include <vector>
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

    // Random asteroid models
    std::vector<mesh> asteroid_meshes;
    std::vector<mesh_drawable> asteroid_mesh_drawables;

    // Objects
    std::vector<Object> objects; // Asteroid physical objects
    // TODO : create a class to associate to meshes ? How to simplify the instanced factorized calls ?
};