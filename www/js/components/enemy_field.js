const FieldCard = function(props) {
	return <div style={{
		position: "absolute",
		top: (100*props.y/3000) + "%",
		left: (100*props.x/3000) + "%",
	}}>
		<Card card={props.card || {image_location: {set: 1, top: 0, left: 0}}} scale={((300/3000) * props.height)/500} />
	</div>;
};

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
		</>;
	}
}

window.EnemyField = EnemyField;


