/*global THREE, requestAnimationFrame, console*/

class Table extends THREE.Group {
	constructor(dimensions, n_balls) {
		'use strict';
		super();
		this.dimensions = dimensions;
		this.holes = new THREE.Group();
		this.sticks = [];
		this.selected = -1;

	  this.add(createBox(new THREE.Vector3(dimensions.x,1,dimensions.y),
                      		 0x00aa00));

	  var hole_radius = 1.5;
	  this.holes.add(this.create_hole(hole_radius, -1, -1));
	  this.holes.add(this.create_hole(hole_radius, -1, 1));
	  this.holes.add(this.create_hole(hole_radius, 1, -1));
	  this.holes.add(this.create_hole(hole_radius, 1, 1));
	  this.holes.add(this.create_hole(hole_radius, 0, -1));
	  this.holes.add(this.create_hole(hole_radius, 0, 1));
	  this.add(this.holes);

	  this.create_sticks();
	  this.create_walls();
	}

	getWorldPosition() {
		var tmp = new THREE.Vector3();
		super.getWorldPosition(tmp);
		return tmp;
	}

	create_hole(radius, weight_x, weight_y) {
		var tmp_object = new Hole(radius);
		tmp_object.position.set(weight_x * (this.dimensions.x/2 - radius), 0, weight_y * (this.dimensions.y/2 - radius));
		return tmp_object;
	}

	create_sticks() {
		var tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(-this.dimensions.x/4, 1.5, -this.dimensions.y/2 + 2);
		tmp_object.rotateX(Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);

		tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(this.dimensions.x/4, 1.5, -this.dimensions.y/2 + 2);
		tmp_object.rotateX(Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);

		tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(this.dimensions.x/2 - 2, 1.5, 0);
		tmp_object.rotateX(Math.PI/2);
		tmp_object.rotateZ(Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);

		tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(this.dimensions.x/4, 1.5, this.dimensions.y/2 - 2);
		tmp_object.rotateX(-Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);

		tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(-this.dimensions.x/4, 1.5, this.dimensions.y/2 - 2);
		tmp_object.rotateX(-Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);

		tmp_object = new Stick(Math.PI/4, 20, 0.5, 0x111111, 0xff0000);
		tmp_object.position.set(-this.dimensions.x/2 + 2, 1.5, 0);
		tmp_object.rotateX(-Math.PI/2);
		tmp_object.rotateZ(-Math.PI/2);
		this.add(tmp_object);
		this.sticks.push(tmp_object);
	}

	create_walls() {
		var tmp_object = new HorizontalWall(new THREE.Vector3(this.dimensions.x,4,1), 0x943d03);
	  tmp_object.position.set(0, 2, -this.dimensions.y/2);
	  this.add(tmp_object);

	  tmp_object = new HorizontalWall(new THREE.Vector3(this.dimensions.x,4,1), 0x943d03);
	  tmp_object.position.set(0, 2, this.dimensions.y/2);
	  this.add(tmp_object);

	  tmp_object = new VerticalWall(new THREE.Vector3(1,4,this.dimensions.y), 0x943d03);
	  tmp_object.position.set(this.dimensions.x/2, 2, 0);
	  this.add(tmp_object);

	  tmp_object = new VerticalWall(new THREE.Vector3(1,4,this.dimensions.y), 0x943d03);
	  tmp_object.position.set(-this.dimensions.x/2, 2, 0);
	  this.add(tmp_object);
	}
	select(number) {
		if(this.selected != -1)
			this.sticks[this.selected].reset();
		this.selected = number;
		return this.sticks[this.selected].select();
	}

	shoot(camera) {
		if(this.selected == -1)
			return;
		var ball = this.sticks[this.selected].shoot();
		ball.add_camera(camera);
		return ball;
	}

	rotate_stick(deltatime) {
		if(this.selected == -1)
			return;
		this.sticks[this.selected].rotate(deltatime * Math.PI/2);
	}	
}