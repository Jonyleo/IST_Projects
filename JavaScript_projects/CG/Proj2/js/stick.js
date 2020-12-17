/*global THREE, requestAnimationFrame, console*/

class Stick extends THREE.Group {
	constructor(max_rotation, length, radius, color, alt_color) {
		super();
		
		this.main_stick = createConeSec(radius/1.5, radius, length, color, new THREE.Vector3());
		this.main_stick.position.set(0,-length/2 - 5 ,0);
		this.add(this.main_stick);

		this.dir_helper = new THREE.Object3D();

		this.dir_helper.position.set(0,1,0);
		this.add(this.dir_helper);

		this.stick_rotation = 0;
		this.max_rotation = max_rotation;

		this.select_stick = createConeSec(radius/1.5, radius, length, alt_color, new THREE.Vector3());
		this.select_stick.position.set(0,-length/2 - 5 ,0);
		this.add(this.select_stick);
		this.select_stick.visible = false;

		this.ghost_ball = createSphere(1, 0xffffff);
		this.ghost_ball.visible = false;
		this.add(this.ghost_ball);
	}

	getWorldPosition() {
		var tmp = new THREE.Vector3();
		super.getWorldPosition(tmp);
		return tmp;
	}

	rotate(degrees) {
		if(this.stick_rotation < 0) {
			if(this.stick_rotation + degrees < -this.max_rotation) {
				degrees = -this.max_rotation - this.stick_rotation;
			}
		} else {
			if(this.stick_rotation + degrees > this.max_rotation) {
				degrees = this.max_rotation - this.stick_rotation;
			}
		}
				
		
		this.stick_rotation += degrees;
		
		this.rotateZ(degrees);
	}

	shoot() {
		var ball = new Ball();
		this.add(ball);
		var pos = ball.getWorldPosition();
		this.parent.add(ball);
		ball.position.set(pos.x,pos.y,pos.z);
		
		var dir = new THREE.Vector3();
		this.dir_helper.getWorldPosition(dir);
		
		dir.sub(this.getWorldPosition());
		dir.multiplyScalar(100);
		ball.speed.set(dir.x, 0, dir.z);
		return ball
	}

	reset() {
		this.rotate(-this.stick_rotation);
		this.select_stick.visible = false;
		this.main_stick.visible = true;
		this.ghost_ball.visible = false;
	}

	select() {
		this.select_stick.visible = true;
		this.main_stick.visible = false;
		this.ghost_ball.visible = true;
		return this.getWorldPosition();
	}
}