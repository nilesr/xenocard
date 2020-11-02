class Hand extends React.Component {
	render() {
		// TODO distribute across the div
		const width = 1458; // temp hardcoded
		const distr_end = width - 350;
		const distr_interval = distr_end / (this.props.cards.length - 1)
		return <div style={{position: "relative"}}>
			{this.props.cards.map((card, i) =>
				<div
					key={i}
					style={{
						position: "absolute",
						top: 0,
						left: (i * distr_interval) + "px",
					}}>
						<Card card={card} scale={2/5} />
				</div>)}
		</div>;
	}
}

window.Hand = Hand;
