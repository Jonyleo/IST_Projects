/*global THREE, requestAnimationFrame, console*/

/*
  Constants
*/
const DIRECTIONAL_LIGHT = 68; // d
const POINT_LIGHT = 80; // p
const WIREFRAME = 87;  // 2
const NO_LIGHT = 73;  // i
const BALL_MOVE = 66;  // b
const RESET = 82  // r
const PAUSE = 83  // s

const SPEED = 10;
const ROT_SPEED = Math.PI;

const camera_pos = new THREE.Vector3(0,40, 30);


/*
	Global variables
*/

var camera, scene, renderer, controls;
var pause_scene;
var pause_camera;

var camera;
var isPaused = false;

var clock;

var left = false, right = false;
var noLight = false;

var directionalLight;
var pointLight;

var grass;
var golf_ball;
var poste;
var isMoving = true;
var direction = 1;

function createScene() {
  "use strict";
  scene = new THREE.Scene();
  pause_scene = new THREE.Scene();

  var types = ["phong", "basic"];


  directionalLight = new THREE.DirectionalLight( 0xffffff, 0.5 );
  directionalLight.position.set(0, 100, 20);

  pointLight = new THREE.PointLight(0xffffff, 1, 100);
  pointLight.position.set(2,2,2);

  scene.add(pointLight);

  scene.add( directionalLight );
  scene.add( directionalLight.target );

  grass = new Grass(types, 10,10)
  scene.add(grass);

  golf_ball = new GolfBall(types)
  scene.add(golf_ball);

  poste = new THREE.Group();

  var tmp = createCyl(0.5, 20, 0xcccccc, new THREE.Vector3());
  tmp.position.y += 10;
  poste.add(new AlternateMaterial(getColorByTypes(types, 0xcccccc), tmp));
  

  tmp = createBox(new THREE.Vector3(6, 4, 0.1), 0xd47a31);
  tmp.position.y += 17;
  tmp.position.x += 3;
  poste.add(new AlternateMaterial(getColorByTypes(types, 0xd47a31), tmp));

  poste.position.z -= 7;

  scene.add(poste);

  const loader = new THREE.CubeTextureLoader();
  const texture = loader.load([
    "assets/cubemap/nx.png",
		"assets/cubemap/px.png",
		"assets/cubemap/ny.png",
		"assets/cubemap/nz.png",
		"assets/cubemap/py.png",
		"assets/cubemap/pz.png"
  ]);
  scene.background = texture;

  tmp = createPlane(new THREE.Vector3(126,37), getMaterialByTexture("basic", {"basecolor" : "assets/paused/pause.png"}));
  pause_scene.add(tmp);
}



function init() {
  'use strict';
  renderer = new THREE.WebGLRenderer({
      antialias: true,
  });
  renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.autoClear = false;

  document.body.appendChild(renderer.domElement);

  clock = new THREE.Clock();

  camera = createPrespectiveCamera(camera_pos);
  pause_camera = createPrespectiveCamera(new THREE.Vector3(0, 100, 0)); 

  controls = new THREE.OrbitControls( camera, renderer.domElement );

  createScene(); 
  
  window.addEventListener("keydown", onKeyDown);
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
  
  if(isMoving) {
    golf_ball.position.x += deltatime * SPEED * direction;
    golf_ball.position.y = -0.1 * golf_ball.position.x * golf_ball.position.x  + 20;

    if(golf_ball.position.y <= 1) {
      direction *= -1;
    }    
  }

  poste.rotateY(ROT_SPEED * deltatime);

  render();
  
  requestAnimationFrame(animate);
}

function render() {
  'use strict';
  renderer.render(scene, camera);
  renderer.clearDepth();
  if(isPaused) 
  	renderer.render(pause_scene, pause_camera);
 
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
  
    case DIRECTIONAL_LIGHT:
    	directionalLight.visible = ! directionalLight.visible;
    	break;

    case POINT_LIGHT:
      pointLight.visible = ! pointLight.visible;
      break;

    case NO_LIGHT:
    	var new_type;
    	if(noLight)
    		new_type = "phong";
    	else
    		new_type = "basic";

      noLight = !noLight;
    	for(var x in alternateMats) {
    		alternateMats[x].changeMaterialType(new_type);
    	}
    	break;

    case WIREFRAME:
      for(var x in colorMaterialCache)
        colorMaterialCache[x].wireframe = !colorMaterialCache[x].wireframe;
      golf_ball.wireframe();
      grass.wireframe();
      break;

    case BALL_MOVE:
      isMoving = ! isMoving;
      break;

    case RESET:
      if(!isPaused)
        return;

      golf_ball.position.set(0,20);
      direction = 1;
      isMoving = true;
      poste.rotation.set(0,0,0);
      poste.updateMatrix();

      noLight = false;
      for(var x in alternateMats) {
        alternateMats[x].changeMaterialType("phong");
      }

      pointLight.visible = true;
      directionalLight.visible = true;

      for(var x in colorMaterialCache)
        colorMaterialCache[x].wireframe = false;
      golf_ball.wireframe(false);
      grass.wireframe(false);

      camera.position.set(camera_pos.x, camera_pos.y, camera_pos.z);
      camera.updateMatrix();
      controls.update();

    case PAUSE:
  		if(isPaused)
  			clock.start();
			else
				clock.stop();
      isPaused = !isPaused;
      break;
  }
}


function onResize() {
  'use strict';

  renderer.setSize(window.innerWidth, window.innerHeight);
  
  resizeCamera(camera);
  resizeCamera(pause_camera);
}