#pragma once

// Handle drawing asteroids using instancing
// This class does not handle the physics, just the drawing

#include "celestial_bodies/asteroid_belt/asteroid_thread_pool.hpp"
#include "utils/display/drawable.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include <memory>
#include <vector>

// ************************************************** //
//                  ASTEROID CONSTANTS                //
// ************************************************** //
constexpr float ASTEROID_MASS = 1e22;
constexpr float DISTANCE = SATURN_RADIUS * 2500; // Orbit distance : 1 billion meters, for saturn. TODO : update this for generic use
constexpr float ASTEROID_ORBIT_FACTOR = 10;      // Accelerate asteroids orbit for visual purposes

constexpr perlin_noise_parameters ASTEROID_NOISE_PARAMS{
    0.1f,
    0.1f, // Influence of small frequencies
    6,    // Level of detail
    0.4f,
    1.0f, // Global noise scale
};

enum BeltPresets
{
    SATURN,
    SUN,
    KUIPER,
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
    Object object; // TODO : store index in object vector instead (it will be coming from the thread pool)
    int mesh_index;
    float scale = 1.0f;
};

class AsteroidBelt : public Drawable
{
public:
    AsteroidBelt() : preset(BeltPresets::SATURN), pool({}){};
    AsteroidBelt(BeltPresets preset);

    ~AsteroidBelt()
    {
        // Stop thread pool in destructor
        pool.stop();
    };

    // Initialize member meshs
    virtual void initialize() override;

    // Draw function
    virtual void draw(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true) override;

    // Simulation
    void simulateStep(float step = 24.0f * 3600 / 60);

    // Setters & getters useless in this case
    virtual void setPosition(cgp::vec3) override{};
    virtual cgp::vec3 getPosition() const override { return cgp::vec3{}; };

    // Setters
    // void setAttractor(Object *attractor) { this->attractor = attractor; };
    void addAttractor(Object *attractor) { this->attractors.push_back(attractor); };

private:
    void generateRandomAsteroids(int n);

    std::vector<Object *> attractors; // Pointer to the attractor object of the simulation
    cgp::vec3 last_attractor_position;

    // Random asteroid models
    std::vector<mesh_drawable> asteroid_mesh_drawables;
    std::vector<MeshInstancesData> asteroid_instances_data; // For each mesh drawable
    std::vector<DistanceMeshHandler> distance_mesh_handlers;

    // Objects
    std::vector<Asteroid> asteroids; // Asteroid physical objects
    std::vector<cgp::vec3> asteroid_offsets;
    float orbit_factor; // Orbit acceleration factor in order to display faster orbits (for visual purposes)

    BeltPresets preset;

    // test : use a computing thread pool
    AsteroidThreadPool pool;
};