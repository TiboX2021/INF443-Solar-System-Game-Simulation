#pragma once

#include "cgp/geometry/vec/vec3/vec3.hpp"
#include "environment.hpp"

/**
 * Abstract base drawable class for billboards
 */
class BillboardDrawable
{
    // Initialize member meshs
    virtual void initialize(){};

    virtual void setPosition(cgp::vec3 position) = 0;

    // Draw function
    virtual void drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) = 0;
};