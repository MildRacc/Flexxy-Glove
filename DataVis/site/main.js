import * as THREE from "./node_modules/three/build/three.module.js";
import { OrbitControls } from "./node_modules/three/examples/jsm/controls/OrbitControls.js";

const ws = new WebSocket("ws://localhost:9002/");

const thumbVal = document.getElementById("thumbVal")
const indexVal = document.getElementById("indexVal")
const middleVal = document.getElementById("middleVal")
const ringVal = document.getElementById("ringVal")
const pinkyVal = document.getElementById("pinkyVal")

const gestureVal = document.getElementById("gestureVal")


const handValues = {
  MAX: 4095,
  SEMICLOSED: 3072,
  MID: 2048,
  SEMIOPEN: 1024,
  MIN: 0,
}


// [thumb[], index[], middle[], ring[], pinky[]]
var digits =[[],[],[],[],[]]


let gesture = "";
let values = [0, 0, 0, 0, 0]
let offsets = [0, 0, 0, 0, 0]

ws.onopen = function () {
  console.log("Connected to WebSocket server");
};

ws.onmessage = function (event) {
  const message = event.data;

  // Parse the received message
  const regex = /([a-eG])(\d+|[a-zA-Z]+)/g;
  let match;
  while ((match = regex.exec(message)) !== null) {
    const label = match[1];
    const value = match[2];
    if (label >= 'a' && label <= 'e') {
      const index = label.charCodeAt(0) - 'a'.charCodeAt(0);
      values[index] = parseInt(value, 10);
    } else if (label === 'G') {
      gesture = value;
    }
  }


  // Log the parsed values
  console.log("Parsed values:", values);

  // Update the corresponding HTML elements
  thumbVal.textContent = values[0];
  indexVal.textContent = values[1];
  middleVal.textContent = values[2];
  ringVal.textContent = values[3];
  pinkyVal.textContent = values[4];
};

ws.onclose = function () {
  console.log("Disconnected from WebSocket server");
};

ws.onerror = function (error) {
  console.log("WebSocket error: " + error);
};


const scene = new THREE.Scene();

const renderer = new THREE.WebGLRenderer({ antialias: true });

const container = document.getElementById("threecontainer");

renderer.setSize(container.clientWidth, container.clientHeight);
renderer.setClearColor(0x0f0f16, 1);

container.appendChild(renderer.domElement);

const camera = new THREE.PerspectiveCamera(
  75,
  container.clientWidth / container.clientHeight,
  0.1,
  1000
);
const controls = new OrbitControls(camera, renderer.domElement);

createHand();

camera.position.z = 5;



/*============ MAIN ANIMATE FUNCTION ============*/
function animate() {
  renderer.render(scene, camera);
  animateFingers()
  updateGesture()
  updateDataVisualization()
}
/*============ MAIN ANIMATE FUNCTION ============*/



renderer.setAnimationLoop(animate);

function baseMat(hex) {
  return new THREE.MeshBasicMaterial({ color: hex });
}

function createHand() {
  // Create a hand
  // Width, Height, Depth -> X, Z, Y
  const handBaseMesh = new THREE.Mesh(
    new THREE.BoxGeometry(1.125, 0.25, 1.25),
    baseMat(0x999999)
  );
  // Thumb
  const thumbOneMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.45, 0.235, 0.85),
    baseMat(0xbb0000)
  );
  digits[0][0] = thumbOneMesh
  const thumbTwoMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3125, 0.23, 0.55),
    baseMat(0xdd0000)
  );
  digits[0][1] = thumbTwoMesh
  const thumbThreeMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3, 0.225, 0.35),
    baseMat(0xff0000)
  );
  digits[0][2] = thumbThreeMesh
  // Index
  const indexOneMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3, 0.225, 0.6),
    baseMat(0x00bb00)
  );
  digits[1][0] = indexOneMesh
  const indexTwoMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.275, 0.225, 0.5),
    baseMat(0x00dd00)
  );
  digits[1][1] = indexTwoMesh
  const indexThreeMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.25, 0.225, 0.4),
    baseMat(0x00ff00)
  );
  digits[1][2] = indexThreeMesh

  // Middle
  const middleOneMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3, 0.225, 0.65),
    baseMat(0x0000bb)
  );
  digits[2][0] = middleOneMesh
  const middleTwoMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.275, 0.225, 0.55),
    baseMat(0x0000dd)
  );
  digits[2][1] = middleTwoMesh
  const middleThreeMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.255, 0.225, 0.45),
    baseMat(0x0000ff)
  );
  digits[2][2] = middleThreeMesh

  // Ring
  const ringOneMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3, 0.225, 0.6),
    baseMat(0xbb00bb)
  );
  digits[3][0] = ringOneMesh
  const ringTwoMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.275, 0.225, 0.5),
    baseMat(0xdd00dd)
  );
  digits[3][1] = ringTwoMesh
  const ringThreeMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.255, 0.225, 0.4),
    baseMat(0xff00ff)
  );
  digits[3][2] = ringThreeMesh

  // Pinky
  const pinkyOneMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.3, 0.225, 0.5),
    baseMat(0x00bbbb)
  );
  digits[4][0] = pinkyOneMesh
  const pinkyTwoMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.275, 0.225, 0.4),
    baseMat(0x00dddd)
  );
  digits[4][1] = pinkyTwoMesh
  const pinkyThreeMesh = new THREE.Mesh(
    new THREE.BoxGeometry(0.255, 0.225, 0.3),
    baseMat(0x00ffff)
  );
  digits[4][2] = pinkyThreeMesh

  // Add hand
  handBaseMesh.add(thumbOneMesh);
  thumbOneMesh.add(thumbTwoMesh);
  thumbTwoMesh.add(thumbThreeMesh);

  handBaseMesh.add(indexOneMesh);
  indexOneMesh.add(indexTwoMesh);
  indexTwoMesh.add(indexThreeMesh);

  handBaseMesh.add(middleOneMesh);
  middleOneMesh.add(middleTwoMesh);
  middleTwoMesh.add(middleThreeMesh);

  handBaseMesh.add(ringOneMesh);
  ringOneMesh.add(ringTwoMesh);
  ringTwoMesh.add(ringThreeMesh);

  handBaseMesh.add(pinkyOneMesh);
  pinkyOneMesh.add(pinkyTwoMesh);
  pinkyTwoMesh.add(pinkyThreeMesh);

  scene.add(handBaseMesh);

  // Position Hand
  handBaseMesh.rotateX(0.45);

  // Position Thumb One
  thumbOneMesh.translateX(-0.775);
  thumbOneMesh.translateY(-0.15);
  thumbOneMesh.translateZ(0.25);
  thumbOneMesh.rotateX(-0.45);
  thumbOneMesh.rotateY(-1.53);
  thumbOneMesh.rotateZ(1.2);

  // Position Thumb Two
  thumbTwoMesh.translateZ(-0.65);
  thumbTwoMesh.translateX(-0.05);

  // Position Thumb Three
  thumbThreeMesh.translateZ(-0.45);

  // Position Index One
  indexOneMesh.translateZ(-0.815);
  indexOneMesh.translateX(-0.525);
  indexOneMesh.rotateY(0.125);

  // Position Index Two
  indexTwoMesh.translateZ(-0.55);

  //Position Index Three
  indexThreeMesh.translateZ(-0.45);

  // Position Middle One
  middleOneMesh.translateZ(-0.915);
  middleOneMesh.translateX(-0.175);
  middleOneMesh.rotateY(0.05);

  // Position Middle Two
  middleTwoMesh.translateZ(-0.575);

  // Position Middle Three
  middleThreeMesh.translateZ(-0.45);

  // Position Ring One
  ringOneMesh.translateZ(-0.915);
  ringOneMesh.translateX(0.175);
  ringOneMesh.rotateY(-0.035);

  // Position Ring Two
  ringTwoMesh.translateZ(-0.55);

  // Position Ring Three
  ringThreeMesh.translateZ(-0.45);

  // Position Pinky One
  pinkyOneMesh.translateZ(-0.715);
  pinkyOneMesh.translateX(0.55);
  pinkyOneMesh.rotateY(-0.235);

  // Position Pinky Two
  pinkyTwoMesh.translateZ(-0.45);

  // Position Pinky Three
  pinkyThreeMesh.translateZ(-0.35);
}


function animateFingers() {
  for (let i = 0; i < digits.length; i++) {
    for (let j = 0; j < digits[i].length; j++) {
      const digit = digits[i][j];
      const value = values[i];
      const targetY = 0.225 + (value / 100) * 0.225;
      const diff = targetY - digit.scale.y;
      if (i === 0 && j === 0) {
        digit.rotation.y = (Math.PI * 2) / 360 * diff * -0.95;
        digit.rotation.z =  (0.93) + (Math.PI * 2) / 360 * diff * -0.95;
      } else {
        digit.rotation.x = (Math.PI * 2) / 360 * diff * -8
        digit.position.y = (0.225 + diff) * -0.05
      }
    }
    for(let i = 1; i < digits.length; i++) {
      const base = digits[i][0]
      base.rotation.y = offsets[i] + (Math.PI * 2) / 360 * offsets[i] * -0.85;
    }
  }
}


function updateGesture(){
  gestureVal.textContent = gesture;
}


function updateDataVisualization(){

  // dataVis.addData(0, values[0]);
  // dataVis.addData(1, values[1]);
  // dataVis.addData(2, values[2]);
  // dataVis.addData(3, values[3]);
  // dataVis.addData(4, values[4]);
  

}