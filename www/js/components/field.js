class Field extends React.Component {
	render() {
		const enemy_player = this.props.player == "P1" ? "P2" : "P1";
		return <div style={{height: "100%", width: "100%"}}>
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
				cards={this.props.field[enemy_player]} />
			<MyField
				height={this.props.height}
				cards={this.props.field[this.props.player]} />
		</div>;
	}
}

window.Field = Field;

