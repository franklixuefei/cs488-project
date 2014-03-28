#include "a4.hpp"
#include "image.hpp"
#include "renderer.hpp"

void basic_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights,
                const string& type
               )
{
  // Fill in raytracing code here.

    std::cerr << "Stub: basic_render(" << root << ",\n     filename:"
    << filename << ", width:" << width << ", height:" << height << ",\n     eye:"
    << eye << ", view:" << view << ", up:" << up << ", type:" << type  << ", fov:" << fov << ",\n     "
    << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;
  
  Renderer *renderer = new BasicRenderer(root, width, height, eye, view, up, fov, ambient, lights, type);

  Image img = renderer->render();
  
  img.savePng(filename);
  
  delete renderer;
}

void stochastic_render(// What to render
                  SceneNode* root,
                  // Where to output the image
                  const std::string& filename,
                  // Image size
                  int width, int height,
                  // Viewing parameters
                  const Point3D& eye, const Vector3D& view,
                  const Vector3D& up, double fov,
                  // Lighting parameters
                  const Colour& ambient,
                    const std::list<Light*>& lights,
                    const string& type,
                    size_t pixelSize
                  )
{
    // Fill in raytracing code here.
    
    std::cerr << "Stub: stochastic_render(root:" << root << ",\n     filename:"
    << filename << ", width:" << width << ", height:" << height << ",\n     eye:"
    << eye << ", view:" << view << ", up:" << up << ", type:" << type << ", pixelSize:" << pixelSize << ", fov:" << fov << ",\n     "
    << ambient << ",\n     {";
    
    for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
        if (I != lights.begin()) std::cerr << ", ";
        std::cerr << **I;
    }
    std::cerr << "});" << std::endl;
    
    Renderer *renderer = new StochasticRenderer(root, width, height, eye, view, up, fov, ambient, lights, type, pixelSize);
    
    Image img = renderer->render();
    
    img.savePng(filename);
    
    delete renderer;
}