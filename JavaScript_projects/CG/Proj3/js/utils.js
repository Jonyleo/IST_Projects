/*global THREE, requestAnimationFrame, console*/

// Caching for materials of the same color
var colorMaterialCache = {};
var textureMaterialCache = {};


function createMaterialByType(text_type, obj) {
  switch(text_type) {
    case "lambert":
      return new THREE.MeshLambertMaterial(obj);
    case "phong":
      return new THREE.MeshPhongMaterial(obj);
    case "basic":
      return new THREE.MeshBasicMaterial(obj);
  }
}

function getMaterialByColor(text_type, color, extra_props) {
	if((color + text_type) in colorMaterialCache)
		return colorMaterialCache[color + text_type];
	
	var material  = createMaterialByType(text_type, Object.assign({ color: color, wireframe: false }, extra_props));
	colorMaterialCache[color + text_type] = material;

	return material;
}

function getMaterialByTexture(text_type, file_prefix, file_suffix) {
  if((file_prefix + text_type) in textureMaterialCache)
    return textureMaterialCache[file_prefix + text_type];

  var material = createMaterialByType(text_type, {});
  textureMaterialCache[file_prefix + text_type] = material;

  const textureLoader = new THREE.TextureLoader();
  textureLoader.load( file_prefix + "basecolor" + file_suffix, function ( map ) {
    material.map = map;
    material.needsUpdate = true;
  } );

  textureLoader.load( file_prefix + "normal" + file_suffix, function ( map ) {
    material.normalMap = map;
    material.needsUpdate = true;
  } );
  return material;
}

function getTexturesByTypes(types, file_prefix, file_suffix) {
  var text_dict = {}
  for(var x in types) {
    text_dict[types[x]] = getMaterialByTexture(types[x], file_prefix, file_suffix);
  }
  return text_dict;
}

function getColorByTypes(types, color, extra_props) {
  var color_dict = {}
  for(var x in types) {
    if(types[x] == "basic")
      color_dict[types[x]] = getMaterialByColor(types[x], color);
    else
      color_dict[types[x]] = getMaterialByColor(types[x], color, extra_props);
  }
  return color_dict;
}

/*
  Primitive Creators

  createBox
  createCyl

*/

function createBox(dim, mat) {
  if(! (mat instanceof THREE.Material))
    mat = getMaterialByColor("lambert", mat);

  var geometry = new THREE.BoxBufferGeometry(dim.x, dim.y, dim.z, 1000, 1, 1000);
  
  var mesh = new THREE.Mesh(geometry, mat);
  
  return mesh;
}

function createCyl(radius, height, mat, xyz_rot) {
  if(! (mat instanceof THREE.Material))
    mat = getMaterialByColor("lambert", mat);
  var geometry = new THREE.CylinderBufferGeometry(radius, radius, height, 100);
  
  var mesh = new THREE.Mesh(geometry, mat);

  mesh.rotateX(xyz_rot.x);
  mesh.rotateY(xyz_rot.y);
  mesh.rotateZ(xyz_rot.z);

  return mesh;
}

function createConeSec(rad_a, rad_b, height, mat, xyz_rot) {
  if(! (mat instanceof THREE.Material))
    mat = getMaterialByColor("lambert", mat);
  var geometry = new THREE.CylinderBufferGeometry(rad_a, rad_b, height, 10);
  
  var mesh = new THREE.Mesh(geometry, mat);

  mesh.rotateX(xyz_rot.x);
  mesh.rotateY(xyz_rot.y);
  mesh.rotateZ(xyz_rot.z);

  return mesh;
}

function createSphere(radius, mat) {
  if(! (mat instanceof THREE.Material))
    mat = getMaterialByColor("lambert", mat);

  var geometry = new THREE.SphereBufferGeometry(radius, 10, 10);
  
  return new THREE.Mesh(geometry, mat);
}

function createMesh(vertices, mat) {
  if(! (mat instanceof THREE.Material))
    mat = getMaterialByColor("lambert", mat);

  var geometry = new THREE.BufferGeometry();

  geometry.setAttribute( 'position', new THREE.BufferAttribute( vertices, 3 ) );
  geometry.computeVertexNormals();

  return new THREE.Mesh(geometry, mat);
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