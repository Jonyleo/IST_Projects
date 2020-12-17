/*global THREE, requestAnimationFrame, console*/

class GolfBall extends THREE.Group {
	constructor(types) {
		super();
		var ball_files = {
			"normal" : "assets/golf_ball/golf_normal.jpg"
		};

		this.mats = this.create_material(types, ball_files);


		this.add(new AlternateMaterial(this.mats,
			new createSphere(1, this.mats["phong"])));
	}

	create_material(types, files) {
		var materials = {};
		
		for(var x in types) {
			materials[types[x]] = getMaterialByTexture(types[x], files);
			materials[types[x]].metalness = 0;
			materials[types[x]].roughness = 0.1;
			materials[types[x]].clearcoat = 1;
			materials[types[x]].shininess = 1;
			materials[types[x]].specular = new THREE.Color(0xffffff);

		}
		return materials;
	}

	wireframe(state) {

		for(var x in this.mats) {
			if(state == undefined)
				state =  !this.mats[x].wireframe;
			this.mats[x].wireframe = state;
		}
	}
}