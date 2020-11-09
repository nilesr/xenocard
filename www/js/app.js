window.v = false;

function makeListener(down) {
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
	}
}

document.onkeydown = makeListener(true);
document.onkeyup = makeListener(false);

ReactDOM.render(<App />, document.getElementById("root"));
