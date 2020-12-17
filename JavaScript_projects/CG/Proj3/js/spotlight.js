/*global THREE, requestAnimationFrame, console*/

class Spotlight extends THREE.Group {
	constructor(types, from_center, color) {
		super();

		var holder = new THREE.Group();

		this.cone =
			new AlternateMaterial(getColorByTypes(types, 0xebba34), 
														createConeSec(0.1, 2, 6, 0xebba34, new THREE.Vector3(-Math.PI/2, 0, 0)));
	

		var lampada = 
			new AlternateMaterial(getColorByTypes(types, color),
														createSphere(2, color));

		this.light = new THREE.SpotLight(0xffffff, 1);
		
		holder.add(this.light);
		holder.add(this.cone);
		holder.add(lampada);

		holder.position.y += 30;
		holder.position.x = from_center;
		holder.lookAt(new THREE.Vector3());

		this.add(holder);

	
	}

	switch() {
		this.light.visible = ! this.light.visible;
		this.cone.visible = ! this.cone.visible;
	}
}