class Card extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			mouseover: false,
		};
	}
	render() {
		const scale = this.props.scale || 1;
		const style = {
			width: (688*scale) + "px",
			height: (488*scale) + "px",
			borderRadius: (15*scale) + "px",
			display: "inline-block",
		};
		if (this.props.card) {
			const position = this.props.card.image_location;
			style.backgroundImage = "url(images/cards" + position.set + ".jpg)";
			style.backgroundPosition = "-" + ((position.left * 700 + 6) * scale) + "px -" + ((position.top * 500 + 6) * scale) + "px";
			style.backgroundSize = (7000*scale) + "px";
		}
		// TODO: e, down, weapon, hover, onclick
		return <>
				<div style={style}
				onClick={() => { if (this.props.onClick) this.props.onClick(); }}
				onMouseEnter={() => this.setState({mouseover: true})}
				onMouseLeave={() => this.setState({mouseover: false})}>
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
						<Card card={this.props.card} scale={(window.innerHeight - 200) / 500} />
					</div>
				: null}
		</>
	}
}

window.Card = Card;


