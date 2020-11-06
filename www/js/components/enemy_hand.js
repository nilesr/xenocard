class EnemyHand extends React.Component {
	render() {
		const width = window.innerWidth/4;
		const distr_end = width - 350;
		const distr_interval = width / this.props.cards;
		return <>
			<div style={{position: "fixed", top: 0, right: 0, width: width + "px", height: "75px"}}>
				{Array(this.props.cards).fill().map((_, i) =>
					<div
						key={i}
						style={{
							position: "absolute",
							bottom: 0,
							left: (i * distr_interval) + "px",
						}}>
							<Card
								card={{image_location: {set: 1, top: 0, left: 0}}}
								onClick={() => this.props.onClick(i)}
								scale={100/500}
								selected={i == this.props.selectedIndex} />
					</div>)}
			</div>
			<div style={{position: "fixed", top: "75px", right: 0, color: "yellow"}}>
				{this.props.cards} card{this.props.cards == 1 ? "" : "s"}
			</div>
		</>;
	}
}

window.EnemyHand = EnemyHand;

