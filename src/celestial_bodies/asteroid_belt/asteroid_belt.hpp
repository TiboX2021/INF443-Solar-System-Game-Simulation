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
    2.0f, // Global noise scale
};

// TODO : add arrays of this for the different meshes ?
// TODO : how to use different mesh resolutions, taking into account the distance ?
/**
Struct to handle the data for mesh instancing (keep it clean and fast)
Using a linear scan on each frame, the data of the different asteroids is prepared in these instances before instancing call
*/
struct MeshInstancesData
{
    int mesh_index;
    int data_count;
    std::vector<cgp::vec3> positions;
    std::vector<cgp::mat3> rotations;

    // Initial size allocation
    void allocate(int n)
    {
        positions.resize(n);
        rotations.resize(n);
    }

    void resetData()
    {
        data_count = 0;
    }

    void addData(const cgp::vec3 &position, const cgp::mat3 &rotation)
    {
        positions[data_count] = position;
        rotations[data_count] = rotation;
        data_count++;
    }
};

// Data for an asteroid
struct Asteroid
{
    Object object;
    int mesh_index;
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
    std::vector<MeshInstancesData> asteroid_instances_data;

    // Objects
    std::vector<Asteroid> asteroids; // Asteroid physical objects
};