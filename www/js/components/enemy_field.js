class EnemyField extends React.Component {
	render() {
		return <>
			<FieldCard height={this.props.height} x={130} y={148} card={this.props.cards.SITUATION[3]} />
			<FieldCard height={this.props.height} x={130} y={474} card={this.props.cards.SITUATION[2]} />
			<FieldCard height={this.props.height} x={130} y={800} card={this.props.cards.SITUATION[1]} />
			<FieldCard height={this.props.height} x={130} y={1127} card={this.props.cards.SITUATION[0]} />

			<FieldCard height={this.props.height} x={2455} y={148} card={this.props.cards.STANDBY[3]} />
			<FieldCard height={this.props.height} x={2455} y={474} card={this.props.cards.STANDBY[2]} />
			<FieldCard height={this.props.height} x={2455} y={800} card={this.props.cards.STANDBY[1]} />
			<FieldCard height={this.props.height} x={2455} y={1127} card={this.props.cards.STANDBY[0]} />

			<FieldCard height={this.props.height} x={1075} y={766} card={this.props.cards.BATTLEFIELD[3]} />
			<FieldCard height={this.props.height} x={1516} y={766} card={this.props.cards.BATTLEFIELD[2]} />
			<FieldCard height={this.props.height} x={1075} y={1085} card={this.props.cards.BATTLEFIELD[1]} />
			<FieldCard height={this.props.height} x={1516} y={1085} card={this.props.cards.BATTLEFIELD[0]} />
		</>;
	}
}

window.EnemyField = EnemyField;


