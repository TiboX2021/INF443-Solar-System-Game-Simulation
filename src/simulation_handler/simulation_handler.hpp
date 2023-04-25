#pragma once

// TODO : class with arrays of drawables and arrays of objects, that handles everything
// TODO: faire une méthode statique qui génère le système solaire

#include "celestial_bodies/ring_planet/ring_planet.hpp"
#include "utils/display/drawable.hpp"
class SimulationHandler
{

private:
    Drawable *drawables; // TODO : clear drawables in the destructor

    // To draw their billboards ?
    RingPlanet *ring_planets;
};