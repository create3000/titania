var buttons = new Array();
function button (name) {
	args=button.arguments;
	buttons[name] = new Array();
	for (i=1; i<(args.length); ++i) {
		buttons[name][i-1] = new Image();
		buttons[name][i-1].src = args[i];
	}
}

function button_over (name) {
	findObject(name).src = buttons[name][1].src;
}

function button_down (name) {
	findObject(name).src = buttons[name][2].src;
}

function button_out (name) {
	findObject(name).src = buttons[name][0].src;
}
