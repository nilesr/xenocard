window.v = false;

function makeListener(down) {
	return function keychange(event) {
		if (event.repeat) return;
		event = event || window.event;
		var x = event.key;
		if (x == "v") {
			window.v = down;
			console.log("v: " + window.v);
		}
	}
}

document.onkeydown = makeListener(true);
document.onkeyup = makeListener(false);

ReactDOM.render(<App />, document.getElementById("root"));
