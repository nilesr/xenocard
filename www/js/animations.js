function animate_draw(notification, callback) {
	const me = notification.player == notification.extras.side;
	const deck = document.getElementById(me ? "deck" : "enemy_deck");
	const box = deck.getBoundingClientRect();
	const root = document.createElement("div");
	const drawn_card = me ? notification.hand[notification.hand.length - 1] : FLIPPED_CARD;
	const translate_adj = me ? 1 : -1;
	const translate_x = me ? 0 : window.innerWidth/4;
	root.style.position = "fixed";
	root.style.top = box.top + "px";
	root.style.left = box.left + "px";
	root.style.zIndex = "50";
	ReactDOM.render(<Card card={FLIPPED_CARD} scale={deck.getAttribute("data-scale")} />, root);
	document.body.appendChild(root);
	anime({
		targets: root,
		easing: "linear",
		translateY: -260 * translate_adj,
		scale: 2,
		complete: function() {
			anime({
				targets: root,
				easing: "linear",
				rotateY: 90,
				duration: 500,
				complete: function() {
					ReactDOM.render(<Card card={drawn_card} scale={deck.getAttribute("data-scale")} />, root);
					anime({
						targets: root,
						easing: "linear",
						duration: 500,
						rotateY: 0,
						complete: function() {
							anime({
								targets: root,
								easing: "linear",
								translateY: 0,
								duration: 400,
								translateX: translate_x,
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
