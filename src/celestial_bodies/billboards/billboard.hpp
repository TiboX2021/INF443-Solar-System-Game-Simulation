#pragma once

// TODO : billboards that always face the camera (gas clouds, halo with a picture ?)

#include "cgp/geometry/shape/mesh/structure/mesh.hpp"
#include "utils/display/billboard_drawable.hpp"
class Billboard : public BillboardDrawable
{

public:
    Billboard(std::string texture_path, float radius, cgp::vec3 position = {0, 0, 0}) : texture_path(texture_path), radius(radius), position(position){};

    virtual void initialize() override;

    virtual void setPosition(cgp::vec3 position) override
    {
        this->position = position;
        billboard_mesh_drawable.model.translation = position;
    };

    virtual void drawBillboards(environment_structure const &environment, camera_controller_orbit_euler const &camera, bool show_wireframe = true) override;

private:
    std::string texture_path;

    float radius; // Billboard radius

    cgp::vec3 position; // Is it the physics position ?

    // Meshes
    cgp::mesh billboard_mesh;
    cgp::mesh_drawable billboard_mesh_drawable;
};