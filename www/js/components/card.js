class Card extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			mouseover: false,
			mouseoverweapon: false,
		};
	}
	render() {
		const scale = this.props.scale || 1;
		const width = (700-12)*scale;
		const height = (500-12)*scale;
		const style = {
			width: width + "px",
			height: height + "px",
			borderRadius: (15*scale) + "px",
			display: "inline-block",
		};
		let e = null;
		let health = null;
		let weapon = null;
		if (this.props.card) {
			// z-indexes
			// 0: card image itself (default, not set explicitly)
			// 5: card weapon (if present)
			// 8: card health (if present)
			// 9: down overlay (TODO)
			// 10: card overlay (the green overlay when a card is selected)
			const position = this.props.card.image_location;
			if (position.set == "flipped_card") {
				style.backgroundImage = "url(images/flipped_card.png)";
				style.backgroundSize = "cover";
			} else {
				style.backgroundImage = "url(images/cards" + position.set + ".jpg)";
				style.backgroundPosition = "-" + ((position.left * 700 + 6) * scale) + "px -" + ((position.top * 500 + 6) * scale) + "px";
				style.backgroundSize = (7000*scale) + "px";
			}
			if (this.props.rotate) {
				style.transform = "rotate(180deg)";
			}
			if (this.props.card.e && !this.props.from_hovercard) {
				e = <img
					src="images/e.png"
					style={{
						position: "absolute",
						right: 0,
						bottom: "4px",
						width: (width/3) + "px",
						opacity: 0.55,
						zIndex: 5,
					}} />;
			}
			if (this.props.card.weapon && !this.props.from_hovercard) {
				weapon = <div
					style={{
						position: "absolute",
						right: 0,
						bottom: "4px",
					}}
					onMouseEnter={() => this.setState({mouseoverweapon: true})}
					onMouseLeave={() => this.setState({mouseoverweapon: false})}>
					<Card card={this.props.card.weapon} scale={scale/2} />
				</div>;

			}
			if (this.props.displayHealth) {
				health = <div style={{position: "absolute", bottom: "12px", right: 0, color: "yellow", height: "25%", textAlign: "right", fontSize: (height*0.3) + "px", zIndex: 8}}>
					{this.props.card.health}
				</div>
			}
		}
		// TODO: down
		return <>
				<div style={style}
				id={this.props.id}
				data-scale={scale}
				onClick={() => { if (this.props.onClick) this.props.onClick(); }}
				onMouseEnter={() => this.setState({mouseover: true})}
				onMouseLeave={() => this.setState({mouseover: false})}>
				{this.props.selected ? <CardOverlay /> : null}
				{e}
				{health}
				{weapon}
			</div>
			{this.state.mouseover && !this.state.mouseoverweapon
				? <div style={{
						position: "fixed",
						top: 0,
						left: 0,
						pointerEvents: "none",
						zIndex: 100,
						display: window.v ? "block" : "none"
					}}
					className="hovercard">
						<Card card={this.props.card} scale={(window.innerHeight - 200) / 500} from_hovercard={true} />
					</div>
				: null}
		</>
	}
}

window.Card = Card;


