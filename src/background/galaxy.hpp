#pragma once

#include "environment.hpp"
#include "utils/display/drawable.hpp"

/**
 * Galaxy environment. Sphere that we are inside of.
 It must move along with the player !
*/
class Galaxy : public Drawable
{

public:
    // Constructor
    Galaxy();
    Galaxy(double radius);

    // Draw function
    virtual void initialize() override;
    virtual void draw(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

    // Setters
    virtual void setPosition(cgp::vec3 position) override;

    // Getter
    virtual cgp::vec3 getPosition() const override;

private:
    double radius;
    cgp::vec3 position;

    // CGP elements
    cgp::mesh galaxy_mesh;
    cgp::mesh_drawable galaxy_mesh_drawable;
};