class EnemyField extends React.Component {
	render() {
		return <>
			{/* Situation */}
			<FieldCard
				height={this.props.height}
				x={130}
				y={148}
				card={this.props.cards.SITUATION[3]}
				onClick={() => this.props.onCardClick("SITUATION", 3)}
				selected={this.isSelected("SITUATION", 3)}
				rotate={this.props.rotate} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={474}
				card={this.props.cards.SITUATION[2]}
				onClick={() => this.props.onCardClick("SITUATION", 2)}
				selected={this.isSelected("SITUATION", 2)}
				rotate={this.props.rotate} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={800}
				card={this.props.cards.SITUATION[1]}
				onClick={() => this.props.onCardClick("SITUATION", 1)}
				selected={this.isSelected("SITUATION", 1)}
				rotate={this.props.rotate} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={1127}
				card={this.props.cards.SITUATION[0]}
				onClick={() => this.props.onCardClick("SITUATION", 0)}
				selected={this.isSelected("SITUATION", 0)}
				rotate={this.props.rotate} />

			{/* Standby */}
			<FieldCard
				height={this.props.height}
				x={2455}
				y={148}
				card={this.props.cards.STANDBY[3]}
				onClick={() => this.props.onCardClick("STANDBY", 3)}
				selected={this.isSelected("STANDBY", 3)}
			rotate={this.props.rotate} 
				displayHealth={true} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={474}
				card={this.props.cards.STANDBY[2]}
				onClick={() => this.props.onCardClick("STANDBY", 2)}
				selected={this.isSelected("STANDBY", 2)}
			rotate={this.props.rotate} 
				displayHealth={true} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={800}
				card={this.props.cards.STANDBY[1]}
				onClick={() => this.props.onCardClick("STANDBY", 1)}
				selected={this.isSelected("STANDBY", 1)}
			rotate={this.props.rotate} 
				displayHealth={true} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={1127}
				card={this.props.cards.STANDBY[0]}
				onClick={() => this.props.onCardClick("STANDBY", 0)}
				selected={this.isSelected("STANDBY", 0)}
			rotate={this.props.rotate} 
				displayHealth={true} />

			{/* Battlefield */}
			<FieldCard
				height={this.props.height}
				x={1075}
				y={766}
				card={this.props.cards.BATTLEFIELD[3]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 3)}
				selected={this.isSelected("BATTLEFIELD", 3)}
				rotate={this.props.rotate} />
			<BattlefieldInfo
				height={this.props.height}
				x={636}
				y={766}
				card={this.props.cards.BATTLEFIELD[3]} />

			<FieldCard
				height={this.props.height}
				x={1516}
				y={766}
				card={this.props.cards.BATTLEFIELD[2]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 2)}
				selected={this.isSelected("BATTLEFIELD", 2)}
				rotate={this.props.rotate} />
			<BattlefieldInfo
				height={this.props.height}
				x={1952}
				y={766}
				card={this.props.cards.BATTLEFIELD[2]} />

			<FieldCard
				height={this.props.height}
				x={1075}
				y={1085}
				card={this.props.cards.BATTLEFIELD[1]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 1)}
				selected={this.isSelected("BATTLEFIELD", 1)}
				rotate={this.props.rotate} />
			<BattlefieldInfo
				height={this.props.height}
				x={636}
				y={1085}
				card={this.props.cards.BATTLEFIELD[1]} />

			<FieldCard
				height={this.props.height}
				x={1516}
				y={1085}
				card={this.props.cards.BATTLEFIELD[0]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 0)}
				selected={this.isSelected("BATTLEFIELD", 0)}
				rotate={this.props.rotate} />
			<BattlefieldInfo
				height={this.props.height}
				x={1952}
				y={1085}
				card={this.props.cards.BATTLEFIELD[0]} />

			{/* junk */}
			<FieldCard height={this.props.height} x={684} y={148} card={this.props.junk[this.props.junk.length - 1]} rotate={this.props.rotate} />
			<FieldDeckCount height={this.props.height} x={684} y={435} count={this.props.junk.length} />
			{/* deck */}
			<FieldCard id="enemy_deck" height={this.props.height} x={1293} y={310} card={this.props.deckSize == 0 ? null : FLIPPED_CARD} rotate={this.props.rotate} />
			<FieldDeckCount height={this.props.height} x={1293} y={50} count={this.props.deckSize} />
			{/* lost */}
			<FieldCard height={this.props.height} x={1902} y={148} card={this.props.lostSize == 0 ? null : FLIPPED_CARD} rotate={this.props.rotate} />
			<FieldDeckCount height={this.props.height} x={1902} y={435} count={this.props.lostSize} />

		</>;
	}
	isSelected(fs, index) {
		return this.props.player == this.props.selectedPlayer && fs == this.props.selectedFieldSegment && index == this.props.selectedIndex;
	}
}

window.EnemyField = EnemyField;


