-- textrue mapped version
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

-- cube

cube_1 = gr.cube('cube_1')
bn:add_child(cube_1)
cube_1:set_material(bump1)
cube_1:translate(-95.0/60.0, 90.0/60.0, 130.0/60.0)
cube_1:rotate('Z', 25)
cube_1:rotate('Y', 25)
cube_1:rotate('X', 25)
cube_1:translate(-20.0/60.0, -20.0/60.0, 0)
cube_1:scale(50.0/60.0 ,50.0/60.0, 50.0/60.0)

cube_2 = gr.cube('cube_2')
bn:add_child(cube_2)
cube_2:set_material(bump2)
cube_2:translate(-20.0/60.0, 90.0/60.0, 130.0/60.0)
cube_2:rotate('Z', 40)
cube_2:rotate('Y', 40)
cube_2:rotate('X', 40)
cube_2:translate(-20.0/60.0, -20.0/60.0, 0)
cube_2:scale(50.0/60.0 ,50.0/60.0, 50.0/60.0)

cube_3 = gr.cube('cube_3')
bn:add_child(cube_3)
cube_3:set_material(bump3)
cube_3:translate(55.0/60.0, 90.0/60.0, 130.0/60.0)
cube_3:rotate('Z', 65)
cube_3:rotate('Y', 65)
cube_3:rotate('X', 65)
cube_3:translate(-20.0/60.0, -20.0/60.0, 0)
cube_3:scale(50.0/60.0 ,50.0/60.0, 50.0/60.0)

-- sphere

sphere_1 = gr.sphere('sphere_1')
bn:add_child(sphere_1)
sphere_1:set_material(bump1)
sphere_1:translate(-75.0/60.0, 0, 150.0/60.0)
sphere_1:rotate('X', 90)
sphere_1:scale(25.0/60.0 ,25.0/60.0, 25.0/60.0)

sphere_2 = gr.sphere('sphere_2')
bn:add_child(sphere_2)
sphere_2:set_material(bump2)
sphere_2:translate(0.0/60.0, 0, 150.0/60.0)
sphere_2:rotate('X', 90)
sphere_2:scale(20.0/60.0 ,25.0/60.0, 25.0/60.0)

sphere_3 = gr.sphere('sphere_3')
bn:add_child(sphere_3)
sphere_3:set_material(bump3)
sphere_3:translate(75.0/60.0, 0, 150.0/60.0)
sphere_3:scale(25.0/60.0, 20.0/60.0, 25.0/60.0)

-- cylinder

cyl_1 = gr.cylinder('cyl_1')
bn:add_child(cyl_1)
cyl_1:set_material(bump1)
cyl_1:translate(-75.0/60.0, -90.0/60.0, 150.0/60.0)
cyl_1:rotate('X', 10)
cyl_1:rotate('Y', 30)
cyl_1:translate(0,0,-30.0/60.0)
cyl_1:scale(16.0/60.0 ,16.0/60.0, 60.0/60.0)

cyl_2 = gr.cylinder('cyl_2')
bn:add_child(cyl_2)
cyl_2:set_material(bump2)
cyl_2:translate(0.0/60.0, -90.0/60.0, 150.0/60.0)
cyl_2:rotate('X', 30)
cyl_2:rotate('Y', 60)
cyl_2:translate(0,0,-30.0/60.0)
cyl_2:scale(16.0/60.0 ,16.0/60.0, 60.0/60.0)

cyl_3 = gr.cylinder('cyl_3')
bn:add_child(cyl_3)
cyl_3:set_material(bump3)
cyl_3:translate(75.0/60.0, -90.0/60.0, 150.0/60.0)
cyl_3:rotate('X', 20)
cyl_3:rotate('Y', 120)
cyl_3:translate(0,0,-30.0/60.0)
cyl_3:scale(16.0/60.0 ,16.0/60.0, 60.0/60.0)

bn:translate(0,0,70.0/60.0)

gr.render(scene,
      'normal_mapping_1.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.48, 5}, {0.7, 0.7, 0.7}, {1, 0, 0}, 500000, 64, 1, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), power bias, size(if non point light).
      }, 5, 0.1, 1000, 'ray tracing', 32, 'pinhole', 180.0, 4) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering

