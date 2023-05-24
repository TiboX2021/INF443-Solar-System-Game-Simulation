#include "asteroid_belt.hpp"
#include "celestial_bodies/asteroid_belt/asteroid_thread_pool.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/instancing/instancing.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include "utils/random/random.hpp"
#include "utils/shaders/shader_loader.hpp"
#include <cmath>
#include <iostream>

AsteroidBelt::AsteroidBelt(BeltPresets preset) : pool(4) // DEBUG : initialize pool with 4 threads
{
    this->preset = preset;
}

void AsteroidBelt::initialize()
{
    // ************************************************** //
    //               RANDOM ASTEROID GENERATION           //
    // ************************************************** //

    // Generate one random mesh per available texture. They will be reused and scaled using instancing
    const int n_base_asteroids = 3;
    std::string asteroid_textures[] = {"assets/asteroids/grey_asteroid.jpg", "assets/asteroids/grey_asteroid_2.png", "assets/asteroids/rocky_asteroid.jpg"};
    cgp::vec3 asteroid_mean_colors[] = {{102.0f / 255, 102.0f / 255, 102.0f / 255}, {84.0f / 255, 84.0f / 255, 84.0f / 255}, {132.0f / 255, 124.0f / 255, 116.0f / 255}};

    for (int i = 0; i < n_base_asteroids; i++)
    {
        // Generate high poly mesh
        cgp::mesh high_poly_asteroid_mesh = mesh_primitive_perlin_sphere(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, 50, 25, ASTEROID_NOISE_PARAMS);

        // Generate mesh drawable
        cgp::mesh_drawable high_poly_asteroid_mesh_drawable;
        high_poly_asteroid_mesh_drawable.initialize_data_on_gpu(high_poly_asteroid_mesh);

        // Set mesh drawable parameters
        // Add texture
        high_poly_asteroid_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + asteroid_textures[i],
                                                                                       GL_CLAMP_TO_EDGE,
                                                                                       GL_CLAMP_TO_EDGE);
        high_poly_asteroid_mesh_drawable.material.phong.specular = 0; // No shining reflection for the asteroid display
        high_poly_asteroid_mesh_drawable.shader = ShaderLoader::getShader("instanced");

        // Generate low poly mesh
        cgp::mesh low_poly_asteroid_mesh = mesh_primitive_perlin_sphere(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, 10, 5, ASTEROID_NOISE_PARAMS);

        // Generate mesh drawable
        cgp::mesh_drawable low_poly_asteroid_mesh_drawable;
        low_poly_asteroid_mesh_drawable.initialize_data_on_gpu(low_poly_asteroid_mesh);

        // Set mesh drawable parameters
        // Add texture
        low_poly_asteroid_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + asteroid_textures[i],
                                                                                      GL_CLAMP_TO_EDGE,
                                                                                      GL_CLAMP_TO_EDGE);
        low_poly_asteroid_mesh_drawable.material.phong.specular = 0; // No shining reflection for the asteroid display
        low_poly_asteroid_mesh_drawable.shader = ShaderLoader::getShader("instanced");

        // Generate low poly disk
        cgp::mesh low_poly_disk_mesh = cgp::mesh_primitive_disc(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, {0, 0, 1}, LOW_POLY_RESOLUTION);
        cgp::mesh_drawable low_poly_disk_mesh_drawable;
        low_poly_disk_mesh_drawable.initialize_data_on_gpu(low_poly_disk_mesh);
        low_poly_disk_mesh_drawable.material.phong.specular = 0; // No reflection for the low poly display
        low_poly_disk_mesh_drawable.material.color = asteroid_mean_colors[i];
        low_poly_disk_mesh_drawable.shader = ShaderLoader::getShader("instanced");

        // Add the asteroid mesh to the list
        asteroid_mesh_drawables.push_back(high_poly_asteroid_mesh_drawable);
        asteroid_mesh_drawables.push_back(low_poly_asteroid_mesh_drawable);
        asteroid_mesh_drawables.push_back(low_poly_disk_mesh_drawable);

        // Add the mesh data for each shader
        asteroid_instances_data.push_back({3 * i, 0, {}, {}, {}});
        asteroid_instances_data.push_back({3 * i + 1, 0, {}, {}, {}});
        asteroid_instances_data.push_back({3 * i + 2, 0, {}, {}, {}});

        // Add the mesh handler for the 3 meshes
        distance_mesh_handlers.push_back({3 * i, 3 * i + 1, 3 * i + 2});
    }

    int n_asteroids;

    switch (preset)
    {
    case BeltPresets::SATURN:
        n_asteroids = 2000;
        break;
    case BeltPresets::SUN:
        n_asteroids = 10000;
        break;
    case BeltPresets::KUIPER:
        n_asteroids = 10000;
        break;
    default:
        n_asteroids = 1000;
        break;
    }

    generateRandomAsteroids(n_asteroids);

    // Preallocate memory for the instancing
    for (auto &mesh_data : asteroid_instances_data)
    {
        mesh_data.allocate(n_asteroids);
    }
    last_attractor_position = attractors[0]->getPhysicsPosition();

    // Start pool
    pool.start();
}

void AsteroidBelt::generateRandomAsteroids(int n)
{
    cgp::mat3 rotation_matrix;
    double distance;
    double orbit_factor;
    double radius_min;
    double radius_max;
    float scale_min;
    float scale_max;

    // Load presets
    if (preset == BeltPresets::SATURN)
    {
        rotation_matrix = cgp::rotation_transform::from_vector_transform({0, 0, 1}, SATURN_ROTATION_AXIS).matrix();
        distance = DISTANCE;
        orbit_factor = ORBIT_FACTOR;
        radius_min = 0.8;
        radius_max = 1.2;
        scale_min = 0.2;
        scale_max = 1.8;
    }
    else if (preset == BeltPresets::SUN)
    {
        rotation_matrix = cgp::mat3::build_identity();
        distance = 3.0817e+11; // Main asteroid belt distance from the sun
        orbit_factor = 1;
        radius_min = 1;
        radius_max = 2;
        scale_min = 0.2;
        scale_max = 1.8;
    }
    else
    {
        // Kuiper belt
        rotation_matrix = cgp::mat3::build_identity();
        distance = 3.5e12;
        orbit_factor = 5; // The Kuiper belt is far away : accelerate its movement by 5
        radius_min = 0.95;
        radius_max = 1.05;
        scale_min = 1;
        scale_max = 5;
    }

    // Generate ateroids with random positions, and bind them to the meshes
    for (int i = 0; i < n; i++)
    {
        // Generate random position
        const float random_distance = distance * random_float(radius_min, radius_max);
        const cgp::vec3 random_position = random_orbit_position(random_distance) + random_normalized_axis() * random_distance / 30;

        // Generate object and its index to bind it to a mesh. How to do this? Linear scan ?
        Object asteroid(ASTEROID_MASS, rotation_matrix * random_position + attractors[0]->getPhysicsPosition(), random_normalized_axis());
        asteroid.setInitialRotationSpeed(SATURN_ROTATION_SPEED * random_float(0.4, 1.5));
        asteroid.setInitialVelocity(orbit_factor * rotation_matrix * Object::computeOrbitalSpeedForPosition(attractors[0]->getMass(), random_position));

        // Assign random mesh index
        int random_mesh_index = random_int(0, distance_mesh_handlers.size() - 1);

        Asteroid asteroid_instance = {asteroid, random_mesh_index, random_float(scale_min, scale_max)};

        asteroids.push_back(asteroid_instance);
    }
}

void AsteroidBelt::draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool)
{
    // Reset structs data
    for (auto &mesh_data : asteroid_instances_data)
    {
        mesh_data.resetData();
    }

    // Linear scan to build the data
    for (const auto &asteroid : asteroids)
    {
        // Compute the asteroid size to camera distance ratio. The higher, the lesser poly count is required
        float ratio = cgp::norm(Object::scaleDownDistanceForDisplay(asteroid.object.getPhysicsPosition()) - camera.camera_model.position()) / (asteroid.scale * ASTEROID_DISPLAY_RADIUS);

        int mesh_index;
        bool is_low_poly_disk = false;
        if (ratio < 100) // Maybe lower
        {
            mesh_index = distance_mesh_handlers[asteroid.mesh_index].high_poly;
        }
        else if (ratio < 200) // Maybe higher
        {
            mesh_index = distance_mesh_handlers[asteroid.mesh_index].low_poly;
        }
        else
        {
            mesh_index = distance_mesh_handlers[asteroid.mesh_index].low_poly_disk;
            is_low_poly_disk = true;
        }

        // Add data of this asteroid to the corresponding mesh instancing data
        asteroid_instances_data[mesh_index].addData(Object::scaleDownDistanceForDisplay(asteroid.object.getPhysicsPosition()), is_low_poly_disk ? camera.camera_model.orientation().matrix() : asteroid.object.getPhysicsRotation().matrix(), asteroid.scale);
    }

    // Call instanced drawing function for each dataset
    for (const auto &mesh_data : asteroid_instances_data)
    {
        draw_instanced(asteroid_mesh_drawables[mesh_data.mesh_index], environment, mesh_data.positions, mesh_data.rotations, mesh_data.scales, mesh_data.data_count);
    }
}

// Simulate gravitationnal attraction to the attractor
void AsteroidBelt::simulateStep(float step)
{
    cgp::vec3 delta_attractor_position = attractors[0]->getPhysicsPosition() - last_attractor_position;

    // Clear forces + update positions to match the main attractor
    for (auto &asteroid : asteroids)
    {
        asteroid.object.resetForces();
        asteroid.object.setPhysicsPosition(asteroid.object.getPhysicsPosition() + delta_attractor_position);
    }

    // Compute gravitationnal force to the attractor
    for (auto &asteroid : asteroids)
    {
        bool isMainAttractor = true;
        for (const auto &attractor : attractors)
        {
            asteroid.object.computeGravitationnalForce(attractor, (isMainAttractor && preset == BeltPresets::SATURN) ? ORBIT_FACTOR * ORBIT_FACTOR : 1);
            isMainAttractor = false;
        }
    }

    // Simulate step
    for (auto &asteroid : asteroids)
    {
        asteroid.object.update(step);
    }

    last_attractor_position = attractors[0]->getPhysicsPosition();
}