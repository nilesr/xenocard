class Card extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			mouseover: false,
		};
	}
	render() {
		const card = this.props.card;
		const position = card.image_location;
		const scale = this.props.scale || 1;
		// TODO: e, down, weapon, hover, onclick
		return <>
				<div style={{
					backgroundImage: "url(images/cards" + position.set + ".jpg)",
					backgroundPosition: "-" + ((position.left * 700 + 6) * scale) + "px -" + ((position.top * 500 + 6) * scale) + "px",
					backgroundSize: (7000*scale) + "px",
					width: (688*scale) + "px",
					height: (488*scale) + "px",
					borderRadius: (15*scale) + "px",
					display: "inline-block",
				}}
				onClick={() => { if (this.props.onClick) this.props.onClick(); }}
				onMouseEnter={() => this.setState({mouseover: true})}
				onMouseLeave={() => this.setState({mouseover: false})}
			>
				{this.props.selected ? <CardOverlay /> : null}
			</div>
			{this.state.mouseover
				? <div style={{
						position: "fixed",
						top: 0,
						left: 0,
						pointerEvents: "none",
						zIndex: 100,
						display: window.v ? "block" : "none"
					}}
					className="hovercard">
						<Card card={this.props.card} scale={1.6} />
					</div>
				: null}
		</>
	}
}

window.Card = Card;


