#pragma once

// Handle drawing asteroids using instancing
// This class does not handle the physics, just the drawing

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
// BUG : asteroids are not displayed at all ! This is the usual bug. It does not happen with the other planets...
constexpr float ASTEROID_RADIUS = SATURN_RADIUS / 40; // TODO : scale this
const float ASTEROID_DISPLAY_RADIUS = Object::scaleRadiusForDisplay(ASTEROID_RADIUS);
constexpr float ASTEROID_MASS = 1e22;
constexpr float DISTANCE = SATURN_RADIUS * 2500; // Orbit distance : 1 billion meters, for saturn. TODO : update this for generic use

constexpr perlin_noise_parameters ASTEROID_NOISE_PARAMS{
    0.1f,
    0.1f, // Influence of small frequencies
    6,    // Level of detail
    0.4f,
    1.0f, // Global noise scale
};

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
    std::vector<float> scales;

    // Initial size allocation
    void allocate(int n)
    {
        positions.resize(n);
        rotations.resize(n);
        scales.resize(n);
    }

    void resetData()
    {
        data_count = 0;
    }

    void addData(const cgp::vec3 &position, const cgp::mat3 &rotation, float scale)
    {
        positions[data_count] = position;
        rotations[data_count] = rotation;
        scales[data_count] = scale;
        data_count++;
    }
};

// Data for an asteroid
struct Asteroid
{
    Object object;
    int mesh_index;
    float scale = 1.0f;
};

// TODO mesh handler ?
struct DistanceMeshHandler
{
    int high_poly;
    int low_poly;
    int low_poly_disk;
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
    void simulateStep(float step = 24.0f * 3600 / 60);

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
    std::vector<MeshInstancesData> asteroid_instances_data; // For each mesh drawable
    std::vector<DistanceMeshHandler> distance_mesh_handlers;

    // TODO : add scales for randomized sizes
    // TODO : how to store which mesh to associate according to distance ?
    // Base : add mesh in list
    // TODO : compute which mesh to use ? Use mesh handlers with indexes, who point to the right mesh

    // Objects
    std::vector<Asteroid> asteroids; // Asteroid physical objects
};