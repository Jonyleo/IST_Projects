/*global THREE, requestAnimationFrame, console*/

var alternateMats = [];

class AlternateMaterial extends THREE.Object3D {
	constructor(materials, mesh) {
		super();
		this.mesh = mesh;
		this.materials = materials;
		this.add(this.mesh);
		alternateMats.push(this);
	}

	changeMaterialType(mat_type) {
		this.mesh.material = this.materials[mat_type];
	}
}