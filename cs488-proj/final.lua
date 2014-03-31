-- textrue mapped version

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10, 10000000)
mat2 = gr.material({0.5, 1, 0.5}, {0.5, 0.7, 0.5}, 25, 10000000)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50, 10000000)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25, 10000000)
mirror = gr.material({0, 0, 0}, {1, 1, 1}, 100, 10000000)
glass = gr.material({0,0,1}, {1,1,1}, 100, 1.5)
water = gr.material({0,0,0}, {1,1,1}, 100, 1.33)

texture1 = gr.texture_material('checker.png', {0.7, 0.7, 0.7}, 25, 10000000)

scene = gr.node('scene')

cornell_box = gr.cornell_box(5,5)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(water)
b1:translate(0.3,0.5,4)
b1:rotate('X', 20)
b1:rotate('Y', 20)
b1:scale(1.2,1.2,1.2)

s4 = gr.sphere('s4')
bn:add_child(s4)
s4:set_material(glass)
s4:translate(-1.2, -1.8, 4)
s4:rotate('X', 90)
s4:scale(0.7,0.7,0.7)

s5 = gr.sphere('s5')
bn:add_child(s5)
s5:set_material(mirror)
s5:translate(0.6, -1.8, 2.1)
s5:rotate('X', 90)
s5:scale(0.7,0.7,0.7)

c0 = gr.cone('c0');
bn:add_child(c0)
c0:set_material(mirror)
c0:translate(-0.5, 1,2)
c0:rotate('X', 90)
c0:scale(0.5, 0.5, 1.5)

c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(mat2)
c2:translate(-1.33, -0.4, 2.33)
c2:rotate('Y', -40)
c2:scale(0.33, 0.33, 1)

t1 = gr.torus('t1', 0.167, 0.5);
bn:add_child(t1)
t1:set_material(mat3)
t1:translate(0.5, 1, 2.3)
t1:rotate('X', 45)

-- render mode can be 'all', 'photon map'(only show photon-mapped image), 'ray tracing' (only show ray-traced image)
-- eye type can be 'pinhole', 'fisheye'

gr.render(scene,
      'scene.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.45, 3.167}, {1, 1, 1}, {1, 0, 0}, 500000, 64, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), size(if non point light).
      }, 3, 0.1, 800, 'photon map', 32, 'pinhole', 180.0, 1) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering
