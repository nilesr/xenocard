class Hand extends React.Component {
	render() {
		const scale = 0.5;
		return this.props.cards.map((card, i) => {
			const position = card.image_location;
			return <div
				key={i}
				style={{backgroundImage: "url(images/cards" + position.set + ".jpg)", backgroundPosition: "-" + (position.left * 700 * scale) + "px -" + (position.top * 500 * scale) + "px", backgroundSize: (7000*scale) + "px", width: (700*scale) + "px", height: (500*scale) + "px", borderRadius: (10*scale) + "px", display: "inline-block"}}
				/>;
		});
	}
}

window.Hand = Hand;
