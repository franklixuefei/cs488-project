require('readobj')

mat1 = gr.material({0.6, 1.0, 0.6}, {0, 0, 0}, {0,0,0}, 10, 10000000, '')
mat2 = gr.material({0.5, 1, 0.5}, {0, 0, 0}, {0,0,0}, 25, 10000000, '')
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, {0,0,0}, 64, 10000000, '')
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, {0,0,0}, 25, 10000000, '')
mat5 = gr.material({0.1, 0.7, 1.0}, {0.6, 0.6, 0.6}, {0,0,0}, 40, 10000000, '')
light_blue_shiny = gr.material({0, 255.0/255.0, 1.0}, {0.7, 0.7, 0.7}, {0,0,0}, 70, 10000000, '')
mirror = gr.material({0, 0, 0}, {1, 1, 1}, {0,0,0}, 100, 10000000, '')
glass = gr.material({0,0,0}, {1,1,1}, {1,1,1} ,100, 1.5, '') -- kd, ks, the third param is transmitted color, shininess, refraction index

bump1 = gr.material({0,1,1},{0.3,0.3,0.3},{0,0,0}, 60, 10000000, 'bump1.png')
bump2 = gr.material({0.7, 0.6, 1.0},{0.3,0.3,0.3},{0,0,0}, 60, 10000000, 'bump2.png')
bump3 = gr.material({0.5, 1, 0.5},{0.3,0.3,0.3},{0,0,0}, 60, 10000000, 'bump3.png')

texture1 = gr.texture_material('checker.png', {0.2, 0.2, 0.2}, {0,0,0}, 25, 10000000, '')
wooden = gr.texture_material('wooden.png', {0,0,0}, {0,0,0}, 50, 1000000, '')

clock_texture = gr.texture_material('clock.png', {0.5,0.7,0.5}, {0,0,0}, 54, 10000000, '')


scene = gr.node('scene')

cornell_box = gr.cornell_box(5,5)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

bn = gr.node('bn')
scene:add_child(bn)

-- cone

cone_1 = gr.cone('cone_1')
bn:add_child(cone_1)
cone_1:set_material(bump1)
cone_1:translate(-75.0/60.0, 90.0/60.0, 150.0/60.0)
cone_1:rotate('Z', 25)
cone_1:rotate('Y', 25)
cone_1:rotate('X', 60)
cone_1:translate(0, 0, -30.0/60.0)
cone_1:scale(20.0/60.0 ,16.0/60.0, 60.0/60.0)

cone_2 = gr.cone('cone_2')
bn:add_child(cone_2)
cone_2:set_material(bump2)
cone_2:translate(0.0/60.0, 90.0/60.0, 150.0/60.0)
cone_2:rotate('Z', -20)
cone_2:rotate('Y', 40)
cone_2:rotate('X', 90)
cone_2:translate(0, 0, -30.0/60.0)
cone_2:scale(20.0/60.0 ,16.0/60.0, 60.0/60.0)

cone_3 = gr.cone('cone_3')
bn:add_child(cone_3)
cone_3:set_material(bump3)
cone_3:translate(75.0/60.0, 90.0/60.0, 150.0/60.0)
cone_3:rotate('Z', 30)
cone_3:rotate('Y', 0)
cone_3:rotate('X', 120)
cone_3:translate(0, 0, -30.0/60.0)
cone_3:scale(20.0/60.0 ,16.0/60.0, 60.0/60.0)

-- torus

torus_1 = gr.torus('torus_1', 6.0/60.0, 20.0/60.0)
bn:add_child(torus_1)
torus_1:set_material(bump1)
torus_1:translate(-75.0/60.0, 0, 150.0/60.0)
torus_1:rotate('Y', 10)
torus_1:rotate('X', 20)

torus_2 = gr.torus('torus_2', 6.0/60.0, 20.0/60.0)
bn:add_child(torus_2)
torus_2:set_material(bump2)
torus_2:translate(0.0/60.0, 0, 150.0/60.0)
torus_2:rotate('Y', 20)
torus_2:rotate('X', 40)

torus_3 = gr.torus('torus_3', 6.0/60.0, 20.0/60.0)
bn:add_child(torus_3)
torus_3:set_material(bump3)
torus_3:translate(75.0/60.0, 0, 150.0/60.0)
torus_3:rotate('Y', 30)
torus_3:rotate('X', 60)

-- Mesh

frameball_1 = gr.mesh('frameball_1', readobj('frameball.obj'))
bn:add_child(frameball_1)
frameball_1:set_material(bump1)
frameball_1:translate(-75.0/60.0, -90.0/60.0, 120.0/60.0)
frameball_1:rotate('X', 45)
frameball_1:rotate('Y', 45)
frameball_1:rotate('Z', 45)
frameball_1:scale(3.5/60.0,3.5/60.0,3.5/60.0)

frameball_2 = gr.mesh('frameball_2', readobj('frameball.obj'))
bn:add_child(frameball_2)
frameball_2:set_material(bump2)
frameball_2:translate(0/60.0, -90.0/60.0, 120.0/60.0)
frameball_2:rotate('X', 60)
frameball_2:rotate('Y', 60)
frameball_2:rotate('Z', 60)
frameball_2:scale(3.5/60.0,3.5/60.0,3.5/60.0)

frameball_3 = gr.mesh('frameball_3', readobj('frameball.obj'))
bn:add_child(frameball_3)
frameball_3:set_material(bump3)
frameball_3:translate(75.0/60.0, -90.0/60.0, 120.0/60.0)
frameball_3:rotate('X', 30)
frameball_3:rotate('Y', 30)
frameball_3:rotate('Z', 30)
frameball_3:scale(3.5/60.0,3.5/60.0,3.5/60.0)


bn:translate(0,0,70.0/60.0)

gr.render(scene,
      'normal_mapping_2.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.48, 5}, {0.7, 0.7, 0.7}, {1, 0, 0}, 500000, 64, 1, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), power bias, size(if non point light).
      }, 5, 0.1, 1000, 'ray tracing', 32, 'pinhole', 180.0, 4) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering

