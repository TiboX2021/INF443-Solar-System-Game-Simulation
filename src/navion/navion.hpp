#pragma once

#include "cgp/cgp.hpp"

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::vec3;
using cgp::numarray;



class Navion 
{
public:
	void initialize();
	void draw(environment_structure const& environment);
	void set_position(vec3 const& position);
	void set_direction(vec3 const& direction);
	void set_angle_aile(float const& angle);
	

protected:

private:
	float nangle_aile;
	cgp::hierarchy_mesh_drawable hierarchie;


};

//void initialize_navion();
//void bouge_navion();
