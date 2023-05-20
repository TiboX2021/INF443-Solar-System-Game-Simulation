#include "asteroid_belt.hpp"
#include "cgp/core/containers/matrix_stack/special_types/definition/special_types.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "cgp/graphics/drawable/mesh_drawable/mesh_drawable.hpp"
#include "utils/instancing/instancing.hpp"
#include "utils/noise/perlin.hpp"
#include "utils/physics/constants.hpp"
#include "utils/physics/object.hpp"
#include "utils/random/random.hpp"
#include "utils/shaders/shader_loader.hpp"
#include <iostream>

AsteroidBelt::AsteroidBelt() : debugShadable(SUN_MASS, SUN_RADIUS / 10, {0, 0, 0}, "assets/planets/sun.jpg", NO_PERLIN_NOISE)
{
    debugShadable.setShader("instanced");
}

void AsteroidBelt::initialize()
{
    // Initialize the planet
    debugShadable.initialize();

    // ************************************************** //
    //               RANDOM ASTEROID GENERATION           //
    // ************************************************** //

    // Generate one random mesh per available texture. They will be reused and scaled using instancing
    const int n_base_asteroids = 3;
    std::string asteroid_textures[] = {"assets/asteroids/grey_asteroid.jpg", "assets/asteroids/grey_asteroid_2.png", "assets/asteroids/rocky_asteroid.jpg"};
    cgp::vec3 asteroid_mean_colors[] = {{102.0f / 255, 102.0f / 255, 102.0f / 255}, {84.0f / 255, 84.0f / 255, 84.0f / 255}, {132.0f / 255, 124.0f / 255, 116.0f / 255}};

    for (int i = 0; i < n_base_asteroids; i++)
    {
        // Generate mesh
        // TODO : use smaller meshes to improve performance ?
        cgp::mesh asteroid_mesh = mesh_primitive_perlin_sphere(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, 50, 25, ASTEROID_NOISE_PARAMS);

        // Generate mesh drawable
        cgp::mesh_drawable asteroid_mesh_drawable;
        asteroid_mesh_drawable.initialize_data_on_gpu(asteroid_mesh);

        // Set mesh drawable parameters
        // Add texture
        asteroid_mesh_drawable.texture.load_and_initialize_texture_2d_on_gpu(project::path + asteroid_textures[i],
                                                                             GL_CLAMP_TO_EDGE,
                                                                             GL_CLAMP_TO_EDGE);
        asteroid_mesh_drawable.material.phong.specular = 0; // No shining reflection for the asteroid display
        asteroid_mesh_drawable.shader = ShaderLoader::getShader("instanced");

        // Generate low poly mesh
        cgp::mesh low_poly_mesh = cgp::mesh_primitive_disc(ASTEROID_DISPLAY_RADIUS, {0, 0, 0}, {0, 0, 1}, LOW_POLY_RESOLUTION);
        cgp::mesh_drawable low_poly_drawable;
        low_poly_drawable.initialize_data_on_gpu(low_poly_mesh);
        low_poly_drawable.material.phong.specular = 0; // No reflection for the low poly display
        low_poly_drawable.material.color = asteroid_mean_colors[i];

        // Add the asteroid mesh to the list
        asteroid_mesh_drawables.push_back(asteroid_mesh_drawable);
        low_poly_asteroid_mesh_drawables.push_back(low_poly_drawable);
    }

    generateRandomAsteroids(100); // TODO : generate random asteroids to display
}

void AsteroidBelt::generateRandomAsteroids(int n)
{
    // Generate ateroids with random positions, and bind them to the meshes
    for (int i = 0; i < n; i++)
    {
        // Use the attractor object
        // Generate random position
        const float random_distance = DISTANCE * random_float(0.8, 1.2);
        const cgp::vec3 random_position = random_orbit_position(random_distance) + random_normalized_axis() * cgp::norm(random_position) / 30;

        // Generate object and its index to bind it to a mesh. How to do this? Linear scan ?
        Object asteroid(ASTEROID_MASS, random_position, random_normalized_axis());
        asteroid.setInitialRotationSpeed(SATURN_ROTATION_SPEED / 100 * random_float(0.4, 1.5));
        asteroid.setInitialVelocity(Object::computeOrbitalSpeedForPosition(attractor->getMass(), random_position));

        // std::cout << "Generated asteroid at position " << asteroid.getPhysicsPosition() << std::endl;
        // std::cout << Object::computeOrbitalSpeedForPosition(attractor->getMass(), random_position) << std::endl;

        // TODO : how to bind the texture in order to reuse instancing??

        objects.push_back(asteroid);
    }
}

// TODO : use the instanced drawing function
void AsteroidBelt::draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Draw the planet
    // debugShadable.draw(environment, camera, show_wireframe);

    // TODO : get the model mesh + test random positions
    // TODO : test avec les rotations
    // TODO : compute low polys and all
    // Generate rotations

    // draw_instanced(debugShadable.getMeshDrawable(), environment, {{10, 0, 0}, {0, 0, 0}}, {cgp::rotation_transform::from_axis_angle({1, 0, 0}, 0).matrix(), cgp::rotation_transform::from_axis_angle({1, 0, 0}, 3.14 / 4).matrix()}, 2);

    // TODO : debug draw all asteroids with the first mesh

    // std::cout << "Drawing " << objects.size() << " asteroids at positions" << std::endl;
    //   BUG : positions are nan???? Why??? Where do they become nan? We are almost there

    std::vector<cgp::vec3> positions;
    for (const auto &object : objects)
    {
        positions.push_back(Object::scaleDownDistanceForDisplay(object.getPhysicsPosition()));
        // std::cout << object.getPhysicsPosition() << std::endl;

        // std::cout << Object::scaleDownDistanceForDisplay(object.getPhysicsPosition()) << std::endl;
    }

    // for (auto position : positions)
    // {
    //     std::cout << position << std::endl;
    // }

    std::vector<cgp::mat3> rotations;
    for (const auto &object : objects)
    {
        rotations.push_back(object.getPhysicsRotation().matrix());
    }

    // TODO : generate arrays for instancing
    draw_instanced(asteroid_mesh_drawables[0], environment, positions, rotations, objects.size());
}

// Simulate gravitationnal attraction to the attractor
void AsteroidBelt::simulateStep()
{
    // Clear forces
    for (auto &object : objects)
    {
        object.resetForces();
        // BUG : ici, les positions sont devenues -inf, puis nan
        // Ça se fait avant la première simulation
        // std::cout << object.getPhysicsPosition() << std::endl;
    }

    // Compute gravitationnal force to the attractor
    for (auto &object : objects)
    {
        object.computeGravitationnalForce(attractor);
    }

    // Simulate step
    for (auto &object : objects)
    {
        object.update(24.0f * 3600 / 60 * 100); // TODO : set the time step
    }
}