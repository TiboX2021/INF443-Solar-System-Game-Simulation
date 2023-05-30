#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include <vector>

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::vec3;

class Reacteur 
{
public :
	void initialize(float const &scale = 1.0);
	void display_reacteur(std::vector<vec3> const &positions, std::vector<rotation_transform> const& directions, environment_structure const& environment);

protected :
	mesh feu;
	mesh_drawable feu_reacteur;


private:

};