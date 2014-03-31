#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <list>
#include "scene.hpp"
#include "algebra.hpp"
#include "image.hpp"
#include "light.hpp"
#include <string>
#include "photon_map.hpp"
#include "camera.hpp"


class Renderer {
public:
	Renderer(
             SceneNode* root,
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
             size_t pixelSize=1 // basic renderer by default
	);

	virtual ~Renderer();

	Image render();
    virtual bool getColourForPixel(int x, int y, Colour &c)=0;
    void build_photon_map();
	void threaded_render(unsigned int start_row, size_t render_size);
	SceneNode *get_root() const;
    PhotonMap *getPhotonMap() const;
    double getPhotonSearchRadius() const;
    size_t getNumSearchPhotons() const;
    const string &getRenderMode() const;
    size_t getNumPhotonRecursion() const;
protected:
	Colour background(int x, int y);
    void trace_photons(Point3D orig, Vector3D dir, Colour power, double refl_id, size_t recur_depth);

protected:
 
    
	Image m_img;
    	Image m_bg_img;
    	SceneNode *m_root;
	int m_width;
	int m_height;
	Point3D m_eye;
	Vector3D m_view;
	Vector3D m_up;
	Vector3D m_left;
	double m_fov;
	double m_screen_distance;
	const Colour &m_ambient;
	const std::list<Light*>& m_lights;
    size_t m_photon_recursion;
    double m_photon_search_radius;
    size_t m_num_search_photons;
    const string &m_render_mode;
    size_t m_num_threads;
    const string &m_render_type;
    double m_max_psi;
    size_t m_pixel_size;
    Camera *m_pinhole_camera;
    Camera *m_fisheye_camera;
    PhotonMap *m_photon_map;
    
};

class BasicRenderer : public Renderer {
public:
    BasicRenderer(
                  SceneNode* root,
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
                  double max_psi
             );
    
	virtual ~BasicRenderer();
    bool getColourForPixel(int x, int y, Colour &c);
private:
    bool photon_intersect(const Vector3D &ray_dir, Colour &c);
};

class StochasticRenderer : public Renderer {
public:
    StochasticRenderer(
                       SceneNode* root,
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
    
	virtual ~StochasticRenderer();
    bool getColourForPixel(int x, int y, Colour &c);
private:
    size_t m_pixel_size;
};


struct Args {
  Renderer* renderer;
  unsigned int start_row;
  size_t render_size;
};

#endif