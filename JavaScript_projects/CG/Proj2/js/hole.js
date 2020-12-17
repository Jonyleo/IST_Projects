/*global THREE, requestAnimationFrame, console*/

class Hole extends THREE.Group {
	constructor(radius) {
		super();

		this.add(createCyl(radius, 1.01, 0x000000, new THREE.Vector3()));
		this.radius = radius;
	}

	getWorldPosition() {
		var tmp = new THREE.Vector3();
		super.getWorldPosition(tmp);
		return tmp;
	}

	is_inside(ball) {
		return ball.position.distanceTo(this.position) <= (ball.radius/2 + this.radius);
	}

	update_speed() {
		return;
	}

	colide(other) {
		return;
	}
}