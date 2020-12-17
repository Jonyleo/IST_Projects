/*global THREE, requestAnimationFrame, console*/

/*
  Constants
*/
const LEFT_ARROW = 37, RIGHT_ARROW = 39, UP_ARROW = 38, DOWN_ARROW = 40;
const CAMERA_TOP = 49, CAMERA_SIDE = 50, CAMERA_BALL = 51; // 1, 2, 3


const SPACE_BAR = 32;

const CAMERA_RATIO = 15;

/*
	Global variables
*/

var camera, scene, renderer;

var ball_camera, side_camera, top_camera;

var clock;

const start_balls = 15;
var balls = [];
var table;
var left = false;
var right = false;
var shoot = false;

var colors = [0xff0000, 0x0000ff, 0xffff00, 0x00ffff, 0xff00ff];

function createScene() {
  scene = new THREE.Scene();
  table = new Table(new THREE.Vector2(60,30), 10);

  scene.add(table);

  var pos = table.select(0);
  scene.add(ball_camera);
	ball_camera.position.set(pos.x, 10, pos.y);
	ball_camera.lookAt(pos);

  for(var x = 0; x < start_balls; ++x) {
    var ball = new Ball(1, colors[Math.floor(random(0,5))]);

    while(true) {
      var new_pos = new THREE.Vector3(random(-28, 28), 1.5, random(-13, 13));
      var next_ball = false;

      for(y = 0; y < balls.length; ++y) {
        if(new_pos.distanceTo(balls[y].getWorldPosition()) <= (3 + balls[y].radius)) {
          next_ball = true;
          break;
        }
      }
      if(next_ball)
        continue;

      table.add(ball);
      ball.position.set(new_pos.x, new_pos.y, new_pos.z);
      ball.speed.set(random(-10,10), 0, random(-10,10));
      balls.push(ball);
      break;
    }
  }

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

    
    top_camera = createOrtographicCamera(new THREE.Vector3(0,100,0)); 
    side_camera = createPrespectiveCamera(new THREE.Vector3(0,50,20)); 
    ball_camera = createPrespectiveCamera(new THREE.Vector3(0,0,0));
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

    balls.forEach(ball => ball.update(deltatime));

    for (var x = 0; x < physicsObjects.length; ++x) {
      for (var y = 0; y < physicsObjects.length; ++y) {
        if (x != y && physicsObjects[x].is_coliding(physicsObjects[y])) {
          colisions.push([x, y]);
        }
      }
    }

    if(left && !right) {
      table.rotate_stick(-deltatime);
    } else if(right && !left) {
      table.rotate_stick(deltatime);
    }
    

    for(var x = 0; x < table.holes.children.length; ++x) {
      for(var y = 0; y < balls.length; ++y) {
        if(balls[y].speed.y == 0 && table.holes.children[x].is_inside(balls[y])) {
          balls[y].position.set(table.holes.children[x].position.x, table.holes.children[x].position.y, table.holes.children[x].position.z);
          balls[y].speed.set(0,-15,0);
        }
      }
    }



    for (var x = 0; x < colisions.length; ++x) {
      physicsObjects[colisions[x][0]].parent.colide(physicsObjects[colisions[x][1]].parent);
      physicsObjects[colisions[x][1]].parent.colide(physicsObjects[colisions[x][0]].parent);
    }

    for (var x = 0; x < colisions.length; ++x) {
      physicsObjects[colisions[x][0]].parent.update_speed();
      physicsObjects[colisions[x][1]].parent.update_speed();
    }

    colisions = [];

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
    case CAMERA_BALL:
      camera = ball_camera;
      break;

    case LEFT_ARROW:
        left = true;
        break;

    case RIGHT_ARROW:
        right = true;
        break;

    case SPACE_BAR:
        if(shoot)
          break;
        var ball = table.shoot(ball_camera);
        if(ball) {
        	table.add(ball);
        	balls.push(ball);
        }
        
        shoot = true;
        break;

  }

  var code = e.keyCode;

  if(code >= 52 && code <= 57) {
    code -= 52;
    
    var pos = table.select(code);
    scene.add(ball_camera);
		ball_camera.position.set(pos.x, 10, pos.y);
		ball_camera.lookAt(pos);
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

    case SPACE_BAR:
      shoot= false;
      break;
  } 
 
}

function onResize() {
    'use strict';

    renderer.setSize(window.innerWidth, window.innerHeight);
    
    resizeOrtographicCamera(top_camera);
    resizePrespectiveCamera(ball_camera);
    resizePrespectiveCamera(side_camera);
}