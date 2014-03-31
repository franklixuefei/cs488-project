#include "render_main.hpp"
#include "image.hpp"
#include "renderer.hpp"
#include <cassert>
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
                  )
{
    std::cerr << "Stub: render(\n\tSceneNode root: " << root << ",\n\tfilename: "
    << filename << ",\n\timage width: " << width << "\n\timage height:" << height << ",\n\teye position:"
    << eye << "\n\tview direction:" << view << "\n\tup vector: " << up << ",\n\tfov: " << fov << ",\n\tambient: " << ambient << ",\n\tlights: {";
    for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
        std::cerr << "\t\t" << **I;
    }
    std::cerr << "},\n\tnum photon recursions: " << num_photon_recursion << ",\n\tphoton search radius: " << photon_search_radius << ",\n\tnum search photons: " << num_search_photons << ",\n\trender mode: " << mode << ",\n\tnum threads: " << num_threads << ",\n\trender type: " << type << ",\n\t max psi angle: " << max_psi << ",\n\tpixel size: " << pixelSize << ");" << endl;
    
    // sanity check
    assert(root != NULL);
    assert(filename != "");
    assert(width > 0 && height > 0);
    assert(lights.size() > 0);
    assert(type == "pinhole" || type == "fisheye");
    assert(pixelSize >= 1);
    
    Renderer *renderer = NULL;
    if (pixelSize == 1) {
        renderer = new BasicRenderer(root, width, height, eye, view, up, fov, ambient, lights, num_photon_recursion, photon_search_radius, num_search_photons, mode, num_threads, type, max_psi);
    } else {
        renderer = new StochasticRenderer(root, width, height, eye, view, up, fov, ambient, lights, num_photon_recursion, photon_search_radius, num_search_photons, mode, num_threads, type, max_psi, pixelSize);
    }
    Image img = renderer->render();
    
    img.savePng(filename);
    
    delete renderer;
}

