/* JSB: Separated header file from main file of Jensen, 1991 PhotonMap class:
 Implementation in PhotonMap.cpp */

#ifndef PHOTONMAP_H
#define PHOTONMAP_H

/* This is the photon
 * The power is not compressed so the
 * size is 28 bytes
 */
//**********************
typedef struct Photon {
    //**********************
    double pos[3];                  // photon position
    short plane;                   // splitting plane for kd-tree
    unsigned char theta, phi;      // incoming direction
    double power[3];                // photon power (uncompressed)
} Photon;


/* This structure is used only to locate the
 * nearest photons
 */
//**********************
typedef struct NearestPhotons {
    //**********************
    int max;
    int found;
    int got_heap;
    double pos[3];
    double *dist2;
    const Photon **index;
} NearestPhotons;


/* This is the Photon_map class
 */
//****************
class PhotonMap {
    //****************
    
public:
    PhotonMap(int max_phot);
    ~PhotonMap();
    
    Photon* getPhotons(void);
    int get_stored_photons(void);
    
    void store(
               const double power[3],        // photon power
               const double pos[3],          // photon position
               const double dir[3] );        // photon direction
    
    void scale_photon_power(
                            const double scale);          // 1/(number of emitted photons)
    
    void balance(void);            // balance the kd-tree (before use!)
    
    void irradiance_estimate(
                             double irrad[3],              // returned irradiance
                             const double pos[3],          // surface position
                             const double normal[3],       // surface normal at pos
                             const double max_dist,        // max distance to look for photons
                             const int nphotons ) const;  // number of photons to use
    
    void locate_photons(
                        NearestPhotons *const np,    // np is used to locate the photons
                        const int index) const;      // call with index = 1
    
    void photon_dir(
                    double *dir,                  // direction of photon (returned)
                    const Photon *p) const;      // the photon
    
private:
    void balance_segment(
                         Photon **pbal,
                         Photon **porg,
                         const int index,
                         const int start,
                         const int end );
    
    void median_split(
                      Photon **p,
                      const int start,
                      const int end,
                      const int median, 
                      const int axis );
    
    Photon *photons; 
    
    int stored_photons; 
    int half_stored_photons; 
    int max_photons; 
    int prev_scale; 
    
    double costheta[256]; 
    double sintheta[256]; 
    double cosphi[256]; 
    double sinphi[256]; 
    
    double bbox_min[3];     // use bbox_min
    double bbox_max[3];     // use bbox_max
};

#endif