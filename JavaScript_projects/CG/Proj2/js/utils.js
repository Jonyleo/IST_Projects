/*global THREE, requestAnimationFrame, console*/

// Caching for materials of the same color
var materialCache = {};


function getMaterial(color) {
	if(color in materialCache)
		return materialCache[color];
	
	var material  = new THREE.MeshBasicMaterial({ color: color, wireframe: false });
	materialCache[color] = material;

	return material;
}

/*
  Primitive Creators

  createBox
  createCyl

*/

function createBox(dim, color) {
  var material = getMaterial(color);
  var geometry = new THREE.BoxBufferGeometry(dim.x, dim.y, dim.z);
  
  var mesh = new THREE.Mesh(geometry, material);
  
  return mesh;
}

function createCyl(radius, height, color, xyz_rot) {
  var material = getMaterial(color);
  var geometry = new THREE.CylinderBufferGeometry(radius, radius, height, 10);
  
  var mesh = new THREE.Mesh(geometry, material);

  mesh.rotateX(xyz_rot.x);
  mesh.rotateY(xyz_rot.y);
  mesh.rotateZ(xyz_rot.z);

  return mesh;
}

function createConeSec(rad_a, rad_b, height, color, xyz_rot) {
  var material = getMaterial(color);
  var geometry = new THREE.CylinderBufferGeometry(rad_a, rad_b, height, 10);
  
  var mesh = new THREE.Mesh(geometry, material);

  mesh.rotateX(xyz_rot.x);
  mesh.rotateY(xyz_rot.y);
  mesh.rotateZ(xyz_rot.z);

  return mesh;
}

function createSphere(radius, color) {
  var material = getMaterial(color);
  var geometry = new THREE.SphereBufferGeometry(radius, 10, 10);
  
  var mesh = new THREE.Mesh(geometry, material);

  return mesh;
}


/*
  Camera Creators

  createOrtographicCamera
  createPrespectiveCamera
  
*/

function createOrtographicCamera(pos) {
  'use strict';
  var tmp_camera = new THREE.OrthographicCamera(window.innerWidth / - CAMERA_RATIO, 
                                                window.innerWidth / CAMERA_RATIO, 
                                                window.innerHeight / CAMERA_RATIO, 
                                                window.innerHeight / - CAMERA_RATIO,
                                                1,
                                                1000);
  tmp_camera.position.x = pos.x;
  tmp_camera.position.y = pos.y;
  tmp_camera.position.z = pos.z;
  tmp_camera.lookAt(new THREE.Vector3());
  return tmp_camera;
}

function createPrespectiveCamera(pos) {
    'use strict';
    var tmp_camera = new THREE.PerspectiveCamera(70,
                                         window.innerWidth / window.innerHeight,
                                         1,
                                         1000);
    tmp_camera.position.x = pos.x;
    tmp_camera.position.y = pos.y;
    tmp_camera.position.z = pos.z;
    tmp_camera.lookAt(new THREE.Vector3());
    return tmp_camera;
}


/*
  Camera Resizers

  resizeOrtographicCamera
  resizePrespectiveCamera

*/

function resizeOrtographicCamera(cam) {
  if (window.innerHeight > 0 && window.innerWidth > 0) {
    cam.left = window.innerWidth / - CAMERA_RATIO;
    cam.right = window.innerWidth / CAMERA_RATIO;
    cam.top = window.innerHeight / CAMERA_RATIO; 
    cam.bottom = window.innerHeight / - CAMERA_RATIO;
    cam.updateProjectionMatrix();
  }
}

function resizePrespectiveCamera(cam) {
  if (window.innerHeight > 0 && window.innerWidth > 0) {
    cam.aspect = window.innerWidth / window.innerHeight;
    cam.updateProjectionMatrix();
  }
}


function random(min, max) {
  return min + (max-min) * Math.random();
}