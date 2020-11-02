class Buttons extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			shuffles: 0,
		};
	}
	render() {
		return <div>
			<div>
				{playerSideForPhase(this.props.phase) == this.props.player
					? <button onClick={this.endPhase}>End Phase</button>
					: null}
			</div>
			<div>
				{["P1_SHUFFLE", "P2_SHUFFLE"].includes(this.props.phase) && (playerSideForPhase(this.props.phase) == this.props.player)
					? <button onClick={this.shuffle} disabled={this.state.shuffles == 3}>Re-draw hand ({this.state.shuffles}/3)</button>
					: null}
			</div>
		</div>;
	}
	endPhase = () => this.props.sendInstruction("set_phase", {"phase": nextPhase(this.state.phase)});
	shuffle = () => {
		this.setState(s => {return {shuffles: s.shuffles + 1}});
		this.props.sendInstruction("shuffle");
	}
};

window.Buttons = Buttons;
