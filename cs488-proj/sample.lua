-- A simple test scene featuring some spherical cows grazing
-- around Stonehenge.  "Assume that cows are spheres..."
require('readobj')
marble = gr.material({0.8, 0.7, 0.7}, {0.2, 0.2, 0.2}, 15)
mirror = gr.material({0.84, 0.6, 0.53}, {0.3, 0.3, 0.3}, 40)
mirror2 = gr.material({0, 0, 0}, {1, 1, 1}, 0)
random1 = gr.material({0.65, 0.80, 0.32}, {0.4,0.4,0.4}, 20)
random2 = gr.material({0.15, 0.90, 0.52}, {0.3,0.3,0.3}, 20)
random3 = gr.material({0.65, 0.30, 0.68}, {0.6,0.6,0.6}, 30)
random4 = gr.material({0.35, 0.70, 0.49}, {0.6,0.6,0.6}, 20)
random5 = gr.material({0.25, 0.30, 0.76}, {0.5,0.5,0.5}, 50)
random6 = gr.material({0.75, 0.10, 0.77}, {0.2,0.2,0.2}, 30)
-- ##############################################
-- the ball
-- ##############################################
ball = gr.sphere('ball')
ball:set_material(mirror2)
ball:translate(0, 7, 0)
ball:scale(3.5, 3.5, 3.5)

-- ##############################################
-- the ball
-- ##############################################

frameball = gr.mesh('frameball', readobj('frameball.obj'))
frameball:set_material(random5)
frameball:translate(-5.0, 5.0, 14.0)
frameball:rotate('X', 45)
frameball:rotate('Y', 45)
frameball:rotate('Z', 45)
frameball:scale(0.2,0.2,0.2)



-- ##############################################
-- the monkey
-- ##############################################

monkey_mesh = gr.mesh('monkey', readobj('monkey.obj'))

monkey_mesh:set_material(random3)

monkey_mesh:translate(7.0, 4.0, 0.0)
monkey_mesh:rotate('Y', 90)
monkey_mesh:scale(1.2, 1.2, 1.2)
-- #############################################
-- Let's assume that cows are spheres
-- #############################################

cow = gr.node('the_cow')

for _, spec in pairs({
			{'body', {0, 0, 0}, 1.0},
			{'head', {.9, .3, 0}, 0.6},
			{'tail', {-.94, .34, 0}, 0.2},
			{'lfleg', {.7, -.7, -.7}, 0.3},
			{'lrleg', {-.7, -.7, -.7}, 0.3},
			{'rfleg', {.7, -.7, .7}, 0.3},
			{'rrleg', {-.7, -.7, .7}, 0.3}
		     }) do
   part = gr.nh_sphere(unpack(spec))
   part:set_material(mirror)
   cow:add_child(part)
end

-- ##############################################
-- the scene
-- ##############################################

scene = gr.node('scene')
scene:rotate('X', 23)

-- the floor

plane = gr.mesh('plane', {
		   { -1, 0, -1 }, 
		   {  1, 0, -1 }, 
		   {  1,  0, 1 }, 
		   { -1, 0, 1  }
		}, {
		   {3, 2, 1, 0}
		})
scene:add_child(plane)
plane:set_material(marble)
plane:scale(30, 30, 30)



-- Use the instanced cow model to place some actual cows in the scene.
-- For convenience, do this in a loop.

cow_number = 1

for _, pt in pairs({
		      {{7,1.3,10}, 20},
		      {{0,1.3,11}, 180}}) do
   cow_instance = gr.node('cow' .. tostring(cow_number))
   scene:add_child(cow_instance)
   cow_instance:add_child(cow)
   cow_instance:translate(unpack(pt[1]))
   cow_instance:rotate('Y', pt[2])
   cow_instance:scale(1.4, 1.4, 1.4)
   
   cow_number = cow_number + 1
end

ball_instance = gr.node('ball')
scene:add_child(ball_instance)
ball_instance:add_child(ball)

frameball_instance = gr.node('frameball')
scene:add_child(frameball_instance)
frameball_instance:add_child(frameball)

-- Place a ring of monkeys.

for i = 1, 6 do
   a_monkey = gr.node('monkey' .. tostring(i))
   a_monkey:rotate('Y', (i-1) * 60)
   scene:add_child(a_monkey)
   a_monkey:add_child(monkey_mesh)
end

gr.render(scene,
	  'sample.png', 1200, 1200,
	  {0, 2, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {gr.light({200, 202, 430}, {0.8, 0.8, 0.8}, {1, 0, 0})})
