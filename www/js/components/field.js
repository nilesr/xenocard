class Field extends React.Component {
	render() {
		const enemy_player = this.props.player == "P1" ? "P2" : "P1";
		const transform = this.props.tilt ? "rotateX(15deg) scale(0.9) translateY(-50px)" : "";
		return <div style={{height: "100%", width: "100%", transform}}>
			<div style={{
				height: "100%",
				width: "100%",
				zIndex: "-100",
				backgroundImage: 'url(images/field.jpg)',
				backgroundSize: "cover",
				transform: this.props.player == "P2" ? "rotate(180deg)" : null,
				position: "relative",
			}} />
			<EnemyField
				height={this.props.height}
				cards={this.props.field[enemy_player]}
				deckSize={this.props.enemyDeckSize}
				junk={this.props.enemyJunk}
				lostSize={this.props.enemyLostSize}
				player={enemy_player}
				selectedPlayer={this.props.selectedPlayer}
				selectedFieldSegment={this.props.selectedFieldSegment}
				selectedIndex={this.props.selectedIndex}
				onCardClick={this.makeCardClickHandler(enemy_player)}
				rotate={this.props.rotate} />
			<MyField
				height={this.props.height}
				cards={this.props.field[this.props.player]}
				deckSize={this.props.deckSize}
				junk={this.props.junk}
				lostSize={this.props.lostSize}
				player={this.props.player}
				selectedPlayer={this.props.selectedPlayer}
				selectedFieldSegment={this.props.selectedFieldSegment}
				selectedIndex={this.props.selectedIndex}
				onCardClick={this.makeCardClickHandler(this.props.player)} />
		</div>;
	}
	makeCardClickHandler(player) {
		return (fs, index) => this.props.onCardClick(player, fs, index);
	}
}

window.Field = Field;

