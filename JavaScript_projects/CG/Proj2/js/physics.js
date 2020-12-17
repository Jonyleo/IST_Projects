/*global THREE, requestAnimationFrame, console*/

var physicsObjects = [];
var colisions = [];

class PhysicsObject extends THREE.Object3D {
	constructor() {
		super();
		physicsObjects.push(this);
	}

	is_coliding(other) {
		return false;
	}

	colision_force(other) {
		return new THREE.Vector3();
	}
}

class EnvolvingSphere extends PhysicsObject {
	constructor(radius) {
		super();
		this.radius = radius;
	}

	is_coliding(other) {
		if(other instanceof EnvolvingSphere) {
			return this.parent.position.distanceTo(other.parent.position) <= (this.radius + other.radius)

		} else if (other instanceof ColisionBox) {

			var top_left = other.top_left();
			top_left.sub(new THREE.Vector2(this.radius,this.radius));
			var bot_right = other.bot_right();
			bot_right.add(new THREE.Vector2(this.radius,this.radius));

			if ( this.parent.position.x > top_left.x && this.parent.position.x < bot_right.x ) {
				if ( this.parent.position.z > top_left.y && this.parent.position.z < bot_right.y ) {
					return true;
				}
			}
			return false;
		}
	}
}


// Colision Box 2D (Top Down View)
class ColisionBox extends PhysicsObject {
	constructor(dimensions) {
		super()
		this.dimensions = dimensions;
	}

	top_left() {
		return new THREE.Vector2(this.parent.position.x - this.dimensions.x / 2,
														 this.parent.position.z - this.dimensions.y / 2);
	}
	bot_right() {
		return new THREE.Vector2(this.parent.position.x + this.dimensions.x / 2,
														 this.parent.position.z + this.dimensions.y / 2);
	}

}