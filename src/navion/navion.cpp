#include "cgp/graphics/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "cgp/graphics/drawable/triangles_drawable/triangles_drawable.hpp"
#include "cgp/core/base/basic_types/basic_types.hpp"
#include <cmath>
#include "navion.hpp"
#include "environment.hpp"

void Navion::initialize() {
	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable corps;
	mesh_drawable cocpit;
	mesh_drawable vitre_cocpit;
	mesh_drawable aile_d;
	mesh_drawable aile_g;
	mesh_drawable reacteur;
	mesh_drawable lance_missile;

	


	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	corps.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.35, { -0.5,0,0 }, { 0.5,0,0 }));
	aile_d.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle({ 0.2,0,0 }, { 0.1, 1.5,0 }, { -0.1, 1.5,0 }, { -0.2, 0,0 }));
	aile_g.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle({ -0.2, 0, 0 }, { -0.1, -1.5,0 }, { 0.1, -1.5,0 }, { 0.2,0,0 }));
	reacteur.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.05, { -0.2,0,0 }, { 0.1,0,0 }));
	//cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.35, 0.5,{0,0,0}, {1,0,0}));
	cocpit.initialize_data_on_gpu(create_cocpit_coque(0.35,0.5));
	vitre_cocpit.initialize_data_on_gpu(cocpit_vitre(0.35, 0.5));
	lance_missile.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.05, 0.07, { 0,0,0 }, { 1,0,0 }));

	

	// Set the color of some elements
	vec3 blanc = { 0.8, 0.8, 0.8 };
	corps.material.color = blanc;
	aile_d.material.color = blanc;
	aile_g.material.color = blanc;
	reacteur.material.color = blanc;
	vitre_cocpit.material.color = { 0.1,0.1,0.1 };
	lance_missile.material.color = blanc;

	corps.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);

	cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);

	aile_d.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_g.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	



	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.

	hierarchie.add(corps, "Corps");
	hierarchie.add(cocpit, "Cocpit", "Corps", { 0.5,0,0 });
	hierarchie.add(vitre_cocpit, "Cocpit_Vitre", "Corps", { 0.5,0,0 });
	hierarchie.add(aile_d, "AileDH", "Corps", {0,0.3,0.2});
	hierarchie.add(aile_d, "AileDB", "Corps", { 0,0.3,-0.2 });
	hierarchie.add(aile_g, "AileGH", "Corps", { 0,-0.3,0.2 });
	hierarchie.add(aile_g, "AileGB", "Corps", { 0,-0.3,-0.2 });
	hierarchie.add(reacteur, "reacteurDH", "AileDH", { 0, 1.5,0 });
	hierarchie.add(reacteur, "reacteurGH", "AileGH", { 0, -1.5,0 });
	hierarchie.add(reacteur, "reacteurDB", "AileDB", { 0, 1.5,0 });
	hierarchie.add(reacteur, "reacteurGB", "AileGB", { 0, -1.5,0 });
	hierarchie.add(lance_missile, "LM_DH", "reacteurDH", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_GH", "reacteurGH", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_DB", "reacteurDB", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_GB", "reacteurGB", { 0.1,0,0 });

	

	//return hierarchie;
}


void Navion::draw(environment_structure const& environment) {

	// Update position/angle

	
	hierarchie["AileDH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, nangle_aile);
	hierarchie["AileGH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, - nangle_aile);
	hierarchie["AileDB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, - nangle_aile);
	hierarchie["AileGB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 },  nangle_aile);
	


	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchie.update_local_to_global_coordinates();

	//Draw the hierarchy as a single mesh
	cgp::draw(hierarchie, environment);
}



void Navion::set_position(vec3 const& position) {
	hierarchie["global_frame"].transform_local.translation = position;
}

void Navion::set_direction(vec3 const& direction) {
	//******************** A FAIRE ***********************
}

void Navion::set_angle_aile(float const angle) {
	nangle_aile = angle;
}





mesh Navion::create_cocpit_coque(float const& radius, float const& length) {
	
	mesh semi_cone;
	int N = 20;

	for (int k = 0; k < N; ++k)
	{
		float u = k / (N - 1.0f)/2;
		vec3 p = radius * vec3(0.0f, - std::cos(2 * Pi * u), - std::sin(2 * Pi * u));
		semi_cone.position.push_back(p);
		semi_cone.uv.push_back({ 0.5 + std::cos(2 * Pi * u) * 0.43, 0.5 + std::sin(2 * Pi * u) * 0.43 });

	}
	// middle point
	semi_cone.position.push_back({ length, 0,0 });
	semi_cone.uv.push_back({ 0.5,0.5 });

	for (int k = 0; k < N - 1; ++k)
		semi_cone.connectivity.push_back(uint3{ N, k, k + 1 });

	semi_cone.fill_empty_field();
	return semi_cone;
}

mesh Navion::cocpit_vitre(float const& radius, float const& length) {
	mesh semi_cone;
	int N = 5;

	for (int k = 0; k < N; ++k)
	{
		float u = k / (N - 1.0f) / 2;
		vec3 p = radius * vec3(0.0f, std::cos(2 * Pi * u), std::sin(2 * Pi * u));
		semi_cone.position.push_back(p);
		semi_cone.uv.push_back({ 0.5 + std::cos(2 * Pi * u) * 0.43, 0.5 + std::sin(2 * Pi * u) * 0.43 });

	}
	// middle point
	semi_cone.position.push_back({ length, 0,0 });
	semi_cone.uv.push_back({ 0.5,0.5 });

	for (int k = 0; k < N - 1; ++k)
		semi_cone.connectivity.push_back(uint3{ N, k, k + 1 });

	semi_cone.fill_empty_field();
	return semi_cone;
}


