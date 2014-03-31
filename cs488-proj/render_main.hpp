#ifndef RENDER_MAIN_HPP
#define RENDER_MAIN_HPP

#include <string>
#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"

void render(
            SceneNode* root,
            const std::string& filename,
            int width, int height,
            const Point3D& eye, const Vector3D& view,
            const Vector3D& up, double fov,
            const Colour& ambient,
            const std::list<Light*>& lights,
            size_t num_photon_recursion,
            double photon_search_radius,
            size_t num_search_photons,
            const std::string& mode,
            size_t num_threads,
            const std::string& type,
            double max_psi,
            size_t pixelSize
            );


#endif
