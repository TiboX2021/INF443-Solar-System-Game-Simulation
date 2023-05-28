#pragma once

// TODO : billboards that always face the camera (gas clouds, halo with a picture ?)
// TODO : get a texture
// TODO : change the background color when going through them ?

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

    virtual void drawBillboards(environment_structure const &environment, cgp::vec3 &position, cgp::rotation_transform &rotation, bool show_wireframe = true) override;

    // Setters
    void setFaceCamera(bool faceCamera) { this->faceCamera = faceCamera; };

private:
    std::string texture_path;

    float radius; // Billboard radius

    bool faceCamera = false; // Billboard always faces the camera

    cgp::vec3 position; // Is it the physics position ?

    // Meshes
    cgp::mesh billboard_mesh;
    cgp::mesh_drawable billboard_mesh_drawable;
};