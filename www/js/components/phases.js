class Phases extends React.Component {
	render() {
		const p2 = this.props.phase.indexOf("P2") === 0;
		const phase_index = ["DRAW", "MOVE", "EVENT", "SET", "BLOCK", "BATTLE", "ADJUST"].indexOf(this.props.phase.split("_")[1]);
		if (phase_index < 0) {
			if (["P1_SHUFFLE", "P2_SHUFFLE"].includes(this.props.phase)) {
				const message = this.props.phase.split("_")[0] != this.props.player
					? "The other player is re-drawing their hand up to 3 times before the first turn"
					: "You are re-drawing your hand up to 3 times before the first turn";
				return <div className="panel">
					<b>Current Phase:</b> {message}
				</div>;
			}
			return "Current phase: " + this.state.phase;
		}
		return <div>
			<div style={{
				backgroundImage: "url(images/phases.jpg)",
				transform: p2 ? "rotate(180deg)" : null,
				height: 200,
				width: 253/2,
				backgroundSize: "253px",
				backgroundPosition: p2 ? "-" + (253/2) + "px 0px" : null,
				position: "relative",
			}}>
				<div style={{
					position: "absolute",
					left: 0,
					top: (200*(p2 ? 6-phase_index : phase_index)/7) + "px",
					width: "120px",
					height: (200/7) - 7.5 + "px",
					border: "3px solid red",
					borderRadius: 6,
					animation: "phases_border 3s infinite",
					animationDirection: "alternate",
				}}>
				</div>
			</div>
		</div>;
	}
}

window.Phases = Phases;


