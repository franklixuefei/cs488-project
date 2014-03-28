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

#define INIT_NUM_PHOTONS (500000) // 50000000 will cause my computer to die.
#define MAX_PHOTON_RECURSION (5)
#define MAX_NUM_PHOTONS (INIT_NUM_PHOTONS * MAX_PHOTON_RECURSION)

class Renderer {
public:
	Renderer(
		SceneNode* root,
		// Image size
		int width, int height,
		// Viewing parameters
		const Point3D& eye, const Vector3D& view,
		const Vector3D& up, double fov,
		// Lighting parameters
		const Colour& ambient,
		const std::list<Light*>& lights,
        string type,
        size_t pixelSize=1 // basic renderer by default
	);

	virtual ~Renderer();

	Image render();
    virtual bool getColourForPixel(int x, int y, Colour &c)=0;
    void build_photon_map();
	void threaded_render(unsigned int start_row, size_t render_size);
	SceneNode *get_root() const;
protected:
	Colour background(int x, int y);
    void trace_photons(Point3D orig, Vector3D dir, Colour power, double refl_id, size_t recur_depth);
    Vector3D calc_diffuse_reflection(const Vector3D &n);
    
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
    
    PhotonMap *m_photon_map;
    Camera *m_pinhole_camera;
    Camera *m_fisheye_camera;
};

class BasicRenderer : public Renderer {
public:
    BasicRenderer(
             SceneNode* root,
             // Image size
             int width, int height,
             // Viewing parameters
             const Point3D& eye, const Vector3D& view,
             const Vector3D& up, double fov,
             // Lighting parameters
             const Colour& ambient,
                  const std::list<Light*>& lights,
                  const string& type
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