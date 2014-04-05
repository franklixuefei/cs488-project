mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, {0,0,0}, 10, 10000000, '')
mat2 = gr.material({0.5, 1, 0.5}, {0, 0, 0}, {0,0,0}, 25, 10000000, '')
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, {0,0,0}, 50, 10000000, '')
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, {0,0,0}, 25, 10000000, '')
mat5 = gr.material({0.2, 0.8, 1.0}, {0, 0, 0}, {0,0,0}, 30, 10000000, '')
mirror = gr.material({0, 0, 0}, {1, 1, 1}, {0,0,0}, 100, 10000000, '')
glass = gr.material({0,0,0}, {1,1,1}, {1,1,1} ,100, 1.5, '') -- kd, ks, the third param is transmitted color, shininess, refraction index
water = gr.material({0,0,0}, {1,1,1}, {1,1,1}, 100, 1.33, '')


scene = gr.node('scene')

cornell_box = gr.cornell_box(5,5)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

bn = gr.node('bn')
scene:add_child(bn)

b1 = gr.cube('b1')
bn:add_child(b1)
b1:set_material(mat5)
b1:translate(-40.0/60.0,-40.0/60.0, 150.0/60.0)
b1:rotate('X', 25)
b1:rotate('Y', 25)
b1:scale(80.0/60.0 ,80.0/60.0, 80.0/60.0)


gr.render(scene,
      'stocastic.png', 600, 600,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.5, 8}, {0.7, 0.7, 0.7}, {1, 0, 0}, 500000, 64, 1, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), power bias, size(if non point light).
      }, 5, 0.1, 1000, 'ray tracing', 32, 'pinhole', 180.0, 4) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering
