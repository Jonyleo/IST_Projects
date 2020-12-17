/*global THREE, requestAnimationFrame, console*/

/*
  Constants
*/
const LEFT_ARROW = 37, RIGHT_ARROW = 39;
const SPOT1 = 49, SPOT2 = 50, SPOT3 = 51; // 1, 2, 3
const CAMERA_TOP = 52, CAMERA_SIDE = 53; // 4, 5

const GLOBAL_LIGHT = 81;
const NO_LIGHT = 87;
const LIGHT_SWAP = 69;

const CAMERA_RATIO = 15;

/*
	Global variables
*/

var camera, scene, renderer;

var side_camera, top_camera;

var clock;

var left = false, right = false;
var noLight = false;
var lambert = true;

var directionalLight;
var spot1, spot2, spot3;

var palanque;

class Cybertruck extends THREE.Group {
	constructor(types) {
		super();
		this.carrocaria = new THREE.Group();
		this.carrocaria.add(new AlternateMaterial(getColorByTypes(types, 0x441030) ,
  												createMesh(cybertruck_vert, 0x441030)));

		this.carrocaria.add(new AlternateMaterial(getColorByTypes(types, 0x000000) ,
													createMesh(windows_vert, 0x000020)));

		this.carrocaria.add(new AlternateMaterial(getColorByTypes(types, 0x00ffff, {emissive: 0x00ffff}) ,
													createMesh(front_light, 0x00ffff)));

		this.carrocaria.add(new AlternateMaterial(getColorByTypes(types, 0xff0000, {emissive: 0xff0000}) ,
													createMesh(back_light, 0xff0000)));


		


		this.add(this.carrocaria);

		this.chassis = new THREE.Group();

		var wheel = new AlternateMaterial(getColorByTypes(types, 0x000000) ,
  												createCyl(3, 2, 0x000000, new THREE.Vector3(Math.PI/2,0,0)))
		wheel.position.set(14, 0, 10);
		this.chassis.add(wheel);

		var wheel = new AlternateMaterial(getColorByTypes(types, 0x000000) ,
  												createCyl(3, 2, 0x000000, new THREE.Vector3(Math.PI/2,0,0)))

		wheel.position.set(14, 0, -10);
		this.chassis.add(wheel);

		var wheel = new AlternateMaterial(getColorByTypes(types, 0x000000) ,
  												createCyl(3, 2, 0x000000, new THREE.Vector3(Math.PI/2,0,0)))
		wheel.position.set(-14, 0, 10);
		this.chassis.add(wheel);

		var wheel = new AlternateMaterial(getColorByTypes(types, 0x000000) ,
  												createCyl(3, 2, 0x000000, new THREE.Vector3(Math.PI/2,0,0)))

		wheel.position.set(-14, 0, -10);
		this.chassis.add(wheel);
		this.add(this.chassis);
	}
}



function createScene() {
  "use strict";
  scene = new THREE.Scene();
  var types = ["basic", "lambert", "phong"];
 
 	var floor = 
 		new AlternateMaterial(getColorByTypes(types, 0x22ff22) ,
 			 										createBox(new THREE.Vector3(1000,1,1000), 0x22ff22));

 	floor.position.y -= 1;
  scene.add(floor);

  palanque = 
  	new AlternateMaterial(getColorByTypes(types, 0x888888) ,
  												createCyl(30, 2, 0x888888, new THREE.Vector3()));
  scene.add(palanque);

  var cybertuck = new Cybertruck(types);
  cybertuck.position.y += 4;
  	
  
  palanque.add(cybertuck);
  palanque.add(side_camera);

  directionalLight = new THREE.DirectionalLight( 0xffffff, 0.5 );
  directionalLight.position.set(0, 100, 0);


  scene.add( directionalLight );
  scene.add( directionalLight.target );

  spot1 = new Spotlight(types, 35, 0x000000);
  spot2 = new Spotlight(types, 35, 0x000000);
  spot3 = new Spotlight(types, 35, 0x000000);

  scene.add(spot1);
  scene.add(spot2);
  scene.add(spot3);

  spot2.rotateY(2 * Math.PI / 3);
  spot3.rotateY(4 * Math.PI / 3);
}



function init() {
  'use strict';
  renderer = new THREE.WebGLRenderer({
      antialias: true
  });
  renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.setClearColor( 0xaaaaaa, 1 );
  document.body.appendChild(renderer.domElement);

  clock = new THREE.Clock();

  
  top_camera = createPrespectiveCamera(new THREE.Vector3(0,70,0)); 
  side_camera = createOrtographicCamera(new THREE.Vector3(0,0,50)); 
  
  createScene(); 

  camera = top_camera; // Default Camera
  
  window.addEventListener("keydown", onKeyDown);
  window.addEventListener("keyup", onKeyUp);
  window.addEventListener("resize", onResize);

  clock.start();
}


/*
    Animation

    animate
    render
*/

function animate() {
  'use strict';
  var deltatime = clock.getDelta();
  var lightSpeed = 5

  if(left && !right)
  	palanque.rotateY(-Math.PI * deltatime);
  else if(right && !left)
  	palanque.rotateY(Math.PI * deltatime);

  render();
  
  requestAnimationFrame(animate);
}

function render() {
  'use strict';
  renderer.render(scene, camera);
}


/*
    Events

    onKeyDown
    onKeyUp
    onResize
*/

function onKeyDown(e) {
  'use strict';
    
  switch (e.keyCode) {
    case CAMERA_TOP:
      camera = top_camera;
      break;
    case CAMERA_SIDE:
      camera = side_camera;
      break

    case LEFT_ARROW:
      left = true;
      break;

    case RIGHT_ARROW:
      right = true;
      break;

    case SPOT1:
    	spot1.switch();
    	break;

    case SPOT2:
    	spot2.switch();
    	break;

    case SPOT3:
    	spot3.switch();
    	break;

    case GLOBAL_LIGHT:
    	directionalLight.visible = ! directionalLight.visible;
    	break;

    case NO_LIGHT:
    	var new_type;
    	if(noLight) {
    		if(lambert)
    			new_type = "lambert";
    		else
    			new_type = "phong";

    		noLight = false;
    	} else {	
    		new_type = "basic";
    		noLight = true;
    	}
    	for(var x in alternateMats) {
    		alternateMats[x].changeMaterialType(new_type);
    	}
    	break;

    case LIGHT_SWAP:
    	var new_type;
    	if(noLight)
    		break;

    	if(lambert) {
    		new_type = "phong";
    		lambert = false;
    	} else {
    		new_type = "lambert";
    		lambert = true;
    	}
    	for(var x in alternateMats) {
    		alternateMats[x].changeMaterialType(new_type);
    	}
    	break;
  }
}

function onKeyUp(e) {
  'use strict';

  switch(e.keyCode) {
    case LEFT_ARROW:
      left = false;
      break;

    case RIGHT_ARROW:
      right = false;
      break;
  }  
}


function onResize() {
  'use strict';

  renderer.setSize(window.innerWidth, window.innerHeight);
  
  resizePrespectiveCamera(top_camera);
  resizeOrtographicCamera(side_camera);
}