class Card extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			mouseover: false,
		};
	}
	render() {
		const scale = this.props.scale || 1;
		const width = 668*scale;
		const height = 488*scale;
		const style = {
			width: width + "px",
			height: height + "px",
			borderRadius: (15*scale) + "px",
			display: "inline-block",
		};
		let e = null;
		if (this.props.card) {
			const position = this.props.card.image_location;
			style.backgroundImage = "url(images/cards" + position.set + ".jpg)";
			style.backgroundPosition = "-" + ((position.left * 700 + 6) * scale) + "px -" + ((position.top * 500 + 6) * scale) + "px";
			style.backgroundSize = (7000*scale) + "px";
			if (this.props.card.e) {
				e = <img
					src="images/e.png"
					style={{
						position: "absolute",
						right: 0,
						bottom: "4px",
						width: (width/3) + "px",
						opacity: 0.55
					}} />;
			}
		}
		// TODO: down, weapon, damage counters if in standby
		return <>
				<div style={style}
				onClick={() => { if (this.props.onClick) this.props.onClick(); }}
				onMouseEnter={() => this.setState({mouseover: true})}
				onMouseLeave={() => this.setState({mouseover: false})}>
				{this.props.selected ? <CardOverlay /> : null}
				{e}
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


