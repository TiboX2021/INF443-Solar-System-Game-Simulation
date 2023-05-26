#pragma once

// TODO : some sort of halo (transparent thing, so billboard drawable) that can be bound to an object in order to follow its position
// Try sphere with the alpha coeff, etc...
// First try : with a sphere

#include "utils/display/billboard_drawable.hpp"
#include "utils/physics/object.hpp"
class Halo : public BillboardDrawable
{

public:
    void setObject(Object *object)
    {
        this->object = object;
    }

    virtual void initialize() override;

    virtual void setPosition(cgp::vec3 position) override;

    virtual void drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

private:
    Object *object = nullptr; // The object that must be followed

    cgp::vec3 position; // Is it the physics position ?
};