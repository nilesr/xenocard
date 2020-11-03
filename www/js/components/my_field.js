class MyField extends React.Component {
	render() {
		return <>
			{/* Situation */}
			<FieldCard
				height={this.props.height}
				x={2455}
				y={148+1430}
				card={this.props.cards.SITUATION[0]}
				onClick={() => this.props.onCardClick("SITUATION", 0)}
				selected={this.isSelected("SITUATION", 0)} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={474+1430}
				card={this.props.cards.SITUATION[1]}
				onClick={() => this.props.onCardClick("SITUATION", 1)}
				selected={this.isSelected("SITUATION", 1)} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={800+1430}
				card={this.props.cards.SITUATION[2]}
				onClick={() => this.props.onCardClick("SITUATION", 2)}
				selected={this.isSelected("SITUATION", 2)} />
			<FieldCard
				height={this.props.height}
				x={2455}
				y={1127+1430}
				card={this.props.cards.SITUATION[3]}
				onClick={() => this.props.onCardClick("SITUATION", 3)}
				selected={this.isSelected("SITUATION", 3)} />

			{/* Standby */}
			<FieldCard
				height={this.props.height}
				x={130}
				y={148+1430}
				card={this.props.cards.STANDBY[0]}
				onClick={() => this.props.onCardClick("STANDBY", 0)}
				selected={this.isSelected("STANDBY", 0)} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={474+1430}
				card={this.props.cards.STANDBY[1]}
				onClick={() => this.props.onCardClick("STANDBY", 1)}
				selected={this.isSelected("STANDBY", 1)} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={800+1430}
				card={this.props.cards.STANDBY[2]}
				onClick={() => this.props.onCardClick("STANDBY", 2)}
				selected={this.isSelected("STANDBY", 2)} />
			<FieldCard
				height={this.props.height}
				x={130}
				y={1127+1430}
				card={this.props.cards.STANDBY[3]}
				onClick={() => this.props.onCardClick("STANDBY", 3)}
				selected={this.isSelected("STANDBY", 3)} />

			{/* Battlefield */}
			<FieldCard
				height={this.props.height}
				x={1075}
				y={3000-300-766+5}
				card={this.props.cards.BATTLEFIELD[2]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 2)}
				selected={this.isSelected("BATTLEFIELD", 2)} />
			<FieldCard
				height={this.props.height}
				x={1516}
				y={3000-300-766+5}
				card={this.props.cards.BATTLEFIELD[3]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 3)}
				selected={this.isSelected("BATTLEFIELD", 3)} />
			<FieldCard
				height={this.props.height}
				x={1075}
				y={3000-300-1085+5}
				card={this.props.cards.BATTLEFIELD[0]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 0)}
				selected={this.isSelected("BATTLEFIELD", 0)} />
			<FieldCard
				height={this.props.height}
				x={1516}
				y={3000-300-1085+5}
				card={this.props.cards.BATTLEFIELD[1]}
				onClick={() => this.props.onCardClick("BATTLEFIELD", 1)}
				selected={this.isSelected("BATTLEFIELD", 1)} />

			{/* junk */}
			<FieldCard height={this.props.height} x={1902} y={2556} card={this.props.junk[this.props.junk.length - 1]} />
			{/* deck */}
			<FieldCard height={this.props.height} x={1293} y={2394} card={null} />
			{/* lost */}
			<FieldCard height={this.props.height} x={684} y={2556} card={null} />

		</>;
	}
	isSelected(fs, index) {
		return this.props.player == this.props.selectedPlayer && fs == this.props.selectedFieldSegment && index == this.props.selectedIndex;
	}
}

window.MyField = MyField;

