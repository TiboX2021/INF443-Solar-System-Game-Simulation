#include "cgp/graphics/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "cgp/graphics/drawable/triangles_drawable/triangles_drawable.hpp"
#include "cgp/core/base/basic_types/basic_types.hpp"
#include <cmath>
#include "reacteur.hpp"
#include "environment.hpp"

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::vec3;

void Reacteur::initialize() {
	
	feu = mesh_primitive_quadrangle({ -0.5f,0,0 }, { 0.5f,0,0 }, { 0.5f,0,1 }, { -0.5f,0,1 });
	feu_reacteur.initialize_data_on_gpu(feu);
	feu_reacteur.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/blue-flame-png-hd--1920.png");
	feu_reacteur.material.phong = { 0.4f, 0.6f,0,1 };
}

void Reacteur::display_reacteur(std::vector<vec3> const& positions, std::vector<rotation_transform> const& directions, environment_structure const& environment) {
	// Enable use of alpha component as color blending for transparent elements
	//  alpha = current_color.alpha
	//  new color = previous_color * alpha + current_color * (1-alpha)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Disable depth buffer writing
	//  - Transparent elements cannot use depth buffer
	//  - They are supposed to be display from furest to nearest elements
	glDepthMask(false);

	for (int k = 0; k < positions.size() ; k++) {
		feu_reacteur.model.translation = positions[k];
		feu_reacteur.model.rotation = directions[k];
		cgp::draw(feu_reacteur, environment);
	}






	// Don't forget to re-activate the depth-buffer write
	glDepthMask(true);
	glDisable(GL_BLEND);
}