/*global THREE, requestAnimationFrame, console*/


class Grass extends THREE.Group {
	constructor(types, repeat, side) {
		super();
		var grass_files = {
	    "basecolor" : "assets/grass/Grass_005_BaseColor.jpg",
	    "normal" : "assets/grass/Grass_005_Normal.jpg",
	    "bump" : "assets/grass/Grass_005_Height.png"
  	};

  	this.materials_light = this.create_material(types, grass_files, 0x42f545);
  	this.materials_dark = this.create_material(types, grass_files, 0x1c4d1d);
  	var box;

  	var offset = (repeat - 1) * side / 2 ;

  	for(var x = 0; x < repeat; ++x) {
  		for(var y = 0; y < repeat; ++y) {
  			
  			if(x % 2 == y % 2) {
  				box = createPlane(new THREE.Vector3(side,side), this.materials_light["phong"]);
  				box.position.set(x * side - offset, 0, y * side - offset);
  				this.add(new AlternateMaterial(this.materials_light, box));
  			}
  			else {
  				box = createPlane(new THREE.Vector3(side,side), this.materials_dark["phong"]);
  				box.position.set(x * side - offset, 0, y * side - offset);
  				this.add(new AlternateMaterial(this.materials_dark, box));
  			}

  		}
  	}
  	
	}


	create_material(types, files, albedo) {
		var materials = {};
		albedo = new THREE.Color(albedo);
		
		for(var x in types) {
			materials[types[x]] = getMaterialByTexture(types[x], files);
			materials[types[x]].color = albedo;
		}
		return materials;
	}

	wireframe(state) {
		for(var x in this.materials_light) {
			if(state == undefined)
				state = !this.materials_light[x].wireframe;
			this.materials_light[x].wireframe = state;
		}

		for(var x in this.materials_dark) {
			if(state == undefined)
				state = !this.materials_dark[x].wireframe;
			this.materials_dark[x].wireframe = state;
		}
	}
}