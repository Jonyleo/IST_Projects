/*global THREE, requestAnimationFrame, console*/

class Wall extends THREE.Group {
	constructor(dimensions, color) {
		super()

		this.add(createBox(dimensions, color));
		this.add(new ColisionBox(new THREE.Vector2(dimensions.x, dimensions.z)));
	}

	update_speed() {
		return;
	}

	colide(other) {
		return;
	}
}

class VerticalWall extends Wall {
	constructor(dimensions, color) {
		super(dimensions, color);
	}

	colision_tranform(speed) {
		speed.x *= -1;
	}
}

class HorizontalWall extends Wall {
	constructor(dimensions, color) {
		super(dimensions, color);
	}

	colision_tranform(speed) {
		speed.z *= -1;
	}
}