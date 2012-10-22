function initialize() {
	var angle,
		angleFactor = (2 * (Math.PI / sides));

	// compute the crossSection
	with (Math) {
		for (var i = 0; i <= sides; i++) {
			angle = (i * angleFactor);
			crossSection_changed[i] = new SFVec2f((radius * cos(angle)),
												 -(radius * sin(angle)));
		}
	}
}

function set_radius(value, time) {
	radius = value;
	initialize();
}

function set_sides(value, time) {
	sides = value;
	initialize();
}
