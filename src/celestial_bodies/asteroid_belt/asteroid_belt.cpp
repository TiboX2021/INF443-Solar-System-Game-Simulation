#include "asteroid_belt.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "utils/instancing/instancing.hpp"
#include "utils/physics/constants.hpp"

AsteroidBelt::AsteroidBelt() : debugShadable(SUN_MASS, SUN_RADIUS / 10, {0, 0, 0}, "assets/planets/sun.jpg", NO_PERLIN_NOISE)
{
    // debugShadable = Star(SUN_MASS, SUN_RADIUS / 10, {0, 0, 0}, "assets/planets/sun.jpg", NO_PERLIN_NOISE);

    debugShadable.setShader("instanced");
}

void AsteroidBelt::initialize()
{
    // Initialize the planet
    debugShadable.initialize();
}

// TODO : use the instanced drawing function
void AsteroidBelt::draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe)
{
    // Draw the planet
    // debugShadable.draw(environment, camera, show_wireframe);

    // TODO : get the model mesh + test random positions
    // TODO : test avec les rotations
    // Generate rotations

    draw_instanced(debugShadable.getMeshDrawable(), environment, {{10, 0, 0}, {0, 0, 0}}, {cgp::rotation_transform::from_axis_angle({1, 0, 0}, 0).matrix(), cgp::rotation_transform::from_axis_angle({1, 0, 0}, 3.14 / 4).matrix()}, 2);
}