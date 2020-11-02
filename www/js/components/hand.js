class Hand extends React.Component {
	render() {
		// TODO distribute across the div
		return this.props.cards.map((card, i) => {
			return <Card
				key={i}
				card={card}
				scale={2/5} />
		});
	}
}

window.Hand = Hand;
