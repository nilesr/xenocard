class Hand extends React.Component {
	render() {
		const width = window.innerWidth - 150 - 300;
		const distr_end = width - (700*(150/500));
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
						<Card
							card={card}
							onClick={() => this.props.onClick(i)}
							scale={150/500}
							selected={i == this.props.selectedIndex} />
				</div>)}
		</div>;
	}
}

window.Hand = Hand;
