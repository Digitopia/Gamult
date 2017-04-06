var a1, a2, a3;

var particleArray = [];

var counter = 0;

function preload() {
	a1 = loadSound('sounds/A_KSL2.wav');
	a2 = loadSound('sounds/A_KSL3.wav');
	a3 = loadSound('sounds/A_KSL5.wav');
}

function setup() {
	canvas = createCanvas(windowWidth,windowHeight);
}

function update() {
	

}
function draw() {

	background(250);

	stroke(200);
	line(windowWidth/3, 0, windowWidth/3, windowHeight);
	line(windowWidth/3 * 2, 0, windowWidth/3 * 2, windowHeight);

	for(var i = 0; i<particleArray.length; i++) {
		if(particleArray[i] !== null) {
			particleArray[i].gravity();
			particleArray[i].draw();
		}
		
	}

	for(var i = 0; i<particleArray.length; i++) {
		if(particleArray[i].getHealth() <= 0) particleArray.splice(i, 1);
	}

	particleArray = particleArray.filter(function(n){ return n != undefined }); //removes all undefined elements from array

}

function windowResized() {

}

function mousePressed() {

	/*for(var i = 0; i<counter; i++) {
		if(particleArray[i] == null) {
			particleArray[i] = new Particle(0, counter, mouseX, mouseY, 20);
			return;
		}
	}*/

	particleArray[particleArray.length] = new Particle(0, counter, mouseX, mouseY, 40);


}