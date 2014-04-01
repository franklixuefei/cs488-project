mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25)

scene = gr.node('scene')

cornell_box = gr.cornell_box(300,300)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(mat2)
b1:translate(20,-30,250)
b1:rotate('X', 20)
b1:rotate('Y', 20)
b1:scale(60,60,60)

s4 = gr.nh_sphere('s4', {-50, -10, 160}, 40)
bn:add_child(s4)
s4:set_material(mat3)


c1 = gr.cone('c1');
bn:add_child(c1)
c1:set_material(mat4)
c1:translate(0, -140, 200)
c1:rotate('X', -90)
c1:scale(200, 200, 80)

c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(mat1)
c2:translate(-80, 0, 200)
c2:rotate('Y', -40)
c2:scale(20, 20, 60)

t1 = gr.torus('t1', 10, 30);
bn:add_child(t1)
t1:set_material(mat3)
t1:translate(30, 60, 250)
t1:rotate('X', 45)

gr.render(scene,
	  'scene.png', 600, 600,
	  {0, 0, 600}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {
        gr.light({0, 150, 600}, {0.7, 0.7, 0.7}, {1, 0, 0})
      }, 'pinhole')




-- =================

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25)

scene = gr.node('scene')

cornell_box = gr.cornell_box(300,300)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(mat2)
b1:translate(20,-30,50)
b1:rotate('X', 20)
b1:rotate('Y', 20)
b1:scale(60,60,60)

s4 = gr.nh_sphere('s4', {-50, -10, 60}, 40)
bn:add_child(s4)
s4:set_material(mat3)


c1 = gr.cone('c1');
bn:add_child(c1)
c1:set_material(mat4)
c1:translate(0, -140, 10)
c1:rotate('X', -90)
c1:scale(200, 200, 80)

c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(mat1)
c2:translate(-80, 0, 20)
c2:rotate('Y', -40)
c2:scale(20, 20, 60)

t1 = gr.torus('t1', 10, 30);
bn:add_child(t1)
t1:set_material(mat3)
t1:translate(30, 60, 40)
t1:rotate('X', 45)

gr.render(scene,
	  'scene.png', 600, 600,
	  {0, 0, 100}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {
        gr.light({0, 150, 600}, {0.7, 0.7, 0.7}, {1, 0, 0})
      }, 'fisheye')




-- textrue mapped version

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25)

texture1 = gr.texture_material('checker.png', {0.7, 0.7, 0.7}, 25)

scene = gr.node('scene')

cornell_box = gr.cornell_box(300,300)
scene:add_child(cornell_box)
cornell_box:set_material(texture1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(texture1)
b1:translate(20,-30,250)
b1:rotate('X', 20)
b1:rotate('Y', 20)
b1:scale(60,60,60)

s4 = gr.sphere('s4')
bn:add_child(s4)
s4:set_material(texture1)
s4:translate(-50, -10, 160)
s4:rotate('X', 90)
s4:scale(40,40,40)

c0 = gr.cone('c0');
bn:add_child(c0)
c0:set_material(texture1)
c0:translate(-50, 100, 250)
c0:rotate('X', 70)
c0:scale(20, 20, 80)

c1 = gr.cone('c1');
bn:add_child(c1)
c1:set_material(texture1)
c1:translate(0, -140, 200)
c1:rotate('X', -90)

c1:scale(200, 200, 80)

c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(texture1)
c2:translate(-80, 0, 200)
c2:rotate('Y', -40)
c2:scale(20, 20, 60)

t1 = gr.torus('t1', 10, 30);
bn:add_child(t1)
t1:set_material(texture1)
t1:translate(30, 60, 250)
t1:rotate('X', 45)

gr.render(scene,
      'scene.png', 600, 600,
      {0, 0, 600}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 150, 600}, {0.7, 0.7, 0.7}, {1, 0, 0})
      }, 'pinhole')


-- scaled version
-- textrue mapped version

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25)

texture1 = gr.texture_material('checker.png', {0.7, 0.7, 0.7}, 25)

scene = gr.node('scene')

cornell_box = gr.cornell_box(5,5)
scene:add_child(cornell_box)
cornell_box:set_material(texture1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(texture1)
b1:translate(0.3,-0.5,4)
b1:rotate('X', 20)
b1:rotate('Y', 20)
b1:scale(1.2,1.2,1.2)

s4 = gr.sphere('s4')
bn:add_child(s4)
s4:set_material(texture1)
s4:translate(-0.8, -0.17, 2.67)
s4:rotate('X', 90)
s4:scale(0.7,0.7,0.7)

c0 = gr.cone('c0');
bn:add_child(c0)
c0:set_material(texture1)
c0:translate(-0.8, 1.67, 4.17)
c0:rotate('X', 70)
c0:scale(0.33, 0.33, 1.33)

c1 = gr.cone('c1');
bn:add_child(c1)
c1:set_material(texture1)
c1:translate(0, -2.33, 3.33)
c1:rotate('X', -90)

c1:scale(3.33, 3.33, 1.33)

c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(texture1)
c2:translate(-1.33, 0, 3.33)
c2:rotate('Y', -40)
c2:scale(0.33, 0.33, 1)

t1 = gr.torus('t1', 0.167, 0.5);
bn:add_child(t1)
t1:set_material(texture1)
t1:translate(0.5, 1, 4.167)
t1:rotate('X', 45)

gr.render(scene,
      'scene.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.5, 10}, {0.7, 0.7, 0.7}, {1, 0, 0})
      }, 'pinhole')


------ refraction scene
-- textrue mapped version

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 10, 10000000)
mat2 = gr.material({0.5, 1, 0.5}, {0.5, 0.7, 0.5}, 25, 10000000)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 50, 10000000)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, 25, 10000000)
mirror = gr.material({0, 0, 0}, {1, 1, 1}, 100, 10000000)
glass = gr.material({0,0,0}, {1,1,1}, 100, 1.5)
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
s4:translate(-0.8, -0.17, 4.8)
s4:rotate('X', 90)
s4:scale(0.7,0.7,0.7)

c0 = gr.cone('c0');
bn:add_child(c0)
c0:set_material(mat3)
c0:translate(-0.8, 1.67, 2.17)
c0:rotate('X', 70)
c0:scale(0.33, 0.33, 1.33)



c2 = gr.cylinder('c2');
bn:add_child(c2)
c2:set_material(mat2)
c2:translate(-1.33, 0, 2.33)
c2:rotate('Y', -40)
c2:scale(0.33, 0.33, 1)

t1 = gr.torus('t1', 0.167, 0.5);
bn:add_child(t1)
t1:set_material(mat3)
t1:translate(0.5, 1, 4.167)
t1:rotate('X', 45)

gr.render(scene,
      'scene.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.48, 4}, {0.7, 0.7, 0.7}, {1, 0, 0})
      }, 'pinhole')




---------------------- scene.lua --------------------------

-- textrue mapped version

mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, {0,0,0}, 10, 10000000, '')
mat2 = gr.material({0.5, 1, 0.5}, {0, 0, 0}, {0,0,0}, 25, 10000000, '')
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, {0,0,0}, 50, 10000000, '')
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, {0,0,0}, 25, 10000000, '')
mirror = gr.material({0, 0, 0}, {1, 1, 1}, {0,0,0}, 100, 10000000, '')
glass = gr.material({0,0,0}, {1,1,1}, {1,1,1} ,100, 1.5, '') -- kd, ks, the third param is transmitted color, shininess, refraction index
water = gr.material({0,0,0}, {1,1,1}, {1,1,1}, 100, 1.33, '')

texture1 = gr.texture_material('checker.png', {0.7, 0.7, 0.7}, {0,0,0}, 25, 10000000, '')

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
        gr.light({0, 2.48, 3.167}, {0.7, 0.7, 0.7}, {1, 0, 0}, 70000000, 64, 1, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), power bias, size(if non point light).
      }, 3, 0.1, 1000, 'ray tracing', 32, 'pinhole', 180.0, 1) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering








