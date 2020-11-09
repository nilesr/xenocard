function animate_draw(notification, callback) {
	const deck = document.getElementById("deck");
	const box = deck.getBoundingClientRect();
	const root = document.createElement("div");
	root.style.position = "fixed";
	root.style.top = box.top + "px";
	root.style.left = box.left + "px";
	root.style.zIndex = "50";
	ReactDOM.render(<Card card={FLIPPED_CARD} scale={deck.getAttribute("data-scale")} />, root);
	document.body.appendChild(root);
	anime({
		targets: root,
		easing: "linear",
		translateY: -260,
		scale: 2,
		complete: function() {
			anime({
				targets: root,
				easing: "linear",
				rotateY: 90,
				complete: function() {
					ReactDOM.render(<Card card={notification.hand[notification.hand.length - 1]} scale={deck.getAttribute("data-scale")} />, root);
					anime({
						targets: root,
						easing: "linear",
						rotateY: 0,
						complete: function() {
							anime({
								targets: root,
								easing: "linear",
								translateY: 0, // TODO!!!!
								complete: function() {
									// We are done
									ReactDOM.unmountComponentAtNode(root);
									root.remove();
									callback();
								}
							})
						}
					})
				}
			})
		}
	})
}
