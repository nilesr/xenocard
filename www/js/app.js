window.v = false;
window.rotate = false;
window.tilt = true;

const root = document.getElementById("root");

const render = function render() {
	const app = <App rotate={window.rotate} tilt={window.tilt} />;

	ReactDOM.render(app, root);
}

const makeListener = function makeListener(down) {
	return function keychange(event) {
		event = event || window.event;
		if (event.repeat) return;
		var x = event.key;
		if (x == "v") {
			window.v = down;
			Array.prototype.slice.call(document.querySelectorAll(".hovercard"), 0).forEach(e => {
				e.style.display = down ? "block" : "none";
			});
		}
		if (down && x == "r") {
			window.rotate = !window.rotate;
			render();
		}
		if (down && x == "t") {
			window.tilt = !window.tilt;
			render();
		}
	}
}

document.onkeydown = makeListener(true);
document.onkeyup = makeListener(false);
render();
