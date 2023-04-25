#pragma once

// TODO : class with arrays of drawables and arrays of objects, that handles everything
// TODO: faire une méthode statique qui génère le système solaire

#include "utils/display/base_drawable.hpp"
#include "utils/display/billboard_drawable.hpp"
#include "utils/display/drawable.hpp"
class SimulationHandler
{

public:
private:
    // Drawable objects
    // Store all drawable instances here
    std::vector<BaseDrawable> drawables;

    // Store pointers (references) here for efficient access
    std::vector<Drawable *> drawable_objects;
    std::vector<BillboardDrawable *> billboard_drawable_objects;
};