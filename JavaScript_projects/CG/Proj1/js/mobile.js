/*global THREE, requestAnimationFrame, console*/

/*
  Constants
*/
const LEFT_ARROW = 37, RIGHT_ARROW = 39, UP_ARROW = 38, DOWN_ARROW = 40;
const CAMERA_ORTOG = 49, CAMERA_SIDE = 50, CAMERA_TOP = 51; // 1, 2, 3
const WIREFRAME = 52; // 4

const CAMERA_RATIO = 24;

/*
	Global variables
*/

var camera, scene, renderer;

var ortogonal_camera, front_camera, top_camera;

var clock;


// String representation of the Mobile
var mobileRepr = `
    0
x-----------/
           |
       o-------------h
                 |
            x-------------s
                     |
                 d----------/
                           1
                        x-------------d
                                  |
                              s-------------d
                                        2
                                    o-------------x
                                              |
                                           v------d
`;

// Each character correlates to a shape described here
var shapeDict= {
    "x": ["square", 0xff0000],
    "d": ["elipse", 0x00ff00],
    "o": ["cyl", 0x0000ff],
    "s": ["square", 0xff00ff],
    "h": ["cyl", 123123],
    "v": ["elipse", 0x164e53],
}

// Each pair of keys represents the rotation keys for each level of the mobile
var rotKeys = [[81, 87], [65, 68], [90, 67]];


// Array of Object3D for each level of the mobile
var mobiles = [];
// Rotation array for each level of the mobile
var rots = []
// Rotation speed (angular velocity)
const rot_speed = Math.PI/2;
// Vector that describes the movement of the mobile at any given moment
var mobileMoves
// Movement speed
const mov_speed = 15;

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

function createBox(pos, thicc, side, color) {
  var material = getMaterial(color);
  var geometry = new THREE.BoxBufferGeometry(side, side, thicc);
  
  var mesh = new THREE.Mesh(geometry, material);
  mesh.position.set(pos.x, pos.y, pos.z);
  
  return mesh;
}

function createCyl(pos, radius, height, color, xyz_rot) {
    var material = getMaterial(color);
    var geometry = new THREE.CylinderBufferGeometry(radius, radius, height, 10);
    
    var mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(pos.x, pos.y, pos.z);

    mesh.rotateX(xyz_rot.x);
    mesh.rotateY(xyz_rot.y);
    mesh.rotateZ(xyz_rot.z);

    return mesh;
}

/*
	Mobile Pieces Creators

	createSquarePiece
	createCylPiece
	createElipsePiece
	createWirePiece

*/

function createSquarePiece(x,y, size, color) {
	return createBox(new THREE.Vector3(x, y, 0),
                   0.5, 
                   size, 
                   color);
}

function createCylPiece(x,y, size, color) {
	return createCyl(new THREE.Vector3(x, y, 0), 
                   size, 
                   0.5, 
                   color, 
                   new THREE.Vector3(Math.PI/2,0,0));
}

function createElipsePiece(x,y ,size, color) {
	tmp =  createCyl(new THREE.Vector3(x, y, 0), 
                   size, 
                   0.5, 
                   color, 
                   new THREE.Vector3(Math.PI/2,0,0));

	tmp.scale.z = 1/2;
	return tmp
}

function createWirePiece(start,length, y) {
	return createCyl(new THREE.Vector3(start + length/2, y, 0), 
                   0.2, 
                   length, 
                   0xaaaaaa, 
                   new THREE.Vector3(0,0,Math.PI/2));
}

function createStemPiece(x, y, size) {
	return createCyl(new THREE.Vector3(x, y, 0), 
                   0.2, 
                   size, 
                   0xaaaaaa, 
                   new THREE.Vector3());
}


function createCamera(pos) {
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
  tmp_camera.lookAt(scene.position);
  return tmp_camera;
}


function createScene() {
  scene = new THREE.Scene();

  var curObj = scene;

  // Dimencoes de cada seccao de mobile (x,y,z)
  const size = new THREE.Vector3(1, 4, 0);

  // Iteradores
  var x = 0, y = 0, i = 0;

  // Dimensoes do mobile
  var x_max = 0, y_max = 0;

  // Auxiliares a calculos de posicões
  var x_off = 0;
  var cyl_start = -1, cyl_len = 0;
  

  while(i < mobileRepr.length) {
    while(mobileRepr[i] != '\n') {
      switch(mobileRepr[i]) {
      	case " ": // Optimization
      		break;

        case '0': // New level, extra cases excluded for clarity
        case '1':
        case '2':
          // Creates a new nested Object3D in the location of the new mobile level
          var tmpObj = new THREE.Object3D();
          tmpObj.position.x = size.x * (x-x_off+0.5);
          tmpObj.position.y = -size.y * Math.floor(y/2);
          curObj.add(tmpObj);
          curObj = tmpObj;
          
          rots.push(0);

    
          mobiles.push(tmpObj);
          y = 0;
         	
         	// Adds a stem (cylinder) as the start of the level
         	curObj.add(createStemPiece(0,
          													 -size.y * 0.5,
          													 size.y));
          x_off = x;
          x = 0;
          break;
          
        
        case '|':
          // Adds a stem (cylinder) to the next level
          curObj.add(createStemPiece(size.x * (x-x_off+0.5),
          													 -size.y * (Math.floor(y/2)+0.5),
          													 size.y));
          break;

        case '-':
          // Starts to measure the new horizontal wire for this level of the mobile
          if(cyl_start == -1)
            cyl_start = x;
          cyl_len++;
          break;

        default:

          if(cyl_start != -1) { // Reached end of cylinder
          	// Adjustments to prevent the ends of the wire clipping in the pieces
          	cyl_start += 0.5;
          	cyl_len -= 1;
          	curObj.add(createWirePiece(size.x * (cyl_start - x_off), // Start
          														 size.x * cyl_len, 	// Length
          														 -size.y * (Math.floor(y/2) + 1))); // Y position
            
            // Reset wire calculation
            cyl_start = -1;
            cyl_len = 0;
          }



          if(mobileRepr[i] == "/") // "/" represents end of cylinder but no shape is required
            break;

          var shape = shapeDict[mobileRepr[i]];
          switch(shape[0]) {
            case "square":
            	curObj.add(createSquarePiece(size.x * (x-x_off+0.5),
            															 -size.y * (Math.floor(y/2) + 1),
            															 size.x*2,
            															 shape[1]));
              break;

            case "cyl":
            	curObj.add(createCylPiece(size.x * (x-x_off+0.5), 
            														-size.y * (Math.floor(y/2) + 1),
            														Math.abs(size.x),
            														shape[1]));
              break;

             case "elipse":
             	curObj.add(createElipsePiece(size.x * (x-x_off+0.5), 
	            														 -size.y * (Math.floor(y/2) + 1),
	            														 Math.abs(size.x),
	            														 shape[1]));
              break;

          }
          break;

      }   
      i++;     
      x++;
    }

    i++;

    x = 0;
    y++;
  }

  i = 0;
  x = 0
  while(i < mobileRepr.length) {    // Calculates de dimentions of the mobile to center it later
    while(mobileRepr[i] != '\n') {
      x++;
      i++;
    }
    if(x > x_max)
      x_max = x;
    i++;
    x = 0;
    y_max++;
  }

  mobiles[0].position.y += y_max/2 * size.y / 2;  // Centers the mobile
  mobiles[0].position.x -= x_max * size.x / 2;

  mobileMoves = new THREE.Vector2(); 
}



function init() {
    'use strict';
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    clock = new THREE.Clock();

    createScene();
    ortogonal_camera = createCamera(new THREE.Vector3(50,50,50)); // Ortogonal camera
    front_camera = createCamera(new THREE.Vector3(0,0,50)); // Front camera
    top_camera = createCamera(new THREE.Vector3(0,50,0)); // Top camera

    camera = ortogonal_camera; // Default Camera
    
    clock.start();
    render();
    
    window.addEventListener("keydown", onKeyDown);
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
}


/*
    Animation

    animate
    render
*/

function animate() {
    'use strict';

    var deltatime = clock.getDelta();

    // Rotates all the mobiles
    for(var x = 0; x < rots.length;++x) 
      mobiles[x].rotateY(rots[x] * deltatime);


    // Maintains a constant speed in all directions
    var tmp_movement = mobileMoves.clone().normalize();

    mobiles[0].position.x += tmp_movement.x * mov_speed * deltatime;
    mobiles[0].position.z += tmp_movement.y * mov_speed * deltatime;

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
    	resizeCamera
*/

function onKeyDown(e) {
  'use strict';
  
  switch (e.keyCode) {
  case CAMERA_ORTOG:
    camera = ortogonal_camera;
    break;
  case CAMERA_SIDE:
    camera = front_camera;
    break
  case CAMERA_TOP:
    camera = top_camera;
    break;

  case LEFT_ARROW:
    mobileMoves.x = -1;
    break;

  case UP_ARROW:
    mobileMoves.y = -1;
    break;

  case RIGHT_ARROW:
    mobileMoves.x = 1;
    break;

  case DOWN_ARROW:
    mobileMoves.y = 1;
    break;

  case WIREFRAME:
  	for (var mat in materialCache) {
  		materialCache[mat].wireframe = !materialCache[mat].wireframe;
  	}
  }

  e = e.keyCode;
  for(var x = 0; x < rotKeys.length; ++x) {
    if (e == rotKeys[x][0]) {
      rots[x] = rot_speed;
    } else if (e == rotKeys[x][1]) {
      rots[x] = -rot_speed;
    }
  }


}

function onKeyUp(e) {
  e = e.keyCode;
  for(var x = 0; x < rotKeys.length; ++x) {
    if (e == rotKeys[x][0] || e == rotKeys[x][1]) {
      rots[x] = 0;
    }
  }

  switch(e) {
    case LEFT_ARROW:
    case RIGHT_ARROW:
      mobileMoves.x = 0;
      break;

    case UP_ARROW:
    case DOWN_ARROW:
      mobileMoves.y= 0;
      break;
  }
}

function resizeCamera(cam) {
  if (window.innerHeight > 0 && window.innerWidth > 0) {
    cam.left = window.innerWidth / - CAMERA_RATIO;
    cam.right = window.innerWidth / CAMERA_RATIO;
    cam.top = window.innerHeight / CAMERA_RATIO; 
    cam.bottom = window.innerHeight / - CAMERA_RATIO;
    cam.updateProjectionMatrix();
  }
}

function onResize() {
    'use strict';

    renderer.setSize(window.innerWidth, window.innerHeight);
    
    resizeCamera(ortogonal_camera);
    resizeCamera(front_camera);
    resizeCamera(top_camera);

}