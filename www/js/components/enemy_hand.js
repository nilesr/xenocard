class EnemyHand extends React.Component {
	render() {
		const width = window.innerWidth/4;
		const distr_end = width - (700*(100/500));
		const distr_interval = width / (this.props.cards + 1);
		return <>
			<div style={{position: "fixed", top: 0, right: 0, width: width + "px", height: "75px"}}>
				{Array(this.props.cards).fill().map((_, i) =>
					<div
						key={i}
						style={{
							position: "absolute",
							bottom: 0,
							left: ((this.props.cards - i - 1) * distr_interval) + "px",
						}}>
							<Card
								card={FLIPPED_CARD}
								onClick={() => this.props.onClick(i)}
								scale={100/500}
								selected={i == this.props.selectedIndex}
								rotate={true} />
					</div>)}
			</div>
			<div style={{position: "fixed", top: "75px", right: 0, color: "yellow"}}>
				{this.props.cards} card{this.props.cards == 1 ? "" : "s"}
			</div>
		</>;
	}
}

window.EnemyHand = EnemyHand;

