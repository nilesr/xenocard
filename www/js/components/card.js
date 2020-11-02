class Card extends React.Component {
	render() {
		const card = this.props.card;
		const position = card.image_location;
		const scale = this.props.scale || 1;
		// TODO: e, down, weapon, hover, onclick
		return <div style={{
				backgroundImage: "url(images/cards" + position.set + ".jpg)",
				backgroundPosition: "-" + (position.left * 700 * scale) + "px -" + (position.top * 500 * scale) + "px",
				backgroundSize: (7000*scale) + "px",
				width: (700*scale) + "px",
				height: (500*scale) + "px",
				borderRadius: (10*scale) + "px",
				display: "inline-block",
			}}
		/>;
	}
}

window.Card = Card;


