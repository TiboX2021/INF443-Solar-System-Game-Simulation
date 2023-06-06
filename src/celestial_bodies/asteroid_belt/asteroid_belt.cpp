#include "asteroid_belt.hpp"
#include "celestial_bodies/asteroid_belt/asteroid_thread_pool.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "cgp/graphics/opengl/texture/texture.hpp"
#include "utils/display/display_constants.hpp"
#include "utils/display/low_poly.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/opengl/instancing.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include "utils/random/random.hpp"
#include "utils/shaders/shader_loader.hpp"
#include <cmath>
#include <iostream>

AsteroidBelt::AsteroidBelt(BeltPresets preset) : pool({})
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
    std::string normal_maps[] = {"assets/asteroids/normal_map_grey_asteroid.png", "assets/asteroids/normal_map_grey_asteroid_2.png", "assets/asteroids/normal_map_rocky_asteroid.png"};
    cgp::vec3 asteroid_mean_colors[] = {{102.0f / 255, 102.0f / 255, 102.0f / 255}, {84.0f / 255, 84.0f / 255, 84.0f / 255}, {132.0f / 255, 124.0f / 255, 116.0f / 255}};
    std::vector<DistanceMeshHandler> distance_mesh_handlers;

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
        // Add additionnal bump map texture (and get it in the frag shader, to sample it and get the normal)
        cgp::opengl_texture_image_structure normal_map;
        normal_map.load_and_initialize_texture_2d_on_gpu(project::path + normal_maps[i],
                                                         GL_CLAMP_TO_EDGE,
                                                         GL_CLAMP_TO_EDGE);
        high_poly_asteroid_mesh_drawable.supplementary_texture["normal_map"] = normal_map;

        high_poly_asteroid_mesh_drawable.material.phong.specular = 0; // No shining reflection for the asteroid display
        high_poly_asteroid_mesh_drawable.material.phong.ambient = ASTEROID_PHONG_AMBIENT;
        high_poly_asteroid_mesh_drawable.material.phong.diffuse = ASTEROID_PHONG_DIFFUSE;

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
        low_poly_asteroid_mesh_drawable.supplementary_texture["normal_map"] = normal_map;

        low_poly_asteroid_mesh_drawable.material.phong.specular = 0; // No shining reflection for the asteroid display
        low_poly_asteroid_mesh_drawable.material.phong.ambient = ASTEROID_PHONG_AMBIENT;
        low_poly_asteroid_mesh_drawable.material.phong.diffuse = ASTEROID_PHONG_DIFFUSE;

        low_poly_asteroid_mesh_drawable.shader = ShaderLoader::getShader("instanced");

        // Generate low poly disk
        cgp::mesh low_poly_disk_mesh = cgp::mesh_primitive_disc(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, {0, 0, 1}, LOW_POLY_RESOLUTION);
        cgp::mesh_drawable low_poly_disk_mesh_drawable;
        low_poly_disk_mesh_drawable.initialize_data_on_gpu(low_poly_disk_mesh);
        low_poly_disk_mesh_drawable.material.phong.specular = 0; // No reflection for the low poly display
        low_poly_asteroid_mesh_drawable.material.phong.ambient = ASTEROID_PHONG_AMBIENT;
        low_poly_asteroid_mesh_drawable.material.phong.diffuse = ASTEROID_PHONG_DIFFUSE;

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
        n_asteroids = 5000;
        orbit_factor = ORBIT_FACTOR;
        break;
    case BeltPresets::SUN:
        n_asteroids = 10000;
        orbit_factor = 3;
        break;
    case BeltPresets::KUIPER:
        n_asteroids = 100000; // Can go up to 200 000 with a beefy enough gpu
        orbit_factor = 10;    // The Kuiper belt is far away : accelerate its movement by 10
        break;
    default:
        n_asteroids = 1000;
        break;
    }

    std::vector<Asteroid> asteroids = generateRandomAsteroids(n_asteroids, distance_mesh_handlers);

    // Preallocate memory for the instancing
    for (auto &mesh_data : asteroid_instances_data)
    {
        mesh_data.allocate(n_asteroids);
    }

    // Initialize thread pool data
    pool.setAttractor(attractors[0]);
    pool.setDistanceMeshHandlers(distance_mesh_handlers);
    pool.loadAsteroids(asteroids);
    pool.setOrbitFactor(orbit_factor);
    pool.allocateBuffers();

    // Start pool
    pool.start();

    // Initialize instancing buffers
}

std::vector<Asteroid> AsteroidBelt::generateRandomAsteroids(int n, const std::vector<DistanceMeshHandler> &distance_mesh_handlers)
{
    std::vector<Asteroid> asteroids;
    ;
    cgp::mat3 rotation_matrix;
    double distance;
    double radius_std;
    float scale_min;
    float scale_max;
    float random_deviation_factor; // Position offset for the asteroid (fluffy asteroid belt)

    // Load presets
    if (preset == BeltPresets::SATURN)
    {
        rotation_matrix = cgp::rotation_transform::from_vector_transform({0, 0, 1}, SATURN_ROTATION_AXIS).matrix();
        distance = DISTANCE;
        radius_std = distance / 10;
        scale_min = 0.1;
        scale_max = 1;
        random_deviation_factor = 1.0f / 20;
    }
    else if (preset == BeltPresets::SUN)
    {
        rotation_matrix = cgp::mat3::build_identity();
        distance = 4.0817e+11; // Main asteroid belt distance from the sun
        radius_std = distance / 10;
        scale_min = 0.2;
        scale_max = 1.8;
        random_deviation_factor = 1.0f / 12;
    }
    else //  if (preset == BeltPresets::KUIPER)
    {
        // Kuiper belt
        rotation_matrix = cgp::mat3::build_identity();
        distance = 4e12;
        radius_std = distance / 8;
        scale_min = 1;
        scale_max = 5;
        random_deviation_factor = 1.0f / 15;
    }

    // Generate ateroids with random positions, and bind them to the meshes
    for (int i = 0; i < n; i++)
    {
        // Generate random position with gaussian distribution
        const float random_gaussian_distance = random_gaussian(distance, radius_std);
        const cgp::vec3 random_deviation = random_normalized_axis() * distance * random_deviation_factor;
        const cgp::vec3 random_position = random_orbit_position(random_gaussian_distance) + random_deviation;
        const cgp::vec3 asteroid_offset = rotation_matrix * cgp::vec3{0, 0, random_deviation.z};

        // Generate object and its index to bind it to a mesh. How to do this? Linear scan ?
        Object asteroid(ASTEROID_MASS, rotation_matrix * random_position + attractors[0]->getPhysicsPosition(), random_normalized_axis());
        asteroid.setInitialRotationSpeed(SATURN_ROTATION_SPEED * random_float(1, 2));
        asteroid.setVelocity(orbit_factor * rotation_matrix * Object::computeOrbitalSpeedForPosition(attractors[0]->getMass(), random_position));

        // Assign random mesh index
        int random_mesh_index = random_int(0, distance_mesh_handlers.size() - 1);

        Asteroid asteroid_instance = {asteroid, random_mesh_index, random_float(scale_min, scale_max), asteroid_offset};

        asteroids.push_back(asteroid_instance);
    }

    return asteroids;
}

void AsteroidBelt::draw(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &, bool)
{
    pool.updateCameraPosition(position); // Update camera position for the next iteration computation

    // Communicate with the threads to get the data.
    pool.swapBuffers();

    auto data_from_worker_threads = pool.getGPUData();
    pool.awaitAndLaunchNextFrameComputation(); // Unlock all threads in order to enable them to compute the next frame data into the buffer (not the one we just got)

    // Reset structs data
    for (auto &mesh_data : asteroid_instances_data)
    {
        mesh_data.resetData();
    }

    // Iterate with i in order to join GPU data and config data
    for (int i = 0; i < data_from_worker_threads.size(); i++)
    {
        // Remove if mesh was deactivated (= -1)
        if (data_from_worker_threads[i].mesh_index != -1)
            asteroid_instances_data[data_from_worker_threads[i].mesh_index].addData(data_from_worker_threads[i].position, data_from_worker_threads[i].rotation, data_from_worker_threads[i].scale);
    }

    // Call instanced drawing function for each dataset
    for (const auto &mesh_data : asteroid_instances_data)
    {
        bool is_low_poly = mesh_data.mesh_index % 3 == 2;
        // Note : no GL_DYNAMIC instancing, as for each mesh the data size can change between each frame
        draw_instanced(asteroid_mesh_drawables[mesh_data.mesh_index], environment, mesh_data.positions, mesh_data.rotations, mesh_data.scales, mesh_data.data_count, !is_low_poly);
    }
}
