-- textrue mapped version
mat1 = gr.material({0.6, 1.0, 0.6}, {0, 0, 0}, {0,0,0}, 10, 10000000, '')
mat2 = gr.material({0.5, 1, 0.5}, {0, 0, 0}, {0,0,0}, 25, 10000000, '')
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, {0,0,0}, 64, 10000000, '')
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.5, 0.8}, {0,0,0}, 25, 10000000, '')
mat5 = gr.material({0.1, 0.7, 1.0}, {0.6, 0.6, 0.6}, {0,0,0}, 40, 10000000, '')
light_blue_shiny = gr.material({0, 255.0/255.0, 1.0}, {0.7, 0.7, 0.7}, {0,0,0}, 70, 10000000, '')
mirror = gr.material({0, 0, 0}, {1, 1, 1}, {0,0,0}, 100, 10000000, '')
glass = gr.material({0,0,0}, {1,1,1}, {1,1,1} ,100, 1.5, '') -- kd, ks, the third param is transmitted color, shininess, refraction index
bump = gr.material({0,0,0},{1,1,1},{0,0,0}, 100, 10000000, 'bump_map.png')

water = gr.material({0,0,0}, {1,1,1}, {1,1,1}, 100, 1.33, '')

texture1 = gr.texture_material('checker.png', {0.2, 0.2, 0.2}, {0,0,0}, 25, 10000000, '')
texture2 = gr.texture_material('checker.png', {0.6, 0.6, 0.6}, {0,0,0}, 66, 10000000, '')
wooden = gr.texture_material('wooden.png', {0,0,0}, {0,0,0}, 50, 1000000, '')

clock_texture = gr.texture_material('clock.png', {0.5,0.7,0.5}, {0,0,0}, 54, 10000000, '')

photo = gr.texture_material('me.png', {0.05, 0.05, 0.05}, {0,0,0}, 20, 10000000, '')

bump_wall = gr.texture_material('rock_texture.png',{0,0,0},{0,0,0}, 30, 10000000, 'back_wall.png')
normal_wall = gr.texture_material('rock_texture.png',{0,0,0},{0,0,0}, 30, 10000000, '')

floor_texture = gr.texture_material('floor.png', {0,0,0},{0,0,0}, 25, 10000000, '')

-- scene node (top node)
scene = gr.node('scene')

cornell_box = gr.cornell_box(5,5)
scene:add_child(cornell_box)
cornell_box:set_material(mat1)

-- table node

table_node = gr.node('tn')
scene:add_child(table_node)

table_leg_1 = gr.cylinder('tl_1');
table_node:add_child(table_leg_1);
table_leg_1:set_material(glass);
table_leg_1:translate(-90.0/60.0, -150.0/60.0, 90.0/60.0);
table_leg_1:rotate('X', -90);
table_leg_1:scale(10.0/60.0, 10.0/60.0, 80.0/60.0);

table_leg_2 = gr.cylinder('tl_2');
table_node:add_child(table_leg_2);
table_leg_2:set_material(glass);
table_leg_2:translate(90.0/60.0, -150.0/60.0, 90.0/60.0);
table_leg_2:rotate('X', -90);
table_leg_2:scale(10.0/60.0, 10.0/60.0, 80.0/60.0);

table_leg_3 = gr.cylinder('tl_3');
table_node:add_child(table_leg_3);
table_leg_3:set_material(glass);
table_leg_3:translate(90.0/60.0, -150.0/60.0, 210.0/60.0);
table_leg_3:rotate('X', -90);
table_leg_3:scale(10.0/60.0, 10.0/60.0, 80.0/60.0);

table_leg_4 = gr.cylinder('tl_4');
table_node:add_child(table_leg_4);
table_leg_4:set_material(glass);
table_leg_4:translate(-90.0/60.0, -150.0/60.0, 210.0/60.0);
table_leg_4:rotate('X', -90);
table_leg_4:scale(10.0/60.0, 10.0/60.0, 80.0/60.0);

table_cap_1 = gr.cylinder('tc_1');
table_node:add_child(table_cap_1);
table_cap_1:set_material(mirror);
table_cap_1:translate(-90.0/60.0, -65.5/60.0, 90.0/60.0);
table_cap_1:rotate('X', -90);
table_cap_1:scale(10.0/60.0, 10.0/60.0, 6.0/60.0);

table_cap_2 = gr.cylinder('tc_2');
table_node:add_child(table_cap_2);
table_cap_2:set_material(mirror);
table_cap_2:translate(90.0/60.0, -65.5/60.0, 90.0/60.0);
table_cap_2:rotate('X', -90);
table_cap_2:scale(10.0/60.0, 10.0/60.0, 6.0/60.0);

table_cap_3 = gr.cylinder('tc_3');
table_node:add_child(table_cap_3);
table_cap_3:set_material(mirror);
table_cap_3:translate(90.0/60.0, -65.5/60.0, 210.0/60.0);
table_cap_3:rotate('X', -90);
table_cap_3:scale(10.0/60.0, 10.0/60.0, 6.0/60.0);

table_cap_4 = gr.cylinder('tc_4');
table_node:add_child(table_cap_4);
table_cap_4:set_material(mirror);
table_cap_4:translate(-90.0/60.0, -65.5/60.0, 210.0/60.0);
table_cap_4:rotate('X', -90);
table_cap_4:scale(10.0/60.0, 10.0/60.0, 6.0/60.0);

-- table top node (belongs to table node)

table_top_node = gr.node('table_top_node');
table_node:add_child(table_top_node);

table_top = gr.cube('tl_top');
table_top_node:add_child(table_top);
table_top:set_material(glass);
table_top:translate(-110.0/60.0, -65/60, 0);
table_top:rotate('X', 90);
table_top:scale(220.0/60.0, 220.0/60.0, 5.0/60.0);

glass_ball_1 = gr.sphere('gb1');
table_top_node:add_child(glass_ball_1);
glass_ball_1:set_material(glass);
glass_ball_1:translate(-50.0/60.0, -39.5/60.0, 150.0/60.0);
glass_ball_1:rotate('X', 90)
glass_ball_1:scale(25.0/60.0, 25.0/60.0, 25.0/60.0);

cylinder_1 = gr.cylinder('cyl_1')
table_top_node:add_child(cylinder_1)
cylinder_1:set_material(texture2)
cylinder_1:translate(50.0/60.0, -54.5/60.0, 150.0/60.0)
cylinder_1:rotate('Y', -45)
cylinder_1:scale(10.0/60.0, 10.0/60.0, 40.0/60.0)


-- torus-cone node (belongs to table top node)

torus_cone_node = gr.node('tcn');
table_top_node:add_child(torus_cone_node);

torus_1 = gr.torus('tor_1',4.0/60.0, 16.0/60.0);
torus_cone_node:add_child(torus_1)
torus_1:set_material(mat3)
torus_1:translate(0, -52.5/60.0, 0)
torus_1:rotate('X', 90);

cone_1 = gr.cone('con_1');
torus_cone_node:add_child(cone_1)
cone_1:set_material(mat5)
cone_1:translate(0, -24.5/60.0, 0)
cone_1:rotate('X', 90)
cone_1:scale(18.0/60.0, 18.0/60.0, 40.0/60.0)


torus_2 = gr.torus('tor_2', 5.0/60.0, 20.0/60.0); -- torus on the corner of the table
table_top_node:add_child(torus_2)
torus_2:set_material(mat3)
torus_2:translate((110.0-2.5)/60.0, -82.5/60.0, (220.0-2.5)/60.0);
torus_2:rotate('Y', 45)

-- move table top node and torus-cone node forward to viewer

torus_cone_node:translate(-10.0/60.0, 0, 100.0/60.0)

table_top_node:translate(0,0,40.0/60.0)

table_node:translate(0, 1.0/60.0, 0);

glass_ball_2 = gr.sphere('gb2') -- glass ball on the ground
scene:add_child(glass_ball_2)
glass_ball_2:set_material(glass)
glass_ball_2:translate(-15.0/60.0, -116.0/60.0, 240.0/60.0)
glass_ball_2:rotate('X', 90)
glass_ball_2:scale(33.0/60.0,33.0/60.0,33.0/60.0)


-- ball on the wall
mirror_ball_1 = gr.sphere('mb_1')
scene:add_child(mirror_ball_1);
mirror_ball_1:set_material(mirror)
mirror_ball_1:translate(0, 60.0/60.0, 0)
mirror_ball_1:scale(60.0/60.0, 60.0/60.0, 30.0/60.0);



-- back wall
back_wall = gr.cube('bw')
scene:add_child(back_wall)
back_wall:set_material(bump_wall)
back_wall:translate(-150.0/60.0, -150.0/60.0, 0.0)
back_wall:scale(300.0/60.0, 300.0/60.0, 1.0/60.0)
-- left wall
left_wall = gr.cube('lw')
scene:add_child(left_wall)
left_wall:set_material(normal_wall)
left_wall:translate(-149.0/60.0, -150.0/60.0, 0)
left_wall:rotate('Y', -90);
left_wall:scale(300.0/60.0, 300.0/60.0, 1.0/60.0)
-- right wall
right_wall = gr.cube('rw')
scene:add_child(right_wall)
right_wall:set_material(normal_wall)
right_wall:translate(150.0/60.0, -150.0/60.0, 0)
right_wall:rotate('Y', -90);
right_wall:scale(300.0/60.0, 300.0/60.0, 1.0/60.0)
-- floor
floor_plane = gr.cube('fp')
scene:add_child(floor_plane)
floor_plane:set_material(floor_texture)
floor_plane:translate(-150.0/60.0, -149.0/60.0, 0)
floor_plane:rotate('X', 90);
floor_plane:scale(300.0/60.0, 300.0/60.0, 1.0/60.0)

cone_2 = gr.cone('con_2') -- cone on the ground
scene:add_child(cone_2)
cone_2:set_material(bump)
cone_2:translate(0.0/60.0, -150.0/60.0, 141.0/60.0)
cone_2:rotate('X', -58)
cone_2:rotate('Y', 60)
cone_2:scale(28.0/60.0, 28.0/60.0, 60.0/60.0)


-- photo frame node (belongs to scene)

photo_frame = gr.node('pf');
scene:add_child(photo_frame)


frame_up = gr.cube('fu')
photo_frame:add_child(frame_up)
frame_up:set_material(wooden)
frame_up:translate(150.0/60.0, 120.0/60.0, 110.0/60.0)
frame_up:rotate('Y', -90)
frame_up:scale(100.0/60.0, 3.5/60.0, 3.5/60.0)

frame_left = gr.cube('fl')
photo_frame:add_child(frame_left)
frame_left:set_material(wooden)
frame_left:translate(146.5/60.0, 120.0/60.0, 110.0/60.0)
frame_left:rotate('Z', -90)
frame_left:scale((120.0-3.5)/60.0, 3.5/60.0, 3.5/60.0)

frame_down = gr.cube('fd')
photo_frame:add_child(frame_down)
frame_down:set_material(wooden)
frame_down:translate(150.0/60.0, 0, 110.0/60.0)
frame_down:rotate('Y', -90)
frame_down:scale(100.0/60.0, 3.5/60.0, 3.5/60.0)

frame_right = gr.cube('fr')
photo_frame:add_child(frame_right)
frame_right:set_material(wooden)
frame_right:translate(146.5/60.0, 120.0/60.0, (210.0-3.5)/60.0)
frame_right:rotate('Z', -90)
frame_right:scale((120.0-3.5)/60.0, 3.5/60.0, 3.5/60.0)

frame_main = gr.cube('fm')
photo_frame:add_child(frame_main)
frame_main:set_material(photo)
frame_main:translate(150.0/60.0, 0, 110.0/60.0)
frame_main:rotate('Y', -90)
frame_main:scale(100.0/60.0, 123.5/60.0, 1.0/60.0)

-- move photo frame out of wall
photo_frame:translate(-1.0/60.0, -10.0/60.0,0);

table_node:translate(0, 0, 20.0/60.0);

-- clock node (belongs to scene)
clock_node = gr.node('clock')
scene:add_child(clock_node)

clock_ring = gr.torus('cr',3.5/60.0, 50.0/60.0);
clock_node:add_child(clock_ring)
clock_ring:set_material(clock_texture)


clock_hour = gr.cylinder('ch') -- hour hand
clock_node:add_child(clock_hour)
clock_hour:set_material(clock_texture)
clock_hour:translate(0, -8.0/60.0, 1.75/60.0)
clock_hour:rotate('Z', -60)
clock_hour:rotate('X', 90)
clock_hour:scale(3.5/60.0, 3.5/60.0, 38.0/60.0)

clock_min = gr.cylinder('cm') -- minut hand
clock_node:add_child(clock_min)
clock_min:set_material(clock_texture)
clock_min:translate(0, -8.0/60.0, 1.75/60.0)
clock_min:rotate('X', -90)
clock_min:scale(3.5/60.0, 3.5/60.0, 43.0/60.0)

clock_node:translate(-145.5/60.0, 50.0/60.0, 150.0/60.0)
clock_node:rotate('Y', 90)

-- render mode can be 'all', 'photon map'(only show photon-mapped image), 'ray tracing' (only show ray-traced image)
-- eye type can be 'pinhole', 'fisheye'

gr.render(scene,
      'final.png', 1000, 1000,
      {0, 0, 10}, {0, 0, -1}, {0, 1, 0}, 50,
      {0.4, 0.4, 0.4}, {
        gr.light({0, 2.48, 3.167}, {0.6, 0.6, 0.6}, {1, 0, 0}, 400000000, 110, 1, 'square', 1.5) -- pos, colour, attenuation, num photons, power of initial photons, light type ('square','round','point'), power bias, size(if non point light).
      }, 6, 0.03, 8000, 'photon map', 32, 'pinhole', 180.0, 1) -- num recur, search radius, num photons per search area, render mode, num threads, eye mode, max psi for fisheye, num sqrt(samples) per pixel for stochastic rendering
