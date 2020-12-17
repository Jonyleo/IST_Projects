/*global THREE, requestAnimationFrame, console*/

class Ball extends THREE.Group {
	constructor(radius = 1, color = 0xffffff, speed = new THREE.Vector3(), friction = 0.5, display = false) {
		super();
		this.type = "Ball";
		// Physics Constants
		this.speed = speed; 			 					// Vector3
		this.speed_before_update = new THREE.Vector3();
		this.friction = friction; 					// Float [0, 1]

		this.display = display;
		this.radius = radius;

		this.mesh = createSphere(radius, color)
		this.add(this.mesh);
		this.add(new EnvolvingSphere(radius));
		this.mesh.add(new THREE.AxesHelper(2));
	}

	getWorldPosition() {
		var tmp = new THREE.Vector3();
		super.getWorldPosition(tmp);
		return tmp;
	}

	update(deltatime) {
		var mov = this.speed.clone().multiplyScalar(deltatime);
		this.position.add(mov);

		this.speed.x *= (1 - this.friction * deltatime); 
		this.speed.z *= (1 - this.friction * deltatime);

		if(this.speed.y < 0)
			return;

		var camera_pos = mov.clone();
		camera_pos.normalize();
		camera_pos.multiplyScalar(-1);
		camera_pos.y += 1;
		camera_pos.multiplyScalar(5);

		for(var obj in this.children) {
			if(this.children[obj] instanceof THREE.PerspectiveCamera) {
				this.children[obj].position.set(camera_pos.x,camera_pos.y,camera_pos.z);
				this.children[obj].lookAt(this.getWorldPosition());
				break;
			}
		}
		var axis = mov.clone();

		axis.cross(new THREE.Vector3(0,1,0));
		this.mesh.rotateOnWorldAxis(axis.normalize(), -mov.length());
	}

	update_speed() {
		this.set_speed(this.speed_before_update);
	}

	colide(other) {
		if(other instanceof Ball) {
			var diference = this.getWorldPosition().clone().sub(other.getWorldPosition());

			var dot_product = this.speed.clone().sub(other.speed).dot(diference);

			var distance = this.getWorldPosition().distanceTo(other.getWorldPosition());
			distance = Math.pow(distance, 2);

			dot_product /= distance;

			var new_speed = diference.multiplyScalar(dot_product);
			
			this.set_speed_before_update(this.speed.clone().sub(new_speed));
		} else if (other instanceof Wall) {
			new_speed = this.speed.clone();
			other.colision_tranform(new_speed);
			this.set_speed_before_update(new_speed);
		}
	}

	set_speed(new_speed) {
		this.speed.set(new_speed.x, new_speed.y, new_speed.z);
	}

	set_speed_before_update(new_speed) {
		this.speed_before_update.set(new_speed.x, new_speed.y, new_speed.z);
	}

	add_camera(camera) {
		this.add(camera);

		camera.position.y = 15;
		
		camera.lookAt(this.getWorldPosition());
	}
}