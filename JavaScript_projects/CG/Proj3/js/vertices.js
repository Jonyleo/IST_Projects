const cybertruck_vert =  new Float32Array ([
		// Left Side Panel
		0, 10, 6,
		-21, 3, 10,
		25, 4, 10,
		
		-21, 3, 10,
		-21, 0, 10,
		25, 4, 10,

		25, 4, 10,
		-21, 0, 10,
		24, 0, 10,

		// Right Side Panel
		-21, 3, -10,
		0, 10, -6,
		25, 4, -10,
		
		-21, 0, -10,
		-21, 3, -10,
		25, 4, -10,

		-21, 0, -10,
		25, 4, -10,
		24, 0, -10,

		// Top Side Panel

		0, 10, -6,
		0, 10, 6,
		25, 4, -10,

		25, 4, -10,
		0, 10, 6,
		25, 4, 10,

		0, 10, -6,
		-16.8, 4.4, -9.2,
		-16.8, 4.4, 9.2,

		0, 10, -6,
		-16.8, 4.4, 9.2,
		0, 10, 6,

		-16.8, 4.4, -9.2,
		-21, 3, -10,
		-16.8, 4.4, 9.2,

		-21, 3, -10,
		-21, 3, 10,
		-16.8, 4.4, 9.2,

		-21, 3, -10,
		-25, 2, 5,
		-21, 3, 10,

		-21, 3, -10,
		-25, 2, -5,
		-25, 2, 5,

		// Front Panel
		-25, 1.5, -5,
		-25, 0, -5,
		-25, 0, 5,

		-25, 1.5, -5,
		-25, 0, 5,
		-25, 1.5, 5,

		-21, 2.5, 10,
		-25, 1.5, 5,
		-21, 0, 10,

		-25, 1.5, 5,
		-25, 0, 5,
		-21, 0, 10,

		-25, 1.5, -5,
		-21, 2.5, -10,
		-21, 0, -10,

		-25, 0, -5,
		-25, 1.5, -5,
		-21, 0, -10,

		// Back Panel
		24.75, 3.5, 10,
		24, 0, 10,
		24, 0, -10,

		24.75, 3.5, 10,
		24, 0, -10,
		24.75, 3.5, -10,
]);

var offset = 0.12;
const windows_vert = new Float32Array ([
	0, 9.05 + offset, 6.4 + offset,
	-15.75, 3.8 + offset, 9.4 + offset,	
	9.38, 6.8 + offset, 7.9 + offset,

	9.38, 6.8 + offset, 7.9 + offset,
	-15.75, 3.8 + offset, 9.4 + offset,
	9.38, 4.4 + offset, 9.4 + offset,

	-15.75, 3.8 + offset, -9.4 - offset,	
	0, 9.05 + offset, -6.4 - offset,
	9.38, 6.8 + offset, -7.9 - offset,

	-15.75, 3.8 + offset, -9.4 - offset,
	9.38, 6.8 + offset, -7.9 - offset,
	9.38, 4.4 + offset, -9.4 - offset,

	-1.68 - offset, 9.44 + offset, -5.12, 
	-15.12 - offset, 4.96 + offset, -7.04,
	-15.12 - offset, 4.96 + offset, 7.04,

	-1.68 - offset, 9.44 + offset, -5.12, 
	-15.12 - offset, 4.96 + offset, 7.04,
	-1.68 - offset, 9.44 + offset, 5.12, 

]);

const front_light = new Float32Array ([
	// Front Panel
		-25, 2, -5,
		-25, 1.5, -5,
		-25, 1.5, 5,

		-25, 2, -5,
		-25, 1.5, 5,
		-25, 2, 5,

		-21, 3, 10,
		-25, 2, 5,
		-21, 2, 10,

		-25, 2, 5,
		-25, 1.5, 5,
		-21, 2, 10,

		-25, 2, -5,
		-21, 3, -10,
		-21, 2, -10,

		-25, 1.5, -5,
		-25, 2, -5,
		-21, 2, -10,
]);

const back_light = new Float32Array ([
	25, 4, 10,
	24.75, 3.5, 10,
	24.75, 3.5, -10,

	25, 4, 10,
	24.75, 3.5, -10,
	25, 4, -10,
]);

